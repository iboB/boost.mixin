//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "basic.hpp"
#include "d3d_renderer.hpp"
#include "rendering_messages.hpp"
#include "transform_messages.hpp"
#include "system_messages.hpp"

using namespace std;


void d3d_renderer::render(int render_target) const
{
    int transform = get_combined_transform(bm_this);

    // main_devince->SetTransform(D3DTS_WORLD, transform);

    cout << "D3D rendering object " << get_id(bm_this) << endl
         << "\ton target " << render_target << endl
         << "\twith transformation: " << transform << endl
         << "\t" << (_casts_shadows ? "" : "not ") << "casting shadows" << endl;
}

void d3d_renderer::trace(std::ostream& o) const
{
    o << "\twith a d3d renderer" << endl;
}

BOOST_DEFINE_MIXIN(d3d_renderer, all_rendering_messages & trace_msg)
