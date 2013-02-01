//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// this file is automatically generated by a script



namespace boost
{
namespace mixin
{
namespace internal
{

template <typename Mixin, typename Ret , Ret (Mixin::*Method)()>
Ret caller0(void* mixin_ptr )
{
    Mixin* m = reinterpret_cast<Mixin*>(mixin_ptr);
    return (m->*Method)();
}

}
}
}


#define _BOOST_MIXIN_MESSAGE0_DECL(export, domain, message_name, method_name, return_type, constness, message_mechanism ) \
    /* mechanism shows whether it's a multicast or unicast */ \
    \
    /* step 1: define the message struct */ \
    struct export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) : public ::boost::mixin::internal::message_t \
    { \
        typedef return_type (*caller_func)(void* ); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)() \
            : ::boost::mixin::internal::message_t(BOOST_PP_STRINGIZE(message_name), message_mechanism) \
        {} \
        template <typename Mixin> \
        ::boost::mixin::internal::func_ptr get_caller_for() const \
        { \
            /* prevent the linker to optimize away the caller function */ \
            static caller_func caller = ::boost::mixin::internal::caller0<constness Mixin, return_type , &Mixin::method_name>; \
            /* cast the caller to a void (*)() - safe acoording to the standard */ \
            return reinterpret_cast< ::boost::mixin::internal::func_ptr>(caller); \
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

#define _BOOST_MIXIN_MESSAGE0_UNI(export, domain, message_name, method_name, return_type, constness ) \
    _BOOST_MIXIN_MESSAGE0_DECL(export, domain, message_name, method_name, return_type, constness, unicast ) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline return_type method_name(constness ::boost::mixin::object* obj ) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::unicast); \
        const ::boost::mixin::internal::object_type_info::call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const ::boost::mixin::internal::message_for_mixin* msg_data = call_entry.message_data; \
        BOOST_ASSERT(msg_data); \
        BOOST_ASSERT(msg_data->message == &self); \
        char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
        return func(mixin_data ); \
    }\

#define _BOOST_MIXIN_MESSAGE0_MULTI(export, domain, message_name, method_name, return_type, constness ) \
    _BOOST_MIXIN_MESSAGE0_DECL(export, domain, message_name, method_name, return_type, constness, multicast ) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline void method_name(constness ::boost::mixin::object* obj ) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::multicast); \
        typedef ::boost::mixin::internal::object_type_info::call_table_entry call_table_entry; \
        const call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const call_table_entry* begin = call_entry.multicast_begin; \
        const call_table_entry* end = call_entry.multicast_end; \
        BOOST_ASSERT(begin); \
        BOOST_ASSERT(end); \
        for(const call_table_entry* iter = begin; iter!=end; ++iter) \
        { \
            const ::boost::mixin::internal::message_for_mixin* msg_data = iter->message_data; \
            BOOST_ASSERT(msg_data); \
            BOOST_ASSERT(msg_data->message == &self); \
            char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
            _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
            return func(mixin_data ); \
        }\
    }

#define BOOST_MIXIN_MESSAGE_0(return_type, message ) \
    _BOOST_MIXIN_MESSAGE0_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() )

#define BOOST_MIXIN_CONST_MESSAGE_0(return_type, message ) \
    _BOOST_MIXIN_MESSAGE0_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const )

#define BOOST_MIXIN_MULTICAST_MESSAGE_0(return_type, message ) \
    _BOOST_MIXIN_MESSAGE0_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() )

#define BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(return_type, message ) \
    _BOOST_MIXIN_MESSAGE0_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const )


namespace boost
{
namespace mixin
{
namespace internal
{

template <typename Mixin, typename Ret , typename arg0_type, Ret (Mixin::*Method)(arg0_type)>
Ret caller1(void* mixin_ptr , arg0_type a0)
{
    Mixin* m = reinterpret_cast<Mixin*>(mixin_ptr);
    return (m->*Method)(a0);
}

}
}
}


