//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "executable_pch.hpp"
#include "object_factory.hpp"
#include "exe_mixin_fwd.hpp"
#include "../common/mixin_fwd.hpp"
#include "../dynlib/dynlib_mixin_fwd.hpp"

using namespace boost::mixin;

object_factory::object_factory()
{
    the_object = new object;

    mutate(the_object)
        .add<common_mixin>()
        .add<exe_mixin>()
        .add<dynlib_mixin>();
}

object_factory::~object_factory()
{
    delete the_object;
}
