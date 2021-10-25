// function.cc: Built-in functions.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "function.h"

// Compute the factorial of num.
double factorial(int num)
{
    if (num == 0) {
        return 1;
    }
    return num * factorial(num - 1);
}
