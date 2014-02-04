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

class furniture
{
    int _id;
    string name;
    // ... other common fields
};

class wood_frame
{
};

class metal_frame
{
};

class ofml_serialization
{
};

class xml_serialization
{
};


BOOST_DEFINE_MIXIN(furniture, none);
BOOST_DEFINE_MIXIN(wood_frame, none);
BOOST_DEFINE_MIXIN(metal_frame, none);
BOOST_DEFINE_MIXIN(ofml_serialization, none);
BOOST_DEFINE_MIXIN(xml_serialization, none);

int main()
{
    add_new_mutation_rule(new mandatory_mixin<furniture>);
    add_new_mutation_rule(new deprecated_mixin<ofml_serialization>);

    mutually_exclusive_mixins* rule = new mutually_exclusive_mixins;
    rule->add<wood_frame>();
    rule->add<metal_frame>();
    add_new_mutation_rule(rule);

    object o;

    mutate(o)
        .add<ofml_serialization>()
        .add<xml_serialization>()
        .add<wood_frame>();

    mutate(o)
        .add<metal_frame>();


    return 0;
}
