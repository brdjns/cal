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
// @return An expression.
double expression();

// @brief Construct a term.
// @pre A factor.
// @return A term.
// @throws std::runtime_error for division by zero.
double term();

// @brief Construct a factor.
// @pre A token that is a number or parentheses.
// @return A factor.
// @throws std::runtime_error if next token is not an expression.
double factor();

// @brief Construct a power expression.
// @pre A factor.
// @return A power expression.
double power_expression();

// @brief Turn a declaration into a statement.
// @return Either a declaration or a statement.
double statement();

// @brief Parse declaration statements.
// @throws std::runtime_error if the variable name is missing in a declaration.
// @throws std::runtime_error if '=' is missing in a declaration.
// @return an expression that is the value of the variable.
double declaration();

// @brief Compute an expression.
// @return An expression.
void calculate();

// @brief Parse assignment expressions.
// @param var a variable identifier.
// @throws std::runtime_error if the variable is undefined.
// @return the variable's value.
double assignment();

// @brief Clean up remaining tokens during an exception.
void clean_up_mess();

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
