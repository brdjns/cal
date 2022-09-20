// token.cc: Lexical analyser.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "token.h"
#include "error.h"
#include <cctype>
#include <iostream>

// @brief Fetch a token from the standard input.
// @pre An ASCII character.
// @returns A token.
// @throws std::runtime_error if token is not alphanumeric or an operator.
Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }

    char ch{};
    std::cin >> ch;

    switch (ch) {
    case Symbol::print_tok:
    case Symbol::lparen_tok:
    case Symbol::rparen_tok:
    case Symbol::lbrace_tok:
    case Symbol::rbrace_tok:
    case Symbol::lbrack_tok:
    case Symbol::rbrack_tok:
    case Symbol::bar_tok:
    case Symbol::mul_tok:
    case Symbol::div_tok:
    case Symbol::mod_tok:
    case Symbol::plus_tok:
    case Symbol::minus_tok:
    case Symbol::bang_tok:
    case Symbol::equals_tok:
    case Symbol::caret_tok:
    case Symbol::comma_tok:
        return Token{ch};
    case Symbol::dot_tok:
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
        std::cin.unget();
        double value{};
        std::cin >> value;
        return Token{Symbol::number_tok, value};
    }
    case eof_tok: // end of file (^Z on MS-Windows, ^D on Unix)
        return Token{Symbol::quit_tok};
    default: // identifiers
        if (std::isalpha(ch)) {
            std::string str;
            str += ch;
            while (std::cin.get(ch) &&
                   (std::isalpha(ch) || ch == '_' || std::isdigit(ch))) {
                str += ch;
            }
            std::cin.putback(ch);
            if (str == kw_let)
                return Token{Symbol::let_tok};
            if (str == kw_const)
                return Token{Symbol::const_tok};
            if (str == kw_set)
                return Token{Symbol::set_tok};
            if (str == kw_exit)
                return Token{Symbol::quit_tok};
            if (str == kw_sqrt)
                return Token{Symbol::sqrt_tok};
            return Token{Symbol::ident_tok, str};
        }
        error("unrecognized token");
    }
    return Token{'?'}; // never reached
}

// @brief Discard characters up to and including a c.
// @param c a character to ignore.
void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;
    char ch{};
    while (std::cin >> ch) {
        if (ch == c) {
            return;
        }
    }
}
