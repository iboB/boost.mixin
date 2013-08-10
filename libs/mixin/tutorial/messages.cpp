//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <iostream>
#include <string>
#include <sstream>
#include <boost/mixin.hpp>

using namespace std;
using namespace boost::mixin;

//[tutorial_messages_A
/*`
For this tutorial will look a simplified piece of code from an imaginary game.
First let's define the mixin classes that we're going to use.

There's a mixin that's a part from evey object of our game. The one that gives
them a unique id. We'll also define a method, called `trace` that will display
information about the mixin in a stream.
*/

class has_id
{
public:
    void set_id(int id) { _id = id; }
    int id() const { return _id; }

    void trace(ostream& out) const;

private:
    int _id;
};

/*`
Next we'll define a class for an animated model. We could have other types of
models in the game, but for this tutorial there's no need to define anything
more.

The mixin offers us a way to set a mesh and two ways to set an animation. It has
a render method and, again the trace method, to display info about this mixin.
*/

class animated_model
{
public:
    void set_mesh(const string& mesh);

    void set_animation(const string& animation);
    void set_animation(int anim_id);

    void render() const;

    void trace(ostream& out) const;

private:
    string _mesh;
    string _animation;
};

/*`
Now we'll define three types of mixins that will give us artificial intelligence
logic for different occasions. They all share a method called `think` for the
AI, and the familiar trace method.
*/

class enemy_ai
{
public:
    void think();

    void trace(ostream& out) const;
};

class ally_ai
{
public:
    void think();

    void trace(ostream& out) const;
};

class stunned_ai
{
public:
    void think();

    void trace(ostream& out) const;
};

/*`
Now it's time to declare the messages our mixins will use.
*/

BOOST_MIXIN_MESSAGE_0(void, think);

BOOST_MIXIN_MESSAGE_1(void, set_mesh, const string&, mesh);
BOOST_MIXIN_CONST_MESSAGE_0(void, render);

BOOST_MIXIN_CONST_MULTICAST_MESSAGE_1(void, trace, ostream&, out);

BOOST_MIXIN_MESSAGE_1_OVERLOAD(set_anim_by_name, void, set_animation, const string&, animation);
BOOST_MIXIN_MESSAGE_1_OVERLOAD(set_anim_by_id, void, set_animation, int, anim_id);

BOOST_MIXIN_DEFINE_MESSAGE(think);
BOOST_MIXIN_DEFINE_MESSAGE(set_mesh);
BOOST_MIXIN_DEFINE_MESSAGE(render);
BOOST_MIXIN_DEFINE_MESSAGE(trace);
BOOST_MIXIN_DEFINE_MESSAGE(set_anim_by_name);
BOOST_MIXIN_DEFINE_MESSAGE(set_anim_by_id);

BOOST_DEFINE_MIXIN(animated_model,
    trace_msg & set_mesh_msg & set_anim_by_id_msg & set_anim_by_name_msg & render_msg);

BOOST_DEFINE_MIXIN(enemy_ai, think_msg & trace_msg);
BOOST_DEFINE_MIXIN(ally_ai, think_msg & trace_msg);

BOOST_DEFINE_MIXIN(has_id, priority(1, trace_msg));
BOOST_DEFINE_MIXIN(stunned_ai, priority(1, think_msg) & priority(-1, trace_msg));

//]

int main()
{
    object enemy;

    mutate(enemy)
        .add<has_id>()
        .add<animated_model>()
        .add<enemy_ai>();

    enemy.get<has_id>()->set_id(1);
    set_mesh(enemy, "spider.mesh");

    trace(enemy, cout);

    object ally;

    mutate(ally)
        .add<has_id>()
        .add<animated_model>()
        .add<ally_ai>();

    ally.get<has_id>()->set_id(5);
    set_mesh(ally, "dog.mesh");

    trace(ally, cout);

    think(enemy);
    think(ally);

    render(enemy);
    render(ally);

    mutate(enemy).add<stunned_ai>();
    think(enemy);
    render(enemy);

    mutate(enemy).remove<stunned_ai>();
    think(enemy);
    render(enemy);

    return 0;
};

void has_id::trace(ostream& out) const
{
    out << "object with id: " << _id << endl;
}

void animated_model::set_mesh(const string& mesh)
{
    _mesh = mesh;
}

void animated_model::set_animation(const string& animation)
{
    _animation = animation;
}

void animated_model::set_animation(int anim_id)
{
    ostringstream sout;
    sout << anim_id;

    _animation = sout.str();
}

void animated_model::render() const
{
    cout << "rendering " << _mesh << " with anim " << _animation << endl;
}

void animated_model::trace(ostream& out) const
{
    out << "\twith animated model " << _mesh << endl;
}

void enemy_ai::think()
{
    set_animation(bm_this, "attack");
    cout << "kicking the player in the shin" << endl;
}

void enemy_ai::trace(ostream& out) const
{
    cout << "\twith enemy ai" << endl;
}

void ally_ai::think()
{
    set_animation(bm_this, "heal");
    cout << "putting a bandage on the players wounded shin" << endl;
}

void ally_ai::trace(ostream& out) const
{
    cout << "\twith ally ai" << endl;
}

void stunned_ai::think()
{
    set_animation(bm_this, "dizzy");
    cout << "cannot do anything because i'm stunned" << endl;
}

void stunned_ai::trace(ostream& out) const
{
    cout << "\tthat is stunned" << endl;
}


//[tutorial_messages
//` (For the complete, working source of this example see
//` [tutorialfile messages.cpp])
//` [tutorial_messages_A]
//]
