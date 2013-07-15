//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "perf.hpp"
#include <boost/bind.hpp>

int A_LOT = 100000000;
int OBJ_NUM = 1000;

using namespace boost::mixin;
using namespace std;

abstract_class** ac_instances;
BOOST_MIXIN_CXX11_NAMESPACE::function<void(int)>* f_add;
BOOST_MIXIN_CXX11_NAMESPACE::function<int()>* f_sum;
boost::mixin::object** bm_objects;
regular_class* regular_objects;

BOOST_DECLARE_MIXIN(regular_class);

class abstract_instance : public abstract_class
{
public:
    abstract_instance() : _sum(0) {}

    void add(int i)
    {
        _sum+=i;
    }

    int sum() const
    {
        return _sum;
    }

    int _sum;
};

class abstract_instance2 : public abstract_class
{
public:
    abstract_instance2() : _sum(0) {}

    void add(int i)
    {
        _sum+=i;
    }

    int sum() const
    {
        return _sum;
    }

    int _sum;
};


extern void initialize_globals()
{
    // don't care about memory leaks

    regular_objects = new regular_class[OBJ_NUM];

    ac_instances = new abstract_class*[OBJ_NUM];

    for(int i=0; i<OBJ_NUM; ++i)
    {
        abstract_class* c;

        if(rand()%2)
        {
            c = new abstract_instance;
        }
        else
        {
            c = new abstract_instance2;
        }

        ac_instances[i] = c;
    }


#if BOOST_MIXIN_USING_CXX11
#   define _1_NAMESPACE placeholders
#else
#   define _1_NAMESPACE
#endif

    f_add = new BOOST_MIXIN_CXX11_NAMESPACE::function<void(int)>[OBJ_NUM];
    f_sum = new BOOST_MIXIN_CXX11_NAMESPACE::function<int()>[OBJ_NUM];

    regular_class* objs = new regular_class[OBJ_NUM];
    for(int i=0; i<OBJ_NUM; ++i)
    {
        f_add[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class::add, objs + i, _1_NAMESPACE::_1);
        f_sum[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class::sum, objs + i);
    }

    bm_objects = new object*[OBJ_NUM];

    for(int i=0; i<OBJ_NUM; ++i)
    {
        bm_objects[i] = new object;
        mutate(bm_objects[i]).add<regular_class>();
    }
}

//////////////////////////////////////////////////////

void regular_class::add(int i)
{
    _sum += i;
}

int  regular_class::sum() const
{
    return _sum;
}

BOOST_DEFINE_MIXIN(regular_class, add_msg & sum_msg);

BOOST_MIXIN_DEFINE_MESSAGE(add);
BOOST_MIXIN_DEFINE_MESSAGE(sum);
