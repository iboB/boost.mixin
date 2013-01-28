# Boost.Mixin

## Introduction to entity component systems

An entity-component system (ECS) allows users to create entities which
consist of components. A simplified way of thinking about that is a
container of `boost::variant` or `boost::any` to unique types. Another way
of thinking about said entities is like they are objects produced from
multiple inheritance only they are constructed in runtime, as opposed to
compile-time.

Here is a piece of code relevant for a simple hypothetical ECS:

    entity e;
    e.add_component(new serializer);
    e.add_component(new data_x);
    // now the entity contains data and a way to serialize this data    
    // ... 
    // here we can test if an entity has the components we need
    if(e.has_component<serializer>())
    {
        e.get_component<serializer>()->serialize();
    }
    
See how we can think of `e` as a container of various objects, or as a
class derived from both `serializer` and `data`. However it is not exactly
a container as it cannot have two instances of the same class, and it is
not exactly a class with multiple inheritance as its contents cannot be
statically analyzed.

### Why are entity-component sytems good?

They allow the users to achieve true decoupling of their subsystems. For
example if we extend the above example to have a registered component of
type `serializer`, which is a pure virtual class with a method `virtual
void serialize() = 0`, we could change the second line of code to:

    e.add_component(new file_serialzer);

Now all functions that want to call this method for an object may as well
be oblivious to what the _actual_ serializer within the entity. Keep in
mind that this is nearly impossible to achieve with plain multiple
inheritance as users would have to handle the combinatorial explosion
of different `serializer`'s, `data`'s and other possible super-classes.

The above example shows that an ECS is, in most cases, a better choice than
multiple inheritance. What about composition?

Well, an ECS is a type of composition, with one difference. An entity has
no idea what kind of components may take be a part of it.

Imagine a the straight-forward way to composition. An object from our
system will have methods like `get_serializer()`, `get_data()`, and more.
This means that every time we add a new sub-system, we'll have to change
the object type to "enrich" it with its new components.

Removing the aforementioned limitation allows independent sub-system
development - without chaing the code of the original product; or
introduction of entire subsystems on dynamic libraries, which don't change
the binary. For example, writing an entirely new serialization mechanism -
say encrypted serialization - for a product and introdusing it as an
optional plugin becomes a brieze. 

### Who uses entity-component systems?

They are very popular in game development, as most objects in a game have
many different aspects for different sub-systems. Still being as powerful
as they are they find their place in many other pieces of software,
developed in C++, Java or C#.

Most script-language objects natively support a form of these systems,
called _Mixins_. Here's where Boost.Mixin comes into play.

## Introduction to mixins

Mixin is the new thing Boost.Mixin brings to the ECS world for C++. As
we've seen an ECS allows the objects to receive different interfaces,
while remaining oblivious to the actual components that may comprise it.

In contrast to regular components, mixins do not share parents to provide
their interface. They just have methods that have provide different
functionalities. To call these methods Boost.Mixin introduces messages.
They are no more than global functions whose first parameter is object.
(If C++ allowed extension methods, like C# does, this would have been the
way to implement them)

### Boost.Mixin

The example from above would look like this with Boost.Mixin:

    using namespace boost::mixin;
    object o;
    mutate(o)
        .add<serializer>()
        .add<data_x>();
    // ...
    serialize(o);

Here `serialize` is a message, implemented by the mixin `serializer`.
Unlike in a typical ECS implementation, here the code calling messages
doesn't need to "see" the declaration of the objects that implement them.
They need only see that such messages exist.

Boost.Mixin has another feature, not present in most ECS implementations.
It allows the mixins to "know" their object. It introduces the bm_this
macro whith which the object is acessible within the methods of a mixin.

Yet another feature are _multicast_ messages. This lacks in most mixin
implementations even when mixins are a built-in part of the language.
Multicast allow a single messsage to be handled by all the mixins in an
object.