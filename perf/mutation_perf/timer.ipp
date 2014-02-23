//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

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