//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "basic.hpp"
#include "transform.hpp"
#include "system_messages.hpp"
#include "transform_messages.hpp"

using namespace boost::mixin;
using namespace std;

has_transform::has_transform()
    : _transform(0)
{
}

void has_transform::translate(int to)
{
    _transform += to;
}

void has_transform::rotate(int with)
{
    _transform *= with;
}

void has_transform::trace(std::ostream& o) const
{
    o << "\twith transform: " << _transform << endl;
}

BOOST_DEFINE_MIXIN(has_transform, translate_msg & rotate_msg & trace_msg & get_combined_transform_msg)
