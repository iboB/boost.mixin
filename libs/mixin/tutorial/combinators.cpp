//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/mixin.hpp>
#include <iostream>

using namespace std;
using namespace boost::mixin;

BOOST_DECLARE_MIXIN(wireframe);

class wireframe
{
public:
    void set_visible(bool value);
    void set_elements_count(int count);

    bool visible() const;
    int elements_count() const;

    // ...

private:
    bool _visible;
    int _elements_count;
};

BOOST_DECLARE_MIXIN(vertices);

class vertices
{
public:
    void set_visible(bool value);
    void set_elements_count(int count);

    bool visible() const;
    int elements_count() const;

    // ...

private:
    bool _visible;
    int _elements_count;
};

BOOST_DECLARE_MIXIN(surface);

class surface
{
public:
    void set_visible(bool value);
    void set_elements_count(int count);

    bool visible() const;
    int elements_count() const;

    // ...

private:
    bool _visible;
    int _elements_count;
};

BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(bool, visible);
BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(int, elements_count);

BOOST_MIXIN_DEFINE_MESSAGE(visible);
BOOST_MIXIN_DEFINE_MESSAGE(elements_count);

BOOST_DEFINE_MIXIN(vertices, visible_msg & elements_count_msg);
BOOST_DEFINE_MIXIN(wireframe, visible_msg & elements_count_msg);
BOOST_DEFINE_MIXIN(surface, visible_msg & elements_count_msg);

const int NUM_OBJECTS = 20;
object objects[NUM_OBJECTS];

const object& o = objects[0];

void fill_objects()
{
    for(int i=0; i<NUM_OBJECTS; ++i)
    {
        object& o = objects[i];

        mutate(o)
            .add<vertices>()
            .add<wireframe>()
            .add<surface>();

        o.get<vertices>()->set_elements_count(24);
        o.get<vertices>()->set_visible(false);

        o.get<wireframe>()->set_elements_count(6);
        o.get<wireframe>()->set_visible(false);

        o.get<surface>()->set_elements_count(1);
        o.get<surface>()->set_visible(true);
    }
}

void built_in_combinators()
{
    bool is_first_visible = visible<combinators::boolean_or>(o);
    cout << "The first object is " << (is_first_visible ? "visible" : "invisible") << "." << endl;
    cout << "There are " << elements_count<combinators::sum>(o) << " elements in the first object." << endl;

    combinators::sum<int> sum;
    for(int i=0; i<NUM_OBJECTS; ++i)
    {
        elements_count(objects[i], sum);
    }
    cout << "There are " << sum.result() << " elements among all objects." << endl;
}

void custom_combinator_arg()
{
    struct more_than_1
    {
        more_than_1() : count(0) {}

        bool add_result(int elements)
        {
            if(elements > 1)
                ++count;

            return true;
        }

        int count;
    };

    more_than_1 counter;

    for(int i=0; i<NUM_OBJECTS; ++i)
    {
        elements_count(objects[i], counter);
    }
    cout << "There are " << counter.count << " mixins with more than 1 element among all objects." << endl;
}

template <typename MsgReturnType>
struct more_than_1_t
{
    more_than_1_t() : count(0) {}

    bool add_result(MsgReturnType elements)
    {
        if(elements > 1)
            ++count;

        return true;
    }

    int result() const
    {
        return count;
    }

    typedef int result_type;

    int count;
};

void custom_combinator_t()
{
    cout << "There are " << elements_count<more_than_1_t>(o) << " mixins with more than 1 element in the first object." << endl;
}

int main()
{
    fill_objects();
    built_in_combinators();
    custom_combinator_arg();
    custom_combinator_t();

    return 0;
}

void wireframe::set_visible(bool value)
{
    _visible = value;
}

void wireframe::set_elements_count(int count)
{
    _elements_count = count;
}

bool wireframe::visible() const
{
    return _visible;
}

int wireframe::elements_count() const
{
    return _elements_count;
}

void vertices::set_visible(bool value)
{
    _visible = value;
}

void vertices::set_elements_count(int count)
{
    _elements_count = count;
}

bool vertices::visible() const
{
    return _visible;
}

int vertices::elements_count() const
{
    return _elements_count;
}

void surface::set_visible(bool value)
{
    _visible = value;
}

void surface::set_elements_count(int count)
{
    _elements_count = count;
}

bool surface::visible() const
{
    return _visible;
}

int surface::elements_count() const
{
    return _elements_count;
}
