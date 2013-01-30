//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED)
#define BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED

#include "message.hpp"

// some macros here have an underscore in front so it doesn't appear as a suggestion in
// ides that support these
// that shows that they're for internal use only

#define _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) BOOST_PP_CAT(boost_mixin_msg_, message_name)
#define _BOOST_MIXIN_MESSAGE_TAG(message_name) BOOST_PP_CAT(message_name, _msg)

#define _BOOST_MIXIN_MESSAGE2(export, domain, message_name, method_name, return_type, arg1_type, a1, arg2_type, a2, constness, message_mechanism) \
        /* mechanism shows whether it's a multicast or unicast */ \
    \
    /* step 1: define the message struct */ \
    struct export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) : public ::boost::mixin::internal::message_t \
    { \
        typedef return_type (::boost::mixin::internal::dummy_type::*method_t)(arg1_type, arg2_type) constness;\
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)() \
            : ::boost::mixin::internal::message_t(BOOST_PP_STRINGIZE(message_name), message_mechanism) \
        {} \
        template <typename Mixin> \
        return_type (Mixin::*get_method_pointer_for())(arg1_type, arg2_type) constness \
        { \
            /* use static_cast to show that we mean exactly the method with this signature and not some overload */ \
            return static_cast<return_type (Mixin::*)(arg1_type, arg2_type)>(&Mixin::method_name); \
        } \
    }; \
    /* step 2: define a message tag, that will be used to identify the message in feature lists */ \
    /* it would have been nice if we could set this global variable to the unique global instance of the feature*/ \
    /* but unfortunately we cannot trust dynamic libraries to keep it straight for us */ \
    /* hence we rely on a getter like the mixin one */ \
    extern export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) * _BOOST_MIXIN_MESSAGE_TAG(message_name); \
    /* step 3: declare the feature getter and manual registrator for the message */ \
    extern export ::boost::mixin::feature& _boost_get_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*); \
    extern export void _boost_register_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*); \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline return_type method_name(constness ::boost::mixin::object* obj, arg1_type a1, arg2_type a2) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        const ::boost::mixin::internal::object_type_info::call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const ::boost::mixin::internal::message_for_mixin* msg_data = call_entry.message_data; \
        BOOST_ASSERT(msg_data); \
        BOOST_ASSERT(msg_data->message == &self); \
        BOOST_ASSERT(msg_data->message->mechanism == ::boost::mixin::internal::message_t::message_mechanism); \
        char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
        (reinterpret_cast< ::boost::mixin::internal::dummy_type*>(mixin_data + msg_data->func_offset) ->* \
            reinterpret_cast<typename _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::method_t>(msg_data->func))(a1, a2); \
    }\


#define BOOST_MIXIN_MESSAGE_2(return_type, message, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE2(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, arg1_type, a1, arg2_type, a2, BOOST_PP_EMPTY(), unicast)


/*#define BOOST_MIXIN_EXPORTED_MESSAGE_2_OVERLOAD_IN_DOMAIN(export, domain, message, return_type, method_name, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE2(export, domain, message, method_name, return_type, arg1_type, a1, arg2_type, a2, BOOST_PP_EMPTY(), unicast)

#define BOOST_MIXIN_MESSAGE_2_OVERLOAD_IN_DOMAIN(domain, message, return_type, method_name, arg1_type, a1, arg2_type, a2) \
    BOOST_MIXIN_EXPORTED_MESSAGE_2_OVERLOAD_IN_DOMAIN(BOOST_PP_EMPTY(), domain, message, return_type, method_name, arg1_type, a1, arg2_type, a2)
#define BOOST_MIXIN_MESSAGE_2_IN_DOMAIN(domain, return_type, message, arg1_type, a1, arg2_type, a2) \
    BOOST_MIXIN_MESSAGE_2_OVERLOAD_IN_DOMAIN(domain, message, return_type, message, arg1_type, a1, arg2_type, a2)
#define BOOST_MIXIN_MESSAGE_2(return_type, message, arg1_t, arg1, arg2_t, arg2) \
    BOOST_MIXIN_MESSAGE_2_IN_DOMAIN(::boost::mixin::default_domain, return_type, message, arg1_type, a1, arg2_type, a2)
#define BOOST_MIXIN_EXPORTED_MESSAGE_2_IN_DOMAIN(export, domain, return_type, message, arg1_type, a1, arg2_type, a2) \
    BOOST_MIXIN_EXPORTED_MESSAGE_2_OVERLOAD_IN_DOMAIN(export, domain, message, return_type, message, arg1_type, a1, arg2_type, a2)
#define BOOST_MIXIN_EXPORTED_MESSAGE_2(export, return_type, message, arg1_t, arg1, arg2_t, arg2) \
    BOOST_MIXIN_EXPORTED_MESSAGE_2_IN_DOMAIN(export, ::boost::mixin::default_domain, return_type, message, arg1_type, a1, arg2_type, a2)
*/


#define BOOST_MIXIN_DEFINE_MESSAGE_IN_DOMAIN(domain_tag, message_name) \
    /* specialize _boost_mixin_domain_for_type to bind this message struct's type to its domain tag */ \
    template <> struct _boost_mixin_domain_for_type<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)> { typedef domain_tag tag; }; \
    /* provide a tag instance */ \
    _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) * _BOOST_MIXIN_MESSAGE_TAG(message_name); \
    /* create a feature getter for the message */ \
    ::boost::mixin::feature& _boost_get_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*) \
    { \
        return ::boost::mixin::internal::feature_instance<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)>::the_feature(); \
    } \
    /* create a feature registrator */ \
    void _boost_register_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*) \
    { \
        ::boost::mixin::internal::get_domain_for_tag<domain_tag>(). \
            register_feature(::boost::mixin::internal::feature_instance<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)>::the_feature()); \
    } \

#define BOOST_MIXIN_DEFINE_MESSAGE(message_name) \
    BOOST_MIXIN_DEFINE_MESSAGE_IN_DOMAIN(::boost::mixin::default_domain, message_name)


#endif // BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED
