//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_COMMON_MUTATION_RULES_HPP_INCLUDED)
#define _BOOST_MIXIN_COMMON_MUTATION_RULES_HPP_INCLUDED

/**
 * \file
 * Common mutation rules classes.
 */

#include "global.hpp"
#include "mutation_rule.hpp"
#include "mixin_collection.hpp"

namespace boost
{
namespace mixin
{

/**
 * A mutation rule for mutually exclusive mixins.
 *
 * If active, such rule will cause a mutation that adds one of the mutually
 * exclusive mixins, to remove all others.
 *
 * For example, if `a`, `b`, and `c` are mutually exclusive mixins,
 * any mutation that adds, say `a`, to an object, will automatically
 * remove `b` and `c` from it.
 */
class BOOST_MIXIN_API mutually_exclusive_mixins : public mutation_rule, private mixin_collection
{
public:
    using mixin_collection::add;
    using mixin_collection::has;
    using mixin_collection::remove;

    /// Applies the rule to a mutation.
    virtual void apply_to(object_type_mutation& mutation) override;
};

namespace internal
{
/// INTERNAL ONLY
class BOOST_MIXIN_API mandatory_mixin_impl : public mutation_rule
{
public:
    mandatory_mixin_impl(mixin_id id)
        : _id(id)
    {
    }

    virtual void apply_to(object_type_mutation& mutation) override;

protected:
    const mixin_id _id;
};

/// INTERNAL ONLY
class BOOST_MIXIN_API deprecated_mixin_impl : public mutation_rule
{
public:
    deprecated_mixin_impl(mixin_id id)
        : _id(id)
    {
    }

    virtual void apply_to(object_type_mutation& mutation) override;

protected:
    const mixin_id _id;
};

/// INTERNAL ONLY
class BOOST_MIXIN_API substitute_mixin_impl : public mutation_rule
{
public:
    substitute_mixin_impl(mixin_id src, mixin_id target)
        : _source_id(src)
        , _target_id(target)
    {
    }

    virtual void apply_to(object_type_mutation& mutation) override;

protected:
    const mixin_id _source_id;
    const mixin_id _target_id;
};
}

/**
 * A mutation rule for a mandatory mixin.
 *
 * If active, such rule will cause every mutation to always add the mixin
 * to an object and will ignore any attempts of a mutation to remove it.
 *
 * \tparam Mixin The mandatory mixin type
*/
template <typename Mixin>
class mandatory_mixin : public internal::mandatory_mixin_impl
{
public:
    mandatory_mixin()
        : mandatory_mixin_impl(_boost_get_mixin_type_info((Mixin*)nullptr).id)
    {}
};

/**
 * A mutation rule for a deprecated mixin.
 *
 * If active, such rule will cause every mutation to always try to remove
 * the mixin from an object and will ignore any attempts of a mutation to
 * add it.
 *
 * \tparam Mixin The deprecated mixin type
*/
template <typename Mixin>
class deprecated_mixin : public internal::deprecated_mixin_impl
{
public:
    deprecated_mixin()
        : deprecated_mixin_impl(_boost_get_mixin_type_info((Mixin*)nullptr).id)
    {}
};

/**
 * A mutation rule for a substitute mixin.
 *
 * If active, such rule will cause any mutation that tries to add
 * `SourceMixin` to instead add `TargetMixin`.
 *
 * \tparam SourceMixin The mixin type to be substituted
 * \tparam TargetMixin The mixin type that is the substitute
*/
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
