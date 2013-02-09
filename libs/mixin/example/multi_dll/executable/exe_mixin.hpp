//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include "exe_mixin_fwd.hpp"

class exe_mixin
{
public:
    void trace(std::ostream& o) const;

    void dl_exported();
    void dl_multicast();
};