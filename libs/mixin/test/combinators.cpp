//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE boost_mixin_multicast_result_combinators

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_DECLARE_MIXIN(a);
BOOST_DECLARE_MIXIN(b);
BOOST_DECLARE_MIXIN(c);

BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(bool, check1); // all true
BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(bool, check2); // some true
BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(bool, check3); // all false
BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(int, ival);
BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(double, dval);

using namespace boost::mixin;
using namespace boost::mixin::combinators;

// some custom combinators
template <int N>
class count_bigger_than
{
public:
    template <typename MessageReturnType>
    class combinator
    {
    public:
        typedef int result_type;
        typedef MessageReturnType type;

        combinator()
            : _result(0)
        {}

        int result() const
        {
            return _result;
        }

        bool add_result(const type& t)
        {
            _result += int(t > _value);
            return true;
        }

    private:
        static const int _value = N;
        int _result;
    };
};

class count_smaller_than
{
public:
    count_smaller_than()
        : _value(0)
        , _count(0)
    {}

    void set_compare_value(double d)
    {
        _value = d;
        _count = 0;
    }

    int count() const
    {
        return _count;
    }

    bool add_result(double d)
    {
        _count += int(d < _value);
        return true;
    }

private:
    double _value;
    int _count;
};

BOOST_AUTO_TEST_CASE(the_test)
{
    object o;
    mutate(o)
        .add<a>()
        .add<b>()
        .add<c>();

    /////////////////////////////////////////
    // ================ and =================
    boolean_and<> b_and;

    BOOST_CHECK(check1<boolean_and>(o));
    check1(o, b_and);
    BOOST_CHECK(b_and.result());
    b_and.reset();

    BOOST_CHECK(!check2<boolean_and>(o));
    check2(o, b_and);
    BOOST_CHECK(!b_and.result());
    b_and.reset();

    BOOST_CHECK(!check3<boolean_and>(o));
    check3(o, b_and);
    BOOST_CHECK(!b_and.result());
    b_and.reset();

    /////////////////////////////////////////
    // ================ or =================
    boolean_or<> b_or;

    BOOST_CHECK(check1<boolean_or>(o));
    check1(o, b_or);
    BOOST_CHECK(b_or.result());
    b_or.reset();

    BOOST_CHECK(check2<boolean_or>(o));
    check2(o, b_or);
    BOOST_CHECK(b_or.result());
    b_or.reset();

    BOOST_CHECK(!check3<boolean_or>(o));
    check3(o, b_or);
    BOOST_CHECK(!b_or.result());
    b_or.reset();

    /////////////////////////////////////////
    // ================ sum =================
    BOOST_CHECK_EQUAL(ival<sum>(o), 111);
    BOOST_CHECK_CLOSE(dval<sum>(o), 0.111, 0.00001);

    /////////////////////////////////////////
    // ============= custom ===============
    BOOST_CHECK_EQUAL(ival<count_bigger_than<0>::combinator>(o), 3);
    BOOST_CHECK_EQUAL(ival<count_bigger_than<5>::combinator>(o), 2);
    BOOST_CHECK_EQUAL(ival<count_bigger_than<50>::combinator>(o), 1);
    BOOST_CHECK_EQUAL(ival<count_bigger_than<500>::combinator>(o), 0);

    count_smaller_than count_smaller;
    dval(o, count_smaller);
    BOOST_CHECK_EQUAL(count_smaller.count(), 0);

    count_smaller.set_compare_value(0.05);
    dval(o, count_smaller);
    BOOST_CHECK_EQUAL(count_smaller.count(), 2);

    count_smaller.set_compare_value(1);
    dval(o, count_smaller);
    BOOST_CHECK_EQUAL(count_smaller.count(), 3);
};


#define all_msg check1_msg & check2_msg & check3_msg & ival_msg & dval_msg

class a
{
public:
    bool check1() const { return true; }
    bool check2() const { return false; }
    bool check3() const { return false; }
    int ival() const { return 1; }
    double dval() const { return 0.1; }
};

BOOST_DEFINE_MIXIN(a, all_msg);

class b
{
public:
    bool check1() const { return true; }
    bool check2() const { return true; }
    bool check3() const { return false; }
    int ival() const { return 10; }
    double dval() const { return 0.01; }
};

BOOST_DEFINE_MIXIN(b, all_msg);

class c
{
public:
    bool check1() const { return true; }
    bool check2() const { return false; }
    bool check3() const { return false; }
    int ival() const { return 100; }
    double dval() const { return 0.001; }
};

BOOST_DEFINE_MIXIN(c, all_msg);

BOOST_MIXIN_DEFINE_MESSAGE(check1);
BOOST_MIXIN_DEFINE_MESSAGE(check2);
BOOST_MIXIN_DEFINE_MESSAGE(check3);
BOOST_MIXIN_DEFINE_MESSAGE(ival);
BOOST_MIXIN_DEFINE_MESSAGE(dval);

