//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(BOOST_MIXIN_FEATURE_PARSER_HPP_INCLUDED)
#define BOOST_MIXIN_FEATURE_PARSER_HPP_INCLUDED

#include "global.hpp"
#include "message.hpp"
#include "object_type_info.hpp"

namespace boost
{
namespace mixin
{
namespace internal
{

// see comments in feature_instance on why this class is needed
class BOOST_MIXIN_API feature_registrator
{
public:
    template <typename Feature>
    feature_registrator& operator & (const Feature*)
    {
        _boost_register_mixin_feature((Feature*)nullptr);
        return *this;
    }

    template <typename Message>
    feature_registrator& operator & (message_priority<Message>)
    {
        _boost_register_mixin_feature((Message*)nullptr);
        return *this;
    }

    void operator & (const no_features_t*)
    {
    }
};


// parse mixin features and apply them to the mixin type info
template <typename Mixin>
class BOOST_MIXIN_API feature_parser
{
public:
    template <typename Feature>
    feature_parser& operator & (const Feature*)
    {
        Feature& f = static_cast<Feature&>(_boost_get_mixin_feature((Feature*)nullptr));
        BOOST_ASSERT(f.id != INVALID_FEATURE_ID);

        mixin_type_info& mixin_info = _boost_get_mixin_type_info((Mixin*)nullptr);
        parse_feature(mixin_info, f, typename Feature::feature_tag());

        return *this;
    }

    template <typename Message>
    feature_parser& operator & (message_priority<Message> mp)
    {
        Message& msg = static_cast<Message&>(_boost_get_mixin_feature((Message*)nullptr));
        BOOST_ASSERT(msg.id != INVALID_FEATURE_ID);

        mixin_type_info& mixin_info = _boost_get_mixin_type_info((Mixin*)nullptr);
        parse_message(mixin_info, msg, mp.priority);

        return *this;
    }

    void operator & (const no_features_t*)
    {
    }

private:
    template <typename Message>
    void parse_feature(mixin_type_info& mixin_info, Message& msg, const message_feature_tag&)
    {
        parse_message(mixin_info, msg, 0);
    }

    template <typename Message>
    void parse_message(mixin_type_info& mixin_info, Message& msg, int priority)
    {
#if defined(BOOST_MIXIN_DEBUG)
        // check for duplicate entries
        for(size_t i=0; i<mixin_info.message_infos.size(); ++i)
        {
            const message_for_mixin& msg_info = mixin_info.message_infos[i];
            BOOST_ASSERT(msg_info.message);
            BOOST_ASSERT(msg_info.message != &msg);
        }
#endif
        mixin_info.message_infos.resize(mixin_info.message_infos.size()+1);
        mixin_info.message_infos.back().message = &msg;
        mixin_info.message_infos.back().priority = priority;
        mixin_info.message_infos.back().mixin_info = &mixin_info;

        get_method_and_method_offset((Mixin*)nullptr,
                                     msg.template get_method_pointer_for<Mixin>(),
                                     mixin_info.message_infos.back().func,
                                     mixin_info.message_infos.back().func_offset);
    }
};

}
}
}

#endif
