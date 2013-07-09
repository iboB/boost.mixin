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

//#if BOOST_MIXIN_USING_CXX11

#include "common/common.hpp"

using namespace boost::mixin;

BOOST_DECLARE_MIXIN(receive_rvalue);

struct movable
{
    int var;

    movable(int v)
    {
        var = v;
    }

    movable(const movable& m)
    {
        var = m.var;
    }

    movable(movable&& m) throw()
    {
        var = m.var;
        m.var = 0;
    }
};

BOOST_MIXIN_MESSAGE_1(void, i_like, movable&&, m);

BOOST_AUTO_TEST_CASE(message_move_semantics)
{
    object to;

    mutate(to).add<receive_rvalue>();

    movable it(17);

    i_like(&to, std::move(it));

    BOOST_CHECK_EQUAL(it.var, 0);
}

class receive_rvalue
{
public:
    void i_like(movable&& m)
    {
        movable here_now = std::move(m);

        BOOST_CHECK_EQUAL(here_now.var, 17);
    }
};

BOOST_DEFINE_MIXIN(receive_rvalue, i_like_msg);

BOOST_MIXIN_DEFINE_MESSAGE(i_like);

//#endif
