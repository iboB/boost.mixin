//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "common.hpp"
#include "generated.hpp"

#include "custom_alloc_mixin.hpp"

#include <iostream>

using namespace std;
using namespace boost::mixin;

#include "timer.ipp"

const size_t NUM_TO_MUTATE = 1000000;

int main(int argc, char**)
{
    std::vector<object_type_template*> templates;
    create_type_templates(templates);

    const size_t num_tmpl = templates.size();

    vector<object*> objects(num_tmpl);

    for(size_t i=0; i<num_tmpl; ++i)
    {
        objects[i] = new object;
    }

    cout << "Creating " << num_tmpl << " objects (each with a new type): ";
    {
        timer t;

        for(size_t i=0; i<num_tmpl; ++i)
        {
            templates[i]->apply_to(*objects[i]);
        }
    }

    vector<object_type_template*> changable_templates;

    // collect all type templates which create object that
    // would change from such a mutation
    for(size_t i=0; i<num_tmpl; ++i)
    {
        const object* o = objects[i];

        if(!o->has<mixin_1>() || !o->has<mixin_2>() || o->has<mixin_3>())
        {
            changable_templates.push_back(templates[i]);
        }
    }

    // create a huge amount of objects
    vector<object*> to_mutate(NUM_TO_MUTATE);
    for(size_t i=0; i<NUM_TO_MUTATE; ++i)
    {
        to_mutate[i] = new object;
    }
    cout << "Creating " << NUM_TO_MUTATE << " objects of existing types: ";
    {
        timer t;
        for(size_t i=0; i<NUM_TO_MUTATE; ++i)
        {
            changable_templates[i%changable_templates.size()]->apply_to(*to_mutate[i]);
        }
    }

    // mutate them
    cout << "Mutating " << NUM_TO_MUTATE << " objects: ";
    {
        timer t;
        for(size_t i=0; i<NUM_TO_MUTATE; ++i)
        {
            mutate(to_mutate[i])
                .add<mixin_1>()
                .add<mixin_2>()
                .remove<mixin_3>();
        }
    }

    // create 1 million objects of the same type
    object_type_template tmpl;
    tmpl
        .add<mixin_3>()
        .add<mixin_4>()
        .add<mixin_5>()
        .create();

    vector<object*> same_type_objs(NUM_TO_MUTATE);
    for(size_t i=0; i<NUM_TO_MUTATE; ++i)
    {
        same_type_objs[i] = new object(tmpl);
    }

    same_type_mutator stm;
    stm
        .add<mixin_1>()
        .add<mixin_2>()
        .remove<mixin_3>();

    cout << "Mutating " << NUM_TO_MUTATE << " objects of the same type: ";
    {
        timer t;
        for(size_t i=0; i<NUM_TO_MUTATE; ++i)
        {
            stm.apply_to(*same_type_objs[i]);
        }
    }

    cout << "Adding a mixin with custom allocator to " << NUM_TO_MUTATE << " objects: ";
    {
        timer t;
        for(size_t i=0; i<NUM_TO_MUTATE; ++i)
        {
            mutate(to_mutate[i])
                .add<custom_alloc_mixin>();
        }
    }

    same_type_mutator stm_custom_alloc;
    stm_custom_alloc.add<custom_alloc_mixin>();

    cout << "Adding a mixin with custom allocator to " << NUM_TO_MUTATE << " objects of the same type: ";
    {
        timer t;
        for(size_t i=0; i<NUM_TO_MUTATE; ++i)
        {
            stm_custom_alloc.apply_to(*same_type_objs[i]);
        }
    }


    return 0;
}
