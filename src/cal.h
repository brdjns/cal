// cal.h: Main interface.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace Cal {

    /// Cast a wider type to a narrower type.
    template<class R, class A>
    R narrow_cast(const A& a)
    {
        R r = R(a);
        if (A(r) != a)
            throw std::runtime_error("narrow_cast<>() failed");
        return r;
    }
}
