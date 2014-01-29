//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED)
#define _BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED

/**
 * \file
 * Macros used to declare and define messages.
 */


#include "message.hpp"

// some macros here have an underscore in front so it doesn't appear as a suggestion in
// ides that support these
// that shows that they're for internal use only

/// INTERNAL ONLY
#define _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) BOOST_PP_CAT(boost_mixin_msg_, message_name)
/// INTERNAL ONLY
#define _BOOST_MIXIN_MESSAGE_TAG(message_name) BOOST_PP_CAT(message_name, _msg)

// a macro used in messages to get the mixin data directly, skipping function calls
// GREATLY improves message call time
/// INTERNAL ONLY
#define _BOOST_MIXIN_GET_MIXIN_DATA(obj, id) \
    reinterpret_cast<char*>(const_cast<void*>(obj._mixin_data[obj._type_info->_mixin_indices[id]].mixin()))

// a macro that forwards rvalue references message arguments in c++11
#if BOOST_MIXIN_USING_CXX11
#   define BOOST_MIXIN_FWD(arg_type, arg) std::forward<arg_type>(arg)
#else
#   define BOOST_MIXIN_FWD(arg_type, arg) arg
#endif

#if defined(BOOST_MIXIN_DOXYGEN)
// use there macros for the docs only
#define BOOST_MIXIN_MESSAGE_N(return_type, message)
#define BOOST_MIXIN_CONST_MESSAGE_N(return_type, message)
#define BOOST_MIXIN_MULTICAST_MESSAGE_N(return_type, message)
#define BOOST_MIXIN_CONST_MULTICAST_MESSAGE_N(return_type, message)
#define BOOST_MIXIN_EXPORTED_MESSAGE_N(export, return_type, message)
#define BOOST_MIXIN_EXPORTED_CONST_MESSAGE_N(export, return_type, message)
#define BOOST_MIXIN_EXPORTED_MULTICAST_MESSAGE_N(export, return_type, message)
#define BOOST_MIXIN_EXPORTED_CONST_MULTICAST_MESSAGE_N(export, return_type, message)
#define BOOST_MIXIN_MESSAGE_N_OVERLOAD(message_name, return_type, method_name)
#define BOOST_MIXIN_CONST_MESSAGE_N_OVERLOAD(message_name, return_type, method_name)
#define BOOST_MIXIN_MULTICAST_MESSAGE_N_OVERLOAD(message_name, return_type, method_name)
#define BOOST_MIXIN_CONST_MULTICAST_MESSAGE_N_OVERLOAD(message_name, return_type, method_name)
#define BOOST_MIXIN_EXPORTED_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name)
#define BOOST_MIXIN_EXPORTED_CONST_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name)
#define BOOST_MIXIN_EXPORTED_MULTICAST_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name)
#define BOOST_MIXIN_EXPORTED_CONST_MULTICAST_MESSAGE_N_OVERLOAD(export, message_name, return_type, method_name)
#else
// include the generated macros
#include "gen/message_macros.ipp"
#endif

/**
 * The macro for defining a message
 * Use it once per message in a compilation unit (.cpp file)
 */
#define BOOST_MIXIN_DEFINE_MESSAGE(message_name) \
    /* create a feature getter for the message */ \
    ::boost::mixin::feature& _boost_get_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*) \
    { \
        return ::boost::mixin::internal::feature_instance<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)>::the_feature(); \
    } \
    /* create a feature registrator */ \
    void _boost_register_mixin_feature(const _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*) \
    { \
        ::boost::mixin::internal::domain::instance(). \
            register_feature(::boost::mixin::internal::feature_instance<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)>::the_feature()); \
    } \
    /* provide a tag instance */ \
    _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) * _BOOST_MIXIN_MESSAGE_TAG(message_name)


#endif // _BOOST_MIXIN_MESSAGE_MACROS_HPP_INCLUDED