#define _BOOST_MIXIN_MESSAGE1_DECL(export, domain, message_name, method_name, return_type, constness, message_mechanism , arg0_type, a0) \
    /* mechanism shows whether it's a multicast or unicast */ \
    \
    /* step 1: define the message struct */ \
    struct export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) : public ::boost::mixin::internal::message_t \
    { \
        typedef return_type (*caller_func)(void* , arg0_type); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)() \
            : ::boost::mixin::internal::message_t(BOOST_PP_STRINGIZE(message_name), message_mechanism) \
        {} \
        template <typename Mixin> \
        ::boost::mixin::internal::func_ptr get_caller_for() const \
        { \
            /* prevent the linker to optimize away the caller function */ \
            static caller_func caller = ::boost::mixin::internal::caller1<constness Mixin, return_type , arg0_type, &Mixin::method_name>; \
            /* cast the caller to a void (*)() - safe acoording to the standard */ \
            return reinterpret_cast< ::boost::mixin::internal::func_ptr>(caller); \
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

#define _BOOST_MIXIN_MESSAGE1_UNI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0) \
    _BOOST_MIXIN_MESSAGE1_DECL(export, domain, message_name, method_name, return_type, constness, unicast , arg0_type, a0) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline return_type method_name(constness ::boost::mixin::object* obj , arg0_type a0) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::unicast); \
        const ::boost::mixin::internal::object_type_info::call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const ::boost::mixin::internal::message_for_mixin* msg_data = call_entry.message_data; \
        BOOST_ASSERT(msg_data); \
        BOOST_ASSERT(msg_data->message == &self); \
        char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
        return func(mixin_data , a0); \
    }\

#define _BOOST_MIXIN_MESSAGE1_MULTI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0) \
    _BOOST_MIXIN_MESSAGE1_DECL(export, domain, message_name, method_name, return_type, constness, multicast , arg0_type, a0) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline void method_name(constness ::boost::mixin::object* obj , arg0_type a0) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::multicast); \
        typedef ::boost::mixin::internal::object_type_info::call_table_entry call_table_entry; \
        const call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const call_table_entry* begin = call_entry.multicast_begin; \
        const call_table_entry* end = call_entry.multicast_end; \
        BOOST_ASSERT(begin); \
        BOOST_ASSERT(end); \
        for(const call_table_entry* iter = begin; iter!=end; ++iter) \
        { \
            const ::boost::mixin::internal::message_for_mixin* msg_data = iter->message_data; \
            BOOST_ASSERT(msg_data); \
            BOOST_ASSERT(msg_data->message == &self); \
            char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
            _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
            return func(mixin_data , a0); \
        }\
    }

#define BOOST_MIXIN_MESSAGE_1(return_type, message , arg0_type, a0) \
    _BOOST_MIXIN_MESSAGE1_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0)

#define BOOST_MIXIN_CONST_MESSAGE_1(return_type, message , arg0_type, a0) \
    _BOOST_MIXIN_MESSAGE1_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0)

#define BOOST_MIXIN_MULTICAST_MESSAGE_1(return_type, message , arg0_type, a0) \
    _BOOST_MIXIN_MESSAGE1_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0)

#define BOOST_MIXIN_CONST_MULTICAST_MESSAGE_1(return_type, message , arg0_type, a0) \
    _BOOST_MIXIN_MESSAGE1_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0)


namespace boost
{
namespace mixin
{
namespace internal
{

template <typename Mixin, typename Ret , typename arg0_type, typename arg1_type, Ret (Mixin::*Method)(arg0_type, arg1_type)>
Ret caller2(void* mixin_ptr , arg0_type a0, arg1_type a1)
{
    Mixin* m = reinterpret_cast<Mixin*>(mixin_ptr);
    return (m->*Method)(a0, a1);
}

}
}
}


