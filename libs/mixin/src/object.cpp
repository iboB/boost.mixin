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
    type.apply_to(*this);
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
    return _type_info->has(id);
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

void object::change_type(const object_type_info* new_type, bool manage_mixins /*= true*/)
{
    const object_type_info* old_type = _type_info;
    mixin_data_in_object* old_mixin_data = _mixin_data;
    mixin_data_in_object* new_mixin_data = new_type->alloc_mixin_data();

    BOOST_FOREACH(const mixin_type_info* mixin_info, old_type->_compact_mixins)
    {
        mixin_id id = mixin_info->id;
        if(new_type->has(id))
        {
            new_mixin_data[new_type->mixin_index(id)] = old_mixin_data[old_type->mixin_index(id)];
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
    BOOST_ASSERT(_type_info->has(id));
    mixin_data_in_object& data = _mixin_data[_type_info->mixin_index(id)];
    BOOST_ASSERT(!data.buffer());

    const mixin_type_info& mixin_info = dom()->mixin_info(id);

    char* buffer;
    size_t mixin_offset;
#if defined(BOOST_MIXIN_DEBUG)
    buffer = nullptr;
    mixin_offset = 0;
#endif
    mixin_info.allocator->alloc_mixin(mixin_info.size, mixin_info.alignment, buffer, mixin_offset);

    BOOST_ASSERT(buffer);
    BOOST_ASSERT(mixin_offset >= sizeof(object*)); // we should have room for an object pointer

    data.set_buffer(buffer, mixin_offset);
    data.set_object(this);

    dom()->mixin_info(id).constructor(data.mixin());
}

void object::destroy_mixin(mixin_id id)
{
    BOOST_ASSERT(_type_info->has(id));
    mixin_data_in_object& data = _mixin_data[_type_info->mixin_index(id)];

    const mixin_type_info& mixin_info = dom()->mixin_info(id);

    mixin_info.destructor(data.mixin());

    // dealocate mixin
    mixin_info.allocator->dealloc_mixin(data.buffer());

    data.clear();
}

bool object::implements_message(feature_id id) const
{
    return !!_type_info->_call_table[id].message_data;
}

}
}
