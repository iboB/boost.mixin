//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include "config.hpp"

BOOST_MIXIN_EXPORTED_MESSAGE_0(DYNLIB_API, void, dl_mixin_specific);
BOOST_MIXIN_EXPORTED_MESSAGE_0(DYNLIB_API, void, dl_exported);

BOOST_MIXIN_EXPORTED_MULTICAST_MESSAGE_0(DYNLIB_API, void, dl_multicast);
