//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/object_mutator.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/mixin/mixin_type_info.hpp>
#include <boost/mixin/exception.hpp>
#include <boost/mixin/domain.hpp>

namespace boost
{
namespace mixin
{
namespace internal
{

object_mutator::object_mutator()
    : _domain(nullptr)
{
}

object_mutator::object_mutator(domain* dom)
    : _domain(dom)
{
}

void object_mutator::cancel()
{
    _to_add.clear();
    _to_remove.clear();

    // intentionally not setting the domain to nullptr
    // once a mutator is bound to a domain it should stay bound
}

void object_mutator::check_valid_mutation(const internal::mixin_type_info& mixin_info)
{
    BOOST_ASSERT(mixin_info.is_valid());

    if(_domain)
    {
        BOOST_MIXIN_THROW_UNLESS(_domain == mixin_info.dom, invalid_domain);
    }
    else
    {
        _domain = mixin_info.dom;
    }
}

void object_mutator::internal_add(const internal::mixin_type_info& mixin_info)
{
    check_valid_mutation(mixin_info);

    BOOST_ASSERT(_domain);
    // could be that the input parameter has been instantiated from a different module
    // that's why get the info that's actually in our domain
    const internal::mixin_type_info& domain_info = _domain->mixin_info(mixin_info.id);

    // intentionally using linear search instead of binary
    // cache locality makes it faster for small arrays
    if(has_elem(_to_add, &domain_info))
    {
        return; // already adding
    }

    _to_add.push_back(&domain_info);
}

void object_mutator::internal_remove(const internal::mixin_type_info& mixin_info)
{
    check_valid_mutation(mixin_info);

    BOOST_ASSERT(_domain);
    // could be that the input parameter has been instantiated from a different module
    // that's why get the info that's actually in our domain
    const internal::mixin_type_info& domain_info = _domain->mixin_info(mixin_info.id);

    // intentionally using linear search instead of binary
    // cache locality makes it faster for small arrays
    if(has_elem(_to_remove, &domain_info))
    {
        return;  // already removing
    }

    _to_remove.push_back(&domain_info);
}

}
}
}
