//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_OBJECT_TYPE_MUTATION_HPP_INCLUDED)
#define _BOOST_MIXIN_OBJECT_TYPE_MUTATION_HPP_INCLUDED

// the object type mutation represents an object mutation
// is used by mutators and mutation rules

#include "global.hpp"
#include "mixin_type_info.hpp"
#include "mixin_collection.hpp"

namespace boost
{
namespace mixin
{
namespace internal
{
    class object_mutator;
}

class BOOST_MIXIN_API object_type_mutation
{
public:
    object_type_mutation();
    object_type_mutation(const mixin_collection* src);

    void set_source(const mixin_collection* src) { _source = src; }

    template <typename Mixin>
    bool is_adding() const
    {
        return _adding.has<Mixin>();
    }
    template <typename Mixin>
    bool is_removing() const
    {
        return _removing.has<Mixin>();
    }
    template <typename Mixin>
    bool source_has() const
    {
        return _source->has<Mixin>();
    }

    bool is_adding(mixin_id id) const
    {
        return _adding.has(id);
    }
    bool is_removing(mixin_id id) const
    {
        return _removing.has(id);
    }
    bool source_has(mixin_id id) const
    {
        return _source->has(id);
    }

    template <typename Feature>
    bool is_adding(const Feature* f) const
    {
        return _adding.implements(f);
    }
    template <typename Feature>
    bool is_removing(const Feature* f) const
    {
        return _removing.implements(f);
    }
    template <typename Feature>
    bool source_implements(const Feature* f) const
    {
        return _source->implements(f);
    }

    template <typename Mixin>
    void stop_adding()
    {
        _adding.remove<Mixin>();
    }
    template <typename Mixin>
    void stop_removing()
    {
        _removing.remove<Mixin>();
    }

    void stop_adding(mixin_id id)
    {
        _adding.remove(id);
    }
    void stop_removing(mixin_id id)
    {
        _removing.remove(id);
    }

    template <typename Feature>
    void stop_adding(const Feature* f)
    {
        _adding.clear_all_implementing(f);
    }
    template <typename Feature>
    void stop_removing(const Feature* f)
    {
        _removing.clear_all_implementing(f);
    }

    template <typename Mixin>
    void start_adding()
    {
        _adding.add<Mixin>();
        check_valid();
    }
    template <typename Mixin>
    void start_removing()
    {
        _removing.add<Mixin>();
        check_valid();
    }

    void start_adding(mixin_id id)
    {
        _adding.add(id);
        check_valid();
    }
    void start_removing(mixin_id id)
    {
        _removing.add(id);
        check_valid();
    }

    template <typename Feature>
    void start_removing(const Feature* f);

    bool empty() const { return _adding.empty() && _removing.empty(); }

    // normalize _addind and _removing
    // that is, if an element is in both arrays it will be removed from them
    void normalize();

    void clear();

private:
    friend class internal::object_mutator;
    void check_valid();
    internal::domain* dom() const;

    mixin_collection _adding;
    mixin_collection _removing;
    const mixin_collection* _source;
};

}
}

#endif // _BOOST_MIXIN_OBJECT_TYPE_MUTATION_HPP_INCLUDED