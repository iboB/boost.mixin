//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/object_transformer.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/mixin/mixin_type_info.hpp>
#include <boost/mixin/exception.hpp>
#include <boost/mixin/object.hpp>
#include <boost/mixin/domain.hpp>

using namespace std;

namespace boost
{
namespace mixin
{

using namespace internal;

object_transformer::object_transformer(object* o)
    : _object(o)
    , _domain(nullptr)
{
    BOOST_ASSERT(o);
}

object_transformer::~object_transformer()
{
    apply();
}

void object_transformer::apply()
{
    if(_to_add.empty() && _to_remove.empty())
        return; // nothing to do

    mixin_type_info_vector new_type_mixins;
    const mixin_type_info_vector& object_mixins = _object->_type_info->_compact_mixins;
    new_type_mixins.reserve(_to_add.size() + object_mixins.size());

    new_type_mixins = _to_add;

    for(size_t i=0; i<object_mixins.size(); ++i)
    {
        const mixin_type_info_data* mixin_info = object_mixins[i];

        // intentionally using linear search instead of binary
        // cache locality makes it faster for small arrays
        if(!has_elem(_to_remove, mixin_info))
        {
            // elements are part of the new type only if they're not removed
            new_type_mixins.push_back(mixin_info);
        }
    }

    if(new_type_mixins.empty())
    {
        // special case for new empty object
        // remove its domain too
        _object->clear();
    }

    BOOST_ASSERT(_domain);

    sort(new_type_mixins.begin(), new_type_mixins.end());

    for(size_t i=0; i<_to_remove.size(); ++i)
    {
        _object->destroy_mixin(_to_remove[i]->id);
    }

    const object_type_info* new_type = _domain->get_object_type_info(new_type_mixins);
    _object->change_type(new_type);

    for(size_t i=0; i<_to_add.size(); ++i)
    {
        _object->construct_mixin(_to_add[i]->id);
    }

    cancel(); // to go back to initial state
}

void object_transformer::cancel()
{
    _to_add.clear();
    _to_remove.clear();
}

void object_transformer::check_valid_mutation(const internal::mixin_type_info_data& mixin_info)
{
    BOOST_ASSERT(mixin_info.is_valid());
    if(_object->dom())
    {
        BOOST_MIXIN_THROW_UNLESS(_object->dom() == mixin_info.dom, invalid_domain);
        BOOST_ASSERT(_object->dom() == mixin_info.dom);
    }

    if(_domain)
    {
        BOOST_MIXIN_THROW_UNLESS(_domain == mixin_info.dom, invalid_domain);
    }
    else
    {
        _domain = mixin_info.dom;
    }
}

void object_transformer::internal_add(const internal::mixin_type_info_data& mixin_info)
{
    check_valid_mutation(mixin_info);

    if(_object->_type_info->has_mixin(mixin_info.id))
    {
        return; // nothing to add
    }

    // intentionally using linear search instead of binary
    // cache locality makes it faster for small arrays
    if(has_elem(_to_add, &mixin_info))
    {
        return; // already adding
    }

    BOOST_MIXIN_THROW_UNLESS(!has_elem(_to_remove, &mixin_info), invalid_transform);

    _to_add.push_back(&mixin_info);
}

void object_transformer::internal_remove(const internal::mixin_type_info_data& mixin_info)
{
    check_valid_mutation(mixin_info);

    if(!_object->_type_info->has_mixin(mixin_info.id))
    {
        return; // nothing to remove
    }

    // intentionally using linear search instead of binary
    // cache locality makes it faster for small arrays
    if(has_elem(_to_remove, &mixin_info))
    {
        return;  // already removing
    }

    BOOST_MIXIN_THROW_UNLESS(!has_elem(_to_add, &mixin_info), invalid_transform);

    _to_remove.push_back(&mixin_info);
}


}
}
