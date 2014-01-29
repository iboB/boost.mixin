//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_MIXIN_COLLECTION_HPP_INCLUDED)
#define _BOOST_MIXIN_MIXIN_COLLECTION_HPP_INCLUDED

#include "global.hpp"
#include "mixin_type_info.hpp"

namespace boost
{
namespace mixin
{

/// A mixin collection is a class that allows the user to
/// represent a number of mixins
class BOOST_MIXIN_API mixin_collection : public internal::noncopyable
{
public:
    mixin_collection();
    mixin_collection(const internal::mixin_type_info_vector& mixins);

    /// Checks if a mixin type is present in the collection
    template <typename Mixin>
    bool has() const
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        return has(info.id);
    }
    bool has(mixin_id id) const { return _mixins[id]; }

    /// Adds a mixin type is to the collection
    template <typename Mixin>
    void add()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        check_valid_mixin(info);
        add(info.id);
    }
    void add(mixin_id id);

    /// Removes a mixin type from the collection
    template <typename Mixin>
    void remove()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        remove(info.id);
    }
    void remove(mixin_id id);

    /// Checks if a feature is implemented by any mixin in the collection
    template <typename Feature>
    bool implements(const Feature* f) const;

    /// Clears all mixins implementing a feature from the collection
    template <typename Feature>
    void clear_all_implementing(const Feature* f);

    internal::available_mixins_bitset _mixins;
    // only the mixins the objects of this type have
    internal::mixin_type_info_vector _compact_mixins;

    void rebuild_from_compact_mixins();
    void check_valid_mixin(const internal::mixin_type_info& mixin_info);

    /// Removes all elements from the collection
    void clear();

    /// Checks if a collection is empty.
    bool empty() const { return _compact_mixins.empty(); }
};

}
}

#endif // _BOOST_MIXIN_MIXIN_COLLECTION_HPP_INCLUDED