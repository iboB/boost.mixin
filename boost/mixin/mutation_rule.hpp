//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_MUTATION_RULE_HPP_INCLUDED)
#define _BOOST_MIXIN_MUTATION_RULE_HPP_INCLUDED

#include "global.hpp"
#include "mixin_collection.hpp"
#include "domain.hpp"

// base class for all mutation rules

namespace boost
{
namespace mixin
{

class object_type_mutation;

class BOOST_MIXIN_API mutation_rule
{
public:
    virtual ~mutation_rule() {}

    virtual void apply_to(object_type_mutation& mutation) = 0;
};

// add mutation rule to default domain
void BOOST_MIXIN_API add_new_mutation_rule(mutation_rule* rule);

// add mutation rule to specific domain
template <typename DomainTag>
void add_new_mutation_rule_for_domain(mutation_rule* rule)
{
    internal::get_domain_for_tag<DomainTag>().add_new_mutation_rule(rule);
}

} // namespace mixin
} // namespace boost

#endif // _BOOST_MIXIN_MUTATION_RULE_HPP_INCLUDED
