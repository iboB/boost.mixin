//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED)
#define BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED

#include "global.hpp"

// object type info is an immutable class that represents the type information for a
// group of objects

namespace boost
{
namespace mixin
{

class object_transformer;
class object;

namespace internal
{

class domain;
class mixin_type_info_data;

class BOOST_MIXIN_API object_type_info : public noncopyable
{
public:
    object_type_info();
    ~object_type_info();

    static const object_type_info& null();

    domain* dom() const { return _domain; }

    bool has_mixin(mixin_id id) const { return _mixins[id]; }
    const mixin_type_info_data& mixin_info(mixin_id id) const { return *_mixins[id]; }
    size_t mixin_index(mixin_id id) const { return _mixin_indices[id]; }

    char** alloc_mixin_data(bool set_to_null = true) const;
    void dealloc_mixin_data(char** data) const;

    char* alloc_mixin(mixin_id id) const;
    void dealloc_mixin(mixin_id id, char* mem) const;

boost_mixin_internal:
    domain* _domain; // owning domain

    // not available mixins are null
    size_t _mixin_indices[BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN];
    const mixin_type_info_data* _mixins[BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN];

    // only the mixins the objects of this type have
    mixin_type_info_vector _compact_mixins;
};

}
}
}


#endif // BOOST_MIXIN_OBJECT_TYPE_INFO_HPP_INCLUDED
