//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED)
#define _BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED

/**
 * \file
 * Defines internal classes that contain the type information for an object -
 * mixins, implemented features, etc.
 */

#include "global.hpp"
#include "mixin_collection.hpp"
#include "message.hpp"

// object type info is an immutable class that represents the type information for a
// group of objects

namespace boost
{
namespace mixin
{

class object_mutator;
class object;

namespace internal
{

class mixin_data_in_object;

class BOOST_MIXIN_API object_type_info : private mixin_collection
{
public:
    object_type_info();
    ~object_type_info();

    using mixin_collection::has;

    const mixin_collection* as_mixin_collection() const { return this; }

    size_t mixin_index(mixin_id id) const { return _mixin_indices[id]; }

    static const object_type_info& null();

    mixin_data_in_object* alloc_mixin_data() const;
    void dealloc_mixin_data(mixin_data_in_object* data) const;

    void generate_call_table();

boost_mixin_internal:
    using mixin_collection::_mixins;
    using mixin_collection::_compact_mixins;

    // indices in the _compact_mixins member
    size_t _mixin_indices[BOOST_MIXIN_MAX_MIXINS];

    struct call_table_entry
    {
        union
        {
            // this view is for unicast messages
            // it will contain the message with the highest priority
            struct
            {
                const message_for_mixin* message_data;
            };

            // this view is used for multicast messages
            // it will contain a dynamically allocated array of entries sorted by priority
            struct
            {
                call_table_entry* multicast_begin;
                call_table_entry* multicast_end;
            };
        };

    };

    call_table_entry* _multicast_buffer; // a single buffer for all multicast messages to save allocation calls
    call_table_entry _call_table[BOOST_MIXIN_MAX_MESSAGES];

    // this should be called after the mixins have been initialized
    void fill_call_table();
};

// represents the mixin data in an object
class mixin_data_in_object
{
public:
    mixin_data_in_object()
        : _buffer(nullptr)
        , _mixin(nullptr)
    {
    }

    void set_buffer(char* buffer, size_t mixin_offset)
    {
        BOOST_ASSERT(buffer);
        BOOST_ASSERT(mixin_offset >= sizeof(object*));
        _buffer = buffer;
        _mixin = buffer + mixin_offset;
    }

    void set_object(object* o)
    {
        BOOST_ASSERT(o);
        BOOST_ASSERT(_buffer);
        object** data_as_objec_ptr = reinterpret_cast<object**>(_mixin - sizeof(object*));
        *data_as_objec_ptr = o;
    }

    void clear()
    {
        _buffer = nullptr;
        _mixin = nullptr;
    }

    char* buffer() { return _buffer; }
    void* mixin() { return _mixin; }
    const char* buffer() const { return _buffer; }
    const void* mixin() const { return _mixin; }
private:
    char* _buffer;
    char* _mixin;
};

}
}
}


#endif // _BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED
