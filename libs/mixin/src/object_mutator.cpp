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
#include <boost/mixin/object.hpp>
#include <boost/foreach.hpp>

namespace boost
{
namespace mixin
{
namespace internal
{

object_mutator::object_mutator()
    : _target_type_info(nullptr)
    , _is_created(false)
{
}

object_mutator::object_mutator(const mixin_collection* source_mixins)
    : _mutation(source_mixins)
    , _target_type_info(nullptr)
    , _is_created(false)
{
}

void object_mutator::cancel()
{
    _mutation.clear();
    _is_created = false;
}

void object_mutator::create()
{
    BOOST_ASSERT(_mutation._source);

    if(_is_created)
    {
        return;
    }
    _is_created = true;

    _mutation.normalize();

    // pass the mutation through all the mutation rules here

    // in case the rules broke it somehow
    _mutation.normalize();

    if(_mutation.empty())
    {
        // nothing to do
        return;
    }

    mixin_type_info_vector new_type_mixins;
    const mixin_type_info_vector& old_mixins = _mutation._source->_compact_mixins;
    new_type_mixins.reserve(_mutation._adding._compact_mixins.size() + old_mixins.size());

    new_type_mixins = _mutation._adding._compact_mixins;

    for(size_t i=0; i<old_mixins.size(); ++i)
    {
        const mixin_type_info* mixin_info = old_mixins[i];

        // intentionally using linear search instead of binary
        // cache locality makes it faster for small arrays
        if(!has_elem(_mutation._removing._compact_mixins, mixin_info))
        {
            // elements are part of the new type only if they're not removed
            new_type_mixins.push_back(mixin_info);
        }
    }

    if(new_type_mixins.empty())
    {
        _target_type_info = &object_type_info::null();
        return;
    }

    sort(new_type_mixins.begin(), new_type_mixins.end());

    domain* dom = new_type_mixins.front()->dom;
    _target_type_info = dom->get_object_type_info(new_type_mixins);

    if(_target_type_info->as_mixin_collection() == _mutation._source)
    {
        // since we allow adding of existing mixins it could be that this new type is
        // actually the mutatee's type
        _target_type_info = nullptr;
        return;
    }
}

void object_mutator::apply_to(object* obj) const
{
    BOOST_ASSERT(_is_created);
    BOOST_ASSERT(_mutation._source);
    // we need to mudate only objects of the same type
    BOOST_ASSERT(obj->_type_info->as_mixin_collection() == _mutation._source);
    // shouldn't be trying to set the same type info
    BOOST_ASSERT(obj->_type_info != _target_type_info);

    if(!_target_type_info)
    {
        // this is an empty mutation
        return;
    }

    if(_target_type_info == &object_type_info::null())
    {
        obj->clear();
        return;
    }

    BOOST_FOREACH(const mixin_type_info* rem, _mutation._removing._compact_mixins)
    {
        // we allow removing of mixins that aren't even there
        if(obj->internal_has_mixin(rem->id))
            obj->destroy_mixin(rem->id);
    }

    obj->change_type(_target_type_info, false);

    BOOST_FOREACH(const mixin_type_info* add, _mutation._adding._compact_mixins)
    {
        // we allow adding mixins that are already there
        if(!obj->internal_get_mixin(add->id))
            obj->construct_mixin(add->id);
    }
}

}
}
}
