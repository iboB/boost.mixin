//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/domain.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace boost
{
namespace mixin
{
namespace internal
{

typedef ptr_vector<domain> domains_container;

// using a function instead of a simple global variable because this guarantees that the container
// is constructed when the function is called
static domains_container& all_domains()
{
    static ptr_vector<domain> container;
    return container;
}

domain& domain::create(const char* domain_name)
{
    domains_container& domains = all_domains();
    if(domains.empty())
    {
        // make sure that the default_domain is registered first
        domains.push_back(new domain(0, BOOST_MIXIN_TYPE_NAME(default_domain)));
    }

    // check if we haven't already created this domain
    for(domains_container::iterator r=domains.begin(); r!=domains.end(); ++r)
    {
        // domain re-registration
        // we have no guarantee that BOOST_MIXIN_TYPE_NAME returns the same address every time
        // that's why we use strcmp and not just ==
        if(strcmp(r->_name, domain_name) == 0)
            return *r;
    }

    domain* new_domain = new domain(domains.size(), domain_name);
    domains.push_back(new_domain);

    return domains.back();
}

domain& get_domain(domain_id id)
{
    BOOST_ASSERT(id < all_domains().size());
    return all_domains()[id];
}

domain::domain(domain_id id, const char* name)
    : _id(id)
    , _name(name)
    , _num_registered_mixins(0)
{
    zero_memory(_mixin_type_infos, sizeof(_mixin_type_infos));
}

domain::~domain()
{
    // why is there no boost::ptr_unordered_map ?
    for(object_type_info_map::iterator i=_object_type_infos.begin(); i!=_object_type_infos.end(); ++i)
    {
        delete i->second;
    }
}

const object_type_info* domain::get_object_type_info(const mixin_type_info_vector& mixins)
{
    BOOST_ASSERT(is_sorted(mixins));

    available_mixins_bitset query;

    for(size_t i=0; i<mixins.size(); ++i)
    {
        BOOST_ASSERT(mixins[i]->dom == this);
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
        new_type->_domain = this;
        new_type->_compact_mixins = mixins;

        for(size_t i=0; i<mixins.size(); ++i)
        {
            new_type->_mixins[mixins[i]->id] = mixins[i];
            new_type->_mixin_indices[mixins[i]->id] = i + 1; // reserve mixin index 0 for nullptr
        }

        _object_type_infos.insert(make_pair(query, new_type));
        return new_type;
    }
}

}
}
}
