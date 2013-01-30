//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include "transform_mixin.hpp"

class has_transform
{
public:
    has_transform();

    int get_combined_transform() const { return _transform; }

    void translate(int to);
    void rotate(int with);

    void trace(std::ostream& o) const;

private:
    int _transform;
};
