// error.cc: Error reporting and recovery.
// SPDX-FileCopyrightText: © 2021-2022 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "error.h"
#include <sstream>
#include <stdexcept>

// @brief Throw a runtime exception.
// @param msg an error message.
// @throws std::runtime_error when called.
void error(const std::string& msg)
{
    throw std::runtime_error(msg);
}

// @brief Throw a runtime exception.
// @param msg an error message.
// @param msg2 a message to append.
// @throws std::runtime_error when called.
void error(const std::string& msg, const std::string& msg2)
{
    error(msg + msg2);
}

// @brief Throw a runtime exception.
// @param msg an error message.
// @param val a character value.
// @throws std::runtime_error when called.
void error(const std::string& msg, int val)
{
    std::ostringstream os;
    os << msg << ": " << val;
    error(os.str());
}

// @brief Clean up remaining tokens during an exception.
// @param ts a stream of tokens.
void cleanup(Token_stream& ts)
{
    ts.ignore(print_tok);
}
