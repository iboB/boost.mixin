//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_COMBINATORS_HPP_INCLUDED)
#define _BOOST_MIXIN_COMBINATORS_HPP_INCLUDED

/**
 * \file
 * Common multicast combinator classes.
 */

#include "global.hpp"
#include "mixin_collection.hpp"

namespace boost
{
namespace mixin
{
namespace combinators
{

/**
 * A combinator for a boolean `and` operation.
 *
 * \tparam MessageReturnType The actual return type of the messages.
 * Its default value is `bool`, but such an operation is valid for
 * various other classes that can be cast to `bool`, such as pointers,
 * std streams, etc.
 */
template <typename MessageReturnType = bool>
class boolean_and
{
public:
    typedef bool result_type;

    boolean_and()
        : _result(true)
    {}

    /// The function used by the code generated for multicast messages.
    /// Stops the execution of the multicast chain at the first `false`.
    bool add_result(const MessageReturnType& r)
    {
        _result = _result && bool(r);

        return _result; // stop at the first false
    }

    /// The result of the operation if the multicast call has been made
    /// with an input/ouput parameter - an instance of `boolean_and`
    bool result() const
    {
        return _result;
    }

    /// Resets the result, so the instance could be reused.
    void reset()
    {
        _result = true;
    }

private:
    bool _result;
};

/**
 * A combinator for a boolean `or` operation.
 *
 * \tparam MessageReturnType The actual return type of the messages.
 * Its default value is `bool`, but such an operation is valid for
 * various other classes that can be cast to `bool`, such as pointers,
 * std streams, etc.
 */
template <typename MessageReturnType = bool>
class boolean_or
{
public:
    typedef bool result_type;

    boolean_or()
        : _result(false)
    {}

    /// The function used by the code generated for multicast messages.
    /// Stops the execution of the multicast chain at the first `true`.
    bool add_result(const MessageReturnType& r)
    {
        _result = _result || bool(r);

        return !_result; // stop at the first true
    }

    /// The result of the operation if the multicast call has been made
    /// with an input/output parameter - an instance of `boolean_or`
    bool result() const
    {
        return _result;
    }

    /// Resets the result, so the instance could be reused.
    void reset()
    {
        _result = false;
    }

private:
    bool _result;
};

/**
 * A combinator for a sum operation. Sums all the return values of
 * the multicast chain.
 *
 * \tparam MessageReturnType The actual return type of the messages.
 */
template <typename MessageReturnType>
class sum
{
public:
    typedef MessageReturnType result_type;

    sum()
        : _result(0)
    {}

    /// The function used by the code generated for multicast messages.
    bool add_result(const MessageReturnType& r)
    {
        _result += r;
        return true;
    }

    /// The result of the operation if the multicast call has been made
    /// with an input/output parameter - an instance of `sum`
    const result_type& result() const
    {
        return _result;
    }

    /// Resets the result, so the instance could be reused.
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
