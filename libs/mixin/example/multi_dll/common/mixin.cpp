//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include "common_pch.hpp"
#include "messages.hpp"
#include "mixin.hpp"

using namespace boost::mixin;
using namespace std;

common_mixin::common_mixin()
    : _id(0)
{
}

void common_mixin::set_id(int id)
{
    _id = id;
}

int common_mixin::get_id() const
{
    return _id;
}

void common_mixin::trace(std::ostream& o) const
{
    o << "object with id " << _id << endl;
}

BOOST_DEFINE_MIXIN(common_mixin, get_id_msg & set_id_msg & priority(1000, trace_msg));