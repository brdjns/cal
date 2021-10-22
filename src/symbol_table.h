// symbol_table.h: Symbol table and variable interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <vector>

// @class Variable
// @brief A variable type.
class Variable {
public:
    std::string name; // a variable identifier
    double value;     // a variable value
    bool is_const;    // true if variable is a constant

    // @brief Construct a variable with a name and value.
    // @param[in] id a variable identifier.
    // @param[in] v a variable value.
    Variable(std::string id, double v) : name{id}, value{v} {}
};

// @class Symbol_table
// @brief A symbol table type.
class Symbol_table {
public:
    std::vector<Variable> var_table; // table of variables

    // @brief Retrieve a variable's value.
    // @param[in] var a variable identifier.
    // @throws std::runtime_error if the variable is undefined.
    // @return the variable's value.
    double get(std::string var);

    // @brief Assign a new value to a variable.
    // @param[in] var a variable identifier.
    // @param[in] val a value.
    // @throws std::runtime_error if the variable is undefined.
    void set(std::string var, double val);

    // @brief Determine if the specified variable is declared.
    // @param[in] var the variable identifier to be tested.
    // @returns true if the variable is declared; false otherwise.
    bool is_declared(std::string var);

    // @brief Add a variable to the symbol table.
    // @param[in] var a variable identifier.
    // @param[in] val a value.
    double declare(std::string var, double val);

    // @brief Construct a symbol table.
    Symbol_table() {}
};
