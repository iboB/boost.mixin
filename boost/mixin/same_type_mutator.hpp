//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_SAME_TYPE_MUTATOR_HPP_INCLUDED)
#define _BOOST_MIXIN_SAME_TYPE_MUTATOR_HPP_INCLUDED

#include "global.hpp"
#include "object_mutator.hpp"

namespace boost
{
namespace mixin
{

/// The same type object mutator mutates objects of the same type.
/// Namely, it adds and removes mixins from objects that initially have the same mixins.
///
/// If you want to mutate many objects of the same type, using this class
/// will make it much faster than using the `single_object_mutator` multiple times.
class BOOST_MIXIN_API same_type_mutator : public internal::object_mutator
{
public:
    same_type_mutator();
    same_type_mutator(const internal::object_type_info* info);

    // not using but hiding parent's function
    void apply_to(object& o);
};

}
}

#endif // _BOOST_MIXIN_SAME_TYPE_MUTATOR_HPP_INCLUDED
