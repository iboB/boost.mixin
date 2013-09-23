//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_ALLOCATORS_HPP_INCLUDED)
#define _BOOST_MIXIN_ALLOCATORS_HPP_INCLUDED

#include "global.hpp"

namespace boost
{
namespace mixin
{

/**
    * The class @ref should be the parent to your custom
    * domain allocators, i.e. allocators that are set to a
    * domain and used for all mixin allocation within it
    */
class BOOST_MIXIN_API domain_allocator
{
public:
    virtual ~domain_allocator() {}

    // allocate memory for count mixin_data_in_object instances
    virtual char* alloc_mixin_data(size_t count) = 0;
    virtual void dealloc_mixin_data(char* ptr) = 0;

    // calulcates appropriate size for a mixin buffer
    // so as to satisfy the requirements of mixin size and alignment
    // AND leave a room for its owning object in front
    static size_t calculate_mem_size_for_mixin(size_t mixin_size, size_t mixin_alignment);

    // calculates the appropriate offset of the mixin in the buffer
    // so as to satisfy the requirements of its alignment
    // AND leave a room for its owning object in front
    static size_t calculate_mixin_offset(const char* buffer, size_t mixin_alignment);

    // allocate memory for a mixin instance
    // the library will requiest a buffer in which to put the mixin
    // it should fill the output parameters with the address of the allocated memory
    // and the offset of the mixin (according to the alignment)
    // BUT IN SUCH A WAY AS TO ALLOW A POINTER TO BE PLACED IN FRONT
    // you may use calculate_mem_size_for_mixin and calculate_mixin_offset if you're not sure what to do
    virtual void alloc_mixin(size_t mixin_size, size_t mixin_alignment, char*& out_buffer, size_t& out_mixin_offset) = 0;
    virtual void dealloc_mixin(char* ptr) = 0;

    // use this to determine how many bytes you'll allocate for single mixin data
    static const size_t mixin_data_size;

#if defined(BOOST_MIXIN_DEBUG)
    // checks to see if an allocator is changed after it has already started allocating
    // it could be a serious bug to allocate from one and deallocate from another

    // users are encouraged to make use of this when debugging
    domain_allocator() : _has_allocated(false) {}

    bool has_allocated() const { return _has_allocated; }

protected:
    bool _has_allocated;
#endif
};

// this class can be set through the allocator feature to mixins
class BOOST_MIXIN_API mixin_allocator : public domain_allocator
{
private:
    virtual char* alloc_mixin_data(size_t count);
    virtual void dealloc_mixin_data(char* ptr);
};

namespace internal
{

// this is the default domain allocator that
// allocates/deallocates memory through new/delete
class BOOST_MIXIN_API default_domain_allocator : public domain_allocator
{
public:
    virtual char* alloc_mixin_data(size_t count);
    virtual void dealloc_mixin_data(char* ptr);
    virtual void alloc_mixin(size_t mixin_size, size_t mixin_alignment, char*& out_buffer, size_t& out_mixin_offset);
    virtual void dealloc_mixin(char* ptr);
};


} // namespace internal

// feature list entry for custom mixin allocators
template <typename CusomAllocator>
mixin_allocator& allocator()
{
    static CusomAllocator alloc;
    return alloc;
}

} // namespace mixin
} // namespace boost

#endif // _BOOST_MIXIN_ALLOCATORS_HPP_INCLUDED
