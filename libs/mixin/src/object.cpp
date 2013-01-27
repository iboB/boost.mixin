//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/object.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/foreach.hpp>
#include <boost/mixin/mixin_type_info.hpp>

namespace boost
{
namespace mixin
{

using namespace internal;

object::object()
    : _type_info(&object_type_info::null())
    , _mixin_data(nullptr)
{
}

object::~object()
{
    clear();
}

domain* object::dom() const
{
    return _type_info->dom();
}

void* object::internal_get_mixin(const internal::mixin_type_info_data& mixin_info)
{
    if(_type_info->has_mixin(mixin_info.id))
    {
        return _mixin_data[_type_info->mixin_index(mixin_info.id)] + sizeof(object*);
    }
    else
    {
        return nullptr;
    }
}

const void* object::internal_get_mixin(const internal::mixin_type_info_data& mixin_info) const
{
    if(_type_info->has_mixin(mixin_info.id))
    {
        return _mixin_data[_type_info->mixin_index(mixin_info.id)] + sizeof(object*);
    }
    else
    {
        return nullptr;
    }
}

bool object::internal_has_mixin(const internal::mixin_type_info_data& mixin_info)
{
    return _type_info->has_mixin(mixin_info.id);
}

void object::clear()
{
    BOOST_FOREACH(const mixin_type_info_data* mixin_info, _type_info->_compact_mixins)
    {
        destroy_mixin(mixin_info->id);
    }

    _type_info->dealloc_mixin_data(_mixin_data);
    _mixin_data = nullptr;

    _type_info = &object_type_info::null();
}

void object::change_type(const object_type_info* new_type, bool manage_mixins /*= false*/)
{
    const object_type_info* old_type = _type_info;
    char** old_mixin_data = _mixin_data;
    char** new_mixin_data = new_type->alloc_mixin_data();

    BOOST_FOREACH(const mixin_type_info_data* mixin_info, old_type->_compact_mixins)
    {
        mixin_id id = mixin_info->id;
        if(new_type->has_mixin(id))
        {
            new_mixin_data[new_type->mixin_index(id)] = old_mixin_data[old_type->mixin_index(id)];
        }
        else if(manage_mixins)
        {
            destroy_mixin(id);
        }
    }

    old_type->dealloc_mixin_data(old_mixin_data);

    _type_info = new_type;
    _mixin_data = new_mixin_data;

    if(manage_mixins)
    {
        BOOST_FOREACH(const mixin_type_info_data* mixin_info, new_type->_compact_mixins)
        {
            size_t index = new_type->mixin_index(mixin_info->id);
            if(!new_mixin_data[index])
            {
                construct_mixin(mixin_info->id);
            }
        }
    }
}

void object::construct_mixin(mixin_id id)
{
    BOOST_ASSERT(_type_info->has_mixin(id));
    size_t index = _type_info->mixin_index(id);
    BOOST_ASSERT(!_mixin_data[index]);

    char* buffer = _type_info->alloc_mixin(id);
    _mixin_data[index] = buffer;

    object** self = reinterpret_cast<object**>(buffer);
    *self = this;

    buffer += sizeof(object*);

    _type_info->mixin_info(id).constructor(buffer);
}

void object::destroy_mixin(mixin_id id)
{
    BOOST_ASSERT(_type_info->has_mixin(id));
    size_t index = _type_info->mixin_index(id);

    _type_info->mixin_info(id).destructor(_mixin_data[index] + sizeof(object*));
    _type_info->dealloc_mixin(id, _mixin_data[index]);
    _mixin_data[index] = nullptr;
}

}
}
