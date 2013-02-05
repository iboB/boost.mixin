//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_OBJECTL_HPP_INCLUDED)
#define BOOST_MIXIN_OBJECT_HPP_INCLUDED

#include "global.hpp"

namespace boost
{
namespace mixin
{

namespace internal
{
    class object_type_info;
    class mixin_data_in_object;
    class domain;
	struct message_t;
	struct message_feature_tag;
}

// main object class
class BOOST_MIXIN_API object : public internal::noncopyable
{
public:
    // constructs an empty object - no mixins
    object();
    ~object();

    template <typename Mixin>
    bool has() const
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        return internal_has_mixin(info);
    }

    template <typename Mixin>
    Mixin* get()
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        return reinterpret_cast<Mixin*>(internal_get_mixin(info));
    }

    template <typename Mixin>
    const Mixin* get() const
    {
        const internal::mixin_type_info& info = _boost_get_mixin_type_info((Mixin*)nullptr);
        return reinterpret_cast<const Mixin*>(internal_get_mixin(info));
    }

	template <typename Feature>
	bool implements(const Feature*) const
	{
		const Feature& f = static_cast<Feature&>(_boost_get_mixin_feature((Feature*)nullptr));
        BOOST_ASSERT(f.id != INVALID_FEATURE_ID);
		return internal_implements(f, typename Feature::feature_tag());
	}

    void* get_raw_mixin(mixin_id id);
    const void* get_raw_mixin(mixin_id id) const;

    // destroys all mixins and sets null type info
    void clear();

boost_mixin_internal:
    friend class object_transformer;

    void* internal_get_mixin(const internal::mixin_type_info& mixin_info);
    const void* internal_get_mixin(const internal::mixin_type_info& mixin_info) const;
    bool internal_has_mixin(const internal::mixin_type_info& mixin_info) const;


    // reorganizes the mixins for the new type
    // if manage_mixins is true the object will destroy all mixins removed and construct all new ones
    void change_type(const internal::object_type_info* new_type, bool manage_mixins = false);

    void construct_mixin(mixin_id id);
    void destroy_mixin(mixin_id id);

    internal::domain* dom() const;

    const internal::object_type_info* _type_info;

    // each element of this array points to a buffer which cointains a pointer to
    // this - the object and then the mixin
    // thus each mixin can get its own object
    internal::mixin_data_in_object* _mixin_data;

	template <typename Feature>
	bool internal_implements(const Feature& f, const internal::message_feature_tag&) const
	{
		return implements_message(f);
	}

	bool implements_message(const internal::message_t& m) const;
};

} // namespace mixin
} // namespace boost

#endif // BOOST_MIXIN_OBJECT_HPP_INCLUDED
