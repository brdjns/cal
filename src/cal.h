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
    double power_expression();

    // @brief Turn a declaration into a statement.
    // @return Either a declaration or a statement.
    double statement();

    // @brief Declare a variable.
    // @throws runtime_error if the variable name is missing in a declaration.
    // @throws runtime_error if '=' is missing in a declaration.
    // @return an expression that is the value of the variable.
    double declaration();

    // @brief Add a variable to the symbol table.
    // @param variable a variable.
    // @param value the variable's value.
    // @throws runtime_error if the variable is already declared.
    // @return the variable's value.
    double define_name(std::string variable, double value);

    // @brief Bind a value to an identifier.
    // @param variable a variable identifier string.
    // @param value a numeric value.
    // @throws std::runtume_error if str is undefined.
    void set_value(std::string variable, double value);

    // @brief Retrieve a variable's value.
    // @param variable a variable identifier.
    // @throws runtume_error if the variable is undefined.
    // @return the variable's value.
    double get_value(std::string variable);

    // @brief Compute an expression.
    // @return An expression.
    void calculate();

    // @brief Clean up remaining tokens during an exception.
    void clean_up_mess();

    // @brief Determine if the specified variable is declared.
    // @param variable the variable identifier to be tested.
    // @return true if the variable is declared; false otherwise.
    bool is_declared(std::string variable);

    // @brief Throw a runtime exception.
    // @param message an error message.
    // @throws runtime_error when called.
    inline void error(const std::string& message)
    {
        throw std::runtime_error(message);
    }

    // @brief Throw a runtime exception.
    // @param message an error message.
    // @param message2 a string to append to the error message.
    // @throws runtime_error when called.
    inline void error(const std::string& message, const std::string& message2)
    {
        error(message + message2);
    }

    // @brief Throw a runtime exception.
    // @param message an error message.
    // @param value a character value.
    // @throws runtime_error when called.
    inline void error(const std::string& message, int value)
    {
        std::ostringstream os;
        os << message << ": " << value;
        error(os.str());
    }

}
