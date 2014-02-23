//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include "has_id_mixin.hpp"

class has_id
{
public:
    int get_id() const { return _id; }
    void set_id(int id) { _id = id; }

    void trace(std::ostream& o) const;

private:
    int _id;
};
