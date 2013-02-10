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

// this class is a parent to various concrete types that deal with object mutataion

#include "global.hpp"

namespace boost
{
namespace mixin
{

class object;

namespace internal
{

class object_type_info;
class domain;

class BOOST_MIXIN_API object_mutator : public internal::noncopyable
{
public:
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

    // todo: add operators + and -

    void cancel();
protected:
    object_mutator();
    object_mutator(domain* dom);

    void check_valid_mutation(const internal::mixin_type_info& mixin_info);
    void internal_add(const internal::mixin_type_info& mixin_info);
    void internal_remove(const internal::mixin_type_info& mixin_info);

    domain* _domain;

    mixin_type_info_vector _to_add;
    mixin_type_info_vector _to_remove;
};

}
}
}

#endif // _BOOST_MIXIN_OBJECT_MUTATOR_HPP_INCLUDED
