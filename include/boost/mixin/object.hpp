//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_OBJECT_HPP_INCLUDED)
#define _BOOST_MIXIN_OBJECT_HPP_INCLUDED

#include "global.hpp"
#include "mixin_type_info.hpp"

namespace boost
{
namespace mixin
{

namespace internal
{
    class object_type_info;
    class mixin_data_in_object;
    struct message_t;
    struct message_feature_tag;
}

class object_type_template;

/// The main object class.
class BOOST_MIXIN_API object : public internal::noncopyable
{
public:
    /// Constructs an empty object - no mixins.
    object();
    /// Constructs an object from a specific type template.
    explicit object(const object_type_template& type_template);

#if BOOST_MIXIN_USING_CXX11
    /// Move constructor from an existing object
    object(object&& o);
#endif

    ~object();


    /////////////////////////////////////////////////////////////////
    // mixin info

    /// Checks if the object has a specific mixin.
    template <typename Mixin>
    bool has() const
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        // intentionally disregarding the actual info
        return internal_has_mixin(info.id);
    }

    /// Gets a specific mixin from the object. Returns nullptr if the mixin
    /// isn't available.
    template <typename Mixin>
    Mixin* get()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        // intentionally disregarding the actual info
        return reinterpret_cast<Mixin*>(internal_get_mixin(info.id));
    }

    /// Gets a specific mixin from the object. Returns nullptr if the mixin
    /// isn't available.
    template <typename Mixin>
    const Mixin* get() const
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        // intentionally disregarding the actual info
        return reinterpret_cast<const Mixin*>(internal_get_mixin(info.id));
    }

    /// Checks if the object has a specific mixin by id.
    bool has(mixin_id id) const;

    /// Gets a specific mixin by id from the object. Returns nullptr if the mixin
    /// isn't available. It is the user's responsibility to cast the returned
    /// value to the appropriate type
    void* get(mixin_id id);

    /// Gets a specific mixin by id from the object. Returns nullptr if the mixin
    /// isn't available. It is the user's responsibility to cast the returned
    /// value to the appropriate type
    const void* get(mixin_id id) const;
    /////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////
    // feature info

    /// Checks if the mixin implements a feature.
    template <typename Feature>
    bool implements(const Feature*) const
    {
        const Feature& f = static_cast<Feature&>(_boost_get_mixin_feature((Feature*)nullptr));
        BOOST_ASSERT(f.id != INVALID_FEATURE_ID);
        // intentionally disregarding the actual feature
        return internal_implements(f.id, typename Feature::feature_tag());
    }
    /////////////////////////////////////////////////////////////////

    /// Destroys all mixins within an object and resets its type info
    // (sets null type info)
    void clear();

    /// Returns true if the object is empty - has no mixins
    bool empty() const;

    /////////////////////////////////////////////////////////////////
    // logging and diagnostics

    /// Adds the names of the messages implemented by the object to the vector
    void get_message_names(std::vector<const char*>& out_message_names) const;

    /// Adds the names of the object's mixins to the vector
    void get_mixin_names(std::vector<const char*>& out_mixin_names) const;

    /////////////////////////////////////////////////////////////////

boost_mixin_internal:
    void* internal_get_mixin(mixin_id id);
    const void* internal_get_mixin(mixin_id id) const;
    bool internal_has_mixin(mixin_id id) const;

    // reorganizes the mixins for the new type
    // if manage_mixins is true the object will destroy all mixins removed and construct all new ones
    // we have the manage_mixins argument because its a lot faster to manage from the outside
    // if we know exactly what's added and removed
    void change_type(const internal::object_type_info* new_type, bool manage_mixins/* = true*/);

    void construct_mixin(mixin_id id);
    void destroy_mixin(mixin_id id);

    const internal::object_type_info* _type_info;

    // each element of this array points to a buffer which cointains a pointer to
    // this - the object and then the mixin
    // thus each mixin can get its own object
    internal::mixin_data_in_object* _mixin_data;

    bool internal_implements(feature_id id, const internal::message_feature_tag&) const
    {
        return implements_message(id);
    }

    bool implements_message(feature_id id) const;
};

} // namespace mixin
} // namespace boost

#endif // _BOOST_MIXIN_OBJECT_HPP_INCLUDED
