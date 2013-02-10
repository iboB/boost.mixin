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
#include <boost/mixin/message.hpp>
#include <boost/mixin/domain.hpp>
#include <boost/mixin/object_type_template.hpp>

namespace boost
{
namespace mixin
{

using namespace internal;

// used by objects with no mixin data, so they would
// return nullptr on get<Mixin>() without having to
// check or crashing
static mixin_data_in_object null_mixin_data;

object::object()
    : _type_info(&object_type_info::null())
    , _mixin_data(&null_mixin_data)
{
}

object::object(const object_type_template& type)
    : _type_info(&object_type_info::null())
    , _mixin_data(&null_mixin_data)
{
    type.apply_to(this);
}

object::~object()
{
    clear();
}

domain* object::dom() const
{
    return _type_info->dom();
}

void* object::internal_get_mixin(mixin_id id)
{
    return _mixin_data[_type_info->mixin_index(id)].mixin();
}

const void* object::internal_get_mixin(mixin_id id) const
{
    return _mixin_data[_type_info->mixin_index(id)].mixin();
}

bool object::internal_has_mixin(mixin_id id) const
{
    return _type_info->has_mixin(id);
}

void object::clear()
{
    BOOST_FOREACH(const mixin_type_info* mixin_info, _type_info->_compact_mixins)
    {
        destroy_mixin(mixin_info->id);
    }

    if(_mixin_data != &null_mixin_data)
    {
        _type_info->dealloc_mixin_data(_mixin_data);
        _mixin_data = &null_mixin_data;
    }

    _type_info = &object_type_info::null();
}

void object::change_type(const object_type_info* new_type, bool manage_mixins /*= false*/)
{
    const object_type_info* old_type = _type_info;
    mixin_data_in_object* old_mixin_data = _mixin_data;
    mixin_data_in_object* new_mixin_data = new_type->alloc_mixin_data();

    BOOST_FOREACH(const mixin_type_info* mixin_info, old_type->_compact_mixins)
    {
        mixin_id id = mixin_info->id;
        if(new_type->has_mixin(id))
        {
            new_mixin_data[new_type->mixin_index(id)].set_buffer(old_mixin_data[old_type->mixin_index(id)].buffer());
        }
        else if(manage_mixins)
        {
            destroy_mixin(id);
        }
    }

    if(old_mixin_data != &null_mixin_data)
    {
        old_type->dealloc_mixin_data(old_mixin_data);
    }

    _type_info = new_type;
    _mixin_data = new_mixin_data;

    if(manage_mixins)
    {
        BOOST_FOREACH(const mixin_type_info* mixin_info, new_type->_compact_mixins)
        {
            size_t index = new_type->mixin_index(mixin_info->id);
            if(!new_mixin_data[index].buffer())
            {
                construct_mixin(mixin_info->id);
            }
        }
    }
}

void object::construct_mixin(mixin_id id)
{
    BOOST_ASSERT(_type_info->has_mixin(id));
    mixin_data_in_object& data = _mixin_data[_type_info->mixin_index(id)];
    BOOST_ASSERT(!data.buffer());

    char* buffer = _type_info->alloc_mixin(id);
    data.set_buffer(buffer);
    data.set_object(this);

    dom()->mixin_info(id).constructor(data.mixin());
}

void object::destroy_mixin(mixin_id id)
{
    BOOST_ASSERT(_type_info->has_mixin(id));
    mixin_data_in_object& data = _mixin_data[_type_info->mixin_index(id)];

    dom()->mixin_info(id).destructor(data.mixin());
    _type_info->dealloc_mixin(id, data.buffer());

    data.clear();
}

bool object::implements_message(feature_id id) const
{
    return !!_type_info->_call_table[id].message_data;
}

}
}
