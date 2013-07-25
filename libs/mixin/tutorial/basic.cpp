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
players
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

//]

BOOST_MIXIN_MESSAGE_0(string, get_sound);
BOOST_MIXIN_DEFINE_MESSAGE(get_sound);

BOOST_DECLARE_MIXIN(cd_player);
BOOST_DEFINE_MIXIN(cd_player, get_sound_msg);

BOOST_DECLARE_MIXIN(mp3_player);
BOOST_DEFINE_MIXIN(mp3_player, get_sound_msg);

class headphones_player
{
public:
    void play()
    {
        cout << "playing " << get_sound(bm_this)
            << " through headphones" << endl;
    }
};

class speakers_player
{
public:
    void play()
    {
        cout << "PLAYING " << get_sound(bm_this)
            << " THROUGH SPEAKERS" << endl;
    }
};

BOOST_MIXIN_MESSAGE_0(void, play);
BOOST_MIXIN_DEFINE_MESSAGE(play);

BOOST_DECLARE_MIXIN(headphones_player);
BOOST_DEFINE_MIXIN(headphones_player, play_msg);
BOOST_DECLARE_MIXIN(speakers_player);
BOOST_DEFINE_MIXIN(speakers_player, play_msg);

int main()
{
    object sound_player;

    mutate(sound_player)
        .add<cd_player>()
        .add<headphones_player>();

    sound_player.get<cd_player>()->insert_cd("Led Zeppelin IV (1971)");

    play(sound_player);

    mutate(sound_player)
        .remove<cd_player>()
        .add<mp3_player>();

    sound_player.get<mp3_player>()->copy_mp3("Led Zeppelin - Black Dog.mp3");

    play(sound_player);

    mutate(sound_player)
        .remove<headphones_player>()
        .add<speakers_player>();

    play(sound_player);

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

//[basic
//` (For the complete, working source of this example see
//` [@boost:/tutorial/basic.cpp basic.cpp])
//` [basic_A]
//]