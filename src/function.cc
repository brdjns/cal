// function.cc: Built-in functions.
// SPDX-FileCopyrightText: © 2021-2022 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "function.h"

// Compute the factorial of num.
double fn_factorial(int num)
{
    if (num == 0) {
        return 1;
    }
    return num * fn_factorial(num - 1);
}
