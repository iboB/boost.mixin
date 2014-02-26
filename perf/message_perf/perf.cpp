//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "perf.hpp"
#include <boost/bind.hpp>

size_t A_LOT = 10000000;

int OBJ_NUM = 100000;

using namespace boost::mixin;
using namespace std;

abstract_class** ac_instances;
BOOST_MIXIN_CXX11_NAMESPACE::function<void(int)>* f_add;
BOOST_MIXIN_CXX11_NAMESPACE::function<int()>* f_sum;
BOOST_MIXIN_CXX11_NAMESPACE::function<void()>* f_noop;
boost::mixin::object** bm_objects;
regular_class* regular_objects;

BOOST_DECLARE_MIXIN(regular_class);
BOOST_DECLARE_MIXIN(regular_class2);

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

    void noop() const {}

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

    void noop() const {}

    int _sum;
};

class regular_class2
{
public:
    regular_class2() : _sum(0) {}

    void add(int i);

    int sum() const;

    void noop() const;

private:
    int _sum;
};


#if BOOST_MIXIN_USING_CXX11
#   define _1_NAMESPACE placeholders
#else
#   define _1_NAMESPACE
#endif

extern void initialize_globals()
{
    // don't care about memory leaks

    regular_objects = new regular_class[OBJ_NUM];

    ac_instances = new abstract_class*[OBJ_NUM];

    f_add = new BOOST_MIXIN_CXX11_NAMESPACE::function<void(int)>[OBJ_NUM];
    f_sum = new BOOST_MIXIN_CXX11_NAMESPACE::function<int()>[OBJ_NUM];
    f_noop = new BOOST_MIXIN_CXX11_NAMESPACE::function<void()>[OBJ_NUM];
    regular_class* objs = new regular_class[OBJ_NUM];
    regular_class2* objs2 = new regular_class2[OBJ_NUM];

    bm_objects = new object*[OBJ_NUM];

    for(int i=0; i<OBJ_NUM; ++i)
    {
        abstract_class* c;
        bm_objects[i] = new object;

        if(rand()%2)
        {
            c = new abstract_instance;

            f_add[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class::add, objs + i, _1_NAMESPACE::_1);
            f_sum[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class::sum, objs + i);
            f_noop[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class::noop, objs + i);

            mutate(bm_objects[i]).add<regular_class>();
        }
        else
        {
            c = new abstract_instance2;

            f_add[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class2::add, objs2 + i, _1_NAMESPACE::_1);
            f_sum[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class2::sum, objs2 + i);
            f_noop[i] = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class2::noop, objs2 + i);

            mutate(bm_objects[i]).add<regular_class2>();
        }

        ac_instances[i] = c;
    }
}

//////////////////////////////////////////////////////

void regular_class::add(int i)
{
    _sum += i;
}

int regular_class::sum() const
{
    return _sum;
}

void regular_class::noop() const {}

void regular_class2::add(int i)
{
    _sum += i;
}

int regular_class2::sum() const
{
    return _sum;
}

void regular_class2::noop() const {}

BOOST_DEFINE_MIXIN(regular_class, add_msg & sum_msg & noop_msg);
BOOST_DEFINE_MIXIN(regular_class2, add_msg & sum_msg & noop_msg);

BOOST_MIXIN_DEFINE_MESSAGE(add);
BOOST_MIXIN_DEFINE_MESSAGE(sum);
BOOST_MIXIN_DEFINE_MESSAGE(noop);
