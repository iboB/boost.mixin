//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_COMMON_MUTATION_RULES_HPP_INCLUDED)
#define _BOOST_MIXIN_COMMON_MUTATION_RULES_HPP_INCLUDED

#include "global.hpp"
#include "mutation_rule.hpp"
#include "mixin_collection.hpp"

// common mutation rule classes

namespace boost
{
namespace mixin
{

// common mutation rules
class BOOST_MIXIN_API mutually_exclusive_mixins : public mutation_rule, private mixin_collection
{
public:
    using mixin_collection::add;
    using mixin_collection::has;
    using mixin_collection::remove;

    virtual void apply_to(object_type_mutation& mutation); // override
};

class BOOST_MIXIN_API mandatory_mixin : public mutation_rule
{
public:
    template <typename Mixin>
    mandatory_mixin()
        : _id(_boost_get_mixin_type_info((Mixin*)nullptr).id)
    {
    }

    virtual void apply_to(object_type_mutation& mutation); // override

protected:
    const mixin_id _id;
};

class BOOST_MIXIN_API deprecated_mixin : public mutation_rule
{
public:
    template <typename Mixin>
    deprecated_mixin()
        : _id(_boost_get_mixin_type_info((Mixin*)nullptr).id)
    {
    }

    virtual void apply_to(object_type_mutation& mutation); // override

protected:
    const mixin_id _id;
};

} // namespace mixin
} // namespace boost

#endif // _BOOST_MIXIN_COMMON_MUTATION_RULES_HPP_INCLUDED
