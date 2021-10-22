// symbol_table.cc: Symbol table and variable management.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "symbol_table.h"
#include "cal.h"

static Symbol_table names;

// Retrieve a variable's value.
double Symbol_table::get(std::string var)
{
    for (Variable& i : names.var_table) {
        if (i.name == var) {
            return i.value;
        }
    }
    error(var, " is undefined");
    return 1;
}

// Assign a new value to a variable.
void Symbol_table::set(std::string var, double val)
{
    for (Variable& i : names.var_table) {
        if (i.name == var) {
            i.value = val;
            return;
        }
    }
    error(var, " is undefined");
}

// Determine if the specified variable is declared.
bool Symbol_table::is_declared(std::string var)
{
    for (Variable& i : names.var_table) {
        if (i.name == var) {
            return true;
        }
    }
    return false;
}

// Add a variable to the symbol table.
double Symbol_table::declare(std::string var, double val)
{
    if (names.is_declared(var)) {
        error(var, " is defined");
    }
    names.var_table.push_back(Variable{var, val});
    return val;
}
