// token.h: Lexical analyser interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

#include <string>

// @class Token
// @brief A token class.
// @details Represents a token that has a kind and a value.
class Token {
public:
    char kind;        // a token kind
    double value;     // a token value
    std::string name; // an identifier name

    // @brief Construct a token from a character.
    // @param[in] ch a kind.
    Token(char ch) : kind{ch}, value{0} {}

    // @brief Construct a token from a character and value.
    // @param[in] ch a kind.
    // @param[in] val a value.
    Token(char ch, double val) : kind{ch}, value{val} {}

    // @brief Construct a token from a character and name.
    // @param[in] ch a kind.
    // @param[in] n an identifier.
    Token(char ch, std::string id) : kind{ch}, name{id} {}
};

// @class Token_stream
// @brief A token stream class.
// @details Converts characters into tokens.
class Token_stream {
public:
    // @brief Construct a stream of tokens that reads from the standard input.
    Token_stream() : full{false}, buffer{0} {}

    // @brief Construct a stream of tokens that reads from the standard input.
    // @param[in] is an input stream.
    Token_stream(std::istream& is) : full{false}, buffer{0} {}

    // @brief Fetch a token from the standard input.
    Token get();

    // @brief Put a token back into the token stream.
    // @param[in] t a token.
    void putback(Token t)
    {
        buffer = t;
        full = true;
    }

    // @brief Discard characters up to and including a c.
    // @param[in] c character to ignore.
    void ignore(char c);

private:
    bool full;    // True when the token buffer is full
    Token buffer; // A buffer of tokens
};

// Recognised scanner symbols.
enum Symbol {
    // lists
    comma = ',',

    // bracketing
    lparen = '(',
    rparen = ')',
    lbrace = '{',
    rbrace = '}',
    lbrack = '[',
    rbrack = ']',
    bar = '|',

    // arithmetic operators
    star = '*',
    slash = '/',
    percent = '%',
    plus = '+',
    minus = '-',
    bang = '!',
    caret = '^',

    // assignment and initialisation
    equals = '=',

    // general keywords
    let = 'L',
    set = 'S',
    readonly = 'C',
    quit = 'E',
    number = '#',
    ident = '@',

    // function operators
    sq_rt = 'R',

    // non-printing
    eof = '\0',

    // other
    print = ';',
    dot = '.',
};

// Keywords.
const std::string kw_decl{"let"};
const std::string kw_assn{"set"};
const std::string kw_const{"const"};
const std::string kw_exit{"exit"};
const std::string kw_sqrt{"sqrt"};
