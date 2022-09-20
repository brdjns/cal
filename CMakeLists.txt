# CMakeLists.txt: Build settings for cal.
# SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.8)

project("cal")

# Add source to this project's executable.
add_executable(
    cal 
    "src/cal.cc"
    "src/parse.cc" 
    "src/error.cc"
    "src/function.cc" 
    "src/symbol_table.cc"
    "src/token.cc"
    )

# TODO: Add tests and install targets if needed.
install(TARGETS cal DESTINATION "cal/bin")
