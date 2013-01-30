//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED)
#define BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED

#include "global.hpp"
#include "message.hpp"

// object type info is an immutable class that represents the type information for a
// group of objects

namespace boost
{
namespace mixin
{

class object_transformer;
class object;

namespace internal
{

class domain;
class mixin_data_in_object;

class BOOST_MIXIN_API object_type_info : public noncopyable
{
public:
    object_type_info();
    ~object_type_info();

    static const object_type_info& null();

    domain* dom() const { return _domain; }

    bool has_mixin(mixin_id id) const { return !!_mixins[id]; }
    const mixin_type_info& mixin_info(mixin_id id) const { return *_mixins[id]; }
    size_t mixin_index(mixin_id id) const { return _mixin_indices[id]; }

    mixin_data_in_object* alloc_mixin_data() const;
    void dealloc_mixin_data(mixin_data_in_object* data) const;

    char* alloc_mixin(mixin_id id) const;
    void dealloc_mixin(mixin_id id, char* mem) const;

    void generate_call_table();

boost_mixin_internal:
    domain* _domain; // owning domain

    // not available mixins are null
    size_t _mixin_indices[BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN];
    const mixin_type_info* _mixins[BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN];
    // only the mixins the objects of this type have
    mixin_type_info_vector _compact_mixins;

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
            // it will contain a dynamically allocated array of entries sorte by priority
            struct
            {
                call_table_entry* multicast_begin;
                call_table_entry* multicast_end;
            };
        };

    };

    call_table_entry* _multicast_buffer; // a single buffer for all multicast messages to save allocation calls
    call_table_entry _call_table[BOOST_MIXIN_MAX_MESSAGES_PER_DOMAIN];

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

    void set_buffer(char* buffer)
    {
        BOOST_ASSERT(buffer);
        _buffer = buffer;
        _mixin = buffer + sizeof(object*);
    }

    void set_object(object* o)
    {
        BOOST_ASSERT(o);
        BOOST_ASSERT(_buffer);
        object** data_as_objec_ptr = reinterpret_cast<object**>(_buffer);
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
    void* _mixin;
};

}
}
}


#endif // BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED
