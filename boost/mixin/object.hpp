//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_OBJECTL_HPP_INCLUDED)
#define BOOST_MIXIN_OBJECT_HPP_INCLUDED

#include "global.hpp"

namespace boost
{
namespace mixin
{

namespace internal
{
    class object_type_info;
    class domain;
    struct mixin_type_info_data;
}

// main object class
class BOOST_MIXIN_API object : public noncopyable
{
public:
    // constructs an empty object - no mixins
    object();
    ~object();

    template <typename Mixin>
    bool has() const
    {
        const internal::mixin_type_info_data& info = _boost_get_mixin_type_info_data((Mixin*)nullptr);
        return internal_has_mixin(info);
    }

    template <typename Mixin>
    Mixin* get()
    {
        const internal::mixin_type_info_data& info = _boost_get_mixin_type_info_data((Mixin*)nullptr);
        return reinterpret_cast<Mixin*>(internal_get_mixin(info));
    }

    template <typename Mixin>
    const Mixin* get() const
    {
        const internal::mixin_type_info_data& info = _boost_get_mixin_type_info_data((Mixin*)nullptr);
        return reinterpret_cast<const Mixin*>(internal_get_mixin(info));
    }

    // destroys all mixins and sets null type info
    void clear();

private:
    friend class object_transformer;

    void* internal_get_mixin(const internal::mixin_type_info_data& mixin_info);
    const void* internal_get_mixin(const internal::mixin_type_info_data& mixin_info) const;
    bool internal_has_mixin(const internal::mixin_type_info_data& mixin_info);


    // reorganizes the mixins for the new type
    // if manage_mixins is true the object will destroy all mixins removed and construct all new ones
    void change_type(const internal::object_type_info* new_type, bool manage_mixins = false);

    void construct_mixin(mixin_id id);
    void destroy_mixin(mixin_id id);

    internal::domain* dom() const;

    const internal::object_type_info* _type_info;

    // each element of this array points to a buffer which cointains a pointer to
    // this - the object and then the mixin
    // thus each mixin can get its own object
    char** _mixin_data;
};

} // namespace mixin
} // namespace boost

#endif // BOOST_MIXIN_OBJECT_HPP_INCLUDED
