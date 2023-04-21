// calc.cc: calc, a portable desk calculator.
// SPDX-FileCopyrightText: © 2021-2022 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "calc.h"
#include "error.h"
#include "parse.h"
#include "symbol_table.h"
#include "token.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

static Symbol_table names;

int main(int argc, char* argv[])
try {
    // Load predefined constants: these are constants in the sense that they
    // cannot be assigned to. These constants are based on the non-standard
    // 'M_*' macro constants available under <cmath> and <math.h> in many C and
    // C++ implementations.
    names.declare("E", Constant::e, true);
    names.declare("LOG2E", Constant::log2e, true);
    names.declare("LOG10E", Constant::log10e, true);
    names.declare("LN2", Constant::ln2, true);
    names.declare("LN10", Constant::ln10, true);
    names.declare("PI", Constant::pi, true);
    names.declare("PI_2", Constant::pi_2, true);
    names.declare("PI_4", Constant::pi_4, true);
    names.declare("SQRT2", Constant::sqrt2, true);

    Token_stream ts;
    compute(ts);
    return EXIT_SUCCESS;
}
catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}
catch (...) {
    std::cerr << "exception\n";
    return EXIT_FAILURE;
}

// Compute an expression.
void compute(Token_stream& ts)
{
    // Get the greatest available precision from a double: ordinarily a two-word
    // double holds 10 significant digits.  For calc, we squeeze out 17
    // significant digits to get the most out of our doubles.
    std::cout.precision(std::numeric_limits<double>::max_digits10 + 2);

    const std::string prompt{"> "};

    for (;;) try {
            std::cout << prompt;
            Token t{ts.get()};
            for (; t.kind == Symbol::print_tok;) { // discard all 'print' tokens
                t = ts.get();
            }
            if (t.kind == Symbol::quit_tok) {
                return;
            }
            ts.putback(t);
            std::cout << statement(ts) << '\n';
        }
        catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << '\n';
            cleanup(ts);
        }
}
