//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_SINGLE_OBJECT_MUTATOR_HPP_INCLUDED)
#define _BOOST_MIXIN_SINGLE_OBJECT_MUTATOR_HPP_INCLUDED

#include "global.hpp"
#include "object_mutator.hpp"

namespace boost
{
namespace mixin
{

/// The single object mutator mutates objects.
/// Namely it adds and removes mixins from an object
/// While synchronizing the object's
/// type information
///
/// The usage is to call add and remove for the desired components and then
/// call `single_object_mutator::apply()` or simply have it leave the scope,
/// or use the typedef mutate to do the mutation on a single line
class BOOST_MIXIN_API single_object_mutator : public internal::object_mutator
{
public:
    single_object_mutator(object* o);
    single_object_mutator(object& o);
    ~single_object_mutator();

    /// Applies the mutation to the object.
    void apply();

private:
    object& _object;
    bool _is_manually_applied;

    // create must be called by apply
    using object_mutator::create;
};

typedef single_object_mutator mutate;

}
}

#endif // _BOOST_MIXIN_SINGLE_OBJECT_MUTATOR_HPP_INCLUDED
