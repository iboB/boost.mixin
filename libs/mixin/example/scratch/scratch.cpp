//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/mixin.hpp>
#include <iostream>

using namespace boost::mixin;
using namespace std;

BOOST_DECLARE_MIXIN(has_unio1_multio1);
BOOST_DECLARE_MIXIN(has_unio2_multio2);
BOOST_DECLARE_MIXIN(has_unio3_multio1);
BOOST_DECLARE_MIXIN(has_unio4_multio2);

BOOST_MIXIN_MULTICAST_MESSAGE_1_OVERLOAD(multi_1_overload_1, int, multi, int&, out);
BOOST_MIXIN_MULTICAST_MESSAGE_2_OVERLOAD(multi_1_overload_2, int, multi, int&, out, int, a1);

BOOST_MIXIN_MESSAGE_0_OVERLOAD(uni_1_overload_1, int, uni);
BOOST_MIXIN_MESSAGE_1_OVERLOAD(uni_1_overload_2, int, uni, int, a1);
BOOST_MIXIN_MESSAGE_2_OVERLOAD(uni_1_overload_3, int, uni, int, a1, int, a2);
BOOST_MIXIN_MESSAGE_3_OVERLOAD(uni_1_overload_4, int, uni, int, a1, int, a2, int, a3);

int main()
{
    object o;
    mutate(o)
        .add<has_unio1_multio1>()
        .add<has_unio2_multio2>()
        .add<has_unio3_multio1>()
        .add<has_unio4_multio2>();

    int a1 = 1, a2 = 2, a3 = 3;

    cout << uni(o) << endl;
    cout << uni(o, a1) << endl;
    cout << uni(o, a1, a2) << endl;
    cout << uni(o, a1, a2, a3) << endl;

    int out = 0;

    multi(o, out);
    cout << out << endl;
    out = 0;

    multi(o, out, a1);
    cout << out << endl;
    out = 0;

    combinators::sum<int> s;
    
    multi(o, out, s);
    cout << s.result() << endl;
    s.reset();
    out = 0;

    multi(o, out, a1, s);
    cout << s.result() << endl;

    return 0;
}

class has_unio1_multio1
{
public:
    int uni() { return 0; }
    int multi(int& out) { return out += 0; }
};

class has_unio2_multio2
{
public:
    int uni(int a) { return a; }
    int multi(int& out, int a) { return out += a; }
};

class has_unio3_multio1
{
public:
    int uni(int a1, int a2) { return a1 + a2; }
    int multi(int& out) { return out += 1; }
};

class has_unio4_multio2
{
public:
    int uni(int a1, int a2, int a3) { return a1 + a2 + a3; }
    int multi(int& out, int a) { return out += a + 1; }
};

// this order should be important if the messages aren't sorted by mixin name
BOOST_DEFINE_MIXIN(has_unio1_multio1, uni_1_overload_1_msg & multi_1_overload_1_msg);
BOOST_DEFINE_MIXIN(has_unio2_multio2, uni_1_overload_2_msg & multi_1_overload_2_msg);
BOOST_DEFINE_MIXIN(has_unio3_multio1, uni_1_overload_3_msg & multi_1_overload_1_msg);
BOOST_DEFINE_MIXIN(has_unio4_multio2, uni_1_overload_4_msg & multi_1_overload_2_msg);

BOOST_MIXIN_DEFINE_MESSAGE(multi_1_overload_1);
BOOST_MIXIN_DEFINE_MESSAGE(multi_1_overload_2);
BOOST_MIXIN_DEFINE_MESSAGE(uni_1_overload_1);
BOOST_MIXIN_DEFINE_MESSAGE(uni_1_overload_2);
BOOST_MIXIN_DEFINE_MESSAGE(uni_1_overload_3);
BOOST_MIXIN_DEFINE_MESSAGE(uni_1_overload_4);
