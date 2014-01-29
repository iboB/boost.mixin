//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_OBJECT_MUTATOR_HPP_INCLUDED)
#define _BOOST_MIXIN_OBJECT_MUTATOR_HPP_INCLUDED

/**
 * \file
 * Defines an internal base class, that is a parent to various concrete types
 * that deal with object mutataion.
 */

#include "global.hpp"
#include "object_type_mutation.hpp"

namespace boost
{
namespace mixin
{

class object;

namespace internal
{

class object_type_info;

class BOOST_MIXIN_API object_mutator : public internal::noncopyable
{
public:
    object_mutator();
    object_mutator(const mixin_collection* source_mixins);

    template <typename Mixin>
    object_mutator& add()
    {
        BOOST_ASSERT(!_is_created); // once a mutator is created, you cannot change its mutation
        _mutation.start_adding<Mixin>();
        return *this;
    }

    template <typename Mixin>
    object_mutator& remove()
    {
        BOOST_ASSERT(!_is_created); // once a mutator is created, you cannot change its mutation
        _mutation.start_removing<Mixin>();
        return *this;
    }

    // todo: add operators + and -

    void cancel();

    void create();
protected:

    void apply_to(object& obj) const;

    object_type_mutation _mutation;
    const object_type_info* _target_type_info;

    bool _is_created;
};

}
}
}

#endif // _BOOST_MIXIN_OBJECT_MUTATOR_HPP_INCLUDED
