//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/mixin.hpp>
#include <iostream>

using namespace std;
using namespace boost::mixin;

class game_object
{
    int _id;
    string name;
    // ... other common fields
};

class opengl_rendering
{
};

class directx_rendering
{
};

BOOST_DEFINE_MIXIN(game_object, none);
BOOST_DEFINE_MIXIN(opengl_rendering, none);
BOOST_DEFINE_MIXIN(directx_rendering, none);

int main()
{
    object obj1;
    mutate(obj1)
        .add<game_object>()
        .add<opengl_rendering>();

    mutate(obj1)
        .remove<opengl_rendering>()
        .add<directx_rendering>();

    single_object_mutator mutation(obj1);

    mutation.remove<directx_rendering>();
    mutation.add<opengl_rendering>();

    mutation.apply();

    mutation.remove<game_object>();
    mutation.cancel();
    mutation.apply(); // no effect

    object_type_template directx_rendering_template;
    directx_rendering_template
        .add<game_object>()
        .add<directx_rendering>()
        .create();

    object obj2(directx_rendering_template);

    object_type_template opengl_rendering_template;
    opengl_rendering_template
        .add<game_object>()
        .add<opengl_rendering>()
        .create();

    opengl_rendering_template.apply_to(obj2);

    same_type_mutator directx;
    directx
        .remove<opengl_rendering>()
        .add<directx_rendering>();

    directx.apply_to(obj1);
    directx.apply_to(obj2);

    return 0;
}
