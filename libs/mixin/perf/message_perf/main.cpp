//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "perf.hpp"
#include <sys/time.h>

using namespace std;

// compare an unicast mixin message to
// regular functions
// virtual method
// std::function
//
// make sure link time optimizations are turned of
// gcc with no -flto
// msvc with no link time code generation

struct timer
{
    timer()
    {
        gettimeofday(&start, nullptr);
    }

    ~timer()
    {
        timeval end;
        gettimeofday(&end, nullptr);

        int secs = end.tv_sec - start.tv_sec;
        int usecs = end.tv_usec - start.tv_usec;

        int ms = int(((secs) * 1000 + usecs/1000.0) + 0.5);

        cout << ms << " ms " << endl;
    }

    timeval start;
};

#define PERF(name, summer, getter) \
    cout << #name << ":"; \
    { \
        timer t; \
        for(int i=0; i<A_LOT; ++i) \
        { \
            summer; \
        } \
    } \
    cout << getter << endl \


int main(int argc, char**)
{
    initialize_globals();

    regular_class rgc;
    PERF(simple, rgc.add(argc), rgc.sum());

    PERF(virtual, ac_instance->add(argc), ac_instance->sum());

    PERF(std_func, f_add(argc), f_sum());

    PERF(mixin, add(bm_object, argc), sum(bm_object));

    return 0;
}
