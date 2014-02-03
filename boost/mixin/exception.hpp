//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_EXCEPTION_HPP_INCLUDED)
#define _BOOST_MIXIN_EXCEPTION_HPP_INCLUDED

/**
 * \file
 * The library's exceptions.
 */

#include "global.hpp"
#include <boost/preprocessor/stringize.hpp>

#if BOOST_MIXIN_USE_EXCEPTIONS

#include <boost/exception/all.hpp>

namespace boost
{
namespace mixin
{

/// Parent class of all Boost.Mixin exceptions.
class exception : public ::boost::exception, public ::std::exception {};

/// Thrown when an attempt is made to mutate an object whose type doesn't correspond
/// to the expected source type of the mutation (the one to mutate *from*).
///
/// The most likely cause is when using a `same_type_mutator` on objects that are
/// not of the same type
class bad_mutation_source : public exception {};

/// Thrown when a message is called on a object that doesn't implement it.
class bad_message_call : public exception {};

/// Thrown when an object type is created which has mixins that implement 
/// the same unicast message with the same priority
class unicast_clash : public exception {};

}
}

/// A macro that throws an exception if `BOOST_MIXIN_USE_EXCEPTIONS`
/// is defined. If it's not, it falls back to an assert.
#   define BOOST_MIXIN_THROW_UNLESS(test, exception) do { if(!(test)) { BOOST_THROW_EXCEPTION(exception()); } } while(false)

#else // BOOST_MIXIN_USE_EXCEPTIONS

#   define BOOST_MIXIN_THROW_UNLESS(test, exception) BOOST_ASSERT_MSG(test, BOOST_PP_STRINGIZE(exception))

#endif // BOOST_MIXIN_USE_EXCEPTIONS

#endif // _BOOST_MIXIN_EXCEPTION_HPP_INCLUDED
