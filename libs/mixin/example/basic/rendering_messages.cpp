//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "basic.hpp"
#include "rendering_messages.hpp"

BOOST_MIXIN_DEFINE_MESSAGE(render_on_target);
BOOST_MIXIN_DEFINE_MESSAGE(render_on_backbuffer);
BOOST_MIXIN_DEFINE_MESSAGE(set_casts_shadows);

BOOST_MIXIN_DEFINE_MESSAGE(opengl_specific);
BOOST_MIXIN_DEFINE_MESSAGE(opengl_specific_multicast);