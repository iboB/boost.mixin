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
#include <boost/mixin/domain.hpp>
#include <boost/mixin/object.hpp>

using namespace std;

namespace boost
{
namespace mixin
{

using namespace internal;

object_type_template::object_type_template()
    : _type_info(nullptr)
{
}

void object_type_template::create()
{
    BOOST_ASSERT(!_type_info);
    BOOST_ASSERT(_to_remove.empty());

    sort(_to_add.begin(), _to_add.end());
    _type_info = _domain->get_object_type_info(_to_add);
}

void object_type_template::apply_to(object* o) const
{
    BOOST_ASSERT(_type_info);
    o->clear();
    o->change_type(_type_info, true);
}

} // namespace mixin
} // namespace boost
