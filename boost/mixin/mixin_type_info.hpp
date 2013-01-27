//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_MIXIN_TYPE_INFO_HPP_INCLUDED)
#define BOOST_MIXIN_MIXIN_TYPE_INFO_HPP_INCLUDED

#include "global.hpp"

namespace boost
{
namespace mixin
{
namespace internal
{

class domain;

static const mixin_id INVALID_MIXIN_ID = ~mixin_id(0);

typedef void (*mixin_constructor_proc)(void* memory);
typedef void (*mixin_destructor_proc)(void* memory);

// this struct contains information for a given mixin
struct mixin_type_info_data : public noncopyable
{
    domain* dom; // the domain this mixin belongs to
    mixin_id id; // the mixin's id within said domain

    const char* name; // mixin name = name of the actual class

    size_t size; // size of the mixin object

    // a procedure obtained from the mixin definition that makes the actual construction
    mixin_constructor_proc constructor;
    mixin_destructor_proc destructor;

    bool is_valid() const { return id != INVALID_MIXIN_ID && dom; }

    mixin_type_info_data()
        : id(INVALID_MIXIN_ID)
        // since this is always static, other member will be initialized with 0
    {
    }
};

// this metafunction binds the type info of a mixin to its type
template <typename Mixin>
struct mixin_type_info : public noncopyable
{
    // have this static function instead of a simple member to guarantee
    // that mixin_type_info_data's constructor is called the first time
    static mixin_type_info_data& data()
    {
        static mixin_type_info_data d;
        return d;
    }

    // this static member registers the mixin
    // we need to reference it somewhere so as to call its constructor
    static mixin_type_info registrator;

    // the constructor is defined in mixin.h because it refernces the domain object
    mixin_type_info();

    // to prevent warnings and optimizations that will say that we're not using mixin_type_info
    // by simply referencing it
    int unused;
};
template <typename Mixin>
mixin_type_info<Mixin> mixin_type_info<Mixin>::registrator;

// this procedure is used for the mixin construction
template <typename Mixin>
void call_mixin_constructor(void* memory)
{
    new (memory) Mixin;
}

template <typename Mixin>
void call_mixin_destructor(void* memory)
{
    reinterpret_cast<Mixin*>(memory)->~Mixin();
}

} // namespace internal
} // namespace mixin
} // namespace boost

#endif // BOOST_MIXIN_MIXIN_TYPE_INFO_HPP_INCLUDED
