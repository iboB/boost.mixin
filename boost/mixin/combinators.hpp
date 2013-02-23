//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#pragma once
#if !defined(_BOOST_MIXIN_COMBINATORS_HPP_INCLUDED)
#define _BOOST_MIXIN_COMBINATORS_HPP_INCLUDED

#include "global.hpp"
#include "mixin_collection.hpp"

// common multicast result combinators

namespace boost
{
namespace mixin
{
namespace combinators
{

template <typename MessageReturnType = bool>
class boolean_and
{
public:
    typedef bool result_type;

    boolean_and() 
        : _result(true)
    {}

    bool add_result(const MessageReturnType& r)
    {
        _result = _result && bool(r);

        return _result; // stop at the first false
    }

    bool result() const
    {
        return _result;
    }

    void reset()
    {
        _result = true;
    }

private:
    bool _result;
};

template <typename MessageReturnType = bool>
class boolean_or
{
public:
    typedef bool result_type;

    boolean_or() 
        : _result(false)
    {}

    bool add_result(const MessageReturnType& r)
    {
        _result = _result || bool(r);

        return !_result; // stop at the first true
    }

    bool result() const
    {
        return _result;
    }

    void reset()
    {
        _result = false;
    }

private:
    bool _result;
};

template <typename MessageReturnType>
class sum
{
public:
    typedef MessageReturnType result_type;

    sum()
        : _result(0)
    {}

    bool add_result(const MessageReturnType& r)
    {
        _result += r;
        return true;
    }

    const result_type& result() const
    {
        return _result;
    }

    void reset()
    {
        _result = 0;
    }

private:
    result_type _result;
};

} // namespace combinators
} // namespace mixin
} // namespace boost

#endif // _BOOST_MIXIN_COMBINATORS_HPP_INCLUDED
