//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_FEATURE_HPP_INCLUDED)
#define _BOOST_MIXIN_FEATURE_HPP_INCLUDED

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

    bool is_private; // true when it's never used outised a single module
    // registrators will register features of the same name as the same feature
    // if they're not private
    // having different features with the same name in different modules
    // may cause dangerous crashes if they're not private

protected:
    feature(const char* name, bool is_private)
        : id(INVALID_FEATURE_ID)
        , name(name)
        , is_private(is_private)
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

#endif // _BOOST_MIXIN_FEATURE_HPP_INCLUDED
