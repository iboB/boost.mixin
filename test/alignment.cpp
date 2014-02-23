//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_TEST_MODULE boost_mixin_alignment

#include <boost/mixin.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

using namespace boost::mixin;

BOOST_AUTO_TEST_CASE(sub_ptr_align)
{
    size_t ptr_size = sizeof(uintptr_t);
    for(size_t i=1; i<=ptr_size; ++i)
    {
        BOOST_CHECK_EQUAL(2*ptr_size,
            global_allocator::calculate_mem_size_for_mixin(i, 1));
    }

    for(size_t i=2; i<=ptr_size; i+=2)
    {
        BOOST_CHECK_EQUAL(2*ptr_size,
            global_allocator::calculate_mem_size_for_mixin(i, 2));
    }
}

BOOST_DECLARE_MIXIN(align_default);
BOOST_DECLARE_MIXIN(align_8);
BOOST_DECLARE_MIXIN(align_16);
BOOST_DECLARE_MIXIN(align_32);

BOOST_MIXIN_MULTICAST_MESSAGE_0(void, check_alignment);

BOOST_AUTO_TEST_CASE(aligned_mixin)
{
    object o;

    mutate(o)
        .add<align_default>()
        .add<align_8>()
        .add<align_16>()
        .add<align_32>();

    BOOST_CHECK(intptr_t(o.get<align_8>()) % 8 == 0);
    BOOST_CHECK(intptr_t(o.get<align_16>()) % 16 == 0);
    BOOST_CHECK(intptr_t(o.get<align_32>()) % 32 == 0);

    check_alignment(o);
}

class align_default
{
public:
    void check_alignment()
    {
        BOOST_CHECK(intptr_t(this) % boost::alignment_of<align_default>::value == 0);
    }
};

#if defined _MSC_VER
#   define ALIGN(n) __declspec(align(n))
#else
#   define ALIGN(n) __attribute__((aligned(n)))
#endif


class ALIGN(16) align_8
{
public:
    void check_alignment()
    {
        BOOST_CHECK(intptr_t(this) % 8 == 0);
    }
};

class ALIGN(16) align_16
{
public:
    void check_alignment()
    {
        BOOST_CHECK(intptr_t(this) % 16 == 0);
    }
};

class ALIGN(32) align_32
{
public:
    void check_alignment()
    {
        BOOST_CHECK(intptr_t(this) % 32 == 0);
    }
};

BOOST_DEFINE_MIXIN(align_default, check_alignment_msg);
BOOST_DEFINE_MIXIN(align_8, check_alignment_msg);
BOOST_DEFINE_MIXIN(align_16, check_alignment_msg);
BOOST_DEFINE_MIXIN(align_32, check_alignment_msg);

BOOST_MIXIN_DEFINE_MESSAGE(check_alignment);