#define _BOOST_MIXIN_MESSAGE2_DECL(export, domain, message_name, method_name, return_type, constness, message_mechanism , arg0_type, a0, arg1_type, a1) \
    /* mechanism shows whether it's a multicast or unicast */ \
    \
    /* step 1: define the message struct */ \
    struct export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) : public ::boost::mixin::internal::message_t \
    { \
        typedef return_type (*caller_func)(void* , arg0_type, arg1_type); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)() \
            : ::boost::mixin::internal::message_t(BOOST_PP_STRINGIZE(message_name), message_mechanism) \
        {} \
        template <typename Mixin> \
        ::boost::mixin::internal::func_ptr get_caller_for() const \
        { \
            /* prevent the linker to optimize away the caller function */ \
            static caller_func caller = ::boost::mixin::internal::caller2<constness Mixin, return_type , arg0_type, arg1_type, &Mixin::method_name>; \
            /* cast the caller to a void (*)() - safe acoording to the standard */ \
            return reinterpret_cast< ::boost::mixin::internal::func_ptr>(caller); \
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

#define _BOOST_MIXIN_MESSAGE2_UNI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0, arg1_type, a1) \
    _BOOST_MIXIN_MESSAGE2_DECL(export, domain, message_name, method_name, return_type, constness, unicast , arg0_type, a0, arg1_type, a1) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline return_type method_name(constness ::boost::mixin::object* obj , arg0_type a0, arg1_type a1) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::unicast); \
        const ::boost::mixin::internal::object_type_info::call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const ::boost::mixin::internal::message_for_mixin* msg_data = call_entry.message_data; \
        BOOST_ASSERT(msg_data); \
        BOOST_ASSERT(msg_data->message == &self); \
        char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
        return func(mixin_data , a0, a1); \
    }\

#define _BOOST_MIXIN_MESSAGE2_MULTI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0, arg1_type, a1) \
    _BOOST_MIXIN_MESSAGE2_DECL(export, domain, message_name, method_name, return_type, constness, multicast , arg0_type, a0, arg1_type, a1) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline void method_name(constness ::boost::mixin::object* obj , arg0_type a0, arg1_type a1) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::multicast); \
        typedef ::boost::mixin::internal::object_type_info::call_table_entry call_table_entry; \
        const call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const call_table_entry* begin = call_entry.multicast_begin; \
        const call_table_entry* end = call_entry.multicast_end; \
        BOOST_ASSERT(begin); \
        BOOST_ASSERT(end); \
        for(const call_table_entry* iter = begin; iter!=end; ++iter) \
        { \
            const ::boost::mixin::internal::message_for_mixin* msg_data = iter->message_data; \
            BOOST_ASSERT(msg_data); \
            BOOST_ASSERT(msg_data->message == &self); \
            char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
            _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
            return func(mixin_data , a0, a1); \
        }\
    }

#define BOOST_MIXIN_MESSAGE_2(return_type, message , arg0_type, a0, arg1_type, a1) \
    _BOOST_MIXIN_MESSAGE2_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0, arg1_type, a1)

#define BOOST_MIXIN_CONST_MESSAGE_2(return_type, message , arg0_type, a0, arg1_type, a1) \
    _BOOST_MIXIN_MESSAGE2_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0, arg1_type, a1)

#define BOOST_MIXIN_MULTICAST_MESSAGE_2(return_type, message , arg0_type, a0, arg1_type, a1) \
    _BOOST_MIXIN_MESSAGE2_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0, arg1_type, a1)

#define BOOST_MIXIN_CONST_MULTICAST_MESSAGE_2(return_type, message , arg0_type, a0, arg1_type, a1) \
    _BOOST_MIXIN_MESSAGE2_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0, arg1_type, a1)


namespace boost
{
namespace mixin
{
namespace internal
{

template <typename Mixin, typename Ret , typename arg0_type, typename arg1_type, typename arg2_type, Ret (Mixin::*Method)(arg0_type, arg1_type, arg2_type)>
Ret caller3(void* mixin_ptr , arg0_type a0, arg1_type a1, arg2_type a2)
{
    Mixin* m = reinterpret_cast<Mixin*>(mixin_ptr);
    return (m->*Method)(a0, a1, a2);
}

}
}
}


