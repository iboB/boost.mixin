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

object_transformer::object_transformer(object& o)
    : _object(&o)
    , _domain(nullptr)
{
}

object_transformer::~object_transformer()
{
    apply();
}

void object_transformer::apply()
{
    // normalize _to_add and _to_remove
    // that is, if an element is in both arrays it will be removed from them

    // intentionally not sorting both containers and not using set_intersection
    // they should be small so this o(a*b) algorithm should be fine

    for(mixin_type_info_vector::iterator add = _to_add.begin(); add!=_to_add.end(); ++add)
    {
        for(mixin_type_info_vector::iterator rem = _to_remove.begin(); rem!=_to_remove.end(); ++rem)
        {
            if(*add == *rem)
            {
                add = _to_add.erase(add);
                _to_remove.erase(rem);

                // the for ++ will increase add, so decrement it here
                if(add == _to_add.end())
                {
                    goto break2;
                }

                break;
            }
        }
    }

break2:

    if(_to_add.empty() && _to_remove.empty())
    {
        cancel();
        return; // nothing to do
    }

    mixin_type_info_vector new_type_mixins;
    const mixin_type_info_vector& object_mixins = _object->_type_info->_compact_mixins;
    new_type_mixins.reserve(_to_add.size() + object_mixins.size());

    new_type_mixins = _to_add;

    for(size_t i=0; i<object_mixins.size(); ++i)
    {
        const mixin_type_info* mixin_info = object_mixins[i];

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
        cancel();
        return;
    }

    BOOST_ASSERT(_domain);

    sort(new_type_mixins.begin(), new_type_mixins.end());

    for(size_t i=0; i<_to_remove.size(); ++i)
    {
        if(_object->internal_has_mixin(_to_remove[i]->id))
            _object->destroy_mixin(_to_remove[i]->id);
    }

    const object_type_info* new_type = _domain->get_object_type_info(new_type_mixins);

    // since we allow adding of existing mixins it could be that this new type is
    // actually the object's current type
    if(_object->_type_info == new_type)
    {
        cancel();
        return;
    }

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

void object_transformer::check_valid_mutation(const internal::mixin_type_info& mixin_info)
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

void object_transformer::internal_add(const internal::mixin_type_info& mixin_info)
{
    check_valid_mutation(mixin_info);

    BOOST_ASSERT(_domain);
    // could be that the input parameter has been instantiated from a different module
    // that's why get the info that's actually in our domain
    const internal::mixin_type_info& domain_info = _domain->mixin_info(mixin_info.id);

    // intentionally not checking if the object already has this mixin

    // intentionally using linear search instead of binary
    // cache locality makes it faster for small arrays
    if(has_elem(_to_add, &domain_info))
    {
        return; // already adding
    }

    _to_add.push_back(&domain_info);
}

void object_transformer::internal_remove(const internal::mixin_type_info& mixin_info)
{
    check_valid_mutation(mixin_info);

    BOOST_ASSERT(_domain);
    // could be that the input parameter has been instantiated from a different module
    // that's why get the info that's actually in our domain
    const internal::mixin_type_info& domain_info = _domain->mixin_info(mixin_info.id);

    // intentionally not checking if the object even has this mixin

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
