// cal.h: Main interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <string>
#include <cctype>

namespace Cal {

    /// Cast a wider type to a narrower type.
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
    // @post Return an expression.
    double expression();

    // @brief Construct a term.
    // @pre A factor.
    // @post Return a term.
    // @throws runtime_error for division by zero.
    double term();

    // @brief Construct a factor.
    // @pre A token that is a number or parentheses.
    // @post Return a factor.
    // @throws runtime_error if next token is not an expression.
    double factor();

    // @brief Construct a unary expression.
    // @pre A factor.
    // @post Return a unary expression.
    double postfix_expression();

    double statement();

    double declaration();

    double define_name(std::string var, double val);

    void set_value(std::string str, double val);

    double get_value(std::string str);

    void calculate();

    void clean_up_mess();

    bool is_declared(std::string str);

    void ignore(char ch);

}
