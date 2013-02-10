//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/object_type_template.hpp>
#include <boost/mixin/object_type_info.hpp>
#include <boost/mixin/object.hpp>

using namespace std;

namespace boost
{
namespace mixin
{

using namespace internal;

object_type_template::object_type_template()
    : object_mutator(object_type_info::null().as_mixin_collection())
{
}

void object_type_template::apply_to(object* o) const
{
    BOOST_ASSERT(o);
    o->clear();
    object_mutator::apply_to(o);
}

} // namespace mixin
} // namespace boost
