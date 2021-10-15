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

namespace Cal {

    /// @brief Cast a wider type to a narrower type.
    template<class R, class A>
    R narrow_cast(const A& a)
    {
        R r = R(a);
        if (A(r) != a)
            throw std::runtime_error("narrow_cast<>() failed");
        return r;
    }

    // @brief Construct an expression.
    // @pre A term.
    // @return An expression.
    double expression();

    // @brief Construct a term.
    // @pre A factor.
    // @return A term.
    // @throws runtime_error for division by zero.
    double term();

    // @brief Construct a factor.
    // @pre A token that is a number or parentheses.
    // @return A factor.
    // @throws runtime_error if next token is not an expression.
    double factor();

    // @brief Construct a postfix expression.
    // @pre A factor.
    // @return A postfix expression.
    double postfix_expression();

    // @brief Turn a declaration into a statement.
    // @return Either a declaration or a statement.
    double statement();

    // @brief Declare a variable.
    // @throws runtime_error if the variable name is missing in a declaration.
    // @throws runtime_error if '=' is missing in a declaration.
    // @return an expression that is the value of the variable.
    double declaration();

    // @brief Add a variable to the symbol table.
    // @param var a variable.
    // @param val the variable's value.
    // @throws runtime_error if the variable is already declared.
    // @return the variable's value.
    double define_name(std::string var, double val);

    // @brief Bind a value to an identifier.
    // @param str a variable identifier string.
    // @param val a numeric value.
    // @throws std::runtume_error if str is undefined.
    void set_value(std::string str, double val);

    // @brief Retrieve a variable's value.
    // @param str a variable identifier.
    // @throws runtume_error if the variable is undefined.
    // @return the variable's value.
    double get_value(std::string str);

    // @brief Compute an expression.
    // @return An expression.
    void calculate();

    // @brief Clean up remaining tokens during an exception.
    void clean_up_mess();

    // @brief Determine if the specified variable is declared.
    // @param str the variable identifier to be tested.
    // @return true if the variable is declared; false otherwise.
    bool is_declared(std::string str);

    // @brief Throw a runtime exception.
    // @param str an error message.
    // @throws runtime_error when called.
    inline void error(const std::string& str) { throw std::runtime_error(str); }

    // @brief Throw a runtime exception.
    // @param str an error message.
    // @param str2 an error message to append to str.
    // @throws runtime_error when called.
    inline void error(const std::string& str, const std::string& str2)
    {
        error(str + str2);
    }

    // @brief Throw a runtime exception.
    // @param str an error message.
    // @param i a character value.
    // @throws runtime_error when called.
    inline void error(const std::string& str, int i)
    {
        std::ostringstream os;
        os << str << ": " << i;
        error(os.str());
    }

}
