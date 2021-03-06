//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_OBJECT_TYPE_TEMPLATE_HPP_INCLUDED)
#define _BOOST_MIXIN_OBJECT_TYPE_TEMPLATE_HPP_INCLUDED

#include "global.hpp"
#include "object_mutator.hpp"

namespace boost
{
namespace mixin
{
namespace internal
{
    class object_type_info;
}

/// An object type template. Internally it represents a prepared object type
/// information.
///
/// It makes the construction of objects from the same type (same mixins)
/// slightly faster, than using `mutate`.
class BOOST_MIXIN_API object_type_template : private internal::object_mutator
{
public:
    object_type_template();

    using internal::object_mutator::add;
    // does the actual creation of the type template
    using internal::object_mutator::create;

    // hiding the parent function, not using it
    void apply_to(object& o) const;
};

}
}

#endif // _BOOST_MIXIN_OBJECT_TYPE_TEMPLATE_HPP_INCLUDED
