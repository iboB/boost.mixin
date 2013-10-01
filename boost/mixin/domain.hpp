//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_DOMAIN_HPP_INCLUDED)
#define _BOOST_MIXIN_DOMAIN_HPP_INCLUDED

#include <boost/mixin/global.hpp>
#include <boost/mixin/mixin_type_info.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/mixin/feature.hpp>
#include <boost/mixin/feature_parser.hpp>
#include <boost/mixin/message.hpp>

#include <boost/ptr_container/ptr_set.hpp>
#include <boost/type_traits/alignment_of.hpp>

#if BOOST_MIXIN_USING_CXX11
#   include <unordered_map>
#else
#   include <boost/unordered_map.hpp>
#endif

/**
 * \file
 * Domain related classes and functions.
 */

// domains are a collection of mixins and messages that can be used together
// if you have two independent sets of mixins, use two domains
// for example <game mixins> and <database mixins>
// since no object will have mixins of both gameplay and database
// create two domains

// to create a domain simply define an empty class (this is the domain tag)
// and pass it to BOOST_DEFINE_MIXIN_IN_DOMAIN
// if BOOST_MIXIN_USE_TYPEID is false you need to add the static method boost_mixin_name

namespace boost
{
namespace mixin
{


/**
 * Tag class for the default domain.
 *
 * A tag class is not an actual domain object but just a dummy
 * used for its symbol name only
 */
class default_domain
{
#if !BOOST_MIXIN_USE_TYPEID
public:
    /// The domain name function in case `BOOST_MIXIN_USE_TYPEID` is not defined
    static const char* boost_mixin_name() { return "default_domain"; }
#endif
};

typedef size_t domain_id;

class mutation_rule;
class object_type_mutation;
class domain_allocator;

namespace internal
{

struct message_t;

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

    void add_new_mutation_rule(mutation_rule* rule);
    void apply_mutation_rules(object_type_mutation& mutation);

    size_t num_registered_mixins() const { return _num_registered_mixins; }

    template <typename Mixin>
    void register_mixin_type(mixin_type_info& info)
    {
        BOOST_ASSERT(info.id == INVALID_MIXIN_ID);
        BOOST_ASSERT_MSG(_num_registered_mixins < BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN,
                         "you have to increase the maximum number of mixins");

        info.dom = this;
        info.name = BOOST_MIXIN_TYPE_NAME(Mixin);
        info.size = sizeof(Mixin);
        info.alignment = alignment_of<Mixin>::value;
        info.constructor = &call_mixin_constructor<Mixin>;
        info.destructor = &call_mixin_destructor<Mixin>;
        info.allocator = _own_allocator;

        internal_register_mixin_type(info);

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

        internal_register_feature(feature);
    }

    // creates a new type info if needed
    const object_type_info* get_object_type_info(const mixin_type_info_vector& mixins);

    const mixin_type_info& mixin_info(mixin_id id) const
    {
        BOOST_ASSERT(id != INVALID_MIXIN_ID);
        BOOST_ASSERT(id <= _num_registered_mixins);
        BOOST_ASSERT(_mixin_type_infos[id]);

        return *_mixin_type_infos[id];
    }

    // sets the current domain allocator
    void set_allocator(domain_allocator* allocator);
    domain_allocator* allocator() const { return _own_allocator; }

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

    mixin_type_info* _mixin_type_infos[BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN];
    size_t _num_registered_mixins;

    void internal_register_mixin_type(mixin_type_info& info);

    const message_t* _messages[BOOST_MIXIN_MAX_MESSAGES_PER_DOMAIN];
    size_t _num_registered_messages;

#if BOOST_MIXIN_USING_CXX11
    typedef std::unordered_map<available_mixins_bitset, object_type_info*> object_type_info_map;
#else
    typedef boost::unordered_map<available_mixins_bitset, object_type_info*, hash_avaliable_mixins_bitset > object_type_info_map;
#endif

    object_type_info_map _object_type_infos;

    std::vector<mutation_rule*> _mutation_rules;

    // feature registration functions for the supported kinds of features
    void internal_register_feature(message_t& m);

    // allocators
    domain_allocator* _own_allocator; // domain-specific allocator (usually equal to global_allocator
};

BOOST_MIXIN_API domain& get_domain(domain_id id);

// bind the domain tag to the actual domain
template <typename DomainTag>
domain& get_domain_for_tag()
{
    // bind the domain per name (instead of per type) because
    // this may be instantiated from different modules
    static domain_id id = domain::create(BOOST_MIXIN_TYPE_NAME(DomainTag)).id();
    return get_domain(id);
}

} // namespace internal

// allocator functions

/// Sets an allocator to all domains
void BOOST_MIXIN_API set_global_domain_allocator(domain_allocator* allocator);

/// Set an allocator to the default domain, only
void BOOST_MIXIN_API set_default_domain_allocator(domain_allocator* allocator);

/// Sets an allocator to a specific domain, identified by its tag
template <typename DomainTag>
void set_allocator_for_domain(domain_allocator* allocator)
{
    internal::get_domain_for_tag<DomainTag>().set_allocator(allocator);
}

} // namespace mixin
} // namespace boost

// metafunction that binds a mixin type to its domain tag
// unfortunately it has to be global, because it's specialized by the macro that defines a mixin
// it may or may not be placed in a namespace, and there's no way to make this work in all cases
template <typename T>
struct _boost_mixin_domain_for_type {};

#endif // _BOOST_MIXIN_DOMAIN_HPP_INCLUDED
