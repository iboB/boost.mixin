//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/mixin.hpp>
#include <iostream>

using namespace std;
using namespace boost::mixin;

class test_mixin
{
public:
    int _id;
    string name;
    // ... other common fields

    void test() {}
};

BOOST_MIXIN_MESSAGE_0(void, test);

BOOST_DEFINE_MIXIN(test_mixin, test_msg);

BOOST_MIXIN_DEFINE_MESSAGE(test);

struct parent
{
    virtual void virt() = 0;
};

struct child : public parent
{
    void virt() override {}
};

int main()
{
    object obj1;
    mutate(obj1)
        .add<test_mixin>();

    parent* x = new child;

    x->virt();

    test(obj1);

    return 0;
}
