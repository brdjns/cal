// cal.cc: cal, a portable desk calculator.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "cal.h"
#include "error.h"
#include "parse.h"
#include "symbol_table.h"
#include "token.h"
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
    names.declare("E", 2.71828182845904523536, true);       // e
    names.declare("LOG2E", 1.44269504088896340736, true);   // log2(e)
    names.declare("LOG10E", 0.434294481903251827651, true); // log10(e)
    names.declare("LN2", 0.693147180559945309417, true);    // ln(2)
    names.declare("LN10", 2.30258509299404568402, true);    // ln(10)
    names.declare("PI", 3.14159265358979323846, true);      // pi
    names.declare("PI_2", 1.57079632679489661923, true);    // pi/2
    names.declare("PI_4", 0.785398163397448309616, true);   // pi/4
    names.declare("SQRT2", 1.41421356237309504880, true);   // sqrt(2)

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
    // double holds 10 significant digits.  For cal, we squeeze out 17
    // significant digits to get the most out of our doubles.
    std::cout.precision(std::numeric_limits<double>::max_digits10 + 2);

    const std::string prompt{"> "};

    while (true) try {
            std::cout << prompt;
            Token t{ts.get()};
            while (t.kind == Symbol::print_tok) { // discard all 'print' tokens
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
