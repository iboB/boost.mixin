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

namespace boost
{
namespace mixin
{
namespace internal
{

object_type_info::object_type_info()
    : _domain(nullptr)
{
    zero_memory(_mixins, sizeof(_mixins));
    zero_memory(_mixin_indices, sizeof(_mixin_indices));
}

object_type_info::~object_type_info()
{

}

const object_type_info& object_type_info::null()
{
    static const object_type_info n;
    return n;
};

mixin_data_in_object* object_type_info::alloc_mixin_data() const
{
    return new mixin_data_in_object[_compact_mixins.size() + 1]; //reserve idnex 0 for nullptr
}

void object_type_info::dealloc_mixin_data(mixin_data_in_object* data) const
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

