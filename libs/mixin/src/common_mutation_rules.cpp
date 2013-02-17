//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/common_mutation_rules.hpp>
#include <boost/mixin/object_type_mutation.hpp>
#include <boost/foreach.hpp>

namespace boost
{
namespace mixin
{

using namespace internal;

void mutually_exclusive_mixins::apply_to(object_type_mutation& mutation)
{
    // find if the mutation is adding any of the mutually exclusive mixins

    mixin_id adding = INVALID_MIXIN_ID;

    BOOST_FOREACH(const mixin_type_info* mixin_info, _compact_mixins)
    {
        if(mutation.is_adding(mixin_info->id))
        {
            BOOST_ASSERT_MSG(adding == INVALID_MIXIN_ID, "mutation breaking a mutually exclusive mixin rule");
            adding = mixin_info->id;
#if !defined(BOOST_MIXIN_DEBUG)
            break;
#endif
        }
    }

    //
    // what do we do if more than one of the mutually exclusive mixins is being added???
    //

    if(adding == INVALID_MIXIN_ID)
    {
        // nothing to do here
        return;
    }

    // find the object has one of the mutually exclusive mixins
    BOOST_FOREACH(const mixin_type_info* mixin_info, _compact_mixins)
    {
        if(mixin_info->id == adding)
        {
            continue;
        }

        if(mutation.source_has(mixin_info->id))
        {
            // remove all others from the object
            mutation.start_removing(mixin_info->id);
        }
    }
}

void mandatory_mixin::apply_to(object_type_mutation& mutation)
{
    if(mutation.is_removing(_id))
    {
        mutation.stop_removing(_id);
    }

    if(mutation.source_has(_id))
        return;

    mutation.start_adding(_id);
}

void deprecated_mixin::apply_to(object_type_mutation& mutation)
{
    if(mutation.source_has(_id))
    {
        mutation.start_removing(_id);
    }

    if(mutation.is_adding(_id))
    {
        mutation.stop_adding(_id);
    }
}

}
}
