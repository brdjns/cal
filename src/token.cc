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
    case print_tok:
    case lparen_tok:
    case rparen_tok:
    case lbrace_tok:
    case rbrace_tok:
    case lbrack_tok:
    case rbrack_tok:
    case bar_tok:
    case mul_tok:
    case div_tok:
    case mod_tok:
    case plus_tok:
    case minus_tok:
    case bang_tok:
    case equals_tok:
    case caret_tok:
    case comma_tok:
        return Token{ch};
    case dot_tok:
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
        return Token{number_tok, value};
    }
    case eof_tok: // end of file (^Z on MS-Windows, ^D on Unix)
        return Token{quit_tok};
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
                return Token{let_tok};
            if (str == kw_const)
                return Token{const_tok};
            if (str == kw_set)
                return Token{set_tok};
            if (str == kw_exit)
                return Token{quit_tok};
            if (str == kw_sqrt)
                return Token{sqrt_tok};
            return Token{ident_tok, str};
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
