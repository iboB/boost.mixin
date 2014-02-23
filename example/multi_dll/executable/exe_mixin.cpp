//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "executable_pch.hpp"
#include "exe_mixin.hpp"
#include "../common/messages.hpp"
#include "../dynlib/dynlib_messages.hpp"

using namespace std;

void exe_mixin::trace(std::ostream& o) const
{
    o << "\twith exe_mixin" << endl;
}

void exe_mixin::dl_exported()
{
    cout << "calling a message defined in a dynlib over local exe_mixin" << endl;
}

void exe_mixin::dl_multicast()
{
    cout << "dynlib multicast in exe mixin" << endl;
}

BOOST_DEFINE_MIXIN(exe_mixin, trace_msg & dl_exported_msg & dl_multicast_msg);
