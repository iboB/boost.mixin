//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

BOOST_MIXIN_CONST_MESSAGE_1_OVERLOAD(render_on_target, void, render, int, target);
BOOST_MIXIN_CONST_MESSAGE_0_OVERLOAD(render_on_backbuffer, void, render);

BOOST_MIXIN_MESSAGE_1(void, set_casts_shadows, bool, set);

#define all_rendering_messages render_on_target_msg & render_on_backbuffer_msg & set_casts_shadows_msg

BOOST_MIXIN_MESSAGE_0(void, opengl_specific);
BOOST_MIXIN_MULTICAST_MESSAGE_0(void, opengl_specific_multicast);
