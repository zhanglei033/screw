#include "container/ConcurrentMap.h"
#include "googletest/gtest/gtest.h"
#include <random>
struct Wrapped_char
{
    constexpr Wrapped_char(char ch = 0)
        : mych(ch)
    {
    }

    constexpr operator char() const
    {
        return mych;
    }

    char mych;
};

template <class _Ty>
struct Myless
{
    constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {
        return _Left < _Right;
    }
};
class Copyable_int
{ // wrap an integer, copyable
public:
    Copyable_int(int v = 0)
        : val(v)
    { // construct from value
    }

    Copyable_int(const Copyable_int& x)
        : val(x.val)
    { // construct from copied value
    }

    Copyable_int& operator=(const Copyable_int& x)
    { // copy value
        val = x.val;
        return *this;
    }

    operator int() const
    { // convert to int
        return val;
    }

    bool operator==(const Copyable_int& x) const
    { // compare for equality
        return val == x.val;
    }

    bool operator!=(const Copyable_int& x) const
    { // compare for equality
        return val != x.val;
    }

    bool operator<(const Copyable_int& x) const
    { // compare for order
        return val < x.val;
    }

    int val;

    Copyable_int(Copyable_int&& x)
        : val(x.val)
    { // construct from moved value
        x.val = -1;
    }

    Copyable_int& operator=(Copyable_int&& x)
    { // move value
        val   = x.val;
        x.val = -1;
        return *this;
    }
};
class Movable_int : public Copyable_int
{ // wrap a move-only integer
public:
    typedef Copyable_int Mybase;

    Movable_int(int v = 0)
        : Mybase(v)
    { // construct from value
    }

    Movable_int(int v1, int v2)
        : Mybase(v2 + (v1 << 4))
    { // construct from two values
    }

    Movable_int(int v1, int v2, int v3)
        : Mybase(v3 + (v2 << 4) + (v1 << 8))
    { // construct from three values
    }

    Movable_int(int v1, int v2, int v3, int v4)
        : Mybase(v4 + (v3 << 4) + (v2 << 8) + (v1 << 12))
    { // construct from four values
    }

    Movable_int(int v1, int v2, int v3, int v4, int v5)
        : Mybase(v5 + (v4 << 4) + (v3 << 8) + (v2 << 12) + (v1 << 16))
    { // construct from five values
    }

    Movable_int(Movable_int&& right)
        : Mybase(right.val)
    { // construct from moved value
        right.val = -1;
    }

    Movable_int& operator=(Movable_int&& right)
    { // assign from moved value
        if (this != &right)
        { // different, move it
            val       = right.val;
            right.val = -1;
        }
        return *this;
    }

    operator int() const
    { // convert to int
        return val;
    }

    bool operator==(const Movable_int& x) const
    { // compare for equality
        return val == x.val;
    }

    bool operator!=(const Movable_int& x) const
    { // compare for equality
        return val != x.val;
    }

    bool operator<(const Movable_int& x) const
    { // compare for order
        return val < x.val;
    }

    Movable_int(const Movable_int&) = delete;
    Movable_int& operator=(const Movable_int&) = delete;
};

