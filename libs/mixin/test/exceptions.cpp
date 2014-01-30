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
BOOST_DECLARE_MIXIN(has_m1);
BOOST_DECLARE_MIXIN(has_u2m2);
BOOST_DECLARE_MIXIN(has_m2);

BOOST_DECLARE_MIXIN(has_u1m1);
BOOST_DECLARE_MIXIN(has_m1);
BOOST_DECLARE_MIXIN(has_u2m2);
BOOST_DECLARE_MIXIN(has_m2);

BOOST_MIXIN_MULTICAST_MESSAGE_0(int, m1);
BOOST_MIXIN_MULTICAST_MESSAGE_0(int, m2);
BOOST_MIXIN_MESSAGE_0(void, u1);
BOOST_MIXIN_MESSAGE_0(void, u2);

BOOST_AUTO_TEST_CASE(exceptions)
{
    {
        object o;

        mutate(o)
            .add<has_u1m1>()
            .add<has_m1>();

        BOOST_CHECK_NO_THROW(u1(o));
        BOOST_CHECK_NO_THROW(m1(o));
        BOOST_CHECK_NO_THROW(m1<combinators::sum>(o));
        BOOST_CHECK_NO_THROW(m1(o, combinators::sum<int>()));

        BOOST_CHECK_THROW(u2(o), bad_message_call);
        BOOST_CHECK_THROW(m2(o), bad_message_call);
        BOOST_CHECK_THROW(m2<combinators::sum>(o), bad_message_call);
        BOOST_CHECK_THROW(m2(o, combinators::sum<int>()), bad_message_call);
    }
}

class has_u1m1
{
public:
    void u1() {}
    int m1() { return 1; }
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

class has_m2
{
public:
    int m2() { return 4; }
};

BOOST_DEFINE_MIXIN(has_u1m1, u1_msg & m1_msg);
BOOST_DEFINE_MIXIN(has_m1, m1_msg);
BOOST_DEFINE_MIXIN(has_u2m2, u2_msg & m2_msg);
BOOST_DEFINE_MIXIN(has_m2, m2_msg);

BOOST_MIXIN_DEFINE_MESSAGE(m1);
BOOST_MIXIN_DEFINE_MESSAGE(m2);
BOOST_MIXIN_DEFINE_MESSAGE(u1);
BOOST_MIXIN_DEFINE_MESSAGE(u2);
