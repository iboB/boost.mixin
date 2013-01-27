//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include "internal.hpp"
#include <boost/mixin/global.hpp>

namespace boost
{
namespace mixin
{
namespace internal
{
#if BOOST_MIXIN_USE_TYPEID && defined(__GNUC__)
    int cxa_demangle_status; // for getting unmangled types
#endif
}
}
}
