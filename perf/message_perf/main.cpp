//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "perf.hpp"
using namespace std;

// compare an unicast mixin message to
// regular functions
// virtual method
// std::function
//
// make sure link time optimizations are turned of
// gcc with no -flto
// msvc with no link time code generation

#if defined(_MSC_VER)
#include <Windows.h>
struct timer
{
    timer()
    {
        start = GetTickCount();
    }

    ~timer()
    {
        DWORD end = GetTickCount();;

        int ms = end - start;

        cout << ms << " ms " << endl;
    }

    DWORD start;
};

#else // _MSC_VER

#include <sys/time.h>
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
#endif

#define PERF(name, summer, getter) \
    srand(10); \
    cout << #name << ": "; \
    { \
        timer t; \
        for(int i=0; i<A_LOT; ++i) \
        { \
            summer; \
        } \
    } \
    thesum = 0; \
    for(int i=0; i<OBJ_NUM; ++i) \
    { \
        thesum += getter; \
    } \
    cout << thesum << endl \


int main(int argc, char**)
{
    initialize_globals();
    int thesum = 0;

    #define access i%OBJ_NUM
    //#define access rand() % OBJ_NUM

    PERF(simple, regular_objects[access].add(argc), regular_objects[i].sum());

    PERF(virtual, ac_instances[access]->add(argc), ac_instances[i]->sum());

    PERF(std_func, f_add[access](argc), f_sum[i]());

    PERF(mixin, add(bm_objects[access], argc), sum(bm_objects[i]));

    return 0;
}
