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

// used to indicate that there are no features for a mixin
struct BOOST_MIXIN_API no_features_t {};
extern BOOST_MIXIN_API no_features_t* none;

namespace internal
{
    class domain;
}

static const feature_id INVALID_FEATURE_ID = ~feature_id(0);

class BOOST_MIXIN_API feature : public internal::noncopyable
{
public:
    feature_id id;
    internal::domain* dom;
    const char* const name;

protected:
    feature(const char* name)
        : id(INVALID_FEATURE_ID)
        , name(name)
    {
    }
};


namespace internal
{

// like the mixin_type_info_instance this class is as a
// metafunction to bind feature types to their unique instances
template <typename Feature>
struct feature_instance
{
    // using a static function instead of a member to guarantee the constructor is called
    static Feature& the_feature()
    {
        static Feature f;
        return f;
    }

    // unfortunately we cannot do the mixin trick of clobal instantiation here
    // there is no guarantee that the features will be instantiated before the mixins
    // and their id's are needed
    // instead we'll register the features manually when registering the mixin
    // this will be at the cost of having features registered multiple times
};

} // namespace internal
} // namespace mixin
} // namespace boost

#endif // BOOST_MIXIN_FEATURE_HPP_INCLUDED
