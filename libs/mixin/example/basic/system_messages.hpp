//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

BOOST_MIXIN_CONST_MESSAGE_0(int, get_id);
BOOST_MIXIN_MESSAGE_1(void, set_id, int, id);

BOOST_MIXIN_CONST_MULTICAST_MESSAGE_1(void, trace, std::ostream&, o);

