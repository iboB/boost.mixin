//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_MIXIN_COLLECTION_HPP_INCLUDED)
#define _BOOST_MIXIN_MIXIN_COLLECTION_HPP_INCLUDED

// the object type mutation represents an object mutation
// is used by mutators and mutation rules

#include "global.hpp"
#include "mixin_type_info.hpp"

namespace boost
{
namespace mixin
{

namespace internal
{
    class domain;
}

class BOOST_MIXIN_API mixin_collection : public internal::noncopyable
{
public:
    mixin_collection();
    mixin_collection(const internal::mixin_type_info_vector& mixins);

    internal::domain* dom() const { return _domain; }

    template <typename Mixin>
    bool has() const
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        return has(info.id);
    }
    bool has(mixin_id id) const { return _mixins[id]; }

    template <typename Mixin>
    void add()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        check_valid_mixin(info);
        add(info.id);
    }
    void add(mixin_id id);

    template <typename Mixin>
    void remove()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        remove(info.id);
    }
    void remove(mixin_id id);

    template <typename Feature>
    bool implements(const Feature* f) const;
    template <typename Feature>
    void clear_all_implementing(const Feature* f);

    internal::domain* _domain; // owning domain

    internal::available_mixins_bitset _mixins;
    // only the mixins the objects of this type have
    internal::mixin_type_info_vector _compact_mixins;

    void rebuild_from_compact_mixins();
    void check_valid_mixin(const internal::mixin_type_info& mixin_info);
    void clear();

    bool empty() const { return _compact_mixins.empty(); }
};

}
}

#endif // _BOOST_MIXIN_MIXIN_COLLECTION_HPP_INCLUDED