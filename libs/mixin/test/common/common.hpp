//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_COMMON_HPP_INCLUDED)
#define _COMMON_HPP_INCLUDED

// things used in all Boost.Mixin tests

#define STRING_CAT(a, b) a b

#define BOOST_CHECK_NULL(x) \
    BOOST_CHECK_MESSAGE(!(x), #x " is not null, but it should be")

#define BOOST_CHECK_NOT_NULL(x) \
    BOOST_CHECK_MESSAGE(!!(x), #x " is null, but shouldn't be")

#endif // _COMMON_HPP_INCLUDED
