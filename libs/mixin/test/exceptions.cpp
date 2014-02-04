//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE boost_mixin_exceptions

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

using namespace boost::mixin;

BOOST_DECLARE_MIXIN(has_u1m1);
BOOST_DECLARE_MIXIN(has_u1);
BOOST_DECLARE_MIXIN(has_m1);
BOOST_DECLARE_MIXIN(has_u2m2);
BOOST_DECLARE_MIXIN(has_u2);
BOOST_DECLARE_MIXIN(has_m2);

BOOST_MIXIN_MULTICAST_MESSAGE_0(int, m1);
BOOST_MIXIN_MULTICAST_MESSAGE_0(int, m2);
BOOST_MIXIN_MESSAGE_0(void, u1);
BOOST_MIXIN_MESSAGE_0(void, u2);

BOOST_AUTO_TEST_CASE(ex_bad_message_call)
{
    object o;

    mutate(o)
        .add<has_u1m1>()
        .add<has_m1>();

    BOOST_CHECK_NO_THROW(u1(o));
    BOOST_CHECK_NO_THROW(m1(o));
    BOOST_CHECK_NO_THROW(m1<combinators::sum>(o));

    combinators::sum<int> sum;
    BOOST_CHECK_NO_THROW(m1(o, sum));

    BOOST_CHECK_THROW(u2(o), bad_message_call);
    BOOST_CHECK_THROW(m2(o), bad_message_call);
    BOOST_CHECK_THROW(m2<combinators::sum>(o), bad_message_call);
    BOOST_CHECK_THROW(m2(o, sum), bad_message_call);
}

BOOST_AUTO_TEST_CASE(ex_bad_mutation_source)
{
    object o1;
    mutate(o1)
        .add<has_u1m1>()
        .add<has_m1>();

    object o2;
    mutate(o2)
        .add<has_u1m1>()
        .add<has_m1>();

    object o3;
    mutate(o3)
        .add<has_u1m1>()
        .add<has_m1>();

    same_type_mutator mut_1_to_2;
    mut_1_to_2
        .remove<has_u1m1>()
        .remove<has_m1>()
        .add<has_u2m2>()
        .add<has_m2>();

    BOOST_CHECK_NO_THROW(mut_1_to_2.apply_to(o1));
    BOOST_CHECK_NO_THROW(mut_1_to_2.apply_to(o2));

    // here o1 and o2 are now type u2m2

    same_type_mutator mut_2_to_1;
    mut_2_to_1
        .remove<has_u2m2>()
        .remove<has_m2>()
        .add<has_u1m1>()
        .add<has_m1>();

    BOOST_CHECK_NO_THROW(mut_2_to_1.apply_to(o1));
    BOOST_CHECK_NO_THROW(mut_2_to_1.apply_to(o2));

    BOOST_CHECK_THROW(mut_2_to_1.apply_to(o3), bad_mutation_source);
}

BOOST_AUTO_TEST_CASE(ex_unicast_clash)
{
    object o;

    single_object_mutator safe(o);
    safe
        .add<has_u1>()
        .add<has_u2m2>();

    BOOST_CHECK_NO_THROW(safe.apply());

    object o2;

    single_object_mutator clash(o2);
    clash
        .add<has_u2>()
        .add<has_u2m2>();

    BOOST_CHECK_THROW(clash.apply(), unicast_clash);

    single_object_mutator clash2(o);
    clash2
        .add<has_u2>();

    BOOST_CHECK_THROW(clash2.apply(), unicast_clash);

    object_type_template safe_t;
    safe_t
        .add<has_u2>()
        .add<has_u1m1>();

    BOOST_CHECK_NO_THROW(safe_t.create());

    object_type_template clash_t;
    clash_t
        .add<has_u2>()
        .add<has_u2m2>();

    BOOST_CHECK_THROW(clash_t.create(), unicast_clash);
}

class has_u1m1
{
public:
    void u1() {}
    int m1() { return 1; }
};

class has_u1
{
public:
    void u1() {}
};

class has_m1
{
public:
    int m1() { return 2; }
};

class has_u2m2
{
public:
    void u2() {}
    int m2() { return 3; }
};

class has_u2
{
public:
    void u2() {}
};

class has_m2
{
public:
    int m2() { return 4; }
};

BOOST_DEFINE_MIXIN(has_u1m1, u1_msg & m1_msg);
BOOST_DEFINE_MIXIN(has_m1, m1_msg);
BOOST_DEFINE_MIXIN(has_u1, u1_msg);
BOOST_DEFINE_MIXIN(has_u2m2, u2_msg & m2_msg);
BOOST_DEFINE_MIXIN(has_m2, m2_msg);
BOOST_DEFINE_MIXIN(has_u2, u2_msg);

BOOST_MIXIN_DEFINE_MESSAGE(m1);
BOOST_MIXIN_DEFINE_MESSAGE(m2);
BOOST_MIXIN_DEFINE_MESSAGE(u1);
BOOST_MIXIN_DEFINE_MESSAGE(u2);
