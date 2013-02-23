//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE boost_deprecated_mixin_mutation_rule

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include "test_mixins.hpp"

using namespace boost::mixin;

// simple dependency rule
class custom_rule : public mutation_rule
{
public:
    void apply_to(object_type_mutation& mutation)
    {
        if(mutation.is_adding<a>())
        {
            mutation.start_adding<b>();
        }
        else if(mutation.is_removing<a>())
        {
            mutation.start_removing<b>();
        }
    }
};

BOOST_AUTO_TEST_CASE(the_test)
{
    add_new_mutation_rule(new custom_rule());

    object o;

    mutate(o)
        .add<a>()
        .add<c>();

    BOOST_CHECK(o.has<a>());
    BOOST_CHECK(o.has<b>());
    BOOST_CHECK(o.has<c>());

    mutate(o)
        .remove<a>();

    BOOST_CHECK(!o.has<a>());
    BOOST_CHECK(!o.has<b>());
    BOOST_CHECK(o.has<c>());
}
