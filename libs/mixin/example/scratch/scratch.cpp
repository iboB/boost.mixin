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

class character
{
};

class building
{
};

BOOST_DEFINE_MIXIN(game_object, none);
BOOST_DEFINE_MIXIN(character, none);
BOOST_DEFINE_MIXIN(building, none);

int main()
{
    object obj1;
    mutate(obj1)
        .add<game_object>()
        .add<character>();

    mutate(obj1)
        .remove<character>()
        .add<building>();

    single_object_mutator mutation(obj1);

    mutation.remove<building>();
    mutation.add<character>();

    mutation.apply();

    //mutation.remove

    object_type_template character_template;
    character_template
        .add<game_object>()
        .add<character>()
        .create();

    object monster(character_template);

    //object_type_template 
    

    return 0;
}
