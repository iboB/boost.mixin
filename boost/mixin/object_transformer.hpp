//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_OBJECT_TRANSFORMER_HPP_INCLUDED)
#define BOOST_MIXIN_OBJECT_TRANSFORMER_HPP_INCLUDED

// the object transformer mutates objects
// ie it adds and removes mixins while synchronizing the object's
// type information with its domain

// the usage is to call add and remove for the desired components and then
// call object_transformer::apply() or simply have it leave the scope

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

class BOOST_MIXIN_API object_transformer : public internal::noncopyable
{
public:
    object_transformer(object* o);
    object_transformer(object& o);
    ~object_transformer();

    template <typename Mixin>
    object_transformer& add()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        internal_add(info);
        return *this;
    }

    template <typename Mixin>
    object_transformer& remove()
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

typedef object_transformer mutate;

}
}

#endif // BOOST_MIXIN_OBJECT_TRANSFORMER_HPP_INCLUDED
