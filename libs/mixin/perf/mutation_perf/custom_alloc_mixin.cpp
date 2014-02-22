//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "common.hpp"
#include "custom_alloc_mixin.hpp"

using namespace std;
using namespace boost::mixin;

class custom_alloc_mixin
{
    double a, b; // some random values
};

// perfect allocations style allocator
// doesn't care about deallocation
struct custom_allocator : public mixin_allocator
{
    static const size_t NUM_IN_PAGE = 100000;

    vector<char*> md_block;
    size_t md_index;

    const size_t mixin_unit_size;
    vector<char*> mixin_block;
    size_t mixin_index;


    custom_allocator()
        : md_index(0)
        , mixin_unit_size(
            calculate_mem_size_for_mixin(
                sizeof(custom_alloc_mixin),
                boost::alignment_of<custom_alloc_mixin>::value))
    {
        new_md_page();
        new_mixin_page();
    }

    void new_md_page()
    {
        char* page = new char[mixin_data_size * NUM_IN_PAGE];
        md_block.push_back(page);
        md_index = 0;
    }

    void new_mixin_page()
    {
        char* page = new char[mixin_unit_size * NUM_IN_PAGE];
        mixin_block.push_back(page);
        mixin_index = 0;
    }

    // allocate memory for count mixin_data_in_object instances
    virtual char* alloc_mixin_data(size_t count)
    {
        BOOST_ASSERT(count < NUM_IN_PAGE);
        if(md_index + count > NUM_IN_PAGE)
        {
            new_md_page();
        }

        md_index += count;
        return md_block.back() + md_index * mixin_data_size;
    }

    virtual void dealloc_mixin_data(char* ptr)
    {
    }

    virtual void alloc_mixin(size_t mixin_class_size, size_t mixin_alignment, char*& out_buffer, size_t& out_mixin_offset)
    {
        if(mixin_index == NUM_IN_PAGE)
        {
            new_mixin_page();
        }

        ++mixin_index;

        out_buffer = mixin_block.back() + mixin_index * mixin_unit_size;

        out_mixin_offset = calculate_mixin_offset(out_buffer, mixin_alignment);
    }

    virtual void dealloc_mixin(char* ptr)
    {
    }
};

BOOST_DEFINE_MIXIN(custom_alloc_mixin, boost::mixin::allocator<custom_allocator>())
