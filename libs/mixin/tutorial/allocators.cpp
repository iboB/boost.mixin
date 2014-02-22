//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <iostream>
#include <boost/mixin.hpp>

using namespace std;

char* allocate(size_t size);
void deallocate(char* buffer);

class custom_allocator : public boost::mixin::global_allocator
{
    virtual char* alloc_mixin_data(size_t count)
    {
        return allocate(count * mixin_data_size);
    }

    virtual void dealloc_mixin_data(char* ptr)
    {
        deallocate(ptr);
    }

    virtual void alloc_mixin(size_t mixin_size, size_t mixin_alignment, char*& out_buffer, size_t& out_mixin_offset)
    {
        size_t size = calculate_mem_size_for_mixin(mixin_size, mixin_alignment);
        out_buffer = allocate(size);
        out_mixin_offset = calculate_mixin_offset(out_buffer, mixin_alignment);
    }

    virtual void dealloc_mixin(char* ptr)
    {
        deallocate(ptr);
    }
};

template <typename Mixin>
class per_frame_allocator : public boost::mixin::mixin_allocator
{
public:
    static per_frame_allocator& instance()
    {
        static per_frame_allocator i;
        return i;
    }

    per_frame_allocator()
        : mixin_buf_size(
            calculate_mem_size_for_mixin(
                sizeof(Mixin),
                boost::alignment_of<Mixin>::value))
        , _num_allocations(0)
        , page_size(mixin_buf_size * NUM_IN_PAGE)
    {
        new_mixin_page();
    }

    void new_mixin_page()
    {
        char* page = new char[page_size];
        _pages.push_back(page);
        _page_byte_index = 0;
    }

    virtual void alloc_mixin(size_t mixin_class_size, size_t mixin_alignment, char*& out_buffer, size_t& out_mixin_offset)
    {
        if(_page_byte_index == NUM_IN_PAGE)
        {
            new_mixin_page();
        }

        out_buffer = _pages.back() + _page_byte_index * mixin_buf_size;

        out_mixin_offset = calculate_mixin_offset(out_buffer, mixin_alignment);

        ++_page_byte_index;
        ++_num_allocations;
    }

    virtual void dealloc_mixin(char* buf)
    {
#if !defined(NDEBUG)
        for(size_t i=0; i<_pages.size(); ++i)
        {
            const char* page_begin = _pages[i];
            const char* page_end = page_begin + page_size;

            BOOST_ASSERT(buf >= page_begin && buf < page_end);
        }
#else
        buf; // to skip warning for unused parameter
#endif

        --_num_allocations;
    }

    void reset()
    {
        BOOST_ASSERT(_num_allocations == 0); // premature reset
        for(size_t i=1; i<_pages.size(); ++i)
        {
            delete[] _pages[i];
        }

        _page_byte_index = 0;
    }

private:
    static const size_t NUM_IN_PAGE = 1000;

    size_t _num_allocations;
    const size_t mixin_buf_size;
    vector<char*> _pages;
    size_t _page_byte_index;

    const size_t page_size;
};

class dead_character
{
public:
    void die() {}
    //...
};

BOOST_MIXIN_MESSAGE_0(void, die);
BOOST_MIXIN_DEFINE_MESSAGE(die);

//BOOST_DEFINE_MIXIN(dead_character, /*...*/ die_msg & boost::mixin::allocator<per_frame_allocator<dead_character>>());
BOOST_DEFINE_MIXIN(dead_character, /*...*/ die_msg & per_frame_allocator<dead_character>::instance());

int main()
{
    custom_allocator alloc;
    boost::mixin::set_global_allocator(&alloc);

    boost::mixin::object o;

    boost::mixin::mutate(o)
        .add<dead_character>();

    boost::mixin::mutate(o)
        .remove<dead_character>();

    per_frame_allocator<dead_character>::instance().reset();

    return 0;
}

char* allocate(size_t size)
{
    return new char[size];
}

void deallocate(char* buffer)
{
    delete[] buffer;
}
