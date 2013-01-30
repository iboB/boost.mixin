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
#include "has_id_mixin.hpp"
#include "transform_mixin.hpp"
#include "d3d_renderer_mixin.hpp"
#include "gl_renderer_mixin.hpp"

using namespace boost::mixin;


// notice how the manager creates objects without knowing anything but the forward declaration
// of the mixin classes
void object_manager::create_objects()
{
    for(int i=0; i<10; ++i)
    {
        object* o = new object;
        mutate(o)
                .add<has_id>()
                .add<has_transform>()
                .add<d3d_renderer>();

        set_id(o, i);
        _objects.push_back(o);
    }
}

void object_manager::change_rendering_sytem()
{
    for(size_t i=0; i<_objects.size(); ++i)
    {
        object* o = _objects[i];

        mutate(o)
                .remove<d3d_renderer>()
                .add<gl_renderer>();
    }
}
