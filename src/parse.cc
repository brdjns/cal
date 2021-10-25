// parse.cc: cal recursive-descent parser.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "parse.h"
#include "error.h"
#include "function.h"
#include "symbol_table.h"
#include "token.h"

Symbol_table names;
double expression(Token_stream& ts);

// Construct a factor.
double factor(Token_stream& ts)
{
    Token t{ts.get()};

    switch (t.kind) {
    case lparen:
    {
        double temp{expression(ts)};
        t = ts.get();
        if (t.kind != rparen) {
            error("')' missing in expression");
        }
        return temp;
    }
    case lbrace:
    {
        double temp{expression(ts)};
        t = ts.get();
        if (t.kind != rbrace) {
            error("'}' missing in expression");
        }
        return temp;
    }
    case lbrack:
    {
        double temp{expression(ts)};
        t = ts.get();
        if (t.kind != rbrack) {
            error("']' missing in expression");
        }
        return temp;
    }
    case sq_rt: // sqrt(a)
    {
        t = ts.get();
        if (t.kind != lparen) {
            error("'(' missing in expression");
        }
        double temp{expression(ts)};
        if (temp < 0) {
            error("domain error");
        }
        t = ts.get();
        if (t.kind != rparen) {
            error("')' missing in expression");
        }
        return std::sqrt(temp);
    }
    case bar: // |a|
    {
        double temp = expression(ts);
        t = ts.get();
        if (t.kind != bar) {
            error("'|' missing in expression");
        }
        return std::abs(temp);
    }
    case minus: // -a
        return -factor(ts);
    case plus: // +a
        return factor(ts);
    case number: // [.0-9]
        return t.value;
    case ident: // [a-zA-Z_]
        return names.get(t.name);
    default:
        error("factor expected");
    }

    return 0; // never reached
}

// Construct a power expression.
double power_expression(Token_stream& ts)
{
    double left{factor(ts)};
    Token t{ts.get()};

    while (true) {
        switch (t.kind) {
        case bang: // a!
        {
            int temp = narrow_cast<int>(left);
            if (temp < 0) {
                error("domain error");
            }
            return left = factorial(temp);
            break;
        }
        case caret: // a^b
        {
            double temp{factor(ts)};
            return left = std::pow(left, temp);
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Construct a term.
double term(Token_stream& ts)
{
    double left{power_expression(ts)};

    while (true) {
        Token t{ts.get()};
        switch (t.kind) {
        case star: // a*b
            left *= power_expression(ts);
            break;
        case slash: // a/b
        {
            double temp{power_expression(ts)};
            if (temp == 0) {
                error("division by zero");
            }
            left /= temp;
            break;
        }
        case percent: // a%b is defined for floats
        {
            double temp{power_expression(ts)};
            if (temp == 0) {
                error("modulo division by zero");
            }
            left = std::fmod(left, temp);
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Construct an expression.
double expression(Token_stream& ts)
{
    double left{term(ts)};
    Token t{ts.get()};

    while (true) {
        switch (t.kind) {
        case plus: // a+b
            left += term(ts);
            t = ts.get();
            break;
        case minus: // a-b
            left -= term(ts);
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Declare a variable.
double declaration(Token_stream& ts, bool is_const)
{
    Token t{ts.get()};
    if (t.kind != ident) {
        error("identifier missing in declaration");
    }
    std::string name{t.name};

    Token t2{ts.get()};
    if (t2.kind != equals) {
        error("'=' missing in declaration of ", name);
    }

    double value{expression(ts)};
    names.declare(name, value, is_const);
    return value;
}

// Deal with assignments.
double assignment(Token_stream& ts)
{
    Token t{ts.get()};
    if (t.kind != ident) {
        error("identifier missing in assignment");
    }
    std::string name{t.name};

    Token t2{ts.get()};
    if (t2.kind != equals) {
        error("'=' missing in assignment of ", name);
    }
    double value{expression(ts)};
    names.set(name, value);
    return value;
}

// Deal with statements.
double statement(Token_stream& ts)
{
    Token t{ts.get()};
    switch (t.kind) {
    case let:
        return declaration(ts, false);
    case readonly:
        return declaration(ts, true);
    case set:
        return assignment(ts);
    default:
        ts.putback(t);
        return expression(ts);
    }
}

// Compute an expression.
void calculate(Token_stream& ts)
{
    // Get the greatest available precision from a double.
    std::cout.precision(std::numeric_limits<double>::max_digits10 + 2);

    const std::string prompt{"> "};

    while (true) try {
            std::cout << prompt;
            Token t{ts.get()};
            while (t.kind == print) { // discard all 'print' tokens
                t = ts.get();
            }
            if (t.kind == quit) {
                return;
            }
            ts.putback(t);
            std::cout << statement(ts) << '\n';
        }
        catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << '\n';
            clean_up_mess(ts);
        }
}

int main()
try {
    Token_stream ts;
    // Predefined constants.
    names.declare("E", 2.71828182845904523536, true);       // e
    names.declare("LOG2E", 1.44269504088896340736, true);   // log2(e)
    names.declare("LOG10E", 0.434294481903251827651, true); // log10(e)
    names.declare("LN2", 0.693147180559945309417, true);    // ln(2)
    names.declare("LN10", 2.30258509299404568402, true);    // ln(10)
    names.declare("PI", 3.14159265358979323846, true);      // pi
    names.declare("PI_2", 1.57079632679489661923, true);    // pi/2
    names.declare("PI_4", 0.785398163397448309616, true);   // pi/4
    names.declare("SQRT2", 1.41421356237309504880, true);   // sqrt(2)

    calculate(ts);
    return EXIT_SUCCESS;
}
catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}
catch (...) {
    std::cerr << "exception\n";
    return EXIT_FAILURE;
}
