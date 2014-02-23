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

//[allocators_global
/*`
Boost.Mixin allows you to set custom allocators for the persistent pieces of
memory the library may require.

The library allocates some memory on initialization, which happens at a global
scope -- before the entry point of a program. It also has some allocations which
are for objects with a very short lifetime. Currently those are not covered by
the allocators.

What you can control with the custom allocators is the new memory allocated for
object instances - their internal mixin data. You can assign a gloabl allocator
to the library and you could also set individual allocators per mixin type.

First let's see how you can create a global allocator. Let's assume you have
a couple of functions of your own that allocate and deallocate memory in some
way specific to your needs:
*/

char* allocate(size_t size);
void deallocate(char* buffer);

/*`
To create a global allocator you need to create a class derived from
`global_allocator` and override its virtual methods.
*/

class custom_allocator : public boost::mixin::global_allocator
{

    /*`
    The first two methods allocate a buffer for the mixin data pointers. Every
    object has pointers to its mixins within it. This is the array of such
    pointers. The class `global_allocatos` has a static constant member --
    `mixin_data_size` -- which you should use to see the size of a single
    element in that array.
    */
    virtual char* alloc_mixin_data(size_t count)
    {
        return allocate(count * mixin_data_size);
    }

    virtual void dealloc_mixin_data(char* ptr)
    {
        deallocate(ptr);
    }

    /*`
    The other two methods you need to overload, allocate and deallocate the
    memory for an actual mixin class instance. As you may have already read, the
    buffer allocated for a mixin instance is bigger than needed because the
    library stores a pointer to the owning object immediately before the memory
    used by the mixin instance.

    That's why this function is not as simple as the one for the mixin data
    array. It has to conform to the mixin (and also `object*`) alignment.
    */
    virtual void alloc_mixin(size_t mixin_size, size_t mixin_alignment, char*& out_buffer, size_t& out_mixin_offset)
    {
        /*`
        The users are strongly advised to use the static method
        `global_allocator::calculate_mem_size_for_mixin`. It will appropriately
        calculate how much memory is needed for the mixin instance such that
        there is enough room at the beginning for the pointer to the owning
        object and the memory alignment is respected.
        */
        size_t size = calculate_mem_size_for_mixin(mixin_size, mixin_alignment);
        out_buffer = allocate(size);

        /*`
        After you allocate the buffer you should take care of the other output
        parameter - the mixin offset. It calculates the offset of the actual
        mixin instance memory within the buffer, such that there is room for
        the owning object pointer in before it and all alignments are
        respected.

        You are encouraged to use the static method
        `global_allocator::calculate_mixin_offset`  for this purpose.
        */
        out_mixin_offset = calculate_mixin_offset(out_buffer, mixin_alignment);
    }

    /*`
    The mixin instance deallocation method can be trivial
    */
    virtual void dealloc_mixin(char* ptr)
    {
        deallocate(ptr);
    }

    //]
};

//[allocators_mixin
/*`
As we mentioned before, you can have an allocator specific for a mixin type.

A common case for such use is to have a per-frame allocator -- one that has a
preallocated buffer which is used much like a stack, with its pointer reset at
the end of each simulation frame (or at the beginning each new one). Let's
create such an allocator.

First, a mixin instance allocator is not necessarily bound to a concrete mixin
type. You can can the same instance of such an allocator set for many mixins
(which would be a common use of a per-frame allocator), but for our example
let's create one that /is/ bound to an instance. We will make it a template
class because the code for each mixin type will be the same.

A mixin instance allocator needs to be derived from the class `mixin_allocator`.
You then need to overload its two virtual methods which are exactly the same
as the mixin instance allocation/deallocation methods in `global_allocator`.
*/

template <typename Mixin>
class per_frame_allocator : public boost::mixin::mixin_allocator
{
private:
    static const size_t NUM_IN_PAGE = 1000;

    size_t _num_allocations; // number of "living" instances allocated
    const size_t mixin_buf_size; // the size of a single mixin instance buffer
    vector<char*> _pages; // use pages of data where each page can store NUM_IN_PAGE instances
    size_t _page_byte_index; // index within a memory "page"
    const size_t page_size; // size in bytes of a page

public:

