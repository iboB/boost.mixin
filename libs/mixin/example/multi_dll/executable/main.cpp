//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "executable_pch.hpp"
#include "object_factory.hpp"
#include "../common/messages.hpp"
#include "../dynlib/dynlib_messages.hpp"

using namespace std;

#include <Windows.h>
#include <tchar.h>

typedef void (*plugin_modify_object_proc)(boost::mixin::object*);

plugin_modify_object_proc plugin_modify_object;

void load_plugin()
{
    HMODULE hmod = LoadLibrary(_T("plugin_vs11_d.dll"));
    BOOST_ASSERT(hmod);

    FARPROC fp = GetProcAddress(hmod, "modify_object");
    BOOST_ASSERT(fp);

    plugin_modify_object = reinterpret_cast<plugin_modify_object_proc>(fp);
}

void call_messages(boost::mixin::object* o)
{
    cout << " ====== trace ====== " << endl;
    trace(o, cout);

    cout << endl << " ====== dl unicast messages ====== " << endl;
    dl_mixin_specific(o);
    dl_exported(o);
    cout << endl << " ====== dl unicast multicast ====== " << endl;
    dl_multicast(o);
}

int main()
{
    object_factory f;

    set_id(f.the_object, 42);

    call_messages(f.the_object);
    
    cout << endl << endl << endl << " ====== loading plugin ====== " << endl;

    load_plugin();
    plugin_modify_object(f.the_object);

    call_messages(f.the_object);
   
    return 0;
}