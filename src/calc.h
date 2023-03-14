// calc.h: Computation interface.
// SPDX-FileCopyrightText: © 2021-2022 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "token.h"

// @brief Compute an expression.
// @param ts a stream of tokens.
// @return An expression.
void compute(Token_stream& ts);
