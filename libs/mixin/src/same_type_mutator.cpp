//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/same_type_mutator.hpp>
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

same_type_mutator::same_type_mutator()
{
}

same_type_mutator::same_type_mutator(const internal::object_type_info* info)
    : object_mutator(info->as_mixin_collection())
{
}


void same_type_mutator::apply_to(object& o)
{
    if(!_is_created)
    {
        _mutation.set_source(o._type_info->as_mixin_collection());
        create();
    }

    internal::object_mutator::apply_to(o);
}

} // namespace mixin
} // namespace boost
