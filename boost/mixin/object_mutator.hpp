//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_OBJECT_MUTATOR_HPP_INCLUDED)
#define _BOOST_MIXIN_OBJECT_MUTATOR_HPP_INCLUDED

// the object transformer mutates objects
// ie it adds and removes mixins while synchronizing the object's
// type information with its domain

// the usage is to call add and remove for the desired components and then
// call object_mutator::apply() or simply have it leave the scope

#include "global.hpp"

namespace boost
{
namespace mixin
{

class object;

namespace internal
{
    class domain;
}

class BOOST_MIXIN_API object_mutator : public internal::noncopyable
{
public:
    object_mutator(object* o);
    object_mutator(object& o);
    ~object_mutator();

    template <typename Mixin>
    object_mutator& add()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        internal_add(info);
        return *this;
    }

    template <typename Mixin>
    object_mutator& remove()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        internal_remove(info);
        return *this;
    }

    void apply();

    void cancel();

    // todo: add operators + and -

private:

    void check_valid_mutation(const internal::mixin_type_info& mixin_info);
    void internal_add(const internal::mixin_type_info& mixin_info);
    void internal_remove(const internal::mixin_type_info& mixin_info);

    object* _object;
    internal::domain* _domain;

    internal::mixin_type_info_vector _to_add;
    internal::mixin_type_info_vector _to_remove;
};

typedef object_mutator mutate;

}
}

#endif // _BOOST_MIXIN_OBJECT_MUTATOR_HPP_INCLUDED
