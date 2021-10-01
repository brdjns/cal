// cal.cc: Cal, a portable desk calculator.
// Copyright (c) 2021 B. M. Jones.

// This file is part of Cal.

#include "cal.h"

/*
 *  expression ::= term | expression '+' term | expression '-' term.
 *  term       ::= primary | term '*' primary | term '/' primary.
 *  primary    ::= number | '(' expression ')'.
 *  number     ::= floating-point-literal.
 */

/** @class Token
 *  @brief A token class.
 *  @details Represents a token that has a kind and a value.
 */
class Token {
public:
    char kind;    // A token kind
    double value; // A token value

    Token(char ch) : kind {ch}, value {0} {}
    Token(char ch, double val) : kind {ch}, value {val} {}
};

/**
 * @class Token_stream
 * @brief A token stream class.
 * @details Converts characters into tokens.
 */
class Token_stream {
public:
    Token_stream(); // Constructor sets buffer to empty.
    Token get();
    void putback(Token t);

private:
    bool full;    // True when the token buffer is full
    Token buffer; // A token buffer
};

Token_stream::Token_stream() : full {false}, buffer {0}
{
}

/**
 * @brief Put a token back into the token stream.
 * @param t A token
 * @pre There is no token in the buffer.
 * @post The buffer is full.
 */
void Token_stream::putback(Token t)
{
    if (full) {
        throw std::runtime_error("attempt to put back into full buffer");
    }
    buffer = t;
    full = true;
}

/**
  * @brief Get a token from the token stream.
  * @pre An ASCII character.
  * @post A token.
  * @throws runtime_error if token is not a digit, parentheses or operator.
  */
Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }

    char ch;
    std::cin >> ch;

    switch (ch) {
    case ';': // print
    case 'q': // quit
    case '(':
    case ')':
    case '*':
    case '/':
    case '+':
    case '-':
        return Token {ch};
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        std::cin.putback(ch);
        double val;
        std::cin >> val;
        return Token {'8', val};
    }
    default:
        throw std::runtime_error("bad token");
    }
}

Token_stream ts;
double expression();

/** 
 * @brief Read and evaluate a primary.
 * @pre A token that is a number or parentheses.
 * @post Return a primary.
 * @throws runtime_error if next token is not an expression.
 */
double primary()
{
    Token t = ts.get();

    switch (t.kind) {
    case '(': {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') {
            throw std::runtime_error("')' expected");
        }
        return d;
    }
    case '8':
        return t.value;
    default:
        throw std::runtime_error("primary expected");
    }
}

/**
 * @brief Read and evaluate a term.
 * @pre A primary.
 * @post Return a term.
 * @throws runtime_error for division by zero.
 */
double term()
{
    double left = primary();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            t = ts.get();
            break;
        case '/': {
            double d = primary();
            if (d == 0) {
                throw std::runtime_error("division by zero");
                left /= d;
                t = ts.get();
                break;
            }
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

/**
 * @brief Read and evaluate an expression.
 * @pre A term.
 * @post Return an expression.
 */
double expression()
{
    double left = term();
    Token t = ts.get();

    while (true) {
        switch (t.kind) {
        case '+':
            left += term();
            t = ts.get();
            break;
        case '-':
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

int main()
try {
    double val = 0;

    while (std::cin) {
        Token t = ts.get();

        if (t.kind == 'q') {
            break;
        }
        if (t.kind == ';') {
            std::cout << "=" << val << '\n';
        }
        else {
            ts.putback(t);
        }

        val = expression();
    }

    return 0;
}
catch (std::exception& e) {
    std::cerr << "error: " << e.what() << '\n';
    return 1;
}
catch (...) {
    std::cerr << "unknown exception\n";
    return 2;
}
