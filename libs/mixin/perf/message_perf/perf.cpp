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

using namespace boost::mixin;
using namespace std;

abstract_class* ac_instance;
BOOST_MIXIN_CXX11_NAMESPACE::function<void(int)> f_add;
BOOST_MIXIN_CXX11_NAMESPACE::function<int()> f_sum;
boost::mixin::object* bm_object;

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


extern void initialize_globals()
{
    // don't care about memory leaks
    ac_instance = new abstract_instance;

#if BOOST_MIXIN_USING_CXX11
#   define _1_NAMESPACE placeholders
#else
#   define _1_NAMESPACE
#endif

    regular_class* c = new regular_class;
    f_add = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class::add, c, _1_NAMESPACE::_1);
    f_sum = BOOST_MIXIN_CXX11_NAMESPACE::bind(&regular_class::sum, c);

    bm_object = new object;

    mutate(bm_object).add<regular_class>();
}

//////////////////////////////////////////////////////

void regular_class::add(int i)
{
    _sum += i;
}

BOOST_DEFINE_MIXIN(regular_class, add_msg & sum_msg);

BOOST_MIXIN_DEFINE_MESSAGE(add);
BOOST_MIXIN_DEFINE_MESSAGE(sum);
