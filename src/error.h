// error.h: Error reporting and recovery interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

#include "token.h"
#include <string>

// @brief Throw a runtime exception.
// @param msg an error message.
// @throws std::runtime_error when called.
void error(const std::string& msg);

// @brief Throw a runtime exception.
// @param msg an error message.
// @param msg2 a message to append.
// @throws std::runtime_error when called.
void error(const std::string& msg, const std::string& msg2);

// @brief Throw a runtime exception.
// @param msg an error message.
// @param val a character value.
// @throws std::runtime_error when called.
void error(const std::string& msg, int val);

// @brief Clean up remaining tokens during an exception.
// @param ts a stream of tokens.
void cleanup(Token_stream& ts);
