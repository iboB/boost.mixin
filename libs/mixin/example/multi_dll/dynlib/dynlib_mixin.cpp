//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "dynlib_pch.hpp"
#include "dynlib_mixin.hpp"
#include "../common/messages.hpp"
#include "dynlib_messages.hpp"

using namespace std;

void dynlib_mixin::trace(std::ostream& o) const
{
    o << "\twith dynlib_mixin" << std::endl;
}

void dynlib_mixin::dl_mixin_specific()
{
    cout << "dynlib mixin reporting in" << endl;
}

void dynlib_mixin::dl_multicast()
{
    cout << "dynlib multicast in dynlib mixin" << endl;
}

BOOST_DEFINE_MIXIN(dynlib_mixin, trace_msg & dl_mixin_specific_msg & dl_multicast_msg);
