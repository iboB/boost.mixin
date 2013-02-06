//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

#include "gl_renderer_mixin.hpp"

class gl_renderer
{
public:
    gl_renderer() : _casts_shadows(true) {}

    void render(int render_target) const;

    void set_casts_shadows(bool set) { _casts_shadows = set; }

    void trace(std::ostream& o) const;

    void opengl_specific() {}

private:
    bool _casts_shadows;
};
