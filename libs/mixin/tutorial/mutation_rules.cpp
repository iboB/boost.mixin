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

//[tutorial_mutation_rules_A
/*`
Let's define some mixins that may be present in a CAD system specialized for
furniture design. Like the previous example we won't concern ourselves with any
particular messages.

So, again we have a mixin that we want to be persent in every object.
*/

class furniture
{
    int _id;
    string name;
    // ... other common fields
};

BOOST_DEFINE_MIXIN(furniture, none);

/*`
We also have mixins that describe the frame of the piece of furniture.
*/

class wood_frame {};
BOOST_DEFINE_MIXIN(wood_frame, none);

class metal_frame {};
BOOST_DEFINE_MIXIN(metal_frame, none);

/*`
Let's also define some mixins that will be responsible for the object
serialization.
*/

class ofml_serialization {};
BOOST_DEFINE_MIXIN(ofml_serialization, none);

class xml_serialization {};
BOOST_DEFINE_MIXIN(xml_serialization, none);

//]

int main()
{
//[tutorial_mutation_rules_B

/*`
Now, let's move on to the entry point of our program.

We said that each adn every object in our system should be expected to have the
mixin `furniture`. That could be accomplished if we manually add it to all
mutations we make but there is a simpler way to do it. By adding the
`mandatory_mixin` mutation rule.

All mutation rules should be added by calling `add_new_mutation_rule`. Since
`mandatory_mixin` is a mutation rule that the library provides, we can
accomplish this with a single line of code:
*/
    add_new_mutation_rule(new mandatory_mixin<furniture>);
/*`
Now each mutation after this line, will add `furniture` to the objects (even if
it's not been explicitly added) and also if a mutation tries to remove the mixin
from the object it won't be allowed. There won't be an error or a warning. The
library will silently ignore the request to remove `furniture`, or any other
mixin, that's been added as mandatory. Note, that if a mutation tries to remove
`furniture`, and also adds and removes other mixins, only the part removing the
mandatory mixin will be ignored. The others will be performed.

Another common case for using `mandatory_mixin` is if you want to have some
debugging mixin, that you want present in you objects, when you're debugging
your application. This is very easily accomplished if you just set the rule for
it in a conditional comiplation block.

You probably noticed the mixin `ofml_serialization`. OFML is a format
specifically designed for describing furniture that's still used in some
European countries, but hasn't gotten worldwide acceptance. Let's assume we want
to drop the support for OFML, but without removing the actual code, since
third party plugins to our CAD system may still depend on it. All we want is to
prevent anybody from adding the mixin to an object. Basically the exact opposite
of `mandatory_mixin`. This is the mutation rule `deprecated_mixin`
*/

    add_new_mutation_rule(new deprecated_mixin<ofml_serialization>);
/*`
After the previous line of code, any mutation that tries to add
`ofml_serialization` won't be able to, and all mutations will try to remove it
if it's present in an object. Again, as was the case before, if a mutation does
many things, only the part from it, trying to add `ofml_serialization` will be
silently ignored.

The last built-in rule in the library is `mutually_exclusive_mixins`.

Since a piece of furniture has either wood frame or a metal frame and never
both, it would be a good idea to prevent the programmers from accidentally
adding both mixins representing the frame in a single object. This mutation rule
helps us do exactly that.
*/

    mutually_exclusive_mixins* rule = new mutually_exclusive_mixins;
    rule->add<wood_frame>();
    rule->add<metal_frame>();
    add_new_mutation_rule(rule);

/*`
You may add as many mutually exclusive mixins as you wish. If you had, say,
`plastic_frame` you could also add it to that list.

Any object mutated after that rule is set will implicitly remove any of the
mutually exclusive mixins if another is added.

In many of our examples a sample game code was given, with mixins
`opengl_rendering` and `directx_rendering`. The `mutually_exclusive_mixins` is
perfect for this case and any other when we're always doing
`add<x>().remove<y>()` and `add<y>().remove<x>()`.

So to see this in practice:
*/

    object o;
/*`
This object is empty. Mutation rules don't apply if there's no mutation. If,
however, the object had been created with a type template passed in its
constructor, then the rules would have been applied.
*/

    mutate(o)
        .add<ofml_serialization>()
        .add<xml_serialization>()
        .add<wood_frame>();

/*`
Two rules are affected by this mutation. First it will implicitly add
`furniture` to the object, and second it will ignore the attempt to add
`ofml_serialization`. As a result the object will have `furniture`,
`xml_serialization` and `wood_frame`.
*/

    mutate(o)
        .add<metal_frame>();

/*`
The mutually exclusive mixins will ensure that after this line the object won't
have the `wood_frame` mixin.

You can also define your own custom mutation rules. The process is described in
the advanced topics of this book.

Lastly, note that the library will be responsible for freeing the memory and
destroying the rules you've added. All you need to do is call
`add_new_mutation_rule` with a rule, allocated and constructed with `new`.
*/

//]
    return 0;
}

//[tutorial_mutation_rules
//` (For the complete, working source of this example see
//` [tutorialfile mutation_rules.cpp])
//` [tutorial_mutation_rules_A]
//` [tutorial_mutation_rules_B]
//]
