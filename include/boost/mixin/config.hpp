//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_CONFIG_HPP_INCLUDED)
#define _BOOST_MIXIN_CONFIG_HPP_INCLUDED

/**
 * \file
 * The library's configuration file.
 */

#include <boost/config.hpp>

// boost::mixin heavily relies on type names
// setting the BOOST_MIXIN_USE_TYPEID to true will cause it to obtain said names with type_info via typeid.
// setting it to false will require the following extra steps:
//     * mixins must add static const char* boost_mixin_name() { return <mixin class name>; }
#define BOOST_MIXIN_USE_TYPEID 1

#if !defined(NDEBUG)
#   define BOOST_MIXIN_DEBUG // define this for ease of use
#endif

// maximum number of registered mixins
// an assertion will occur if this limit is reached in a program
// *    the object type creation and the object mutation are proportional
//      to this value by a factor of o(n log(word, n)) where word is the bit size of size_t
// *    the object type's size is 2 * <value> / 8 (a bit for each possible mixin)
//      the more global object of different types you have the more type memory they'll consume
// ie: minor cost for increase
#define BOOST_MIXIN_MAX_MIXINS 512

// maximum number of messages (and consequently max per mixin)
// an assertion will occur if this limit is reached in a program
// as with the mixin limits this influences object type with 2 times as many pointers (2 * <word> * value)
// object creation and mutation is not influenced by this value
// minor cost for increase
#define BOOST_MIXIN_MAX_MESSAGES 1024

#if !defined(BOOST_NO_EXCEPTIONS)
// setting this to true will cause some functions to throw exceptions instead of asserting
#   define BOOST_MIXIN_USE_EXCEPTIONS 1
#endif

// there is warning push/pop about this in the main header
#if defined(_MSC_VER)
// msvc complains that template classes don't have a dll interface (they shouldn't).
#   pragma warning (disable: 4251)
#endif

// dynamic link macros
#if defined(BOOST_HAS_DECLSPEC)
#   if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_MIXIN_DYN_LINK)
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

#endif // _BOOST_MIXIN_CONFIG_HPP_INCLUDED
