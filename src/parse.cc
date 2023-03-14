// parse.cc: calc recursive-descent parser.
// SPDX-FileCopyrightText: © 2021-2022 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "parse.h"
#include "error.h"
#include "function.h"
#include "symbol_table.h"
#include "token.h"

Symbol_table names;
double expression(Token_stream& ts);

// Match a token.
void match(Token t, char c)
{
    if (t.kind != c)
        error("expected ", c);
}

// Construct a factor.
double factor(Token_stream& ts)
{
    Token t{ts.get()};

    switch (t.kind) {
    case Symbol::lparen_tok:
    {
        double temp{expression(ts)};
        t = ts.get();
        if (t.kind != Symbol::rparen_tok) {
            error("')' missing in expression");
        }
        return temp;
    }
    case Symbol::lbrace_tok:
    {
        double temp{expression(ts)};
        t = ts.get();
        if (t.kind != Symbol::rbrace_tok) {
            error("'}' missing in expression");
        }
        return temp;
    }
    case lbrack_tok:
    {
        double temp{expression(ts)};
        t = ts.get();
        if (t.kind != Symbol::rbrack_tok) {
            error("']' missing in expression");
        }
        return temp;
    }
    case sqrt_tok: // sqrt(a)
    {
        t = ts.get();
        match(t, '(');
        double temp{expression(ts)};
        if (temp < 0) {
            error("domain error");
        }
        t = ts.get();
        match(t, ')');
        return std::sqrt(temp);
    }
    case abs_tok: // abs(a)
    {
        t = ts.get();
        match(t, '(');
        double temp = expression(ts);
        t = ts.get();
        match(t, ')');
        return std::abs(temp);
    }
    case minus_tok: // -a
        return -factor(ts);
    case plus_tok: // +a
        return factor(ts);
    case number_tok: // [.0-9]
        return t.value;
    case ident_tok: // [a-zA-Z_]
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

    for (;;) {
        switch (t.kind) {
        case Symbol::bang_tok: // a!
        {
            int temp = narrow_cast<int>(left);
            if (temp < 0) {
                error("domain error");
            }
            return left = fn_factorial(temp);
            break;
        }
        case Symbol::caret_tok: // a^b
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

    for (;;) {
        Token t{ts.get()};
        switch (t.kind) {
        case Symbol::mul_tok: // a*b
            left *= power_expression(ts);
            break;
        case div_tok: // a/b
        {
            double temp{power_expression(ts)};
            if (temp == 0) {
                error("division by zero");
            }
            left /= temp;
            break;
        }
        case Symbol::mod_tok: // a%b is defined for floats
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

    for (;;) {
        switch (t.kind) {
        case Symbol::plus_tok: // a+b
            left += term(ts);
            t = ts.get();
            break;
        case Symbol::minus_tok: // a-b
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
    if (t.kind != Symbol::ident_tok) {
        error("identifier missing in declaration");
    }
    std::string name{t.name};

    Token t2{ts.get()};
    if (t2.kind != Symbol::equals_tok) {
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
    if (t.kind != Symbol::ident_tok) {
        error("identifier missing in assignment");
    }
    std::string name{t.name};

    Token t2{ts.get()};
    if (t2.kind != Symbol::equals_tok) {
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
    case Symbol::let_tok:
        return declaration(ts, false);
    case Symbol::const_tok:
        return declaration(ts, true);
    case Symbol::set_tok:
        return assignment(ts);
    default:
        ts.putback(t);
        return expression(ts);
    }
}