    // some way to obtain the instance
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
        new_memory_page();
    }

    void new_memory_page()
    {
        char* page = new char[page_size];
        _pages.push_back(page);
        _page_byte_index = 0;
    }
    virtual void alloc_mixin(size_t mixin_class_size, size_t mixin_alignment, char*& out_buffer, size_t& out_mixin_offset)
    {
        if(_page_byte_index == NUM_IN_PAGE)
        {
            // if we don't have space in our current page, create a new one
            new_memory_page();
        }

        out_buffer = _pages.back() + _page_byte_index * mixin_buf_size;

        // again calculate the offset using this static member function
        out_mixin_offset = calculate_mixin_offset(out_buffer, mixin_alignment);

        ++_page_byte_index;
        ++_num_allocations;
    }

    virtual void dealloc_mixin(char* buf)
    {
#if !defined(NDEBUG)
        // in debug mode check if the mixin is within any of our pages
        for(size_t i=0; i<_pages.size(); ++i)
        {
            const char* page_begin = _pages[i];
            const char* page_end = page_begin + page_size;

            BOOST_ASSERT(buf >= page_begin && buf < page_end);
        }
#else
        buf; // to skip warning for unused parameter
#endif
        // no actual deallocation to be done
        // just decrement our living instances counter

        --_num_allocations;
    }

    // function to be called once each frame that resets the allocator
    void reset()
    {
        BOOST_ASSERT(_num_allocations == 0); // premature reset
        for(size_t i=1; i<_pages.size(); ++i)
        {
            delete[] _pages[i];
        }

        _page_byte_index = 0;
    }
};

/*`
Now this class can be set as a mixin allocator for a given mixin type. A side
effect of the fact that it's bound to the type is that it keeps mixin instances
in a continuous buffer. With some changes (to take care of potential holes in
the buffer) such an allocator can be used by a subsystem that works through
mixins relying on them being in a continuous buffer to avoid cache misses.

To illustrate a usage for our mixin allocator, let's imagine we have a game. If
a character in our game dies, it will be destroyed at the end of the current
frame and should stop responding to any messages. We can create a mixin called
`dead_chacater` which implements all those the messages with a higher priority
than the rest of the mixins. Since every object that has a dead_characer mixin
will be destroyed by the end of the frame, it will be save to use the per-frame
allocator for it.

First let's create the mixin class and sample messages:
*/

class dead_character
{
public:
    void die() {}
    // ...
};

BOOST_MIXIN_MESSAGE_0(void, die);
BOOST_MIXIN_DEFINE_MESSAGE(die);
//...

/*`
Now we define the mixin so that is uses the allocator, we just need to add it
with `&` to the mixin feature list, just like we add messages. There are two
ways to do so. First we could do it like this

    BOOST_DEFINE_MIXIN(dead_character, ... & boost::mixin::priority(1, die_msg)
        & boost::mixin::allocator<per_frame_allocator<dead_character>>());

This will create the instance of the allocator internally and we won't be able
to get it. Since in our case we do care about the instance because we want to
call its `reset` method, we could use an alternative way, by just adding
an actual instance of the allocator to the feature list:
*/

BOOST_DEFINE_MIXIN(dead_character, /*...*/ boost::mixin::priority(1, die_msg)
    & per_frame_allocator<dead_character>::instance());

/*`
If we share a mixin instance allocator between multiple mixins, the second way
is also the way to go.
*/

//]

int main()
{
//[allocators_global_use
    /*`
    To use the custom global allocator you need to instantiate it and then set
    it with `set_global_allocator`. Unlike the mutation rules, the
    responsibility for the allocator instance is yours. You need to make sure
    that the lifetime of the instance is at least as long as the lifetime of
    all objects in the system.

    Unfortunately this means that if you have global or static objects, you need
    to create a new pointer that is, in a way, a memory leak. If you do not have
    global or static objects, it should be safe for it to just be a local
    variable in your program's entry point function.
    */
    custom_allocator alloc;
    boost::mixin::set_global_allocator(&alloc);
//]

//[allocators_mixin_use
    /*`
    Now all mixin allocations and deallocations will pass through our mixin
    allocator:
    */
    boost::mixin::object o;

    boost::mixin::mutate(o)
        .add<dead_character>();

    boost::mixin::mutate(o)
        .remove<dead_character>();

    // safe because we've destroyed all instances of `dead_character`
    per_frame_allocator<dead_character>::instance().reset();
//]

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

//[tutorial_allocators
//` (For the complete, working source of this example see
//` [tutorialfile allocators.cpp])
//` [allocators_global]
//` [allocators_global_use]
//` [allocators_mixin]
//` [allocators_mixin_use]
//]
