//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"

#include <boost/mixin/allocators.hpp>
#include <boost/mixin/object_type_info.hpp>

namespace boost
{
namespace mixin
{

const size_t domain_allocator::mixin_data_size = sizeof(internal::mixin_data_in_object);

static inline char* allocate_mixin_data(size_t count)
{
    BOOST_ASSERT(domain_allocator::mixin_data_size == sizeof(internal::mixin_data_in_object));
    return new char[sizeof(internal::mixin_data_in_object) * count];
}

static inline void deallocate_mixin_data(char* ptr)
{
    delete[] ptr;
}

char* mixin_allocator::alloc_mixin_data(size_t count)
{
    BOOST_ASSERT(false); // a mixin allocator should never have to allocate mixin data
    return allocate_mixin_data(count);
}

void mixin_allocator::dealloc_mixin_data(char* ptr)
{
    BOOST_ASSERT(false); // a mixin allocator should never heve do deallocate mixin data
    deallocate_mixin_data(ptr);
}

namespace internal
{

char* default_domain_allocator::alloc_mixin_data(size_t count)
{
#if defined(BOOST_MIXIN_DEBUG)
    _has_allocated = true;
#endif
    return allocate_mixin_data(count);
}

void default_domain_allocator::dealloc_mixin_data(char* ptr)
{
#if defined(BOOST_MIXIN_DEBUG)
    BOOST_ASSERT(_has_allocated); // what? deallocate without ever allocating?
#endif
    deallocate_mixin_data(ptr);
}

char* default_domain_allocator::alloc_mixin(size_t size)
{
#if defined(BOOST_MIXIN_DEBUG)
    _has_allocated = true;
#endif
    return new char[size];
}

void default_domain_allocator::dealloc_mixin(char* ptr)
{
#if defined(BOOST_MIXIN_DEBUG)
    BOOST_ASSERT(_has_allocated); // what? deallocate without ever allocating?
#endif
    delete[] ptr;
}

} // namespace internal

} // namespace mixin
} // namespace boost
