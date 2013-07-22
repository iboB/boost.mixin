//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/single_object_mutator.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/mixin/mixin_type_info.hpp>
#include <boost/mixin/exception.hpp>
#include <boost/mixin/domain.hpp>
#include <boost/mixin/object.hpp>

using namespace std;

namespace boost
{
namespace mixin
{

using namespace internal;

single_object_mutator::single_object_mutator(object* o)
    : _object(*o)
    , _is_manually_applied(false)
{
    BOOST_ASSERT(o);
}

single_object_mutator::single_object_mutator(object& o)
    : _object(o)
    , _is_manually_applied(false)
{
}

single_object_mutator::~single_object_mutator()
{
    if(!_is_manually_applied)
    {
        // allow single line mutations
        apply();
    }
}

void single_object_mutator::apply()
{
    _mutation.set_source(_object._type_info->as_mixin_collection());
    create();
    apply_to(_object);
    cancel(); // to go back to empty state
    _mutation.set_source(nullptr); // really empty
    _is_manually_applied = true;
}

} // namespace mixin
} // namespace boost
