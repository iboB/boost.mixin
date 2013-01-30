//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "basic.hpp"
#include "object_manager.hpp"
#include "system_messages.hpp"
#include "transform_messages.hpp"
#include "rendering_messages.hpp"
#include "perf.hpp"

// here we have only messages
// no mixin info whatsoever

using namespace boost::mixin;
using namespace std;

int main()
{
    object_manager m;

    m.create_objects();

    object* o = m.objects()[1];

    trace(o, cout);

    rotate(o, 5);
    render(o, 7);

    cout << endl << "======== changing rendering system ========" << endl << endl;
    m.change_rendering_sytem();

    rotate(o, 0);
    trace(o, cout);
    rotate(o, 3);
    render(o, 2);

    cout << endl << "======== small performance check ========" << endl << endl;

    performance_test_object(o);

    return 0;
}
