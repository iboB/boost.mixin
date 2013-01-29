//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_FEATURE_HPP_INCLUDED)
#define BOOST_MIXIN_FEATURE_HPP_INCLUDED

#include "global.hpp"

namespace boost
{
namespace mixin
{

static const feature_id INVALID_FEATURE_ID = ~feature_id(0);

struct BOOST_MIXIN_API feature : public internal::noncopyable
{
    feature()
        : id(INVALID_FEATURE_ID)
    {
    }
    feature_id id;
};


namespace internal
{

// much like the mixin_type_info_instance this class is used to register features and
// also as a metafunction to bind feature types to their unique instances
template <typename Feature>
struct feature_instance
{
    // using a static function instead of a member to guarantee the constructor is called
    static Feature& the_feature()
    {
        static Feature f;
        return f;
    }

    // this static member registers the feature with the domain
    // we need to reference it somewhere so as to call its constructor
    static feature_instance registrator;

    // the constructor is defined in mixin.h because it refernces the domain object
    feature_instance();

    // to prevent warnings and optimizations that will say that we're not using
    // feature_instance by simply referencing it
    int unused;
};

template <typename Feature>
feature_instance<Feature> feature_instance<Feature>::registrator;

} // namespace internal
} // namespace mixin
} // namespace boost

#endif // BOOST_MIXIN_FEATURE_HPP_INCLUDED
