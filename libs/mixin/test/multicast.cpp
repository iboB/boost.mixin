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

#include <sstream>

using namespace boost::mixin;
using namespace std;

BOOST_DECLARE_MIXIN(a);
BOOST_DECLARE_MIXIN(b);
BOOST_DECLARE_MIXIN(c);
BOOST_DECLARE_MIXIN(d);

BOOST_MIXIN_MULTICAST_MESSAGE_1(void, trace, std::ostream&, out);
BOOST_MIXIN_MULTICAST_MESSAGE_1(void, priority_trace, std::ostream&, out);

BOOST_AUTO_TEST_CASE(different_priority)
{
    object o;

    mutate(o)
        .add<a>()
        .add<b>()
        .add<c>()
        .add<d>();

    ostringstream sout;
    priority_trace(&o, sout);
    BOOST_CHECK_EQUAL(sout.str(), "210-1");
}

BOOST_AUTO_TEST_CASE(same_priority)
{
    object o;

    mutate(o)
        .add<a>()
        .add<b>()
        .add<c>()
        .add<d>();

    ostringstream sout;
    trace(&o, sout);
    BOOST_CHECK_EQUAL(sout.str(), "abcd");
}

class a
{
public:
    void trace(std::ostream& out)
    {
        out << "a";
    }

    void priority_trace(std::ostream& out)
    {
        out << "-1";
    }
};

class b
{
public:
    void trace(std::ostream& out)
    {
        out << "b";
    }

    void priority_trace(std::ostream& out)
    {
        out << "2";
    }
};

class c
{
public:
    void trace(std::ostream& out)
    {
        out << "c";
    }

    void priority_trace(std::ostream& out)
    {
        out << "1";
    }
};

class d
{
public:
    void trace(std::ostream& out)
    {
        out << "d";
    }

    void priority_trace(std::ostream& out)
    {
        out << "0";
    }
};

// this order should be important if the messages aren't sorted by mixin name
BOOST_DEFINE_MIXIN(b, trace_msg & priority(2, priority_trace_msg));
BOOST_DEFINE_MIXIN(a, trace_msg & priority(-1, priority_trace_msg));
BOOST_DEFINE_MIXIN(c, trace_msg & priority(1, priority_trace_msg));
BOOST_DEFINE_MIXIN(d, trace_msg & priority_trace_msg);

BOOST_MIXIN_DEFINE_MESSAGE(trace);
BOOST_MIXIN_DEFINE_MESSAGE(priority_trace);
