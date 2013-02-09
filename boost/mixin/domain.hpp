//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_DOMAIN_HPP_INCLUDED)
#define _BOOST_MIXIN_DOMAIN_HPP_INCLUDED

#include <boost/mixin/global.hpp>
#include <boost/mixin/mixin_type_info.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/mixin/feature.hpp>
#include <boost/mixin/feature_parser.hpp>
#include <boost/mixin/message.hpp>

#include <boost/ptr_container/ptr_set.hpp>

#if BOOST_MIXIN_USING_CXX11
#   include <unordered_map>
#else
#   include <boost/unordered_map.hpp>
#endif

// domains are a collection of mixins and messages that can be used together
// if you have two independent sets of mixins, use two domains
// for example <game mixins> and <database mixins>
// since no object will have mixins of both gameplay and database
// create two domains

// to create a domain simply define an empty class (this is the domain tag)
// and pass it to BOOST_DEFINE_MIXIN_IN_DOMAIN
// if BOOST_MIXIN_USE_TYPEID is false you need to add the static method get_boost_mixin_name

namespace boost
{
namespace mixin
{

// tag class for the default domain
// not an actual domain object but just a dummy used for its name only
class default_domain
{
#if !BOOST_MIXIN_USE_TYPEID
public:
    static const char* get_boost_mixin_name() { return "default_domain"; }
#endif
};

typedef size_t domain_id;

class feature;

namespace internal
{

typedef std::bitset<BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN> available_mixins_bitset;

#if !BOOST_MIXIN_USING_CXX11
// we need to define a hash function for bitsets
// curiously boost doesn's seem to have one
struct hash_avaliable_mixins_bitset : std::unary_function<available_mixins_bitset, size_t>
{
    size_t operator()(const available_mixins_bitset& bs) const
    {
        const int bits = sizeof(size_t)*8;
        size_t result = 0;
        for(size_t i=0; i<BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN; ++i)
        {
            // xor groups of bits 
            result ^= size_t(bs[i]) << (i % bits);
        }
        return result;
    }
};
#endif

class BOOST_MIXIN_API domain : public noncopyable
{
public:
    domain_id id() const { return _id; }
    const char* name() const { return _name; }

    size_t num_registered_mixins() const { return _num_registered_mixins; }

    template <typename Mixin>
    void register_mixin_type(mixin_type_info& info)
    {
        BOOST_ASSERT(info.id == INVALID_MIXIN_ID);
        BOOST_ASSERT_MSG(_num_registered_mixins < BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN,
                         "you have to increase the maximum number of mixins");

        info.dom = this;
        info.id = _num_registered_mixins;
        info.name = BOOST_MIXIN_TYPE_NAME(Mixin);
        info.size = sizeof(Mixin);
        info.constructor = &call_mixin_constructor<Mixin>;
        info.destructor = &call_mixin_destructor<Mixin>;

        _mixin_type_infos[_num_registered_mixins++] = &info;

        // see comments in feature_instance on why this manual registration is needed
        feature_registrator reg;
        _boost_parse_mixin_features((Mixin*)nullptr, reg);

        feature_parser<Mixin> parser;
        _boost_parse_mixin_features((Mixin*)nullptr, parser);
    }

    template <typename Feature>
    void register_feature(Feature& feature)
    {
        // see comments in feature_instance on why features may be registered multiple times
        if(feature.id != INVALID_FEATURE_ID)
        {
            return;
        }

        feature.dom = this;

        internal_register_feature<Feature>(feature, typename Feature::feature_tag());
    }

    // creates a new type info if needed
    const object_type_info* get_object_type_info(const mixin_type_info_vector& mixins);

boost_mixin_internal:
    ~domain(); // should be private but making ptr_vector<domain> a friend is kind of a hassle
#if !BOOST_MIXIN_TEST // we want to access these when testing
private:
#endif
    template <typename> friend domain& get_domain_for_tag();
    domain(domain_id id, const char* name);
    static domain& create(const char* name); // actual creator function

    domain_id _id;
    const char* _name;

    const mixin_type_info* _mixin_type_infos[BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN];
    size_t _num_registered_mixins;

    const message_t* _messages[BOOST_MIXIN_MAX_MESSAGES_PER_DOMAIN];
    size_t _num_registered_messages;

#if BOOST_MIXIN_USING_CXX11
    typedef std::unordered_map<available_mixins_bitset, object_type_info*> object_type_info_map;
#else
    typedef boost::unordered_map<available_mixins_bitset, object_type_info*, hash_avaliable_mixins_bitset > object_type_info_map;
#endif

    object_type_info_map _object_type_infos;

    // feature registration functions for the supported kinds of features
    template <typename Message>
    void internal_register_feature(Message& m, const message_feature_tag&)
    {
        BOOST_ASSERT_MSG(_num_registered_messages < BOOST_MIXIN_MAX_MESSAGES_PER_DOMAIN,
                         "you have to increase the maximum number of features");

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
};

BOOST_MIXIN_API domain& get_domain(domain_id id);

// bind the domain tag to the actual domain
template <typename domainTag>
domain& get_domain_for_tag()
{
    // bind the domain per name (instead of per type) because
    // this may be instantiated from different modules
    static domain_id id = domain::create(BOOST_MIXIN_TYPE_NAME(domainTag)).id();
    return get_domain(id);
}

} // namespace internal
} // namespace mixin
} // namespace boost

// metafunction that binds a mixin type to its domain tag
// unfortunately it has to be global, because it's specialized by the macro that defines a mixin
// it may or may not be placed in a namespace, and there's no way to make this work in all cases
template <typename T>
struct _boost_mixin_domain_for_type {};

#endif // _BOOST_MIXIN_DOMAIN_HPP_INCLUDED
