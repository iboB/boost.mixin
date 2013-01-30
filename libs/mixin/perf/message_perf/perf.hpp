//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include <iostream>
#include <boost/mixin.hpp>
#include <functional>

extern int A_LOT; // intentionally not const

//////////////////////////////////
// regular method

class regular_class
{
public:
    regular_class() : _sum(0) {}

    void add(int i);

    int sum() const { return _sum; }

private:
    int _sum;
};

//////////////////////////////////
// virtual dispatch

class abstract_class
{
public:
    virtual void add(int) = 0;
    virtual int sum() const = 0;
};

extern abstract_class* ac_instance;

//////////////////////////////////
// std function

extern std::function<void(int)> f_add;
extern std::function<int()> f_sum;

//////////////////////////////////
// boost mixin

extern boost::mixin::object* bm_object;

BOOST_MIXIN_MESSAGE_1(void, add, int, val);
BOOST_MIXIN_CONST_MESSAGE_0(int, sum);


//////////////////////////////////

extern void initialize_globals();
