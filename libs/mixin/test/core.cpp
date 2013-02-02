//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE core_boost_mixin_features

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <common/common.hpp>

using namespace boost::mixin;

class other_domain {};

// some mixins and messages
BOOST_DECLARE_MIXIN(counter);

class counter
{
public:
    counter() {}

    void count_uni();
    int get_count();
    void count_multi();

};

BOOST_AUTO_TEST_CASE(simple_mutation)
{
    object* o = new object;

    //BOOST_CHECK(!o->has<counter>());
    //BOOST_CHECK_NULL(o->get<counter>());

    object_transformer mutation(o);

    // check apply mutation
    mutation.add<counter>();
    mutation.apply();

    BOOST_CHECK(o->has<counter>());
    BOOST_CHECK_NOT_NULL(o->get<counter>());


    delete o;
}


BOOST_DEFINE_MIXIN(counter, none);
