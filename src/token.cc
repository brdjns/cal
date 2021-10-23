// token.cc: Lexical analyser.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "token.h"
#include "cal.h"

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
    case print:
    case lparen:
    case rparen:
    case lbrace:
    case rbrace:
    case lbrack:
    case rbrack:
    case bar:
    case star:
    case slash:
    case percent:
    case plus:
    case minus:
    case bang:
    case equals:
    case caret:
    case comma:
        return Token{ch};
    case dot:
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
        return Token{number, value};
    }
    case eof: // ^Z
        return Token{quit};
    default: // identifiers
        if (std::isalpha(ch)) {
            std::string str;
            str += ch;
            while (std::cin.get(ch) &&
                   (std::isalpha(ch) || ch == '_' || std::isdigit(ch))) {
                str += ch;
            }
            std::cin.putback(ch);
            if (str == kw_decl) {
                return Token{let};
            }
            if (str == kw_assn) {
                return Token{set};
            }
            if (str == kw_quit || str == kw_exit) {
                return Token{quit};
            }
            if (str == kw_sqrt) {
                return Token{sq_rt};
            }
            return Token{ident, str};
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
