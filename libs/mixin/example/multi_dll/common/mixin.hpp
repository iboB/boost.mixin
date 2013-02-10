//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include "mixin_fwd.hpp"

class common_mixin
{
public:
    common_mixin();

    void trace(std::ostream& o) const;

    void set_id(int id);
    int get_id() const;

private:
    int _id;
};
