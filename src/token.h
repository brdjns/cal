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
    comma_tok = ',',

    // bracketing
    lparen_tok = '(',
    rparen_tok = ')',
    lbrace_tok = '{',
    rbrace_tok = '}',
    lbrack_tok = '[',
    rbrack_tok = ']',
    bar_tok = '|',

    // arithmetic operators
    mul_tok = '*',
    div_tok = '/',
    mod_tok = '%',
    plus_tok = '+',
    minus_tok = '-',
    bang_tok = '!',
    caret_tok = '^',

    // assignment and initialisation
    equals_tok = '=',

    // general keywords
    let_tok = 'L',
    set_tok = 'S',
    const_tok = 'C',
    quit_tok = 'E',
    number_tok = '#',
    ident_tok = '@',

    // function operators
    sqrt_tok = 'R',

    // non-printing
    eof_tok = '\0',

    // other
    print_tok = ';',
    dot_tok = '.',
};

// Keywords.
const std::string kw_let{"let"};
const std::string kw_set{"set"};
const std::string kw_const{"const"};
const std::string kw_exit{"exit"};
const std::string kw_sqrt{"sqrt"};
