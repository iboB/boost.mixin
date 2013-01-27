//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/mixin_type_info.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/mixin/domain.hpp>
#include <cstring> // for memset

namespace boost
{
namespace mixin
{
namespace internal
{

object_type_info::object_type_info()
    : _domain(nullptr)
{
    std::memset(_mixins, 0, sizeof(_mixins));
    std::memset(_mixin_indices, 0, sizeof(_mixin_indices));
}

object_type_info::~object_type_info()
{

}

const object_type_info& object_type_info::null()
{
    static const object_type_info n;
    return n;
};

char** object_type_info::alloc_mixin_data(bool set_to_null /*= true*/) const
{
    size_t size = _compact_mixins.size() * sizeof(char*);
    char** data = new char*[size];

    if(set_to_null)
    {
        memset(data, 0, size);
    }

    return data;
}

void object_type_info::dealloc_mixin_data(char** data) const
{
    delete[] data;
}

char* object_type_info::alloc_mixin(mixin_id id) const
{
    BOOST_ASSERT(has_mixin(id));
    return new char[sizeof(object*) + mixin_info(id).size];
}

void object_type_info::dealloc_mixin(mixin_id id, char* mem) const
{
    BOOST_ASSERT(has_mixin(id));
    delete[] mem;
}

}
}
}

