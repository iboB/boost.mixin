//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/domain.hpp>
#include <boost/mixin/mixin_collection.hpp>
#include <boost/mixin/exception.hpp>

using namespace std;

namespace boost
{
namespace mixin
{

using namespace internal;

mixin_collection::mixin_collection()
{
}

mixin_collection::mixin_collection(const mixin_type_info_vector& mixins)
    : _mixins(build_available_mixins_from(mixins))
    , _compact_mixins(mixins)
{
}

void mixin_collection::check_valid_mixin(const mixin_type_info& mixin_info)
{
    // this shouldn't be possible without a bug in the library
    // hence no throw
    BOOST_ASSERT(mixin_info.is_valid());
}

void mixin_collection::add(mixin_id id)
{
    // could be that the input parameter has been instantiated from a different module
    // that's why get the info that's actually in the domain, instead of just using an
    // argument of type mixin_type_info
    const mixin_type_info& info = domain::instance().mixin_info(id);

    if(_mixins[id])
    {
        // we already have this
        BOOST_ASSERT(has_elem(_compact_mixins, &info));
        return;
    }

    _mixins[id] = true;
    _compact_mixins.push_back(&info);
}

void mixin_collection::remove(mixin_id id)
{
    // could be that the input parameter has been instantiated from a different module
    // that's why get the info that's actually in the domain, instead of just using an
    // argument of type mixin_type_info
    const mixin_type_info& info = domain::instance().mixin_info(id);

    BOOST_ASSERT(_mixins[id]); // should have it to remove it
    _mixins[id] = false;

    mixin_type_info_vector::iterator i = find(_compact_mixins.begin(), _compact_mixins.end(), &info);
    BOOST_ASSERT(i != _compact_mixins.end());
    _compact_mixins.erase(i);
}

void mixin_collection::rebuild_from_compact_mixins()
{
    _mixins.reset();
    for (const mixin_type_info* info : _compact_mixins)
    {
        _mixins[info->id] = true;
    }
}

void mixin_collection::clear()
{
    _compact_mixins.clear();
    rebuild_from_compact_mixins();
}

}
}
