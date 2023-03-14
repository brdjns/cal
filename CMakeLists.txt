# CMakeLists.txt: Build settings for cal.
# SPDX-FileCopyrightText: © 2021-2022 Bradley M. Jones <brdjns@gmx.us>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.8)

project("calc")

# Add source to this project's executable.
add_executable(
    calc 
    "src/calc.cc"
    "src/parse.cc" 
    "src/error.cc"
    "src/function.cc" 
    "src/symbol_table.cc"
    "src/token.cc"
    )
