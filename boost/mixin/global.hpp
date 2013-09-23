//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#if !defined(_BOOST_MIXIN_GLOBAL_HPP_INCLUDED)
#define _BOOST_MIXIN_GLOBAL_HPP_INCLUDED

#include "config.hpp"

#include <boost/assert.hpp>
#include <algorithm>
#include <vector>
#include <cstring> // for memset

#if BOOST_MIXIN_USING_CXX11
#   define BOOST_MIXIN_CXX11_NAMESPACE std // some c++11 classes exist in boost use this namespace for them
#else
#   define BOOST_MIXIN_CXX11_NAMESPACE boost
#   define nullptr NULL
#endif

// getting a type's name
#if BOOST_MIXIN_USE_TYPEID // using typeid: tested for msvc and gcc
#   if defined(__GNUC__)
        // use cxxabi to unmangle the gcc typeid name
#       include <cxxabi.h>
        namespace boost { namespace mixin { namespace internal { extern BOOST_MIXIN_API int cxa_demangle_status; } } }
#       define BOOST_MIXIN_TYPE_NAME(type) abi::__cxa_demangle(typeid(type).name(), nullptr, nullptr, &::boost::mixin::internal::cxa_demangle_status)
#   elif defined(_MSC_VER)
        // msvc typeid names are "class x" instead of "x", remove the "class " by adding 6
#       define BOOST_MIXIN_TYPE_NAME(type) (typeid(type).name() + 6)
#   else
#       error "getting typenames with typeid hasn't been tested on compilers other than gcc and msvc"
#   endif
#else // safer but more inconvinient way
#   define BOOST_MIXIN_TYPE_NAME(type) type::boost_mixin_name()
#endif

// logically internal data within classes that cannot be private or protected
// due to implementation issues is marked with boost_mixin_internal
// class X
// {
// public:
//    <public stuff>
// boost_mixin_internal:
//    <actually public but logically private stuff>
// };
#define boost_mixin_internal public

#include <bitset>

namespace boost
{
namespace mixin
{
    typedef size_t mixin_id;
    typedef size_t feature_id;

namespace internal
{
    // simple and often used function that just checks if an element is present
    // in a forward-iteratable container
    template <typename Container, typename Elem>
    bool has_elem(const Container& c, const Elem& e)
    {
        return std::find(c.begin(), c.end(), e) != c.end();
    }

    template <typename T>
    bool is_sorted(const std::vector<T>& v)
    {
        return std::adjacent_find(v.begin(), v.end(), std::greater<T>()) == v.end();
    }

    inline void zero_memory(void* mem, size_t size)
    {
        std::memset(mem, 0, size);
    }

    class mixin_type_info;
    typedef std::vector<const mixin_type_info*> mixin_type_info_vector;

    typedef std::bitset<BOOST_MIXIN_MAX_MIXINS_PER_DOMAIN> available_mixins_bitset;

    extern BOOST_MIXIN_API available_mixins_bitset build_available_mixins_from(const mixin_type_info_vector& mixins);

    // msvc complains that boost::noncopyable doesn't have a dll interface
    // instead of disabling the warning, use our own noncopyable
    class BOOST_MIXIN_API noncopyable
    {
    protected:
        noncopyable() {}
        ~noncopyable() {}
    private:  // emphasize the following members are private
        noncopyable( const noncopyable& );
        const noncopyable& operator=( const noncopyable& );
    };

} // namespace internal
}
}

#endif // _BOOST_MIXIN_GLOBAL_HPP_INCLUDED
