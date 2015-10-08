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

#include <algorithm>

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

BOOST_AUTO_TEST_CASE(mixin_ids)
{
    using internal::domain;
    domain& di = domain::instance();

    mixin_id aid = di.get_mixin_id_by_name("mixin_a");
    mixin_id omid = di.get_mixin_id_by_name("other_mixin");
    mixin_id tid = di.get_mixin_id_by_name("third");
    mixin_id uid = di.get_mixin_id_by_name("unused");

    int paths[] = {aid, omid, tid, uid};
    std::sort(paths, paths + 4);
    BOOST_CHECK_EQUAL(paths[0], 0);
    BOOST_CHECK_EQUAL(paths[1], 1);
    BOOST_CHECK_EQUAL(paths[2], 2);
    BOOST_CHECK_EQUAL(paths[3], 3);

    mixin_id invalid = di.get_mixin_id_by_name("asdfasdf");
    BOOST_CHECK_EQUAL(invalid, INVALID_MIXIN_ID);

    invalid = di.get_mixin_id_by_name("MIXIN_A");
    BOOST_CHECK_EQUAL(invalid, INVALID_MIXIN_ID);

    object o;
    single_object_mutator mut(o);

    BOOST_CHECK_NO_THROW(mut.add(aid));
    BOOST_CHECK_NO_THROW(mut.add(omid));
    BOOST_CHECK_NO_THROW(mut.add(tid));

    BOOST_CHECK_THROW(mut.add(invalid), bad_mutation);
    BOOST_CHECK_THROW(mut.add(1234), bad_mutation);

    mut.apply();

    BOOST_CHECK(o.has<mixin_a>());
    BOOST_CHECK(o.has<other_mixin>());
    BOOST_CHECK(o.has<third>());
    BOOST_CHECK(!o.has<unused>());
}

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