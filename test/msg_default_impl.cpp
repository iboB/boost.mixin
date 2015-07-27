//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE boost_mixin_msg_default_impl

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <string>

#include "common/common.hpp"

using namespace boost::mixin;
using namespace std;

BOOST_DECLARE_MIXIN(mix_a);
BOOST_DECLARE_MIXIN(mix_b);
BOOST_DECLARE_MIXIN(mix_c);

BOOST_MIXIN_MESSAGE_0(int, basic);
BOOST_MIXIN_MESSAGE_1(int, basic_1, int, x);
BOOST_MIXIN_MESSAGE_1(void, setn, int, n);

// basic msg with default impl
BOOST_MIXIN_MESSAGE_0(int, basic_def);

// basic msg with default impl but not implemented in any mixin
BOOST_MIXIN_MESSAGE_0(int, basic_def_no_impl);

// basic msg with 1 arg and default impl
BOOST_MIXIN_MESSAGE_1(int, basic_1_def, int, x);

// basic const msg with 2 arg and default impl
BOOST_MIXIN_CONST_MESSAGE_2(int, basic_2_def, int, x, int, y);

// overloaded message with default impl
BOOST_MIXIN_MESSAGE_1_OVERLOAD(basic_2_def_overload, int, basic_2_def, const string&, str);

// basic multicast test
BOOST_MIXIN_CONST_MULTICAST_MESSAGE_1(int, def_multi, int, i);

////////////////////////////////////////////////////////////////////////////////
// tests
BOOST_AUTO_TEST_CASE(basic_msgs)
{
    object o;

    // empty objects don't implement default implementations
    BOOST_CHECK_THROW(basic_def(o), bad_message_call);

    mutate(o)
        .add<mix_a>()
        .add<mix_b>();

    setn(o, 50);

    BOOST_CHECK_EQUAL(1110, basic_def(o));
    BOOST_CHECK_EQUAL(2111, basic_def_no_impl(o));
    BOOST_CHECK_EQUAL(83, basic_1_def(o, 3));

    mutate(o)
        .add<mix_c>();

    BOOST_CHECK_EQUAL(1000, basic_def(o));
    BOOST_CHECK_EQUAL(2001, basic_def_no_impl(o));
    BOOST_CHECK_EQUAL(55, basic_1_def(o, 5));
}

BOOST_AUTO_TEST_CASE(overloads)
{
    object o;

    mutate(o)
        .add<mix_a>()
        .add<mix_b>();

    BOOST_CHECK_EQUAL(50, basic_2_def(o, 5, 10));
    BOOST_CHECK_EQUAL(50, basic_2_def(o, "50"));

    mutate(o)
        .add<mix_c>();

    BOOST_CHECK_EQUAL(15, basic_2_def(o, 5, 10));
    BOOST_CHECK_EQUAL(2, basic_2_def(o, "50"));
}

BOOST_AUTO_TEST_CASE(multi)
{
    object o;

    mutate(o)
        .add<mix_c>();

    BOOST_CHECK_EQUAL(400, def_multi<combinators::sum>(o, 2));

    mutate(o)
        .add<mix_a>()
        .add<mix_b>();

    setn(o, 3);

    BOOST_CHECK_EQUAL(70, def_multi<combinators::sum>(o, 20));
}

////////////////////////////////////////////////////////////////////////////////
// implementations

class mix_a
{
public:
    int basic()
    {
        return 100;
    }

    int def_multi(int n) const
    {
        return n-10;
    }
};

class mix_b
{
public:
    int basic_1(int x)
    {
        return n + x;
    }

    void setn(int nn)
    {
        n = nn;
    }

    int def_multi(int foo) const
    {
        return n*foo;
    }

    int n;
};

class mix_c
{
public:
    int basic_def()
    {
        return 1000;
    }

    int basic_1_def(int x)
    {
        return basic_1(bm_this, x);
    }

    int basic_2_def(int x, int y) const
    {
        return x + y;
    }

    int basic_2_def(const string& str)
    {
        return str.length();
    }
};

BOOST_DEFINE_MIXIN(mix_a, basic_msg & def_multi_msg);
BOOST_DEFINE_MIXIN(mix_b, basic_1_msg & setn_msg & def_multi_msg);
BOOST_DEFINE_MIXIN(mix_c, basic_def_msg & basic_1_def_msg & basic_2_def_msg & basic_2_def_overload_msg);

BOOST_MIXIN_DEFINE_MESSAGE(basic);
BOOST_MIXIN_DEFINE_MESSAGE(basic_1);
BOOST_MIXIN_DEFINE_MESSAGE(setn);

BOOST_MIXIN_DEFINE_MESSAGE_0_WITH_DEFAULT_IMPL(int, basic_def)
{
    return basic(bm_this) + 1010;
}

BOOST_MIXIN_DEFINE_MESSAGE_0_WITH_DEFAULT_IMPL(int, basic_def_no_impl)
{
    return basic_def(bm_this) + 1001;
}

BOOST_MIXIN_DEFINE_MESSAGE_1_WITH_DEFAULT_IMPL(int, basic_1_def, int, x)
{
    return 10 * x + basic_1(bm_this, x);
}

BOOST_MIXIN_DEFINE_MESSAGE_2_WITH_DEFAULT_IMPL(int, basic_2_def, int, x, int, y)
{
    return x * y;
}

BOOST_MIXIN_DEFINE_MESSAGE_1_WITH_DEFAULT_IMPL(int, basic_2_def_overload, const string&, str)
{
    return atoi(str.c_str());
}

BOOST_MIXIN_DEFINE_MESSAGE_1_WITH_DEFAULT_IMPL(int, def_multi, int, i)
{
    return 200 * i;
}
