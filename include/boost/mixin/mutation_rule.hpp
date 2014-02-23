//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_MUTATION_RULE_HPP_INCLUDED)
#define _BOOST_MIXIN_MUTATION_RULE_HPP_INCLUDED

/**
 * \file
 * Functions and classes related to mutation rules.
 */

#include "global.hpp"
#include "mixin_collection.hpp"

namespace boost
{
namespace mixin
{

class object_type_mutation;

/// Base class for all mutation rules
class BOOST_MIXIN_API mutation_rule
{
public:
    virtual ~mutation_rule() {}

    /// Called when applying the mutation rule.
    virtual void apply_to(object_type_mutation& mutation) = 0;
};

/// Adds a mutation rule to the domain.
///
/// Does *not* perform a topological sort of the rules
/// It is the user's responsibility to add the mutation rules in the appropriate order
void BOOST_MIXIN_API add_new_mutation_rule(mutation_rule* rule);

} // namespace mixin
} // namespace boost

#endif // _BOOST_MIXIN_MUTATION_RULE_HPP_INCLUDED
