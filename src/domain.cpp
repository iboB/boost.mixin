//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/domain.hpp>
#include <boost/mixin/mutation_rule.hpp>
#include <boost/mixin/allocators.hpp>

namespace boost
{
namespace mixin
{

no_features_t* none;

namespace internal
{

BOOST_MIXIN_API default_allocator the_default_allocator;

domain& domain::instance()
{
    static domain the_domain;
    return the_domain;
}

domain::domain()
    : _num_registered_mixins(0)
    , _num_registered_messages(0)
    , _allocator(&the_default_allocator)
{
    zero_memory(_mixin_type_infos, sizeof(_mixin_type_infos));
    zero_memory(_messages, sizeof(_messages));
}

domain::~domain()
{
    // why is there no boost::ptr_unordered_map ?
    for(object_type_info_map::iterator i=_object_type_infos.begin(); i!=_object_type_infos.end(); ++i)
    {
        delete i->second;
    }

    for(size_t i=0; i<_mutation_rules.size(); ++i)
    {
        delete _mutation_rules[i];
    }
}

void domain::add_new_mutation_rule(mutation_rule* rule)
{
    _mutation_rules.push_back(rule);
}

void domain::apply_mutation_rules(object_type_mutation& mutation)
{
    for(size_t i=0; i<_mutation_rules.size(); ++i)
    {
        _mutation_rules[i]->apply_to(mutation);
    }
}

const object_type_info* domain::get_object_type_info(const mixin_type_info_vector& mixins)
{
    // the mixin type infos need to be sorted
    // so as to guarantee that two object type infos of the same mixins
    // will have the exact same content
    BOOST_ASSERT(is_sorted(mixins));

    available_mixins_bitset query;

    for(size_t i=0; i<mixins.size(); ++i)
    {
        query[mixins[i]->id] = true;
    }

    object_type_info_map::iterator i = _object_type_infos.find(query);

    if(i != _object_type_infos.end())
    {
        // get existing
        BOOST_ASSERT(mixins == i->second->_compact_mixins);
        return i->second;
    }
    else
    {
        // create object type info
        object_type_info* new_type = new object_type_info;
        new_type->_compact_mixins = mixins;

        for(size_t i=0; i<mixins.size(); ++i)
        {
            BOOST_ASSERT(mixins[i]);
            new_type->_mixins[mixins[i]->id] = true;
            new_type->_mixin_indices[mixins[i]->id] = i + object_type_info::MIXIN_INDEX_OFFSET;
        }

        new_type->fill_call_table();

        _object_type_infos.insert(make_pair(query, new_type));
        return new_type;
    }
}

void domain::internal_register_feature(message_t& m)
{
    BOOST_ASSERT_MSG(_num_registered_messages < BOOST_MIXIN_MAX_MESSAGES,
                        "you have to increase the maximum number of messages");

    // the messages can be instantiated from different modules
    // for example if different modules use the same static library
    //
    // we need see if we don't already have a message by that name
    // if we do we'll set this instantiation's id to the already existing one
    // and disregard this instantiation
    //
    // HERE IT GETS DANGEROUS
    // there is a possiblity to have messages of the same name, but for totally different methods
    // if this happened in the same module, the linker wouldn't allow it
    // the PRIVATE_MESSAGE macro will define the messages that are used within a single module
    // however if one misses it
    // and there happen to be messages of the same name, but for different methids,
    // registered within a single domain from different modules
    // crashes may ensue (as a message gets called, for objects that can't actually handle it)

    if(!m.is_private)
    {
        // check for message of the same name
        for(size_t i=0; i<_num_registered_messages; ++i)
        {
            const message_t& registered_message = *_messages[i];

            BOOST_ASSERT(registered_message.id != INVALID_FEATURE_ID); // how could this happen?
            if(strcmp(m.name, registered_message.name) == 0)
            {
                // already registered from a different module

                // at least check if the mechanism is the same
                BOOST_ASSERT_MSG(m.mechanism == registered_message.mechanism,
                    "Attempting to register a message that has already been registered "
                    "from a different module with a different mechanism");

                // here we'll have to assume that's the same message
                m.id = registered_message.id;
                return;
            }
        }
    }
    // if the message is private treat it as an unrelated different message
    // although it has the same name

    m.id = _num_registered_messages;

    _messages[_num_registered_messages++] = &m;
}

void domain::internal_register_mixin_type(mixin_type_info& info)
{
    // as is the case with messages, multiple modules may turn out
    // trying to register the same mixin over again
    // due to module specific template instantiation
    // check if we already have this mixin registered

    for(size_t i=0; i<_num_registered_mixins; ++i)
    {
        const mixin_type_info& registered = *_mixin_type_infos[i];
        BOOST_ASSERT(registered.id != INVALID_MIXIN_ID);

        if(strcmp(info.name, registered.name) == 0)
        {
            // we have this mixin registered

            BOOST_ASSERT_MSG(registered.size == info.size,
                "trying to register a mixin with the name of an existing mixin");

            info.id = registered.id;
            return;
        }
    }

    info.id = _num_registered_mixins;
    _mixin_type_infos[_num_registered_mixins++] = &info;
}

void domain::set_allocator(global_allocator* allocator)
{
    BOOST_ASSERT(!_allocator || !_allocator->has_allocated());

    for(size_t i=0; i<_num_registered_mixins; ++i)
    {
        mixin_type_info& registered = *_mixin_type_infos[i];
        if(registered.allocator == _allocator)
        {
            // set the new domain allocator to all mixins that don't
            // have a specific one of their own
            registered.allocator = allocator;
        }
    }

    _allocator = allocator;
}

mixin_id domain::get_mixin_id_by_name(const char* mixin_name) const
{
    for(size_t i=0; i<_num_registered_mixins; ++i)
    {
        const mixin_type_info& registered = *_mixin_type_infos[i];

        if(strcmp(mixin_name, registered.name) == 0)
        {
            return registered.id;
        }
    }

    // no mixin of this name found
    return INVALID_MIXIN_ID;
}

} // namespace internal

void add_new_mutation_rule(mutation_rule* rule)
{
    internal::domain::instance().add_new_mutation_rule(rule);
}

// set allocator to all domains
void set_global_allocator(global_allocator* allocator)
{
    internal::domain::instance().set_allocator(allocator);
}

}
}
