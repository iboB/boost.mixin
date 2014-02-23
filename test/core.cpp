//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
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

// some mixins and messages
BOOST_DECLARE_MIXIN(counter);
BOOST_DECLARE_MIXIN(no_messages);
BOOST_DECLARE_MIXIN(type_checker);
//BOOST_DECLARE_MIXIN(overrider);
//BOOST_DECLARE_MIXIN(foo);
//BOOST_DECLARE_MIXIN(bar);

BOOST_MIXIN_MESSAGE_0(void, dummy);
BOOST_MIXIN_CONST_MESSAGE_0(const void*, get_self);

class no_messages
{
};

class counter
{
public:
    counter()
        : _count(0)
    {}

    void dummy() {}

    void count_uni() { ++_count; }
    void count_multi();

    int get_count() const { return _count; }
private:
    int _count;
};

class type_checker
{
public:
    const void* get_self() const
    {
        return this;
    }
};


BOOST_AUTO_TEST_CASE(simple_inline_mutation)
{
    object o;

    BOOST_CHECK(!o.has<no_messages>());
    BOOST_CHECK_NULL(o.get<no_messages>());

    mutate(o).add<no_messages>();

    BOOST_CHECK(o.has<no_messages>());
    BOOST_CHECK_NOT_NULL(o.get<no_messages>());

    mutate(o).remove<no_messages>();

    BOOST_CHECK(!o.has<no_messages>());
    BOOST_CHECK_NULL(o.get<no_messages>());
}

BOOST_AUTO_TEST_CASE(basic_message)
{
    object o;
    mutate(o).add<type_checker>();

    BOOST_CHECK_EQUAL(get_self(o), o.get<type_checker>());

    // works as ptr too
    BOOST_CHECK_EQUAL(get_self(&o), o.get<type_checker>());
}

BOOST_AUTO_TEST_CASE(complex_apply_mutation)
{
    object o;

    BOOST_CHECK(!o.has<counter>());
    BOOST_CHECK_NULL(o.get<counter>());
    BOOST_CHECK(!o.implements(dummy_msg));

    single_object_mutator mutation(o);

    mutation.add<no_messages>();
    mutation.apply();
    BOOST_CHECK(o.has<no_messages>());
    BOOST_CHECK_NOT_NULL(o.get<no_messages>());
    BOOST_CHECK(!o.implements(dummy_msg));

    mutation.add<counter>();
    mutation.apply();

    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());
    BOOST_CHECK(o.implements(dummy_msg));
    BOOST_CHECK_EQUAL(o.get<counter>()->get_count(), 0);
    o.get<counter>()->count_uni();
    BOOST_CHECK_EQUAL(o.get<counter>()->get_count(), 1);

    mutation.remove<counter>();
    mutation.cancel();
    mutation.apply();
    // cancelled mutations should do nothing
    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());
    BOOST_CHECK_EQUAL(o.get<counter>()->get_count(), 1);

    mutation.add<counter>();
    mutation.remove<counter>();
    mutation.apply();
    // adding and removing the same thing should do nothing
    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());
    BOOST_CHECK_EQUAL(o.get<counter>()->get_count(), 1);

    mutation.add<counter>();
    mutation.apply();
    // adding something that's there should be fine
    BOOST_CHECK(o.has<counter>());
    BOOST_CHECK_NOT_NULL(o.get<counter>());
    // adding something that's already there shouldn't recreate the mixin
    BOOST_CHECK_EQUAL(o.get<counter>()->get_count(), 1);

    mutation.remove<counter>();
    mutation.apply();
    BOOST_CHECK(!o.has<counter>());
    BOOST_CHECK_NULL(o.get<counter>());

    BOOST_CHECK(!o.implements(dummy_msg));
}

BOOST_AUTO_TEST_CASE(type_template)
{
    object_type_template type;

    type
        .add<counter>()
        .add<no_messages>()
        .create();

    object o1(type);
    BOOST_CHECK(o1.has<no_messages>());
    BOOST_CHECK_NOT_NULL(o1.get<no_messages>());
    BOOST_CHECK(o1.has<counter>());
    BOOST_CHECK_NOT_NULL(o1.get<counter>());
    BOOST_CHECK(o1.implements(dummy_msg));

    object o2;
    type.apply_to(o2);
    BOOST_CHECK(o2.has<no_messages>());
    BOOST_CHECK_NOT_NULL(o2.get<no_messages>());
    BOOST_CHECK(o2.has<counter>());
    BOOST_CHECK_NOT_NULL(o2.get<counter>());
    BOOST_CHECK(o2.implements(dummy_msg));

    object o3;
    mutate(o3)
        .add<counter>();
    o3.get<counter>()->count_uni();
    BOOST_CHECK_EQUAL(o3.get<counter>()->get_count(), 1);
    type.apply_to(o3);
    // applying a type template should reset the object
    BOOST_CHECK_EQUAL(o3.get<counter>()->get_count(), 0);
}

BOOST_DEFINE_MIXIN(no_messages, none);
BOOST_DEFINE_MIXIN(counter, dummy_msg);
BOOST_DEFINE_MIXIN(type_checker, get_self_msg);

BOOST_MIXIN_DEFINE_MESSAGE(dummy);
BOOST_MIXIN_DEFINE_MESSAGE(get_self);
