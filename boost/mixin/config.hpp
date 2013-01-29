//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_CONFIG_HPP_INCLUDED)
#define BOOST_MIXIN_CONFIG_HPP_INCLUDED

#include <boost/config.hpp>

// set this to true when running the boost mixin unit tests
// this will cause the creation of many additional variables
// and will slow down the execution time
#define BOOST_MIXIN_TEST 1

// boost::mixin hevily relies on type names
// setting the BOOST_MIXIN_USE_TYPEID to true will cause it to obtain said names with type_info via typeid.
// setting it to false will require the following extra steps:
//     * domain tags must add static const char* get_boost_mixin_name() { return <domain tag class name>; }
//     * mixins must add static const char* get_boost_mixin_name() { return <mixin class name>; }
#define BOOST_MIXIN_USE_TYPEID 1

#if !defined(NDEBUG)
#   define BOOST_MIXIN_DEBUG // define this for ease of use
#endif

// some features will compile faster when this is set to true
#if defined(_MSC_VER) && _MSC_VER < 1700
#   define BOOST_MIXIN_USING_CXX11 0
#else
#   define BOOST_MIXIN_USING_CXX11 1
#endif

// set this to true to make the library define "bm_this" as a shortcut to boost::mixin::get_object(this)
// the pointer to the object owning the current mixin
#define BOOST_MIXIN_DEFINE_BM_THIS 1

// maximum number of registered mixins for a domain
// an assertion will occur if this limit is reached in a program
// *    the object type creation and the object mutation are proportional
//      to this value by a factor of o(n log(word, n)) where word is the bit size of size_t
// *    the object type's size is 2 * word * <value>
//      the more global object of different types you have the more type memory they'll consume
// ie: small to minor cost to increase
#define BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN 256

#if !defined(BOOST_NO_EXCEPTIONS)
// setting this to true will cause some functions to throw exceptions instead of asserting
#   define BOOST_MIXIN_USE_EXCEPTIONS 1
#endif

// msvc complains that template classes don't have a dll interface
// can't deal with this right now
#if defined(_MSC_VER)
#   pragma warning (disable: 4251)
#endif

// dynamic link macros
#if 1 // (defined BOOST_HAS_DECLSPEC)
#   if 1 // defined(BOOST_ALL_DYN_LINK) || defined(BOOST_MIXIN_DYN_LINK)
#       if defined(BOOST_MIXIN_SRC)
#           define BOOST_MIXIN_API BOOST_SYMBOL_EXPORT
#       else
#           define BOOST_MIXIN_API BOOST_SYMBOL_IMPORT
#       endif  // BOOST_MIXIN_SRC
#   endif  // DYN_LINK
#endif  // BOOST_HAS_DECLSPEC

#if !defined(BOOST_MIXIN_API)
#   define BOOST_MIXIN_API
#endif

#endif // BOOST_MIXIN_CONFIG_HPP_INCLUDED
