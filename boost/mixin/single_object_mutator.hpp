//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_SINGLE_OBJECT_MUTATOR_HPP_INCLUDED)
#define _BOOST_MIXIN_SINGLE_OBJECT_MUTATOR_HPP_INCLUDED

// the single object mutator mutates objects
// ie it adds and removes mixins from an object
// while synchronizing the object's
// type information with its domain

// the usage is to call add and remove for the desired components and then
// call object_mutator::apply() or simply have it leave the scope
// or use the typedef mutate to do the mutation on a single line

#include "global.hpp"
#include "object_mutator.hpp"

namespace boost
{
namespace mixin
{

class BOOST_MIXIN_API single_object_mutator : public internal::object_mutator
{
public:
    single_object_mutator(object* o);
    single_object_mutator(object& o);
    ~single_object_mutator();

    void apply();

private:
    object* _object;
    bool _is_manually_applied;
};

typedef single_object_mutator mutate;

}
}

#endif // _BOOST_MIXIN_SINGLE_OBJECT_MUTATOR_HPP_INCLUDED