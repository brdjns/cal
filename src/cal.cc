// cal.cc: cal, a portable desk calculator.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "cal.h"
#include "function.h"
#include "symbol_table.h"
#include "token.h"

Symbol_table names;
Token_stream ts;
double expression();

// Construct a factor.
double factor()
{
    Token t{ts.get()};

    switch (t.kind) {
    case lparen:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rparen) {
            error("')' missing in expression");
        }
        return temp;
    }
    case lbrace:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rbrace) {
            error("'}' missing in expression");
        }
        return temp;
    }
    case lbrack:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rbrack) {
            error("']' missing in expression");
        }
        return temp;
    }
    case f_sqrt: // sqrt(a)
    {
        t = ts.get();
        if (t.kind != lparen) {
            error("'(' missing in expression");
        }
        double temp{expression()};
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
        double temp = expression();
        t = ts.get();
        if (t.kind != bar) {
            error("'|' missing in expression");
        }
        return std::abs(temp);
    }
    case minus: // -a
        return -factor();
    case plus: // +a
        return factor();
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
double power_expression()
{
    double left{factor()};
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
            double temp{factor()};
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
double term()
{
    double left{power_expression()};

    while (true) {
        Token t{ts.get()};
        switch (t.kind) {
        case star: // a*b
            left *= power_expression();
            break;
        case slash: // a/b
        {
            double temp{power_expression()};
            if (temp == 0) {
                error("division by zero");
            }
            left /= temp;
            break;
        }
        case percent: // a%b is defined for floats
        {
            double temp{power_expression()};
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
double expression()
{
    double left{term()};
    Token t{ts.get()};

    while (true) {
        switch (t.kind) {
        case plus: // a+b
            left += term();
            t = ts.get();
            break;
        case minus: // a-b
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Declare a variable.
double declaration()
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

    double value{expression()};
    names.declare(name, value);
    return value;
}

// Deal with assignments.
double assignment()
{
    Token t{ts.get()};
    if (t.kind != ident) {
        error("identifier missing in assignment");
    }
    std::string name{t.name};

    Token t2 = ts.get();
    if (t2.kind != equals) {
        error("'=' missing in assignment of ", name);
    }
    double value{expression()};
    names.set(name, value);
    return value;
}

// Turn a declaration into a statement.
double statement()
{
    Token t{ts.get()};
    switch (t.kind) {
    case let: // declaration
        return declaration();
    case set: // assignment
        return assignment();
    default:
        ts.putback(t);
        return expression();
    }
}

// Clean up remaining tokens during an exception.
void clean_up_mess()
{
    ts.ignore(print);
}

// Compute an expression.
void calculate()
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
            std::cout << statement() << '\n';
        }
        catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << '\n';
            clean_up_mess();
        }
}

int main()
try {
    // Predefined constants.
    names.declare("E", 2.71828182845904523536);       // e
    names.declare("LOG2E", 1.44269504088896340736);   // log2(e)
    names.declare("LOG10E", 0.434294481903251827651); // log10(e)
    names.declare("LN2", 0.693147180559945309417);    // ln(2)
    names.declare("LN10", 2.30258509299404568402);    // ln(10)
    names.declare("PI", 3.14159265358979323846);      // pi
    names.declare("PI_2", 1.57079632679489661923);    // pi/2
    names.declare("PI_4", 0.785398163397448309616);   // pi/4
    names.declare("SQRT2", 1.41421356237309504880);   // sqrt(2)

    calculate();
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