#define _BOOST_MIXIN_MESSAGE3_DECL(export, domain, message_name, method_name, return_type, constness, message_mechanism , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    /* mechanism shows whether it's a multicast or unicast */ \
    \
    /* step 1: define the message struct */ \
    struct export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) : public ::boost::mixin::internal::message_t \
    { \
        typedef return_type (*caller_func)(void* , arg0_type, arg1_type, arg2_type); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)() \
            : ::boost::mixin::internal::message_t(BOOST_PP_STRINGIZE(message_name), message_mechanism) \
        {} \
        template <typename Mixin> \
        ::boost::mixin::internal::func_ptr get_caller_for() const \
        { \
            /* prevent the linker to optimize away the caller function */ \
            static caller_func caller = ::boost::mixin::internal::caller3<constness Mixin, return_type , arg0_type, arg1_type, arg2_type, &Mixin::method_name>; \
            /* cast the caller to a void (*)() - safe acoording to the standard */ \
            return reinterpret_cast< ::boost::mixin::internal::func_ptr>(caller); \
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

#define _BOOST_MIXIN_MESSAGE3_UNI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE3_DECL(export, domain, message_name, method_name, return_type, constness, unicast , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline return_type method_name(constness ::boost::mixin::object* obj , arg0_type a0, arg1_type a1, arg2_type a2) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::unicast); \
        const ::boost::mixin::internal::object_type_info::call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const ::boost::mixin::internal::message_for_mixin* msg_data = call_entry.message_data; \
        BOOST_ASSERT(msg_data); \
        BOOST_ASSERT(msg_data->message == &self); \
        char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
        return func(mixin_data , a0, a1, a2); \
    }\

#define _BOOST_MIXIN_MESSAGE3_MULTI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE3_DECL(export, domain, message_name, method_name, return_type, constness, multicast , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline void method_name(constness ::boost::mixin::object* obj , arg0_type a0, arg1_type a1, arg2_type a2) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::multicast); \
        typedef ::boost::mixin::internal::object_type_info::call_table_entry call_table_entry; \
        const call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const call_table_entry* begin = call_entry.multicast_begin; \
        const call_table_entry* end = call_entry.multicast_end; \
        BOOST_ASSERT(begin); \
        BOOST_ASSERT(end); \
        for(const call_table_entry* iter = begin; iter!=end; ++iter) \
        { \
            const ::boost::mixin::internal::message_for_mixin* msg_data = iter->message_data; \
            BOOST_ASSERT(msg_data); \
            BOOST_ASSERT(msg_data->message == &self); \
            char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
            _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
            return func(mixin_data , a0, a1, a2); \
        }\
    }

#define BOOST_MIXIN_MESSAGE_3(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE3_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0, arg1_type, a1, arg2_type, a2)

#define BOOST_MIXIN_CONST_MESSAGE_3(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE3_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0, arg1_type, a1, arg2_type, a2)

#define BOOST_MIXIN_MULTICAST_MESSAGE_3(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE3_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0, arg1_type, a1, arg2_type, a2)

#define BOOST_MIXIN_CONST_MULTICAST_MESSAGE_3(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2) \
    _BOOST_MIXIN_MESSAGE3_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0, arg1_type, a1, arg2_type, a2)


namespace boost
{
namespace mixin
{
namespace internal
{

template <typename Mixin, typename Ret , typename arg0_type, typename arg1_type, typename arg2_type, typename arg3_type, Ret (Mixin::*Method)(arg0_type, arg1_type, arg2_type, arg3_type)>
Ret caller4(void* mixin_ptr , arg0_type a0, arg1_type a1, arg2_type a2, arg3_type a3)
{
    Mixin* m = reinterpret_cast<Mixin*>(mixin_ptr);
    return (m->*Method)(a0, a1, a2, a3);
}

}
}
}


