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

#include "feature.hpp"
#include "third_party/fd_simplify_mem_func.hpp" // code from fastdelegate by Don Clugston

namespace boost
{
namespace mixin
{

namespace internal
{

// feature tags are used by feature parsers and domains to distinguish between feature types
// and register them appropriately
struct message_feature_tag {};

struct BOOST_MIXIN_API message_t : public feature
{
    enum e_mechanism
    {
        unicast,
        multicast,

        num_mechanism_types
    };

    const e_mechanism mechanism;

    /* the way messages identify themselves to domains and feature parsers */
    typedef message_feature_tag feature_tag;

protected:
    message_t(const char* name, e_mechanism mecha)
        : feature(name)
        , mechanism(mecha)
    {}
};

#if defined(FASTDLGT_HASINHERITANCE_KEYWORDS)
class __single_inheritance dummy_type;
#else
class dummy_type;
#endif

typedef void (dummy_type::*method_ptr)();
typedef ptrdiff_t method_offset;

template <typename Message>
struct message_priority
{
    int priority;
};

template <typename Mixin, typename MixinMethod>
void get_method_and_method_offset(const Mixin*, const MixinMethod& actual_method, method_ptr& out_method, method_offset& out_method_offset)
{
    // crunch the method pointer through fastdelegate's SimplifyMemFunc to obtain
    // a unified representation for all methods

    // get some random address
    // we only need it as an offset
    Mixin* offset_base = reinterpret_cast<Mixin*>(0xBAD);

    typedef void (fastdelegate::GenericClass::*fastdelegate_method)();
    fastdelegate_method simplified_method; // output parameter from SimplifyMemFunc

    typedef fastdelegate::GenericClass fastdelegate_class;
    fastdelegate_class* offset_object = fastdelegate::SimplifyMemFunc<sizeof(MixinMethod)>::Convert(offset_base, actual_method, simplified_method);

    // get offset in bytes
    out_method_offset = reinterpret_cast<char*>(offset_object) - reinterpret_cast<char*>(offset_base);

    // get new method location
    out_method = reinterpret_cast<method_ptr>(simplified_method);
}

// a structure that describes a message with specific data for a concrete mixin
struct BOOST_MIXIN_API message_for_mixin
{
    message_t* message; // message object
    mixin_type_info* mixin_info; // mixin type information

    // the following members are obtained via a special function that converts any methods
    // to these two pointers
    method_ptr func;
    method_offset func_offset;

    // message perks
    int priority;
};


} // namespace internal

template <typename Message>
internal::message_priority<Message> priority(int p, Message*)
{
    internal::message_priority<Message> mp;
    mp.priority = p;
    return mp;
}

}
}


#endif // BOOST_MIXIN_MESSAGE_HPP_INCLUDED
