//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#if defined(_TEST_MIXINS_HPP)
#   error("this file is not supposed to be included multiple times in the same module")
#endif

#define _TEST_MIXINS_HPP

BOOST_DECLARE_MIXIN(a);
BOOST_DECLARE_MIXIN(b);
BOOST_DECLARE_MIXIN(c);

class a {};
class b {};
class c {};

BOOST_DEFINE_MIXIN(a, boost::mixin::none);
BOOST_DEFINE_MIXIN(b, boost::mixin::none);
BOOST_DEFINE_MIXIN(c, boost::mixin::none);
