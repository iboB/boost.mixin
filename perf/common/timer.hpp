//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include <iostream>
#include <boost/timer/timer.hpp>

class timer
{
public:
    void start(const char* name)
    {
        _name = name;
        t.start();
    }

    void avg(size_t num_tests)
    {
        t.stop();
        boost::timer::nanosecond_type time = t.elapsed().wall;
        time /= num_tests;
        std::cout << _name << " avg: " << time << " ns" << std::endl;
    }

private:
    boost::timer::cpu_timer t;
    const char* _name;
};