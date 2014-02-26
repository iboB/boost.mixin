//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "perf.hpp"
using namespace std;

#include "../common/timer.hpp"

// compare an unicast mixin message to
// regular functions
// virtual method
// std::function
//
// make sure link time optimizations are turned of
// gcc with no -flto
// msvc with no link time code generation

#define PERF(name, func) \
    srand(10); \
    t.start(name); \
    for(size_t i=0; i<A_LOT; ++i) \
    { \
        func; \
    } \
    t.avg(A_LOT)

#define PERF_SUM(name, func, getter) \
    PERF(name, func); \
    thesum = 0; \
    for(int i=0; i<OBJ_NUM; ++i) \
    { \
        thesum += getter; \
    } \
    cout << "sanity check: " << boolalpha << (thesum == A_LOT) << endl \


int main(int argc, char**)
{
    initialize_globals();
    size_t thesum = 0;

    timer t;

    #define access i%OBJ_NUM
    //#define access rand() % OBJ_NUM

    PERF_SUM("simple", regular_objects[access].add(argc), regular_objects[i].sum());
    PERF_SUM("virtual", ac_instances[access]->add(argc), ac_instances[i]->sum());
    PERF_SUM("std::func", f_add[access](argc), f_sum[i]());
    PERF_SUM("Boost.Mixin", add(bm_objects[access], argc), sum(bm_objects[i]));

    PERF("simple noop", regular_objects[access].noop());
    PERF("virtual noop", ac_instances[access]->noop());
    PERF("std_func noop", f_noop[access]());
    PERF("Boost.Mixin noop", noop(bm_objects[access]));

    return 0;
}
