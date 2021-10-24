// cal.h: Main interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "token.h"

/// @brief Cast a wider type to a narrower type.
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
// @throws std::runtime_error if the variable name is missing in a declaration.
// @throws std::runtime_error if '=' is missing in a declaration.
// @return an expression that is the value of the variable.
double declaration(Token_stream& ts);

// @brief Compute an expression.
// @param ts a stream of tokens.
// @return An expression.
void calculate(Token_stream& ts);

// @brief Parse assignment expressions.
// @param ts a stream of tokens.
// @throws std::runtime_error if the variable is undefined.
// @return the variable's value.
double assignment(Token_stream& ts);

// @brief Clean up remaining tokens during an exception.
// @param ts a stream of tokens.
void clean_up_mess(Token_stream& ts);

// @brief Throw a runtime exception.
// @param msg an error message.
// @throws std::runtime_error when called.
inline void error(const std::string& msg)
{
    throw std::runtime_error(msg);
}

// @brief Throw a runtime exception.
// @param msg an error message.
// @param msg2 a message to append.
// @throws std::runtime_error when called.
inline void error(const std::string& msg, const std::string& msg2)
{
    error(msg + msg2);
}

// @brief Throw a runtime exception.
// @param msg an error message.
// @param val a character value.
// @throws std::runtime_error when called.
inline void error(const std::string& msg, int val)
{
    std::ostringstream os;
    os << msg << ": " << val;
    error(os.str());
}
