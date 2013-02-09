//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#if defined(DYNLIB_SRC)
#   define DYNLIB_API BOOST_SYMBOL_EXPORT
#else
#   define DYNLIB_API BOOST_SYMBOL_IMPORT
#endif