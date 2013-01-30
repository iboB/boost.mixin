//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once

class object_manager
{
public:

    void create_objects();

    void change_rendering_sytem();

    std::vector<boost::mixin::object*>& objects()
    {
        return _objects;
    }

private:
    std::vector<boost::mixin::object*> _objects;
};
