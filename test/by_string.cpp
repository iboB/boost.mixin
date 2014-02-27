//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE boost_mixin_string_ops

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include "common/common.hpp"

using namespace boost::mixin;

class mixin_a {};
BOOST_DEFINE_MIXIN(mixin_a, none);

class other_mixin {};
BOOST_DEFINE_MIXIN(other_mixin, none);

class third {};
BOOST_DEFINE_MIXIN(third, none);

class unused {};
BOOST_DEFINE_MIXIN(unused, none);

BOOST_AUTO_TEST_CASE(mixin_names)
{
    object_type_template tmpl;

    BOOST_CHECK(tmpl.add("mixin_a"));
    BOOST_CHECK(tmpl.add("other_mixin"));
    BOOST_CHECK(tmpl.add("third"));

    BOOST_CHECK(!tmpl.add("asdf"));
    BOOST_CHECK(!tmpl.add("mixin_axxx"));
    BOOST_CHECK(!tmpl.add("MIXIN_A"));

    tmpl.create();

    object o(tmpl);

    BOOST_CHECK(o.has<mixin_a>());
    BOOST_CHECK(o.has<other_mixin>());
    BOOST_CHECK(o.has<third>());
    BOOST_CHECK(!o.has<unused>());

    single_object_mutator mutator(o);

    BOOST_CHECK(mutator.remove("unused"));
    BOOST_CHECK(mutator.remove("other_mixin"));
    BOOST_CHECK(mutator.remove("third"));

    BOOST_CHECK(!mutator.remove("other_mixinxxx"));
    BOOST_CHECK(!mutator.remove("OTHER_MIXIN"));

    mutator.apply();

    BOOST_CHECK(o.has<mixin_a>());
    BOOST_CHECK(!o.has<other_mixin>());
    BOOST_CHECK(!o.has<third>());
    BOOST_CHECK(!o.has<unused>());
}