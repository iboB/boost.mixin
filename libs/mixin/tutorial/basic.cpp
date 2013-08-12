//
// Copyright (c) 2013 Borislav Stanimirov, Zahary Karadjov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <iostream>
#include <string>
#include <boost/mixin.hpp>

using namespace std;
using namespace boost::mixin;

//[basic_A
/*`
Let's imagine a program that needs to create representations for various music
players.
*/

class cd_player
{
public:
    string get_sound();
    void insert_cd(const string& cd);

private:
    string _cd;
};

class mp3_player
{
public:
    string get_sound();
    void copy_mp3(const string& mp3);

private:
    string _mp3;
};

/*`
As you can see with respect to the type media is played we could have those two
classes that share the interface `get_sound` and have some unique methods to
provide the media.

An approach, alternative to Boost.Mixin here could have been to have a common
parent with a pure virtual method `get_sound`.

Now let's add the macros that will make our classes work as mixins and the macro
that will define the message, that they will implement.
*/

BOOST_MIXIN_MESSAGE_0(string, get_sound);
BOOST_MIXIN_DEFINE_MESSAGE(get_sound);

BOOST_DECLARE_MIXIN(cd_player);
BOOST_DEFINE_MIXIN(cd_player, get_sound_msg);

BOOST_DECLARE_MIXIN(mp3_player);
BOOST_DEFINE_MIXIN(mp3_player, get_sound_msg);

/*`
Although we declare and define the messsages and mixins on consecutive lines
here, this is not a requierment. Normally you'd want to have the declarations in
header files and the definitions in cpp files. See the more detailed Examples
section for an example about that.

Now let's define a new axis for our classes: the type of output they'll have:
*/

class headphones_player
{
public:
    void play();
};

class speakers_player
{
public:
    void play();
};

/*`
and then, again "tell" the library about the new message and mixins:
*/

BOOST_MIXIN_MESSAGE_0(void, play);
BOOST_MIXIN_DEFINE_MESSAGE(play);

BOOST_DECLARE_MIXIN(headphones_player);
BOOST_DEFINE_MIXIN(headphones_player, play_msg);

BOOST_DECLARE_MIXIN(speakers_player);
BOOST_DEFINE_MIXIN(speakers_player, play_msg);
//]

int main()
{
//[basic_B
/*`
Now we're ready to create a simple object composed from our newly defined
mixins:
*/
    object sound_player; // just an empty boost::mixin::object

    mutate(sound_player)
        .add<cd_player>()
        .add<headphones_player>();

/*`
Now we have an empty object, which has internally instantiated the mixin
classes. So let's start using it.

Since there is no polymorphic way of setting the music, we'll have to explicitly
get the apporpriate mixin and set it like this:
*/
    sound_player.get<cd_player>()->insert_cd("Led Zeppelin IV (1971)");

/*`
However playing music *is* a polymorphic operation and via the message `play` it
can be done in this simple manner:
*/
    play(sound_player);

/*`
This is the way to call Boost.Mixin messages. They are nothing more than global
functions defined by the `BOOST_MIXIN_MESSAGE` macros. Internally they will find
the appropriate class instance and call the appropriate message (or trigger a
run-time error if this cannot be done).

Something else that we can do with our object is make a live mutation of its
type via the `mutate` class. And change the playing behaviour.
*/

    mutate(sound_player)
        .remove<headphones_player>()
        .add<speakers_player>();

    play(sound_player);

/*`
The music is now being played through speakers instead of headphones.

Naturally we could also mutate the object by changing the media provider part:
*/

    mutate(sound_player)
        .remove<cd_player>()
        .add<mp3_player>();

    sound_player.get<mp3_player>()->copy_mp3("Led Zeppelin - Black Dog.mp3");

    play(sound_player);

/*`
And have "Black Dog" played through the existing mixin `speakers_player`.

Compared to a classic approach to a similar problem: multiple inheritance this
saves us a lot of classes -- in this case four for all the combinations of types
of media and types of speakers, which might not sound like much, but keep in
mind that the library is appropriate for big systems, that may have hundreds or
even thousands of possible combinations.
*/
//]

    return 0;
}


string cd_player::get_sound()
{
    string sound;

    if(_cd.empty())
    {
        sound = "silence";
    }
    else
    {
        sound = "sound from cd: " + _cd;
    }

    return sound;
}

void cd_player::insert_cd(const string& cd)
{
    _cd = cd;
}

string mp3_player::get_sound()
{
    string sound;

    if(_mp3.empty())
    {
        sound = "silence";
    }
    else
    {
        sound = "sound from mp3: " + _mp3;
    }

    return sound;
}

void mp3_player::copy_mp3(const string& mp3)
{
    _mp3 = mp3;
}


//[basic_C
/*`
The last thing we'll cover in this basic example is how the speakers-specific
players knew what sound to play. Remember the message we defined earlier, that
was supposed to be used to retrieve the sound from the media-specific players --
`get_sound`. It wasn't used before, but it is used by the speakers to obtain the
sound polymorphically.

All mixins may use a special macro that refers to their owning object --
`bm_this`.
Thus the following (pseudo) methods are possible:
*/

void headphones_player::play()
{
    cout << "playing " << get_sound(bm_this)
        << " through headphones" << endl;
}

void speakers_player::play()
{
    cout << "PLAYING " << get_sound(bm_this)
        << " THROUGH SPEAKERS" << endl;
}

/*`
And this covers the most basic usage of Boost.Mixin.
*/

//]

//[basic
//` (For the complete, working source of this example see
//` [tutorialfile basic.cpp])
//` [basic_A]
//` [basic_B]
//` [basic_C]
//]
