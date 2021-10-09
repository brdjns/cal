// cal.cc: Cal, a portable desk calculator.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "cal.h"

// The Wirth syntax notation grammar is as follows:
//
//  expression = term {"+" term | "-" term} .
//  term       = factor {"*" factor | "/" factor} .
//  factor     = number
//             | "(" expression ")"
//             | "[" expression "]"
//             | "{" expression "}" .
//  number     = floating-point-literal .

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
    case '[':
    case ']':
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
        return Token{'#', val};
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
    // Crochets.
    case '[':
    {
        double d{expression()};
        t = ts.get();
        if (t.kind != ']') {
            throw std::runtime_error("']' expected");
        }
        return d;
    }
    // Numbers.
    case '#':
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
    std::string prompt{">> "};
    double val{};
   
    // Get the greatest available precision from a double.
    std::cout.precision(std::numeric_limits<double>::max_digits10 - 1);
    while (std::cin) {
        Token t{ts.get()};
        if (t.kind == 'q') {
            break;
        }
        if (t.kind == ';') {
            std::cout << prompt << std::scientific << val << '\n';
        }
        else {
            ts.putback(t);
            val = expression();
        }
    }

    return EXIT_SUCCESS;
}
catch (std::exception& e) {
    std::cerr << "error: " << e.what() << '\n';
    return EXIT_FAILURE;
}
catch (...) {
    std::cerr << "unknown exception\n";
    return EXIT_FAILURE;
}
