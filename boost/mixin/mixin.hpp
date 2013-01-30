//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_MIXIN_HPP_INCLUDED)
#define BOOST_MIXIN_MIXIN_HPP_INCLUDED

#include "global.hpp"
#include "domain.hpp"
#include "mixin_type_info.hpp"
#include "feature.hpp"

#include <boost/preprocessor/empty.hpp>

namespace boost
{
namespace mixin
{
namespace internal
{


template <typename Mixin>
mixin_type_info_instance<Mixin>::mixin_type_info_instance()
{
    // register the mixin within its domain
    get_domain_for_tag<typename _boost_mixin_domain_for_type<Mixin>::tag>().
        // we use the function to get the type info, to guarantee that an instantiation of the template
        // from another module won't override if
        template register_mixin_type<Mixin>(_boost_get_mixin_type_info((Mixin*)nullptr));
}


} // namespace internal

// when using mixins from dynamic libraries use this macro in your headers
// this may be included separately and doesn't need to be in the same header as the actual mixin class declaration
#define BOOST_DECLARE_EXPORTED_MIXIN(export, mixin_type) \
    class mixin_type; \
    extern export ::boost::mixin::internal::mixin_type_info& _boost_get_mixin_type_info(const mixin_type* m); \

// call this in header files to forward declare a mixin type
// this may be included separately and doesn't need to be in the same header as the actual mixin class definition
#define BOOST_DECLARE_MIXIN(mixin_type) \
    BOOST_DECLARE_EXPORTED_MIXIN(BOOST_PP_EMPTY(), mixin_type)

// you must call one of the following two defines in a compilation unit to guarantee
// the internal intializations for a mixin type
#define BOOST_DEFINE_MIXIN_IN_DOMAIN(domain_tag, mixin_type, mixin_features) \
    /* specialize _boost_mixin_domain_for_type to bind this mixin's type to its domain tag */ \
    template <> struct _boost_mixin_domain_for_type<mixin_type> { typedef domain_tag tag; }; \
    /* create a function that will reference mixin_type_info_instance static registrator to guarantee its instantiation */ \
    inline void _boost_register_mixin(mixin_type*) { ::boost::mixin::internal::mixin_type_info_instance<mixin_type>::registrator.unused = true; } \
    /* create a mixin_type_info getter for this type */ \
    ::boost::mixin::internal::mixin_type_info& _boost_get_mixin_type_info(const mixin_type*) { return ::boost::mixin::internal::mixin_type_info_instance<mixin_type>::info(); } \
    /* create a features parsing function */ \
    /* features can be parsed multiple times by different parsers */ \
    template <typename FeaturesParser> \
    void _boost_parse_mixin_features(const mixin_type*, FeaturesParser& parser) { parser & mixin_features; } \


// short version to define mixins in the default domain
#define BOOST_DEFINE_MIXIN(mixin_type, mixin_features) \
    BOOST_DEFINE_MIXIN_IN_DOMAIN(::boost::mixin::default_domain, mixin_type, mixin_features)

class object;

// functions that get the object of a mixin by its address
// most commonly used within a mixin class
template <typename Mixin>
object* get_object(Mixin* mixin_addr)
{
    return *reinterpret_cast<object**>(reinterpret_cast<char*>(mixin_addr) - sizeof(object*));
}

template <typename Mixin>
const object* get_object(const Mixin* mixin_addr)
{
    return *reinterpret_cast<const object**>(reinterpret_cast<const char*>(mixin_addr) - sizeof(object*));
}

} // namespace mixin
} // namespace boost

// this macro makes writing code within mixins nicer
#if BOOST_MIXIN_DEFINE_BM_THIS
#   define bm_this ::boost::mixin::get_object(this)
#endif

#endif // BOOST_MIXIN_MIXIN_HPP_INCLUDED
