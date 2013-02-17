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

namespace internal
{
class BOOST_MIXIN_API mandatory_mixin_impl : public mutation_rule
{
public:
    mandatory_mixin_impl(mixin_id id)
        : _id(id)
    {
    }

    virtual void apply_to(object_type_mutation& mutation); // override

protected:
    const mixin_id _id;
};

class BOOST_MIXIN_API deprecated_mixin_impl : public mutation_rule
{
public:
    deprecated_mixin_impl(mixin_id id)
        : _id(id)
    {
    }

    virtual void apply_to(object_type_mutation& mutation); // override

protected:
    const mixin_id _id;
};

class BOOST_MIXIN_API substitute_mixin_impl : public mutation_rule
{
public:
    substitute_mixin_impl(mixin_id src, mixin_id target)
        : _source_id(src)
        , _target_id(target)
    {
    }

    virtual void apply_to(object_type_mutation& mutation); // override

protected:
    const mixin_id _source_id;
    const mixin_id _target_id;
};
}

template <typename Mixin>
class mandatory_mixin : public internal::mandatory_mixin_impl
{
public:
    mandatory_mixin()
        : mandatory_mixin_impl(_boost_get_mixin_type_info((Mixin*)nullptr).id)
    {}
};

template <typename Mixin>
class deprecated_mixin : public internal::deprecated_mixin_impl
{
public:
    deprecated_mixin()
        : deprecated_mixin_impl(_boost_get_mixin_type_info((Mixin*)nullptr).id)
    {}
};

template <typename SourceMixin, typename TargetMixin>
class substitute_mixin : public internal::substitute_mixin_impl
{
public:
    substitute_mixin()
        : substitute_mixin_impl(
            _boost_get_mixin_type_info((SourceMixin*)nullptr).id,
            _boost_get_mixin_type_info((TargetMixin*)nullptr).id)
    {}
};

} // namespace mixin
} // namespace boost

#endif // _BOOST_MIXIN_COMMON_MUTATION_RULES_HPP_INCLUDED
