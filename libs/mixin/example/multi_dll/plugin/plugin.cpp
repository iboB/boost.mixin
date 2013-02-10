//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <iostream>
#include <boost/mixin.hpp>
#include "../common/messages.hpp"
#include "../dynlib/dynlib_messages.hpp"

using namespace std;
using namespace boost::mixin;

BOOST_DECLARE_MIXIN(plugin_mixin);

extern "C" void modify_object(object* o)
{
    mutate(o).add<plugin_mixin>();
}

class plugin_mixin
{
public:
    void trace(ostream& o) const
    {
        cout << "\twith a plugin mixin" << endl;
    }

    void dl_multicast()
    {
        cout << "dynlib multicast in plugin mixin" << endl;
    }

    void dl_mixin_specific()
    {
        cout << "the plugin has overriden the dynlib specific message" << endl;
    }
};

BOOST_DEFINE_MIXIN(plugin_mixin, trace_msg & priority(10, dl_mixin_specific_msg) & dl_multicast_msg);
