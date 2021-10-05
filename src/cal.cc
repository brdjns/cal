// cal.cc: Cal, a portable desk calculator.
// This file is part of Cal.

// Copyright (c) 2021 B. M. Jones.

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "cal.h"

///  expression ::= term | expression '+' term | expression '-' term.
///  term       ::= factor | term '*' factor | term '/' factor.
///  factor    ::= number | '(' expression ')' | '{' expression '}'.
///  number     ::= floating-point-literal.

/// @class Token
/// @brief A token class.
/// @details Represents a token that has a kind and a value.
class Token {
public:
    char kind;    // a token kind
    double value; // a token value

    /// @brief Construct a token from a character.
    /// @param[in] ch a kind
    Token(char ch) : kind{ch}, value{0} {}

    /// @brief Construct a token from a character and value.
    /// @param[in] ch a kind
    /// @param[in] val a value
    Token(char ch, double val) : kind{ch}, value{val} {}
};

/// @class Token_stream
/// @brief A token stream class.
/// @details Converts characters into tokens.
class Token_stream {
public:
    /// @brief Construct a stream of tokens that reads from the standard input.
    Token_stream();

    /// @brief Fetch a token from the standard input.
    Token get();

    /// @brief Put a token back into the token stream.
    /// @param[in] t a token
    void putback(Token t);

private:
    bool full;    // True when the token buffer is full
    Token buffer; // A buffer of tokens
};

// Constructor sets buffer to empty.
Token_stream::Token_stream() : full{false}, buffer{0}
{}

/// @brief Put a token back into the token stream.
/// @param t A token
/// @pre There is no token in the buffer.
/// @post The buffer is full.
void Token_stream::putback(Token t)
{
    if (full) {
        throw std::runtime_error("attempt to put back into full buffer");
    }
    buffer = t;
    full = true;
}

/// @brief Fetch a token from the standard input.
/// @pre An ASCII character.
/// @post Return a token.
/// @throws runtime_error if token is not a digit, parentheses or operator.
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
    case '{':
    case '}':
    case '*':
    case '/':
    case '+':
    case '-':
        return Token{ch};
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
    case '9':
    {
        std::cin.putback(ch);
        double val{};
        std::cin >> val;
        return Token{'8', val};
    }
    default:
        throw std::runtime_error("bad token");
    }
}

Token_stream ts;
double expression();

/// @brief Construct a factor.
/// @pre A token that is a number or parentheses.
/// @post Return a factor.
/// @throws runtime_error if next token is not an expression.
double factor()
{
    Token t{ts.get()};

    switch (t.kind) {
    // Brackets.
    case '(':
    {
        double d{expression()};
        t = ts.get();
        if (t.kind != ')') {
            throw std::runtime_error("')' expected");
        }
        return d;
    }
    // Braces.
    case '{':
    {
        double d{expression()};
        t = ts.get();
        if (t.kind != '}') {
            throw std::runtime_error("'}' expected");
        }
        return d;
    }
    // Numbers.
    case '8':
        return t.value;
    default:
        throw std::runtime_error("factor expected");
    }
}

/// @brief Construct a term.
/// @pre A factor.
/// @post Return a term.
/// @throws runtime_error for division by zero.
double term()
{
    double left{factor()};
    Token t{ts.get()};

    while (true) {
        switch (t.kind) {
        case '*':
            left *= factor();
            t = ts.get();
            break;
        case '/':
        {
            double d{factor()};
            if (d == 0) {
                throw std::runtime_error("division by zero");
            }
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

/// @brief Construct an expression.
/// @pre A term.
/// @post Return an expression.
double expression()
{
    double left{term()};
    Token t{ts.get()};

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
    double val{};

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
