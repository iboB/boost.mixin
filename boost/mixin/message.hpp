//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_MESSAGE_HPP_INCLUDED)
#define BOOST_MIXIN_MESSAGE_HPP_INCLUDED

// some macros here have an underscore in front so it doesn't appear as a suggestion in
// ides that support these
// that shows that they're for internal use only

#define _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) BOOST_PP_CAT(boost_mixin_msg_, message_name)
#define _BOOST_MIXIN_MESSAGE_TAG(message_name) BOOST_PP_CAT(message_name, _msg)

#define _BOOST_MIXIN_MESSAGE2(export, domain, message_name, method_name, return_type, arg1_type, a1, arg2_type, a2, constness, mechanism) \
        /* mechanism shows whether it's a multicast or unicast */ \
    \
    /* step 1: define the message struct */ \
    struct export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) : public ::boost::mixin::feature \
    { \
        template <typename Mixin> \
        return_type (Mixin::*get_method_pointer_for())(arg1_type, arg2_type) constness \
        { \
            /* use static_cast to show that we mean exactly the method with this signature and not some overload */ \
            return static_cast<return_type (Mixin::*)(arg1_type, arg2_type)>(&Mixin::method_name); \
        } \
    }; \
    /* step 2: define a message tag, that will be used to identify the message in feature lists */ \
    extern export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) * _BOOST_MIXIN_MESSAGE_TAG(message_name); \
    /* step 3: define the message function -> the one that will be called for the objects */ \
    inline return_type method_name(constness ::boost::mixin::object* obj, arg1_type a1, arg2_type a2) \
    {\
    }\
    /* step 4: declare the feature getter for the message */ \
    extern export ::boost::mixin::feature& _boost_get_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*);

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
    /* create a function that will reference feature_type_info's static registrator to guarantee its instantiation */ \
    inline void _boost_register_message(_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*) { ::boost::mixin::internal::feature_instance<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)>::registrator.unused = true; } \
    /* create a feature getter for the message */ \
    ::boost::mixin::feature& _boost_get_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*) { return ::boost::mixin::internal::feature_instance<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)>::the_feature(); } \

#define BOOST_MIXIN_DEFINE_MESSAGE(message_name) \
    BOOST_MIXIN_DEFINE_MESSAGE_IN_DOMAIN(::boost::mixin::default_domain, message_name)


#endif // BOOST_MIXIN_MESSAGE_HPP_INCLUDED
