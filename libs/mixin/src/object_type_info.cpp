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
#include <boost/mixin/allocators.hpp>

namespace boost
{
namespace mixin
{
namespace internal
{

object_type_info::object_type_info()
    : _multicast_buffer(nullptr)
{
    zero_memory(_mixin_indices, sizeof(_mixin_indices));
    zero_memory(_call_table, sizeof(_call_table));
}

object_type_info::~object_type_info()
{
    delete[] _multicast_buffer;
}

const object_type_info& object_type_info::null()
{
    static const object_type_info n;
    return n;
};

mixin_data_in_object* object_type_info::alloc_mixin_data() const
{
    BOOST_ASSERT(_domain);

    size_t num_to_allocate = _compact_mixins.size() + 1; //reserve idnex 0 for nullptr

    char* memory = _domain->allocator()->alloc_mixin_data(num_to_allocate);
    mixin_data_in_object* ret = new (memory) mixin_data_in_object[num_to_allocate];

    return ret;
}

void object_type_info::dealloc_mixin_data(mixin_data_in_object* data) const
{
    for(size_t i=0; i<_compact_mixins.size() + 1; ++i)
    {
        data[i].~mixin_data_in_object();
    }

    BOOST_ASSERT(_domain);
    _domain->allocator()->dealloc_mixin_data(reinterpret_cast<char*>(data));
}

struct bigger_message_priority
{
    bool operator()(const object_type_info::call_table_entry& a, const object_type_info::call_table_entry& b)
    {
        return b.message_data->priority < a.message_data->priority;
    }
};

void object_type_info::fill_call_table()
{
    // first pass
    // set unicast messages and determine total multicast length
    int total_multicast_length = 0;

    for(size_t i=0; i<_compact_mixins.size(); ++i)
    {
        const mixin_type_info& info = *_compact_mixins[i];

        for(size_t j=0; j<info.message_infos.size(); ++j)
        {
            const message_for_mixin& msg = info.message_infos[j];
            call_table_entry& table_entry = _call_table[msg.message->id];

            if(msg.message->mechanism == message_t::unicast)
            {
                if((!table_entry.message_data) || (table_entry.message_data->priority < msg.priority))
                {
                    table_entry.message_data = &msg;
                }
            }
            else if(msg.message->mechanism == message_t::multicast)
            {
                ++total_multicast_length;
                BOOST_ASSERT(table_entry.multicast_begin == nullptr);
                ++table_entry.multicast_end; // hacky count number of clients for this specific multicast
            }
        }
    }

    _multicast_buffer = new call_table_entry[total_multicast_length];
    call_table_entry* multicast_buffer_ptr = _multicast_buffer;

    // second pass
    // set first multicast messages in buffer
    for(size_t i=0; i<_compact_mixins.size(); ++i)
    {
        const mixin_type_info& info = *_compact_mixins[i];

        for(size_t j=0; j<info.message_infos.size(); ++j)
        {
            const message_for_mixin& msg = info.message_infos[j];
            call_table_entry& table_entry = _call_table[msg.message->id];

            if(msg.message->mechanism == message_t::multicast)
            {
                if(!table_entry.multicast_begin)
                {
                    table_entry.multicast_begin = multicast_buffer_ptr;
                    table_entry.multicast_begin->message_data = &msg;

                    BOOST_ASSERT(table_entry.multicast_end);
                    // increase the pointer with the count from the previous pass
                    multicast_buffer_ptr += reinterpret_cast<size_t>(table_entry.multicast_end)/sizeof(call_table_entry);
                    BOOST_ASSERT(multicast_buffer_ptr - _multicast_buffer <= total_multicast_length);

                    // set the end to its supposed value
                    table_entry.multicast_end = table_entry.multicast_begin;
                    ++table_entry.multicast_end;
                }
            }
        }
    }

    // third pass
    // fill the rest of the multicast messages in the buffer
    for(size_t i=0; i<_compact_mixins.size(); ++i)
    {
        const mixin_type_info& info = *_compact_mixins[i];

        for(size_t j=0; j<info.message_infos.size(); ++j)
        {
            const message_for_mixin& msg = info.message_infos[j];
            call_table_entry& table_entry = _call_table[msg.message->id];

            if(msg.message->mechanism == message_t::multicast)
            {
                BOOST_ASSERT(table_entry.multicast_begin);
                BOOST_ASSERT(table_entry.multicast_end);

                // message hasn't been inserted on previous pass
                if(table_entry.multicast_begin->message_data != &msg)
                {
                    table_entry.multicast_end->message_data = &msg;
                    ++table_entry.multicast_end;
                }
            }
        }
    }

    // final pass through all messages of the domain
    // if we implement it AND it is a multicast, sort our buffer
    BOOST_ASSERT(_domain);
    for(size_t i=0; i<_domain->_num_registered_messages; ++i)
    {
        if(_domain->_messages[i]->mechanism == message_t::multicast)
        {
            call_table_entry& table_entry = _call_table[i];

            if(!table_entry.message_data)
            {
                // we dont implement it so we don't care
                continue;
            }

            BOOST_ASSERT(table_entry.multicast_begin);
            BOOST_ASSERT(table_entry.multicast_end);

            std::sort(table_entry.multicast_begin, table_entry.multicast_end, bigger_message_priority());
        }
    }

}

}
}
}

