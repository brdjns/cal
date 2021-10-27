// parse.h: Parser interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

#include "token.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

// @brief Cast a wider type to a narrower type.
// @param a a narrower type
// @return the value of the narrower type specified
template<class R, class A>
R narrow_cast(const A& a)
{
    R r = R(a);
    if (A(r) != a)
        throw std::runtime_error("information loss");
    return r;
}

// @brief Construct an expression.
// @pre A term.
// @param ts a stream of tokens.
// @return An expression.
double expression(Token_stream& ts);

// @brief Construct a term.
// @pre A factor.
// @param ts a stream of tokens.
// @return A term.
// @throws std::runtime_error for division by zero.
double term(Token_stream& ts);

// @brief Construct a factor.
// @pre A token that is a number or parentheses.
// @param ts a stream of tokens.
// @return A factor.
// @throws std::runtime_error if next token is not an expression.
double factor(Token_stream& ts);

// @brief Construct a power expression.
// @pre A factor.
// @param ts a stream of tokens.
// @return A power expression.
double power_expression(Token_stream& ts);

// @brief Turn a declaration into a statement.
// @param ts a stream of tokens.
// @return Either a declaration or a statement.
double statement(Token_stream& ts);

// @brief Parse declaration statements.
// @param ts a stream of tokens.
// @param is_const true if identifier is a constant; false otherwise.
// @throws std::runtime_error if the variable name is missing in a declaration.
// @throws std::runtime_error if '=' is missing in a declaration.
// @return An expression that is the value of the variable.
double declaration(Token_stream& ts, bool is_const);

// @brief Parse assignment expressions.
// @param ts a stream of tokens.
// @throws std::runtime_error if the variable is undefined.
// @return The variable's value.
double assignment(Token_stream& ts);
