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

//[tutorial_combinators_intro
/*`
For this tutorial let's imagine we have a simple CAD program, which deals
designing 3-dimensional objects. In such programs various aspects of an object
need to be visible and editable at different times. Let's assume our objects are
defined by their wireframe, their vertices, and their surface.

We'll define mixins for those and focus on the parts they have in common: namely
whether an part of the object is visible, and how much elements is this part
composed of.
*/

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

/*`
Now let's define messages for the methods we'll want to access polymorphically
and define our mixins to use those messages.
*/

BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(bool, visible);
BOOST_MIXIN_CONST_MULTICAST_MESSAGE_0(int, elements_count);

BOOST_MIXIN_DEFINE_MESSAGE(visible);
BOOST_MIXIN_DEFINE_MESSAGE(elements_count);

BOOST_DEFINE_MIXIN(vertices, visible_msg & elements_count_msg);
BOOST_DEFINE_MIXIN(wireframe, visible_msg & elements_count_msg);
BOOST_DEFINE_MIXIN(surface, visible_msg & elements_count_msg);

/*`
As you can see those are multicast messages. Each of the mixins in the object
will implement and respond to them.

Now let's create some objects.
*/

const int NUM_OBJECTS = 20;
object objects[NUM_OBJECTS];

const object& o = objects[0];

//]

void fill_objects()
{
//[tutorial_combinators_fill_objects
/*`
...and mutate them with some mixins, and give them some values.

For example let's say all of our objects are cubes, that have 24 vertices (4 per
side), 6 squares for the wireframe, and a single (folded) surface.
*/
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
//]
}

void built_in_combinators()
{
//[tutorial_combinators_built_in
/*`
As you may have noticed all our messages are functions that have a return value.
You may have tried making multicast messages with non-void functions and noticed
that the generated message function /is/ void and doesn't return anything.

The things that will help us make use of the values returned from the messages
are the multicast result combinators.

So, let's say we want to see if an object is visible. We say that it is visible
if at least one of its mixins is. To get this value we may use the combinator
`boolean_or` provided by the library (all built-in combinators are in namespace
`boost::mixin::combinators`)
*/
    bool is_first_visible = visible<combinators::boolean_or>(o);
    cout << "The first object is " << (is_first_visible ? "visible" : "invisible") << "." << endl;

/*`
That's it. Giving a combinator as an explicit template argument to a multicast
message call, will call a function that /has/ a return value defined by the
combinator. In this case `boolean_or` causes the message to return a `bool`
which is true if at least one of the messages returns non-zero.

Had we defined that an object is visible if /all/ of its mixins were visible,
then we could have used the built-in combinator `boolean_and`.

Now let's another built-in combinator -- `sum`. You may have guessed that it's
a sum of all values returned by the messages. In our case we may want to check
how many elements are in the entire object like this:
*/
    cout << "There are " << elements_count<combinators::sum>(o) << " elements in the first object." << endl;

/*`
All built-in combinators have an alternative usage. You saw the first, where
putting the combinator as a template argument, causes the message to return a
value.

The second usage keeps the message function `void`, but lets you add the
combinator as an output parameter. This way, for example you may sum all
elements throughout all objects with a single reusable combinator:
*/

    combinators::sum<int> sum;
    for(int i=0; i<NUM_OBJECTS; ++i)
    {
        elements_count(objects[i], sum);
    }
    cout << "There are " << sum.result() << " elements among all objects." << endl;

/*`
That's basically all there is to know about using combinators. Now, let's move
on to creating our own custom ones.
*/
//]
}

void custom_combinator_arg()
{
//[tutorial_combinators_custom_arg
/*`
The built-in combinators are powerful, but sometimes you need to accomplish a
task where you need some specific combinator behavior and need to add a custom
one.

To create a custom combinator that's used as an output parameter is very easy.
All you need to do is create a class, that has a public method called
`add_result`. This public method should take one argument of the same type as
(or one that can be implicitly cast to) the return type of the multicast message
that you're "combining" with it. The method will be repeatedly called with each
successful message with its return value as an argument. It should return
`bool` -- `true` when the execution should continue and `false` when it should
stop.

We mentioned `boolean_or` and `boolean_and`. The function `add_result` in
`boolean_or` returns false on the first non-zero value. That means it has
determined the the final value is true (because at least one true has been met)
and there is no need to execute the rest. Likewise `boolean_and`'s `add_result`
returns false on the first zero value it gets. Exactly as C++'s operators `||`
and `&&` behave.

So let's define our output parameter combinator that counts all mixins that have
more than 1 element. Also, we /could/ call it for a single object, but let's
make use of the fact that it's an output parameter and count all mixins with
more than 1 element among all objects:

*/
    struct more_than_1
    {
        more_than_1() : count(0) {}

        bool add_result(int elements)
        {
            if(elements > 1)
                ++count;

            // Never break. We need this for all mixins in an object
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
//]
}

//[tutorial_combinators_custom_template_A

/*`
Now, the last case we need to cover is when you want your custom combinator to
be added as a template argument to the message's function giving it a return
value of its own.

To do this is only slightly more complicated the the previous, return parameter
case.

You need to create a template class whose template parameter will be provided by
the message call and will be the message return type.

Next, as before you'll need an `add_result` method to be repeatedly called,
again having a single argument of type equal to the message return type (you may
just reuse the template argument of the combinator class), and again returning
`bool` to indicate whether the message execution should continue or stop.

Next, you'll need a typedef `result_type`, which will indicate the return type
of the message function.

Lastly, you'll have to create a method, called result, with no arguments,
that has a return type `result_type`. It will be called when the execution is
completed and it will provide the return value of the message function.

So, let's create an identical combinator as before -- one that counts the mixins
in an object that have more than one element, but this time to be used as a
template argument of the message function.
*/
template <typename MsgReturnType>
struct more_than_1_t
{
    more_than_1_t() : count(0) {}

    bool add_result(MsgReturnType elements)
    {
        if(elements > 1)
            ++count;

        // Never break. We need this for all mixins in an object
        return true;
    }

    // return type of message function
    typedef int result_type;

    result_type result() const
    {
        return count;
    }

    int count;
};
//]

void custom_combinator_t()
{
//[tutorial_combinators_custom_template_B

/*`
Now we can use our new custom combinator as we used `boolean_or` above.
*/
    cout << "There are " << elements_count<more_than_1_t>(o)
        << " mixins with more than 1 element in the first object." << endl;
/*`
And that's all there is about multicast result combinators.
*/
//]
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

//[tutorial_combinators
//` (For the complete, working source of this example see
//` [tutorialfile combinators.cpp])
//` [tutorial_combinators_intro]
//` [tutorial_combinators_fill_objects]
//` [tutorial_combinators_built_in]
//` [tutorial_combinators_custom_arg]
//` [tutorial_combinators_custom_template_A]
//` [tutorial_combinators_custom_template_B]
//]
