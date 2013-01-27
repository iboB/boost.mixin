#include <iostream>
#include <algorithm>
#include <typeinfo>

#include <boost/mixin.hpp>

using namespace std;

class xxx
{
    ~xxx() { cout << "destroyed" << endl; }
};

BOOST_DECLARE_MIXIN(mixin_A);
BOOST_DECLARE_MIXIN(mixin_B);
BOOST_DECLARE_MIXIN(mixin_C);

class mixin_A
{
public:
    void aaa() { cout << "im in AAA\n"; }
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
        bm_this->get<mixin_B>()->bbb();
    }

};

BOOST_DEFINE_MIXIN(mixin_A, asjkd);
BOOST_DEFINE_MIXIN(mixin_B, asjkd);
BOOST_DEFINE_MIXIN(mixin_C, asjkd);

void _boost_mixin_constructor(void*);

using namespace boost::mixin;

int main()
{
    internal::domain& d = internal::get_domain(0);
    cout << d._num_mixins << endl;

    const internal::mixin_type_info_data& t1 = *d._mixins[0];
    cout << t1.name << ": " << t1.id << endl;

    const internal::mixin_type_info_data& t2 = *d._mixins[1];
    cout << t2.name << ": " << t2.id << endl;

    const internal::mixin_type_info_data& t3 = *d._mixins[2];
    cout << t3.name << ": " << t3.id << endl;

    object* o = new object;

    object_transformer transform(o);

    transform.add<mixin_A>();
    transform.add<mixin_B>();
    transform.add<mixin_C>();

    transform.apply();

    o->get<mixin_C>()->ccc();


    delete o;

    return 0;
}
