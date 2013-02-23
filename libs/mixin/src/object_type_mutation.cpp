//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/domain.hpp>
#include <boost/mixin/object_type_mutation.hpp>

using namespace std;

namespace boost
{
namespace mixin
{

using namespace internal;

object_type_mutation::object_type_mutation()
    : _source(nullptr)
{
}

object_type_mutation::object_type_mutation(const mixin_collection* src)
    : _source(src)
{
}

void object_type_mutation::normalize()
{
    // intentionally not sorting both containers and not using set_intersection
    // they should be small so this o(a*b) algorithm should be fine

    mixin_type_info_vector& to_add = _adding._compact_mixins;
    mixin_type_info_vector& to_remove = _removing._compact_mixins;

    for(mixin_type_info_vector::iterator add = to_add.begin(); add!=to_add.end(); ++add)
    {
        for(mixin_type_info_vector::iterator rem = to_remove.begin(); rem!=to_remove.end(); ++rem)
        {
            if(*add == *rem)
            {
                add = to_add.erase(add);
                to_remove.erase(rem);

                // the for ++ will increase add, so decrement it here
                if(add == to_add.end())
                {
                    goto break2;
                }

                break;
            }
        }
    }

break2:

    _adding.rebuild_from_compact_mixins();
    _removing.rebuild_from_compact_mixins();
}

void object_type_mutation::clear()
{
    _adding.clear();
    _removing.clear();
}

void object_type_mutation::check_valid()
{
    domain*& adom = _adding._domain;
    domain*& rdom = _removing._domain;
    domain* src_dom = _source ? _source->dom() : nullptr;

    // if one of the domains is set, set the others

    if(src_dom)
    {
        if(!adom)
        {
            adom = src_dom;
        }
        BOOST_ASSERT(adom == src_dom);

        if(!rdom)
        {
            rdom = src_dom;
        }
        BOOST_ASSERT(rdom == src_dom);
    }
    else if(adom && !rdom)
    {
        rdom = adom;
    }
    else if(rdom && !adom)
    {
        adom = rdom;
    }

    BOOST_ASSERT(adom == rdom);
}

domain* object_type_mutation::dom() const
{
    if(_adding.dom()) return _adding.dom();
    if(_removing.dom()) return _adding.dom();
    if(_source) return _source->dom();

    return nullptr;
}

}
}
