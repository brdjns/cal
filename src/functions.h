// functions.h: Built-in function interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

namespace Cal {

    // @brief Compute a factorial.
    // @pre num is an integer.
    // @post Return a value that is the factorial of num.
    // @param num value to compute factorial of
    double factorial(int num);
}
