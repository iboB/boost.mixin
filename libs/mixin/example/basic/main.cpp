#include <iostream>
#include <algorithm>
#include <typeinfo>

#include <boost/mixin.hpp>

using namespace std;

BOOST_DECLARE_MIXIN(mixin_A)
BOOST_DECLARE_MIXIN(mixin_B)
BOOST_DECLARE_MIXIN(mixin_C)

class mixin_A
{
public:
    void aaa() { cout << "im in AAA\n"; }

    void foo(int bar, float baz)
    {
        cout << "in a bar is: " << bar << " baz is: " << baz << endl;
    }
};

class mixin_B
{
public:
    mixin_B()
        : i(1), j(20), k(300)
    {
    }

    void bbb() { cout << "im in b " << i << ' ' << j << ' ' << k << endl; }
    int i, j, k;

    void foo(int bar, float baz)
    {
        i = bar;
        cout << "whereas in b bar is: " << bar << " and baz is: " << baz << endl;
    }
};

class mixin_C
{
public:
    mixin_C()
    {
        cout << "creating c\n";
    }

    ~mixin_C()
    {
        cout << "destroying c\n";
    }

    void ccc()
    {
        cout << "we're in c\n";
        bm_this->get<mixin_A>()->aaa();
        if(bm_this->get<mixin_B>())
        {
            bm_this->get<mixin_B>()->bbb();
        }
    }

};

BOOST_MIXIN_MESSAGE_2(void, foo, int, bar, float, baz)

BOOST_DEFINE_MIXIN(mixin_A, foo_msg)
BOOST_DEFINE_MIXIN(mixin_B, boost::mixin::priority(-5, foo_msg))
BOOST_DEFINE_MIXIN(mixin_C, boost::mixin::none)

BOOST_MIXIN_DEFINE_MESSAGE(foo)

using namespace boost::mixin;

int main()
{
    internal::domain& d = internal::get_domain(0);
    cout << d._num_registered_mixins << endl;

    const internal::mixin_type_info& t1 = *d._mixin_type_infos[0];
    cout << t1.name << ": " << t1.id << endl;

    const internal::mixin_type_info& t2 = *d._mixin_type_infos[1];
    cout << t2.name << ": " << t2.id << endl;

    const internal::mixin_type_info& t3 = *d._mixin_type_infos[2];
    cout << t3.name << ": " << t3.id << endl;

    object* o1 = new object;

    object_transformer(o1)
        .add<mixin_A>()
        .add<mixin_C>()
        .add<mixin_B>();

    o1->get<mixin_C>()->ccc();

    object* o2 = new object;
    object_transformer(o2)
        .add<mixin_C>()
        .add<mixin_B>()
        .add<mixin_A>();

    o2->get<mixin_B>()->bbb();

    foo(o2, 1, 2);

    delete o1;
    delete o2;

    cout << "type infos: " << d._object_type_infos.size() << endl;

    return 0;
}
