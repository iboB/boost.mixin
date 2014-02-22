//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "common.hpp"
#include "generated.hpp"

#include <iostream>

using namespace std;
using namespace boost::mixin;

#include "timer.ipp"

int main(int argc, char**)
{
    std::vector<object_type_template*> templates;
    create_type_templates(templates);

    const size_t num_tmpl = templates.size();

    cout << num_tmpl << endl;

    vector<object*> objects(num_tmpl);

    for(size_t i=0; i<num_tmpl; ++i)
    {
        objects[i] = new object;
    }

    {
        timer t;

        for(size_t i=0; i<num_tmpl; ++i)
        {
            templates[i]->apply_to(*objects[i]);
        }
    }



    return 0;
}
