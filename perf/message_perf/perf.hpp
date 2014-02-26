//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include <iostream>

#define BOOST_MIXIN_NO_MSG_THROW
#include <boost/mixin.hpp>

#if BOOST_MIXIN_USING_CXX11
#    include <functional>
#else
#    include <boost/function.hpp>
#    include <boost/bind.hpp>
#endif

extern size_t A_LOT; // intentionally not const
extern int OBJ_NUM; // number of objects

//////////////////////////////////
// regular method

class regular_class
{
public:
    regular_class() : _sum(0) {}

    void add(int i);
    int sum() const;

    void noop() const;

private:
    int _sum;
};

extern regular_class* regular_objects;

//////////////////////////////////
// virtual dispatch

class abstract_class
{
public:
    virtual void add(int) = 0;
    virtual int sum() const = 0;

    virtual void noop() const = 0;
};

extern abstract_class** ac_instances;

//////////////////////////////////
// std function

extern BOOST_MIXIN_CXX11_NAMESPACE::function<void(int)>* f_add;
extern BOOST_MIXIN_CXX11_NAMESPACE::function<int()>* f_sum;
extern BOOST_MIXIN_CXX11_NAMESPACE::function<void()>* f_noop;

//////////////////////////////////
// boost mixin

extern boost::mixin::object** bm_objects;

BOOST_MIXIN_MESSAGE_1(void, add, int, val);
BOOST_MIXIN_CONST_MESSAGE_0(int, sum);
BOOST_MIXIN_CONST_MESSAGE_0(void, noop);

//////////////////////////////////

extern void initialize_globals();
