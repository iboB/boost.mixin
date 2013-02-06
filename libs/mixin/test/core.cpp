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

#include "common/common.hpp"

using namespace boost::mixin;

class other_domain {};

// some mixins and messages
BOOST_DECLARE_MIXIN(counter);
//BOOST_DECLARE_MIXIN(overrider);
//BOOST_DECLARE_MIXIN(foo);
//BOOST_DECLARE_MIXIN(bar);

class counter
{
public:
    counter() {}

    void count_uni();
    int get_count();
    void count_multi();
};


BOOST_AUTO_TEST_CASE(simple_inline_mutation)
{
    object o;

    BOOST_CHECK(!o.has<counter>());
    BOOST_CHECK_NULL(o.get<counter>());

    mutate(o).add<counter>();

    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());

    mutate(o).remove<counter>();

    BOOST_CHECK(!o.has<counter>());
    BOOST_CHECK_NULL(o.get<counter>());
}

BOOST_AUTO_TEST_CASE(complex_apply_mutation)
{
    object o;

    BOOST_CHECK(!o.has<counter>());
    BOOST_CHECK_NULL(o.get<counter>());

    object_transformer mutation(o);

    mutation.add<counter>();
    mutation.apply();

    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());

    mutation.remove<counter>();
    mutation.cancel();
    mutation.apply();
    // cancelled mutations should do nothing
    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());

    mutation.add<counter>();
    mutation.remove<counter>();
    mutation.apply();
    // adding and removing the same thing should do nothing
    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());

    mutation.remove<counter>();
    mutation.apply();
    BOOST_CHECK(!o.has<counter>());
    BOOST_CHECK_NULL(o.get<counter>());
}


BOOST_DEFINE_MIXIN(counter, none);
