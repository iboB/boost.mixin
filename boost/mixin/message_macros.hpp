//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED)
#define _BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED

#include "message.hpp"

// some macros here have an underscore in front so it doesn't appear as a suggestion in
// ides that support these
// that shows that they're for internal use only

#define _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) BOOST_PP_CAT(boost_mixin_msg_, message_name)
#define _BOOST_MIXIN_MESSAGE_TAG(message_name) BOOST_PP_CAT(message_name, _msg)

// a macro used in messages to get the mixin data directly, skipping function calls
// GREATLY improves message call time
#define _BOOST_MIXIN_GET_MIXIN_DATA(obj, id) \
    reinterpret_cast<char*>(const_cast<void*>(obj._mixin_data[obj._type_info->_mixin_indices[id]].mixin()))

// a macro that forwards rvalue references message arguments in c++11
#if BOOST_MIXIN_USING_CXX11
#   define BOOST_MIXIN_FWD(arg_type, arg) std::forward<arg_type>(arg)
#else
#   define BOOST_MIXIN_FWD(arg_type, arg) arg
#endif

#include "gen/message_macros.ipp"

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
    /* provide a tag instance */ \
    _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) * _BOOST_MIXIN_MESSAGE_TAG(message_name)

#define BOOST_MIXIN_DEFINE_MESSAGE(message_name) \
    BOOST_MIXIN_DEFINE_MESSAGE_IN_DOMAIN(::boost::mixin::default_domain, message_name)


#endif // _BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED
