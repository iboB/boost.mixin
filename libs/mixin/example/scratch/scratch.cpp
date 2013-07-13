//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE cxx11_boost_mixin_features

#include <boost/mixin.hpp>
#include <iostream>

using namespace boost::mixin;
using namespace std;

BOOST_DECLARE_MIXIN(normal_a);
BOOST_DECLARE_MIXIN(normal_b);
BOOST_DECLARE_MIXIN(custom_1);
BOOST_DECLARE_MIXIN(custom_2_a);
BOOST_DECLARE_MIXIN(custom_2_b);
BOOST_DECLARE_MIXIN(custom_own_var);

template <typename T>
struct alloc_counter
{
    static size_t data_allocations;
    static size_t data_deallocations;
    static size_t mixin_allocations;
    static size_t mixin_deallocations;
};

template <typename T>
size_t alloc_counter<T>::data_allocations;
template <typename T>
size_t alloc_counter<T>::data_deallocations;
template <typename T>
size_t alloc_counter<T>::mixin_allocations;
template <typename T>
size_t alloc_counter<T>::mixin_deallocations;


template <typename T>
struct custom_allocator : public mixin_allocator, public alloc_counter<T>
{
    // allocate memory for count mixin_data_in_object instances
    virtual char* alloc_mixin_data(size_t count)
    {
        ++data_allocations;
        return new char[count * mixin_data_size];
    }

    virtual void dealloc_mixin_data(char* ptr)
    {
        ++data_deallocations;
        delete[] ptr;
    }
    
    // allocate memory for a mixin instance
    virtual char* alloc_mixin(size_t size)
    {
        ++mixin_allocations;
        return new char[size];
    }

    virtual void dealloc_mixin(char* ptr)
    {
        ++mixin_deallocations;
        delete[] ptr;
    }
};

class global_alloc : public custom_allocator<global_alloc> {};
class custom_alloc_1 : public custom_allocator<custom_alloc_1> {};
class custom_alloc_2 : public custom_allocator<custom_alloc_2> {};
class custom_alloc_var : public custom_allocator<custom_alloc_var> {} the_allocator;

int main()
{
    global_alloc* glob = new global_alloc;
    set_global_domain_allocator(glob);

    {
        object o;
        mutate(o)
            .add<normal_a>()
            .add<normal_b>()
            .add<custom_1>()
            .add<custom_2_a>()
            .add<custom_2_b>()
            .add<custom_own_var>();

        cout << alloc_counter<global_alloc>::data_allocations << endl;
        cout << alloc_counter<global_alloc>::mixin_allocations << endl;
    }

    cout << alloc_counter<global_alloc>::data_deallocations << endl;
    cout << alloc_counter<global_alloc>::mixin_deallocations << endl;

    {
        object o1;
        mutate(o1)
            .add<normal_a>()
            .add<custom_1>()
            .add<custom_2_a>();

        object o2;
        mutate(o2)
            .add<normal_a>()
            .add<custom_1>()
            .add<custom_2_a>();

        object o3;
        mutate(o3)
            .add<normal_b>()
            .add<custom_2_b>();

        mutate(o1)
            .remove<normal_a>()
            .add<normal_b>()
            .remove<custom_2_a>()
            .add<custom_2_b>();

        cout << alloc_counter<global_alloc>::data_deallocations << endl;
        cout << alloc_counter<global_alloc>::mixin_deallocations << endl;
    }

    return 0;
}

class normal_a {};
class normal_b {};
class custom_1 {};
class custom_2_a {};
class custom_2_b {};
class custom_own_var {};

BOOST_DEFINE_MIXIN(normal_a, boost::mixin::none);
BOOST_DEFINE_MIXIN(normal_b, boost::mixin::none);
BOOST_DEFINE_MIXIN(custom_1, boost::mixin::allocator<custom_alloc_1>());
BOOST_DEFINE_MIXIN(custom_2_a, boost::mixin::allocator<custom_alloc_2>());
BOOST_DEFINE_MIXIN(custom_2_b, boost::mixin::allocator<custom_alloc_2>());
BOOST_DEFINE_MIXIN(custom_own_var, the_allocator);