//
// Copyright (c) 2013-2014 Borislav Stanimirov, Zahary Karadjov
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

BOOST_MIXIN_CONST_MULTICAST_MESSAGE_1(void, save, ostream&, out);
BOOST_MIXIN_MULTICAST_MESSAGE_1(void, load, istream&, in);

BOOST_MIXIN_DEFINE_MESSAGE(save);
BOOST_MIXIN_DEFINE_MESSAGE(load);

class person
{
public:
    void set_name(const string& name) { _name = name; }

    void save(ostream& out) const;
    void load(istream& in);

    static const int serialize_priority = 1;

private:
    string _name;
};
BOOST_DEFINE_MIXIN(person, priority(person::serialize_priority, save_msg) & priority(person::serialize_priority, load_msg));

class employee
{
public:
    void set_position(const string& position) { _position = position; }

    void save(ostream& out) const;
    void load(istream& in);

    static const int serialize_priority = 2;

private:
    string _position;
};
BOOST_DEFINE_MIXIN(employee, priority(employee::serialize_priority, save_msg) & priority(employee::serialize_priority, load_msg));

class client
{
public:
    void set_organization(const string& position) { _organization = position; }

    void save(ostream& out) const;
    void load(istream& in);

    static const int serialize_priority = 3;

private:
    string _organization;
};
BOOST_DEFINE_MIXIN(client, priority(client::serialize_priority, save_msg) & priority(client::serialize_priority, load_msg));

void save_obj(const object& o, ostream& out);
void load_obj(object& o, istream& in);

int main()
{
    object e;
    mutate(e)
        .add<person>()
        .add<employee>();

    e.get<person>()->set_name("Alice Anderson");
    e.get<employee>()->set_position("Programmer");

    object c;
    mutate(c)
        .add<person>()
        .add<client>();

    c.get<person>()->set_name("Bob Behe");
    c.get<client>()->set_organization("Business Co");

    ostringstream out;
    save_obj(e, out);
    save_obj(c, out);

    string data = out.str();

    istringstream in(data);
    
    object obj1, obj2;
    load_obj(obj1, in);
    load_obj(obj2, in);

    save_obj(obj1, cout);
    save_obj(obj2, cout);

    return 0;
}

void save_obj(const object& obj, ostream& out)
{
    size_t num_mixins = obj._type_info->_compact_mixins.size();
    out << num_mixins << endl;
    for(size_t i=0; i<num_mixins; ++i)
    {
        out << obj._type_info->_compact_mixins[i]->name << endl;
    }

    save(obj, out);
}

void load_obj(object& obj, istream& in)
{
    string line;
    getline(in, line);

    object_type_template tmpl;

    size_t num_mixins = atoi(line.c_str());    
    for(size_t i=0; i<num_mixins; ++i)
    {
        getline(in, line);
        tmpl.add(line.c_str());
    }
    tmpl.create();
    
    tmpl.apply_to(obj);

    load(obj, in);
}

void person::save(ostream& out) const
{
    out << _name << endl;
}

void person::load(istream& in)
{
    getline(in, _name);
}

void employee::save(ostream& out) const
{
    out << _position << endl;
}

void employee::load(istream& in)
{
    getline(in, _position);
}

void client::save(ostream& out) const
{
    out << _organization << endl;
}

void client::load(istream& in)
{
    getline(in, _organization);
}
