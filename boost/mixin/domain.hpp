//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_DOMAIN_HPP_INCLUDED)
#define BOOST_MIXIN_DOMAIN_HPP_INCLUDED

#include <boost/mixin/global.hpp>
#include <boost/mixin/mixin_type_info.hpp>
#include <boost/mixin/object_type_info.hpp>

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

namespace internal
{

class BOOST_MIXIN_API domain : public noncopyable
{
public:
    domain_id id() const { return _id; }
    const char* name() const { return _name; }

    size_t num_mixins() const { return _num_mixins; }

    template <typename Mixin>
    void register_mixin_type(mixin_type_info_data& data)
    {
        BOOST_ASSERT(data.id == INVALID_MIXIN_ID);
        BOOST_ASSERT_MSG(_num_mixins <= BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN, "you have to increase the maximal number of mixins");

        data.dom = this;
        data.id = _num_mixins;
        data.name = BOOST_MIXIN_TYPE_NAME(Mixin);
        data.size = sizeof(Mixin);
        data.constructor = &call_mixin_constructor<Mixin>;
        data.destructor = &call_mixin_destructor<Mixin>;

        _mixins[_num_mixins++] = &data;
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

    const mixin_type_info_data* _mixins[BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN];
    size_t _num_mixins;

#if BOOST_MIXIN_USING_CXX11
    typedef std::unordered_map<available_mixins_bitset, object_type_info*> object_infos_hash_map;
#else
    typedef boost::unordered_map<available_mixins_bitset, object_type_info*> object_infos_hash_map;
#endif

    object_infos_hash_map _types;
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

#endif //BOOST_MIXIN_DOMAIN_HPP_INCLUDED
