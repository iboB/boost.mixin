//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_MESSAGE_HPP_INCLUDED)
#define _BOOST_MIXIN_MESSAGE_HPP_INCLUDED

#include "feature.hpp"

namespace boost
{
namespace mixin
{

namespace internal
{

// feature tags are used by feature parsers and domains to distinguish between feature types
// and register them appropriately
struct BOOST_MIXIN_API message_feature_tag {};

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
    message_t(const char* name, e_mechanism mecha, bool is_private)
        : feature(name, is_private)
        , mechanism(mecha)
    {}
};

typedef void (*func_ptr)();

template <typename Message>
struct message_priority
{
    int priority;
};

// a structure that describes a message with specific data for a concrete mixin
struct BOOST_MIXIN_API message_for_mixin
{
    message_t* message; // message object
    mixin_id _mixin_id; // id of the handling mixin type information

    // the caller member is a pointer to a template function instantiated by the message macros
    // this function takes the appropriate parameters as arguments and is instantiated to call
    // the mixin method, even from void*
    // code based on
    // http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
    func_ptr caller;

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


#endif // _BOOST_MIXIN_MESSAGE_HPP_INCLUDED
