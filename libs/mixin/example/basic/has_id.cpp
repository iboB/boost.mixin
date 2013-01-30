//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "basic.hpp"
#include "has_id.hpp"
#include "system_messages.hpp"

using namespace boost::mixin;
using namespace std;

void has_id::trace(std::ostream& o) const
{
    o << "object with id: " << _id << endl;
}

//                                                    set the priority of trace to a big number so it would be called first
BOOST_DEFINE_MIXIN(has_id, get_id_msg & set_id_msg & priority(10, trace_msg))
