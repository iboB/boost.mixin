//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE cxx11_boost_mixin_features

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

using namespace boost::mixin;

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
        ++alloc_counter<T>::data_allocations;
        return new char[count * mixin_data_size];
    }

    virtual void dealloc_mixin_data(char* ptr)
    {
        ++alloc_counter<T>::data_deallocations;
        delete[] ptr;
    }

    // allocate memory for a mixin instance
    virtual char* alloc_mixin(size_t size)
    {
        ++alloc_counter<T>::mixin_allocations;
        return new char[size];
    }

    virtual void dealloc_mixin(char* ptr)
    {
        ++alloc_counter<T>::mixin_deallocations;
        delete[] ptr;
    }
};

class global_alloc : public custom_allocator<global_alloc> {};
class custom_alloc_1 : public custom_allocator<custom_alloc_1> {};
class custom_alloc_2 : public custom_allocator<custom_alloc_2> {};
class custom_alloc_var : public custom_allocator<custom_alloc_var> {} the_allocator;

BOOST_AUTO_TEST_CASE(allocators)
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

        BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::data_allocations, 1);
        BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::mixin_allocations, 2); // two global mixins
        BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_1>::mixin_allocations, 1); // one of these
        BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_2>::mixin_allocations, 2); // two of these
        BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_var>::mixin_allocations, 1); // one of these
    }

    BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::data_deallocations, 1);
    BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::mixin_deallocations, 2); // two global mixins
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_1>::mixin_deallocations, 1); // one of these
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_2>::mixin_deallocations, 2); // two of these
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_var>::mixin_deallocations, 1); // one of these

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

        BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::data_allocations, 5); // 1 + 4 new objects
        BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::data_deallocations, 2); // 1 + 1 changed object

        BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::mixin_allocations, 6); // 2 + 4
        BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_1>::mixin_allocations, 3); // 1 + 3
        BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_2>::mixin_allocations, 6); // 2 + 4
        BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_var>::mixin_allocations, 1); // 1 + 0
    }

    BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::data_deallocations, 5);

    BOOST_CHECK_EQUAL(alloc_counter<global_alloc>::mixin_allocations, 6); // 2 + 4
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_1>::mixin_allocations, 3); // 1 + 3
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_2>::mixin_allocations, 6); // 2 + 4
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_var>::mixin_allocations, 1); // 1 + 0

    // none of these should have had data allocations
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_1>::data_allocations, 0);
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_2>::data_allocations, 0);
    BOOST_CHECK_EQUAL(alloc_counter<custom_alloc_var>::data_allocations, 0);
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