#define _BOOST_MIXIN_MESSAGE4_DECL(export, domain, message_name, method_name, return_type, constness, message_mechanism , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    /* mechanism shows whether it's a multicast or unicast */ \
    \
    /* step 1: define the message struct */ \
    struct export _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name) : public ::boost::mixin::internal::message_t \
    { \
        typedef return_type (*caller_func)(void* , arg0_type, arg1_type, arg2_type, arg3_type); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)() \
            : ::boost::mixin::internal::message_t(BOOST_PP_STRINGIZE(message_name), message_mechanism) \
        {} \
        template <typename Mixin> \
        ::boost::mixin::internal::func_ptr get_caller_for() const \
        { \
            /* prevent the linker to optimize away the caller function */ \
            static caller_func caller = ::boost::mixin::internal::caller4<constness Mixin, return_type , arg0_type, arg1_type, arg2_type, arg3_type, &Mixin::method_name>; \
            /* cast the caller to a void (*)() - safe acoording to the standard */ \
            return reinterpret_cast< ::boost::mixin::internal::func_ptr>(caller); \
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

#define _BOOST_MIXIN_MESSAGE4_UNI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    _BOOST_MIXIN_MESSAGE4_DECL(export, domain, message_name, method_name, return_type, constness, unicast , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline return_type method_name(constness ::boost::mixin::object* obj , arg0_type a0, arg1_type a1, arg2_type a2, arg3_type a3) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::unicast); \
        const ::boost::mixin::internal::object_type_info::call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const ::boost::mixin::internal::message_for_mixin* msg_data = call_entry.message_data; \
        BOOST_ASSERT(msg_data); \
        BOOST_ASSERT(msg_data->message == &self); \
        char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
        _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
        return func(mixin_data , a0, a1, a2, a3); \
    }\

#define _BOOST_MIXIN_MESSAGE4_MULTI(export, domain, message_name, method_name, return_type, constness , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    _BOOST_MIXIN_MESSAGE4_DECL(export, domain, message_name, method_name, return_type, constness, multicast , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    /* step 4: define the message function -> the one that will be called for the objects */ \
    inline void method_name(constness ::boost::mixin::object* obj , arg0_type a0, arg1_type a1, arg2_type a2, arg3_type a3) \
    {\
        ::boost::mixin::feature& self = _boost_get_mixin_feature((_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)*)nullptr); \
        BOOST_ASSERT(static_cast< ::boost::mixin::internal::message_t&>(self).mechanism == ::boost::mixin::internal::message_t::multicast); \
        typedef ::boost::mixin::internal::object_type_info::call_table_entry call_table_entry; \
        const call_table_entry& call_entry = obj->_type_info->_call_table[self.id]; \
        const call_table_entry* begin = call_entry.multicast_begin; \
        const call_table_entry* end = call_entry.multicast_end; \
        BOOST_ASSERT(begin); \
        BOOST_ASSERT(end); \
        for(const call_table_entry* iter = begin; iter!=end; ++iter) \
        { \
            const ::boost::mixin::internal::message_for_mixin* msg_data = iter->message_data; \
            BOOST_ASSERT(msg_data); \
            BOOST_ASSERT(msg_data->message == &self); \
            char* mixin_data = reinterpret_cast<char*>(const_cast<void*>(obj->internal_get_mixin(*msg_data->mixin_info))); \
            _BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func func = reinterpret_cast<_BOOST_MIXIN_MESSAGE_STRUCT_NAME(message_name)::caller_func>(msg_data->caller); \
            return func(mixin_data , a0, a1, a2, a3); \
        }\
    }

#define BOOST_MIXIN_MESSAGE_4(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    _BOOST_MIXIN_MESSAGE4_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3)

#define BOOST_MIXIN_CONST_MESSAGE_4(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    _BOOST_MIXIN_MESSAGE4_UNI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3)

#define BOOST_MIXIN_MULTICAST_MESSAGE_4(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    _BOOST_MIXIN_MESSAGE4_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, BOOST_PP_EMPTY() , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3)

#define BOOST_MIXIN_CONST_MULTICAST_MESSAGE_4(return_type, message , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3) \
    _BOOST_MIXIN_MESSAGE4_MULTI(BOOST_PP_EMPTY(), ::boost::mixin::default_domain, message, message, return_type, const , arg0_type, a0, arg1_type, a1, arg2_type, a2, arg3_type, a3)

