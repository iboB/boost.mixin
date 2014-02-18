//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#define BOOST_TEST_MODULE cxx11_boost_mixin_features

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#if BOOST_MIXIN_USING_CXX11

#include "common/common.hpp"

using namespace boost::mixin;

class a
{
public:
    a() : val(10) {}
    int val;

    int value() const  { return val; }

    object* get_obj_a() { return bm_this; }
};

class b
{
public:
    b() : val(1) {}
    int val;

    int value() const { return val; }

    const object* get_obj_b() const { return bm_this; }
};

BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(int, value);
BOOST_MIXIN_MESSAGE_0(object*, get_obj_a);
BOOST_MIXIN_CONST_MESSAGE_0(const object*, get_obj_b);

BOOST_DEFINE_MIXIN(a, value_msg & get_obj_a_msg);
BOOST_DEFINE_MIXIN(b, value_msg & get_obj_b_msg);

BOOST_MIXIN_DEFINE_MESSAGE(value);
BOOST_MIXIN_DEFINE_MESSAGE(get_obj_a);
BOOST_MIXIN_DEFINE_MESSAGE(get_obj_b);

object gen_obj()
{
    object obj;
    mutate(obj)
        .add<a>()
        .add<b>();

    return std::move(obj);
}

BOOST_AUTO_TEST_CASE(obj_move_ctor)
{
    object empty;

    object o1 = std::move(empty); // to check that this won't crash
    mutate(o1)
        .add<a>()
        .add<b>();

    // sanity
    BOOST_CHECK_EQUAL(&o1, get_obj_a(o1));
    BOOST_CHECK_EQUAL(&o1, get_obj_b(o1));
    BOOST_CHECK_EQUAL(11, value<combinators::sum>(o1));

    // actual tests
    object o1_moved = std::move(o1);

    BOOST_CHECK_EQUAL(&o1_moved, get_obj_a(o1_moved));
    BOOST_CHECK_EQUAL(&o1_moved, get_obj_b(o1_moved));
    BOOST_CHECK_EQUAL(11, value<combinators::sum>(o1_moved));

    BOOST_CHECK(!o1.has<a>());
    BOOST_CHECK(!o1.has<b>());
    BOOST_CHECK(!o1.implements(value_msg));
    BOOST_CHECK(!o1.implements(get_obj_a_msg));
    BOOST_CHECK(!o1.implements(get_obj_b_msg));

    object o2 = gen_obj();

    BOOST_CHECK_EQUAL(&o2, get_obj_a(o2));
    BOOST_CHECK_EQUAL(&o2, get_obj_b(o2));
    BOOST_CHECK_EQUAL(11, value<combinators::sum>(o2));

    std::vector<object> v;

    v.push_back(std::move(o1_moved));
    assert(v.capacity() == 1);

    v.push_back(std::move(o2));

    BOOST_CHECK_EQUAL(&v.front(), get_obj_a(v.front()));
    BOOST_CHECK_EQUAL(&v.front(), get_obj_b(v.front()));
    BOOST_CHECK_EQUAL(11, value<combinators::sum>(v.front()));

    BOOST_CHECK_EQUAL(&v.back(), get_obj_a(v.back()));
    BOOST_CHECK_EQUAL(&v.back(), get_obj_b(v.back()));
    BOOST_CHECK_EQUAL(11, value<combinators::sum>(v.back()));
}

#endif

