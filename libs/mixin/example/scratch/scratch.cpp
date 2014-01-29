//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/mixin.hpp>
#include <iostream>

using namespace boost::mixin;
using namespace std;

class test_dom;

BOOST_DECLARE_MIXIN(a);
BOOST_DECLARE_MIXIN(b);
BOOST_DECLARE_MIXIN(c);
BOOST_DECLARE_MIXIN(d);

BOOST_MIXIN_MULTICAST_MESSAGE_1(void, trace, std::ostream&, out);

int main()
{
    object o;

    mutate(o)
        .add<a>()
        .add<b>();

    trace(o, cout);

    object o2;

    mutate(o2)
        .add<c>()
        .add<d>();

    trace(o2, cout);
    
    cout << endl;

    return 0;
}

class a
{
public:
    void trace(std::ostream& out)
    {
        out << "a";
    }
};

class b
{
public:
    void trace(std::ostream& out)
    {
        out << "b";
    }
};

class c
{
public:
    void trace(std::ostream& out)
    {
        out << "c";
    }
};

class d
{
public:
    void trace(std::ostream& out)
    {
        out << "d";
    }
};

// this order should be important if the messages aren't sorted by mixin name
BOOST_DEFINE_MIXIN(b, trace_msg);
BOOST_DEFINE_MIXIN(a, trace_msg);
BOOST_DEFINE_MIXIN_IN_DOMAIN(test_dom, c, none);
BOOST_DEFINE_MIXIN_IN_DOMAIN(test_dom, d, none);

BOOST_MIXIN_DEFINE_MESSAGE(trace);