#pragma once
#if !defined(_BOOST_MIXIN_HPP_INCLUDED)
#define _BOOST_MIXIN_HPP_INCLUDED

//
// mixin.hpp - support for compound dynamic objects, using mixins
//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// TODO: docs link
//

#if defined(_MSC_VER)
// don't leave our warning disabled for the outside world
#   pragma warning( push )
#endif

#include "mixin/config.hpp"
#include "mixin/global.hpp"
#include "mixin/domain.hpp"
#include "mixin/feature.hpp"
#include "mixin/mixin_type_info.hpp"
#include "mixin/mixin.hpp"
#include "mixin/message.hpp"
#include "mixin/message_macros.hpp"
#include "mixin/object.hpp"
#include "mixin/single_object_mutator.hpp"
#include "mixin/same_type_mutator.hpp"
#include "mixin/object_type_template.hpp"
#include "mixin/object_type_info.hpp"
#include "mixin/exception.hpp"

// possibly leave those to be included separately ?
#include "mixin/mutation_rule.hpp"
#include "mixin/common_mutation_rules.hpp"
#include "mixin/combinators.hpp"
#include "mixin/allocators.hpp"

#if defined(_MSC_VER)
#   pragma warning( pop )
#endif

#endif // _BOOST_MIXIN_HPP_INCLUDED
