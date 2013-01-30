//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "basic.hpp"
#include "transform_messages.hpp"
#include <time.h>

using namespace std;
using namespace boost::mixin;

static const int A_LOT = 100000000;

struct adder
{
    adder() : sum(0) {}

    void add_to_sum(int n)
    {
        sum += n;
    }

    int sum;
};

clock_t check_with(adder& a)
{
    cout << "simple method check: " << endl;
    clock_t start = clock();
    for(int i=0; i<A_LOT; ++i)
    {
        a.add_to_sum(i);
    }
    clock_t time = clock() - start;
    cout << a.sum << endl;
    cout << "for " << (time/CLOCKS_PER_SEC) << " seconds" << endl;
    return time;
}

struct vadder
{
    virtual void add_to_sum(int n) = 0;
    virtual int get_sum() const = 0;
};

clock_t check_with(vadder& a)
{
    cout << "virtual method check: " << endl;
    clock_t start = clock();
    for(int i=0; i<A_LOT; ++i)
    {
        a.add_to_sum(i);
    }
    clock_t time = clock() - start;
    cout << a.get_sum() << endl;
    cout << "for " << (time/CLOCKS_PER_SEC) << " seconds" << endl;
    return time;
}

struct vadder_impl : public vadder
{
    vadder_impl() : sum(0) {}

    void add_to_sum(int n)
    {
        sum += n;
    }

    int get_sum() const { return sum; }

    int sum;
};


clock_t check_with(object* o)
{
    cout << "mixin message check: " << endl;
    clock_t start = clock();
    for(int i=0; i<A_LOT; ++i)
    {
        translate(o, i);
    }
    clock_t time = clock() - start;
    cout << get_combined_transform(o) << endl;
    cout << "for " << (time/CLOCKS_PER_SEC) << " seconds" << endl;
    return time;
}

void performance_test_object(boost::mixin::object* o)
{
    adder a;
    check_with(a);

    vadder* v = new vadder_impl;
    check_with(*v);

    check_with(o);
}
