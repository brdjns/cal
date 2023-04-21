// calc.h: Computation interface.
// SPDX-FileCopyrightText: © 2021-2022 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "token.h"

// @brief Constants.
namespace Constant {
    constexpr double e = 2.71828182845904523536;       // e
    constexpr double log2e = 1.44269504088896340736;   // log2(e)
    constexpr double log10e = 0.434294481903251827651; // log10(e)
    constexpr double ln2 = 0.693147180559945309417;    // ln(2)
    constexpr double ln10 = 2.30258509299404568402;    // ln(10)
    constexpr double pi = 3.14159265358979323846;      // pi
    constexpr double pi_2 = 1.57079632679489661923;    // pi/2
    constexpr double pi_4 = 0.785398163397448309616;   // pi/4
    constexpr double sqrt2 = 1.41421356237309504880;   // sqrt(2)
}

// @brief Compute an expression.
// @param ts a stream of tokens.
// @return An expression.
void compute(Token_stream& ts);
