//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_VERSION_HPP_INCLUDED)
#define _BOOST_MIXIN_VERSION_HPP_INCLUDED

/**
 * \file
 * Optional header file, which contains library version info.
 */

#define BOOST_MIXIN_VERSION_MAJOR 1
#define BOOST_MIXIN_VERSION_MINOR 0
#define BOOST_MIXIN_VERSION_SUB_MINOR 0

/// The library's version.
///
/// It is composed of `major*10000 + minor*100 + subminor`.
/// For example, version 2.31.11 would be 23111.
#define BOOST_MIXIN_VERSION (BOOST_MIXIN_VERSION_MAJOR*10000 + BOOST_MIXIN_VERSION_MINOR*100 + BOOST_MIXIN_VERSION_SUB_MINOR)

#endif // _BOOST_MIXIN_VERSION_HPP_INCLUDED