TEST(ConcurrentMapTest, SingleThread_Test1)
{
    using namespace screw::container;
    using Myval  = std::pair<const char, int>;
    using Myal   = std::allocator<Myval>;
    using Mypred = std::less<char>;
    using Mycont = ConcurrentMap<char, int, Mypred, Myal>;

    Myval x, xarr[3], xarr2[3];
    for (int i = 0; i < 3; ++i)
    {
        new (&xarr[i]) Myval((char)('a' + i), 1 + i);
        new (&xarr2[i]) Myval((char)('d' + i), 4 + i);
    }

    typename Mycont::key_type* p_key         = (char*)nullptr;
    typename Mycont::mapped_type* p_mapped   = (int*)nullptr;
    typename Mycont::key_compare* p_kcomp    = (Mypred*)nullptr;
    typename Mycont::allocator_type* p_alloc = (Myal*)nullptr;
    typename Mycont::value_type* p_val       = (Myval*)nullptr;
    typename Mycont::value_compare* p_vcomp  = nullptr;
    typename Mycont::pointer p_ptr           = (Myval*)nullptr;
    typename Mycont::const_pointer p_cptr    = (const Myval*)nullptr;
    typename Mycont::reference p_ref         = x;
    typename Mycont::const_reference p_cref  = (const Myval&)x;
    typename Mycont::size_type* p_size       = (size_t*)nullptr;
    typename Mycont::difference_type* p_diff = (ptrdiff_t*)nullptr;

    p_key    = p_key; // to quiet diagnostics
    p_mapped = p_mapped;
    p_kcomp  = p_kcomp;
    p_alloc  = p_alloc;
    p_val    = p_val;
    p_vcomp  = p_vcomp;
    p_ptr    = p_ptr;
    p_cptr   = p_cptr;
    p_ptr    = &p_ref;
    p_cptr   = &p_cref;
    p_size   = p_size;
    p_diff   = p_diff;

    Mycont v0;
    Myal al = v0.get_allocator();
    Mypred pred;
    EXPECT_EQ(v0.empty(), true);
    EXPECT_EQ(v0.size(), 0);

    Mycont v1(xarr, xarr + 3); // differs from hash_map
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ((*v1.begin()).first, 'a');

    const Mycont v4(xarr, xarr + 3);
    EXPECT_EQ(v4.size(), 3);
    EXPECT_EQ((*v4.begin()).first, 'a');
    v0 = v4;
    EXPECT_EQ(v0.size(), 3);
    EXPECT_EQ((*v0.begin()).first, 'a');
    EXPECT_EQ(v0.size() <= v0.max_size(), true);

    ConcurrentMap<char, int, Mypred>* p_cont = &v0;
    p_cont                                   = p_cont; // to quiet diagnostics

    {
        // check iterator generators
        typename Mycont::iterator p_it(v1.begin());
        typename Mycont::const_iterator p_cit(v4.begin());
        typename Mycont::reverse_iterator p_rit(v1.rbegin());
        typename Mycont::const_reverse_iterator p_crit(v4.rbegin());
        EXPECT_EQ((*p_it).first, 'a');
        EXPECT_EQ((*p_it).second, 1);
        EXPECT_EQ((*--(p_it = v1.end())).first, 'c');
        EXPECT_EQ((*p_cit).first, 'a');
        EXPECT_EQ((*--(p_cit = v4.end())).first, 'c');
        EXPECT_EQ((*p_rit).first, 'c');
        EXPECT_EQ((*p_rit).second, 3);
        EXPECT_EQ((*--(p_rit = v1.rend())).first, 'a');
        EXPECT_EQ((*p_crit).first, 'c');
        EXPECT_EQ((*--(p_crit = v4.rend())).first, 'a');

        typename Mycont::const_iterator p_it1 = typename Mycont::const_iterator();
        typename Mycont::const_iterator p_it2 = typename Mycont::const_iterator();
        EXPECT_EQ(p_it1 == p_it2, true); // check null forward iterator comparisons
    }

    {
        // check const iterators generators
        typename Mycont::const_iterator p_it(v1.cbegin());
        typename Mycont::const_iterator p_cit(v4.cbegin());
        typename Mycont::const_reverse_iterator p_rit(v1.crbegin());
        typename Mycont::const_reverse_iterator p_crit(v4.crbegin());
        EXPECT_EQ((*p_it).first, 'a');
        EXPECT_EQ((*p_it).second, 1);
        EXPECT_EQ((*--(p_it = v1.cend())).first, 'c');
        EXPECT_EQ((*p_cit).first, 'a');
        EXPECT_EQ((*--(p_cit = v4.cend())).first, 'c');
        EXPECT_EQ((*p_rit).first, 'c');
        EXPECT_EQ((*p_rit).second, 3);
        EXPECT_EQ((*--(p_rit = v1.crend())).first, 'a');
        EXPECT_EQ((*p_crit).first, 'c');
        EXPECT_EQ((*--(p_crit = v4.crend())).first, 'a');

        typename Mycont::const_iterator p_it1 = typename Mycont::const_iterator();
        typename Mycont::const_iterator p_it2 = typename Mycont::const_iterator();
        EXPECT_EQ(p_it1 == p_it2, true); // check null forward iterator comparisons
    }

    v0.clear();
    std::pair<typename Mycont::iterator, bool> pib = v0.insert(Myval('d', 4));
    EXPECT_EQ((*pib.first).first, 'd');
    EXPECT_EQ(pib.second, true);
    EXPECT_EQ((*--v0.end()).first, 'd');
    pib = v0.insert(Myval('d', 5));
    EXPECT_EQ((*pib.first).first, 'd');
    EXPECT_EQ((*pib.first).second, 4);
    EXPECT_EQ(!pib.second, true);
    EXPECT_EQ((*v0.insert(v0.begin(), Myval('e', 5))).first == 'e', true);
    v0.insert(xarr, xarr + 3);
    EXPECT_EQ(v0.size(), 5);
    EXPECT_EQ((*v0.begin()).first, 'a');
    v0.insert(xarr2, xarr2 + 3);
    EXPECT_EQ(v0.size(), 6);
    EXPECT_EQ((*--v0.end()).first, 'f');
    EXPECT_EQ(v0['c'], 3);
    v0.erase(v0.begin());
    EXPECT_EQ(v0.size(), 5);
    EXPECT_EQ((*v0.begin()).first, 'b');
    v0.erase(v0.begin(), ++v0.begin());
    EXPECT_EQ(v0.size(), 4);
    EXPECT_EQ((*v0.begin()).first, 'c');
    v0.insert(Myval('y', 99));
    EXPECT_EQ(v0.erase('x'), 0);
    EXPECT_EQ(v0.erase('y'), 1);

    { // test added C++11 functionality
        Mycont v1x(xarr, xarr + 3);
        EXPECT_EQ(v1x.at('c'), 3);
    }

    v0.clear();
    EXPECT_EQ(v0.empty(), true);
    v0.swap(v1);
    EXPECT_EQ(!v0.empty(), true);
    EXPECT_EQ(v1.empty(), true);
    std::swap(v0, v1);
    EXPECT_EQ(v0.empty(), true);
    EXPECT_EQ(!v1.empty(), true);
    EXPECT_EQ(v1 == v1, true);
    EXPECT_EQ(v0 < v1, true);
    EXPECT_EQ(v0 != v1, true);
    EXPECT_EQ(v1 > v0, true);
    EXPECT_EQ(v0 <= v1, true);
    EXPECT_EQ(v1 >= v0, true);

    EXPECT_EQ(v0.key_comp()('a', 'c'), true);
    EXPECT_EQ(!v0.key_comp()('a', 'a'), true);
    EXPECT_EQ(v0.value_comp()(Myval('a', 0), Myval('c', 0)), true);
    EXPECT_EQ(!v0.value_comp()(Myval('a', 0), Myval('a', 1)), true);
    EXPECT_EQ((*v4.find('a')).first, 'a');
    EXPECT_EQ(v4.count('x'), 0);
    EXPECT_EQ(v4.count('a'), 1);
    EXPECT_EQ((*v4.lower_bound('a')).first, 'a');
    EXPECT_EQ((*v4.upper_bound('a')).first == 'b', true); // differs from hash_map

    std::pair<typename Mycont::const_iterator, typename Mycont::const_iterator> pcc = v4.equal_range('a');
    EXPECT_EQ((*pcc.first).first, 'a');
    EXPECT_EQ((*pcc.second).first, 'b'); // differs from hash_map

    {
        Mycont v6;
        v6.insert(Myval('a', 1));
        v6.insert(Myval('b', 2));
        Mycont v7(std::move(v6));
        EXPECT_EQ(v6.size(), 0);
        EXPECT_EQ(v7.size(), 2);

        Mycont v8;
        v8 = std::move(v7);
        EXPECT_EQ(v7.size(), 0);
        EXPECT_EQ(v8.size(), 2);

        typedef std::map<Movable_int, int> Mycont2;
        Mycont2 v9;
        v9.insert(std::pair<Movable_int, int>(Movable_int('a'), 1));
        EXPECT_EQ(v9.size(), 1);
        EXPECT_EQ(v9.begin()->first.val, 'a');

        Mycont2 v10;
        std::pair<Movable_int, int> pmi1(Movable_int('e'), 5);
        v10.insert(std::move(pmi1));
        EXPECT_EQ(pmi1.first.val, -1);
        EXPECT_EQ(v10.begin()->first.val, 'e');

        std::pair<Movable_int, int> pmi2(Movable_int('d'), 4);
        v10.insert(v10.end(), std::move(pmi2));
        EXPECT_EQ(pmi2.first.val, -1);
        EXPECT_EQ(v10.begin()->first.val, 'd');

        Movable_int mi3('c');
        EXPECT_EQ(v10[std::move(mi3)], 0);
        EXPECT_EQ(mi3.val, -1);
        EXPECT_EQ(v10.begin()->first.val, 'c');

        v10.clear();
        v10.emplace_hint(v10.end());
        EXPECT_EQ(v10.begin()->first.val, 0);
        EXPECT_EQ(v10.begin()->second, 0);
        v10.clear();
        v10.emplace_hint(v10.end(), 'b', 2);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v10.clear();
        v10.emplace();
        EXPECT_EQ(v10.begin()->first.val, 0);
        EXPECT_EQ(v10.begin()->second, 0);

        v10.clear();
        v10.emplace('b', 2);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v8.clear(); // copyable key
        v8.try_emplace('b', 2);
        EXPECT_EQ(v8.begin()->first, 'b');
        EXPECT_EQ(v8.begin()->second, 2);

        v8.try_emplace('b', 3);
        EXPECT_EQ(v8.begin()->first, 'b');
        EXPECT_EQ(v8.begin()->second, 2);

        v8.insert_or_assign('b', 3);
        EXPECT_EQ(v8.begin()->first, 'b');
        EXPECT_EQ(v8.begin()->second, 3);

        v10.clear(); // movable key
        v10.try_emplace('b', 2);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v10.try_emplace('b', 3);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v10.insert_or_assign('b', 3);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 3);
    }

    { // check for lvalue stealing
        typedef std::map<Copyable_int, int> Mycont3;
        Mycont3 v11;
        std::pair<Copyable_int, int> pci1(Copyable_int('d'), 4);
        v11.insert(pci1);
        EXPECT_EQ(pci1.first.val, 'd');
        EXPECT_EQ(v11.begin()->first.val, 'd');

        std::pair<Copyable_int, int> pci2(Copyable_int('c'), 3);
        v11.clear();
        v11.insert(v11.end(), pci2);
        EXPECT_EQ(pci2.first.val, 'c');
        EXPECT_EQ(v11.begin()->first.val, 'c');

        Mycont3 v12(v11);
        EXPECT_EQ(v11 == v12, true);
    }

    {
        std::initializer_list<Myval> init{xarr[0], xarr[1], xarr[2]};
        Mycont v11(init);
        EXPECT_EQ(v11.size(), 3);
        EXPECT_EQ(v11.begin()->first, 'a');

        v11.clear();
        v11 = init;
        EXPECT_EQ(v11.size(), 3);
        EXPECT_EQ(v11.begin()->first, 'a');

        v11.clear();
        v11.insert(init);
        EXPECT_EQ(v11.size(), 3);
        EXPECT_EQ(v11.begin()->first, 'a');
    }

    ConcurrentMap<int, int> map;
    std::random_device dev;
    for (auto i = 0; i < 100000; i++)
    {
        map.emplace(dev(), i);
    }
    while (!map.empty())
    {
        map.erase(map.begin());
    }
}
TEST(ConcurrentMapTest, SingleThread_Test2)
{
    using namespace screw::container;
    using Myval  = std::pair<const char, int>;
    using Myal   = std::allocator<Myval>;
    using Mypred = Myless<Wrapped_char>;
    using Mycont = ConcurrentMap<char, int, Mypred, Myal>;

    Myval x, xarr[3], xarr2[3];
    for (int i = 0; i < 3; ++i)
    {
        new (&xarr[i]) Myval((char)('a' + i), 1 + i);
        new (&xarr2[i]) Myval((char)('d' + i), 4 + i);
    }

    typename Mycont::key_type* p_key         = (char*)nullptr;
    typename Mycont::mapped_type* p_mapped   = (int*)nullptr;
    typename Mycont::key_compare* p_kcomp    = (Mypred*)nullptr;
    typename Mycont::allocator_type* p_alloc = (Myal*)nullptr;
    typename Mycont::value_type* p_val       = (Myval*)nullptr;
    typename Mycont::value_compare* p_vcomp  = nullptr;
    typename Mycont::pointer p_ptr           = (Myval*)nullptr;
    typename Mycont::const_pointer p_cptr    = (const Myval*)nullptr;
    typename Mycont::reference p_ref         = x;
    typename Mycont::const_reference p_cref  = (const Myval&)x;
    typename Mycont::size_type* p_size       = (size_t*)nullptr;
    typename Mycont::difference_type* p_diff = (ptrdiff_t*)nullptr;

    p_key    = p_key; // to quiet diagnostics
    p_mapped = p_mapped;
    p_kcomp  = p_kcomp;
    p_alloc  = p_alloc;
    p_val    = p_val;
    p_vcomp  = p_vcomp;
    p_ptr    = p_ptr;
    p_cptr   = p_cptr;
    p_ptr    = &p_ref;
    p_cptr   = &p_cref;
    p_size   = p_size;
    p_diff   = p_diff;

    Mycont v0;
    Myal al = v0.get_allocator();
    Mypred pred;
    EXPECT_EQ(v0.empty(), true);
    EXPECT_EQ(v0.size(), 0);

    Mycont v1(xarr, xarr + 3); // differs from hash_map
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ((*v1.begin()).first, 'a');

    const Mycont v4(xarr, xarr + 3);
    EXPECT_EQ(v4.size(), 3);
    EXPECT_EQ((*v4.begin()).first, 'a');
    v0 = v4;
    EXPECT_EQ(v0.size(), 3);
    EXPECT_EQ((*v0.begin()).first, 'a');
    EXPECT_EQ(v0.size() <= v0.max_size(), true);

    ConcurrentMap<char, int, Mypred>* p_cont = &v0;
    p_cont                                   = p_cont; // to quiet diagnostics

    {
        // check iterator generators
        typename Mycont::iterator p_it(v1.begin());
        typename Mycont::const_iterator p_cit(v4.begin());
        typename Mycont::reverse_iterator p_rit(v1.rbegin());
        typename Mycont::const_reverse_iterator p_crit(v4.rbegin());
        EXPECT_EQ((*p_it).first, 'a');
        EXPECT_EQ((*p_it).second, 1);
        EXPECT_EQ((*--(p_it = v1.end())).first, 'c');
        EXPECT_EQ((*p_cit).first, 'a');
        EXPECT_EQ((*--(p_cit = v4.end())).first, 'c');
        EXPECT_EQ((*p_rit).first, 'c');
        EXPECT_EQ((*p_rit).second, 3);
        EXPECT_EQ((*--(p_rit = v1.rend())).first, 'a');
        EXPECT_EQ((*p_crit).first, 'c');
        EXPECT_EQ((*--(p_crit = v4.rend())).first, 'a');

        typename Mycont::const_iterator p_it1 = typename Mycont::const_iterator();
        typename Mycont::const_iterator p_it2 = typename Mycont::const_iterator();
        EXPECT_EQ(p_it1 == p_it2, true); // check null forward iterator comparisons
    }

    {
        // check const iterators generators
        typename Mycont::const_iterator p_it(v1.cbegin());
        typename Mycont::const_iterator p_cit(v4.cbegin());
        typename Mycont::const_reverse_iterator p_rit(v1.crbegin());
        typename Mycont::const_reverse_iterator p_crit(v4.crbegin());
        EXPECT_EQ((*p_it).first, 'a');
        EXPECT_EQ((*p_it).second, 1);
        EXPECT_EQ((*--(p_it = v1.cend())).first, 'c');
        EXPECT_EQ((*p_cit).first, 'a');
        EXPECT_EQ((*--(p_cit = v4.cend())).first, 'c');
        EXPECT_EQ((*p_rit).first, 'c');
        EXPECT_EQ((*p_rit).second, 3);
        EXPECT_EQ((*--(p_rit = v1.crend())).first, 'a');
        EXPECT_EQ((*p_crit).first, 'c');
        EXPECT_EQ((*--(p_crit = v4.crend())).first, 'a');

        typename Mycont::const_iterator p_it1 = typename Mycont::const_iterator();
        typename Mycont::const_iterator p_it2 = typename Mycont::const_iterator();
        EXPECT_EQ(p_it1 == p_it2, true); // check null forward iterator comparisons
    }

    v0.clear();
    std::pair<typename Mycont::iterator, bool> pib = v0.insert(Myval('d', 4));
    EXPECT_EQ((*pib.first).first, 'd');
    EXPECT_EQ(pib.second, true);
    EXPECT_EQ((*--v0.end()).first, 'd');
    pib = v0.insert(Myval('d', 5));
    EXPECT_EQ((*pib.first).first, 'd');
    EXPECT_EQ((*pib.first).second, 4);
    EXPECT_EQ(!pib.second, true);
    EXPECT_EQ((*v0.insert(v0.begin(), Myval('e', 5))).first == 'e', true);
    v0.insert(xarr, xarr + 3);
    EXPECT_EQ(v0.size(), 5);
    EXPECT_EQ((*v0.begin()).first, 'a');
    v0.insert(xarr2, xarr2 + 3);
    EXPECT_EQ(v0.size(), 6);
    EXPECT_EQ((*--v0.end()).first, 'f');
    EXPECT_EQ(v0['c'], 3);
    v0.erase(v0.begin());
    EXPECT_EQ(v0.size(), 5);
    EXPECT_EQ((*v0.begin()).first, 'b');
    v0.erase(v0.begin(), ++v0.begin());
    EXPECT_EQ(v0.size(), 4);
    EXPECT_EQ((*v0.begin()).first, 'c');
    v0.insert(Myval('y', 99));
    EXPECT_EQ(v0.erase('x'), 0);
    EXPECT_EQ(v0.erase('y'), 1);

    { // test added C++11 functionality
        Mycont v1x(xarr, xarr + 3);
        EXPECT_EQ(v1x.at('c'), 3);
    }

    v0.clear();
    EXPECT_EQ(v0.empty(), true);
    v0.swap(v1);
    EXPECT_EQ(!v0.empty(), true);
    EXPECT_EQ(v1.empty(), true);
    std::swap(v0, v1);
    EXPECT_EQ(v0.empty(), true);
    EXPECT_EQ(!v1.empty(), true);
    EXPECT_EQ(v1 == v1, true);
    EXPECT_EQ(v0 < v1, true);
    EXPECT_EQ(v0 != v1, true);
    EXPECT_EQ(v1 > v0, true);
    EXPECT_EQ(v0 <= v1, true);
    EXPECT_EQ(v1 >= v0, true);

    EXPECT_EQ(v0.key_comp()('a', 'c'), true);
    EXPECT_EQ(!v0.key_comp()('a', 'a'), true);
    EXPECT_EQ(v0.value_comp()(Myval('a', 0), Myval('c', 0)), true);
    EXPECT_EQ(!v0.value_comp()(Myval('a', 0), Myval('a', 1)), true);
    EXPECT_EQ((*v4.find('a')).first, 'a');
    EXPECT_EQ(v4.count('x'), 0);
    EXPECT_EQ(v4.count('a'), 1);
    EXPECT_EQ((*v4.lower_bound('a')).first, 'a');
    EXPECT_EQ((*v4.upper_bound('a')).first == 'b', true); // differs from hash_map

    std::pair<typename Mycont::const_iterator, typename Mycont::const_iterator> pcc = v4.equal_range('a');
    EXPECT_EQ((*pcc.first).first, 'a');
    EXPECT_EQ((*pcc.second).first, 'b'); // differs from hash_map

    {
        Mycont v6;
        v6.insert(Myval('a', 1));
        v6.insert(Myval('b', 2));
        Mycont v7(std::move(v6));
        EXPECT_EQ(v6.size(), 0);
        EXPECT_EQ(v7.size(), 2);

        Mycont v8;
        v8 = std::move(v7);
        EXPECT_EQ(v7.size(), 0);
        EXPECT_EQ(v8.size(), 2);

        typedef std::map<Movable_int, int> Mycont2;
        Mycont2 v9;
        v9.insert(std::pair<Movable_int, int>(Movable_int('a'), 1));
        EXPECT_EQ(v9.size(), 1);
        EXPECT_EQ(v9.begin()->first.val, 'a');

        Mycont2 v10;
        std::pair<Movable_int, int> pmi1(Movable_int('e'), 5);
        v10.insert(std::move(pmi1));
        EXPECT_EQ(pmi1.first.val, -1);
        EXPECT_EQ(v10.begin()->first.val, 'e');

        std::pair<Movable_int, int> pmi2(Movable_int('d'), 4);
        v10.insert(v10.end(), std::move(pmi2));
        EXPECT_EQ(pmi2.first.val, -1);
        EXPECT_EQ(v10.begin()->first.val, 'd');

        Movable_int mi3('c');
        EXPECT_EQ(v10[std::move(mi3)], 0);
        EXPECT_EQ(mi3.val, -1);
        EXPECT_EQ(v10.begin()->first.val, 'c');

        v10.clear();
        v10.emplace_hint(v10.end());
        EXPECT_EQ(v10.begin()->first.val, 0);
        EXPECT_EQ(v10.begin()->second, 0);
        v10.clear();
        v10.emplace_hint(v10.end(), 'b', 2);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v10.clear();
        v10.emplace();
        EXPECT_EQ(v10.begin()->first.val, 0);
        EXPECT_EQ(v10.begin()->second, 0);

        v10.clear();
        v10.emplace('b', 2);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v8.clear(); // copyable key
        v8.try_emplace('b', 2);
        EXPECT_EQ(v8.begin()->first, 'b');
        EXPECT_EQ(v8.begin()->second, 2);

        v8.try_emplace('b', 3);
        EXPECT_EQ(v8.begin()->first, 'b');
        EXPECT_EQ(v8.begin()->second, 2);

        v8.insert_or_assign('b', 3);
        EXPECT_EQ(v8.begin()->first, 'b');
        EXPECT_EQ(v8.begin()->second, 3);

        v10.clear(); // movable key
        v10.try_emplace('b', 2);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v10.try_emplace('b', 3);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 2);

        v10.insert_or_assign('b', 3);
        EXPECT_EQ(v10.begin()->first.val, 'b');
        EXPECT_EQ(v10.begin()->second, 3);
    }

    { // check for lvalue stealing
        typedef std::map<Copyable_int, int> Mycont3;
        Mycont3 v11;
        std::pair<Copyable_int, int> pci1(Copyable_int('d'), 4);
        v11.insert(pci1);
        EXPECT_EQ(pci1.first.val, 'd');
        EXPECT_EQ(v11.begin()->first.val, 'd');

        std::pair<Copyable_int, int> pci2(Copyable_int('c'), 3);
        v11.clear();
        v11.insert(v11.end(), pci2);
        EXPECT_EQ(pci2.first.val, 'c');
        EXPECT_EQ(v11.begin()->first.val, 'c');

        Mycont3 v12(v11);
        EXPECT_EQ(v11 == v12, true);
    }

    {
        std::initializer_list<Myval> init{xarr[0], xarr[1], xarr[2]};
        Mycont v11(init);
        EXPECT_EQ(v11.size(), 3);
        EXPECT_EQ(v11.begin()->first, 'a');

        v11.clear();
        v11 = init;
        EXPECT_EQ(v11.size(), 3);
        EXPECT_EQ(v11.begin()->first, 'a');

        v11.clear();
        v11.insert(init);
        EXPECT_EQ(v11.size(), 3);
        EXPECT_EQ(v11.begin()->first, 'a');
    }
}
