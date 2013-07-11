//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_ALLOCATORS_HPP_INCLUDED)
#define _BOOST_MIXIN_ALLOCATORS_HPP_INCLUDED

#include "global.hpp"
#include "domain.hpp"

namespace boost
{
namespace mixin
{

// the class to allocate mixins within objects
// it can be set to a domain and be used to
class BOOST_MIXIN_API domain_allocator
{
public:
    virtual ~domain_allocator() {}

    // allocate memory for count mixin_data_in_object instances
    virtual char* alloc_mixin_data(size_t count) = 0;
    virtual void dealloc_mixin_data(char* ptr) = 0;

    // allocate memory for a mixin instance
    virtual char* alloc_mixin(size_t size) = 0;
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
    virtual char* alloc_mixin(size_t size);
    virtual void dealloc_mixin(char* ptr);
};

} // namespace internal

// set allocator to all domains
void BOOST_MIXIN_API set_global_domain_allocator(domain_allocator* allocator);

// set allocator to default domain
void BOOST_MIXIN_API set_default_domain_allocator(domain_allocator* allocator);

// add mutation rule to specific domain
template <typename DomainTag>
void set_allocator_for_domain(mutation_rule* allocator)
{
    internal::get_domain_for_tag<DomainTag>().set_allocator(allocator);
}

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
