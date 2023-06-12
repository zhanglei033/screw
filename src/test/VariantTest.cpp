#include "base/Variant.h"
#include "test/VariantTest.h"
#include <cassert>
#include <exception>
#include <type_traits>
#define ASSERT_NOT_NOEXCEPT(v)   static_assert(!noexcept(v), "must be not noexcept");
#define ASSERT_NOEXCEPT(v)       static_assert(noexcept(v), "must be noexcept");
#define ASSERT_SAME_TYPE(t1, t2) static_assert(std::is_same_v<t1, t2>, "must be same type");

namespace VariantTest {

namespace bad_variant_access {
int run_test()
{
    static_assert(std::is_base_of<std::exception, std::bad_variant_access>::value, "");
    static_assert(noexcept(std::bad_variant_access{}), "must be noexcept");
    static_assert(noexcept(std::bad_variant_access{}.what()), "must be noexcept");
    std::bad_variant_access ex;
    assert(ex.what());
    return 0;
}
} // namespace bad_variant_access

namespace get_if {
namespace index {
void test_const_get_if()
{
    {
        using V              = std::variant<int>;
        constexpr const V* v = nullptr;
        static_assert(std::get_if<0>(v) == nullptr, "");
    }
    {
        using V = std::variant<int, const long>;
        constexpr V v(42);
        static_assert(noexcept(std::get_if<0>(&v)), "must be noexcept");
        static_assert(std::is_same_v<decltype(std::get_if<0>(&v)), const int*>, "must be same type");
        static_assert(*std::get_if<0>(&v) == 42, "");
        static_assert(std::get_if<1>(&v) == nullptr, "");
    }
    {
        using V = std::variant<int, const long>;
        constexpr V v(42l);

        static_assert(std::is_same_v<decltype(std::get_if<1>(&v)), const long*>, "must be same type");

        static_assert(*std::get_if<1>(&v) == 42, "");
        static_assert(std::get_if<0>(&v) == nullptr, "");
    }
}

void test_get_if()
{
    {
        using V = std::variant<int>;
        V* v    = nullptr;
        assert(std::get_if<0>(v) == nullptr);
    }
    {
        using V = std::variant<int, long>;
        V v(42);
        ASSERT_NOEXCEPT(std::get_if<0>(&v));
        ASSERT_SAME_TYPE(decltype(std::get_if<0>(&v)), int*);
        assert(*std::get_if<0>(&v) == 42);
        assert(std::get_if<1>(&v) == nullptr);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get_if<1>(&v)), const long*);
        assert(*std::get_if<1>(&v) == 42);
        assert(std::get_if<0>(&v) == nullptr);
    }
}

int run_test()
{
    test_const_get_if();
    test_get_if();
    return 0;
}
} // namespace index

namespace type {
void test_const_get_if()
{
    {
        using V              = std::variant<int>;
        constexpr const V* v = nullptr;
        static_assert(std::get_if<int>(v) == nullptr, "");
    }
    {
        using V = std::variant<int, const long>;
        constexpr V v(42);
        ASSERT_NOEXCEPT(std::get_if<int>(&v));
        ASSERT_SAME_TYPE(decltype(std::get_if<int>(&v)), const int*);
        static_assert(*std::get_if<int>(&v) == 42, "");
        static_assert(std::get_if<const long>(&v) == nullptr, "");
    }
    {
        using V = std::variant<int, const long>;
        constexpr V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get_if<const long>(&v)), const long*);
        static_assert(*std::get_if<const long>(&v) == 42, "");
        static_assert(std::get_if<int>(&v) == nullptr, "");
    }
}

void test_get_if()
{
    {
        using V = std::variant<int>;
        V* v    = nullptr;
        assert(std::get_if<int>(v) == nullptr);
    }
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOEXCEPT(std::get_if<int>(&v));
        ASSERT_SAME_TYPE(decltype(std::get_if<int>(&v)), int*);
        assert(*std::get_if<int>(&v) == 42);
        assert(std::get_if<const long>(&v) == nullptr);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get_if<const long>(&v)), const long*);
        assert(*std::get_if<const long>(&v) == 42);
        assert(std::get_if<int>(&v) == nullptr);
    }
}

int run_test()
{
    test_const_get_if();
    test_get_if();

    return 0;
}
} // namespace type

} // namespace get_if

namespace get {
namespace index {
void test_const_lvalue_get()
{
    {
        using V = std::variant<int, const long>;
        constexpr V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(v));
        ASSERT_SAME_TYPE(decltype(std::get<0>(v)), const int&);
        static_assert(std::get<0>(v) == 42, "");
    }
    {
        using V = std::variant<int, const long>;
        const V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(v));
        ASSERT_SAME_TYPE(decltype(std::get<0>(v)), const int&);
        assert(std::get<0>(v) == 42);
    }
    {
        using V = std::variant<int, const long>;
        constexpr V v(42l);
        ASSERT_NOT_NOEXCEPT(std::get<1>(v));
        ASSERT_SAME_TYPE(decltype(std::get<1>(v)), const long&);
        static_assert(std::get<1>(v) == 42, "");
    }
    {
        using V = std::variant<int, const long>;
        const V v(42l);
        ASSERT_NOT_NOEXCEPT(std::get<1>(v));
        ASSERT_SAME_TYPE(decltype(std::get<1>(v)), const long&);
        assert(std::get<1>(v) == 42);
    }
}

void test_lvalue_get()
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(v));
        ASSERT_SAME_TYPE(decltype(std::get<0>(v)), int&);
        assert(std::get<0>(v) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<1>(v)), const long&);
        assert(std::get<1>(v) == 42);
    }
}

void test_rvalue_get()
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<0>(std::move(v))), int&&);
        assert(std::get<0>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<1>(std::move(v))), const long&&);
        assert(std::get<1>(std::move(v)) == 42);
    }
}

void test_const_rvalue_get()
{
    {
        using V = std::variant<int, const long>;
        const V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<0>(std::move(v))), const int&&);
        assert(std::get<0>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        const V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<1>(std::move(v))), const long&&);
        assert(std::get<1>(std::move(v)) == 42);
    }
}

int run_test()
{
    test_const_lvalue_get();
    test_lvalue_get();
    test_rvalue_get();
    test_const_rvalue_get();

    return 0;
}
} // namespace index

namespace type {
void test_const_lvalue_get()
{
    {
        using V = std::variant<int, const long>;
        constexpr V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(v));
        ASSERT_SAME_TYPE(decltype(std::get<int>(v)), const int&);
        static_assert(std::get<int>(v) == 42, "");
    }
    {
        using V = std::variant<int, const long>;
        const V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(v));
        ASSERT_SAME_TYPE(decltype(std::get<int>(v)), const int&);
        assert(std::get<int>(v) == 42);
    }
    {
        using V = std::variant<int, const long>;
        constexpr V v(42l);
        ASSERT_NOT_NOEXCEPT(std::get<const long>(v));
        ASSERT_SAME_TYPE(decltype(std::get<const long>(v)), const long&);
        static_assert(std::get<const long>(v) == 42, "");
    }
    {
        using V = std::variant<int, const long>;
        const V v(42l);
        ASSERT_NOT_NOEXCEPT(std::get<const long>(v));
        ASSERT_SAME_TYPE(decltype(std::get<const long>(v)), const long&);
        assert(std::get<const long>(v) == 42);
    }
}

void test_lvalue_get()
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(v));
        ASSERT_SAME_TYPE(decltype(std::get<int>(v)), int&);
        assert(std::get<int>(v) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<const long>(v)), const long&);
        assert(std::get<const long>(v) == 42);
    }
}

void test_rvalue_get()
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<int>(std::move(v))), int&&);
        assert(std::get<int>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<const long>(std::move(v))),
                         const long&&);
        assert(std::get<const long>(std::move(v)) == 42);
    }
}

void test_const_rvalue_get()
{
    {
        using V = std::variant<int, const long>;
        const V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<int>(std::move(v))), const int&&);
        assert(std::get<int>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        const V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<const long>(std::move(v))),
                         const long&&);
        assert(std::get<const long>(std::move(v)) == 42);
    }
}

int run_test()
{
    test_const_lvalue_get();
    test_lvalue_get();
    test_rvalue_get();
    test_const_rvalue_get();

    return 0;
}
} // namespace type
} // namespace get

namespace holds_alternative {
int run_test()
{
    {
        using V = std::variant<int>;
        constexpr V v;
        static_assert(std::holds_alternative<int>(v), "");
    }
    {
        using V = std::variant<int, long>;
        constexpr V v;
        static_assert(std::holds_alternative<int>(v), "");
        static_assert(!std::holds_alternative<long>(v), "");
    }
    { // noexcept test
        using V = std::variant<int>;
        const V v;
        ASSERT_NOEXCEPT(std::holds_alternative<int>(v));
    }

    return 0;
}
} // namespace holds_alternative

namespace hash {
void test_hash_variant()
{
    {
        using V = std::variant<int, long, int>;
        using H = std::hash<V>;
        const V v(std::in_place_index<0>, 42);
        const V v_copy = v;
        V v2(std::in_place_index<0>, 100);
        const H h{};
        assert(h(v) == h(v));
        assert(h(v) != h(v2));
        assert(h(v) == h(v_copy));
        {
            ASSERT_SAME_TYPE(decltype(h(v)), std::size_t);
            static_assert(std::is_copy_constructible<H>::value, "");
        }
    }
    {
        using V         = std::variant<std::monostate, int, long, const char*>;
        using H         = std::hash<V>;
        const char* str = "hello";
        const V v0;
        const V v0_other;
        const V v1(42);
        const V v1_other(100);
        V v2(100l);
        V v2_other(999l);
        V v3(str);
        V v3_other("not hello");
        const H h{};
        assert(h(v0) == h(v0));
        assert(h(v0) == h(v0_other));
        assert(h(v1) == h(v1));
        assert(h(v1) != h(v1_other));
        assert(h(v2) == h(v2));
        assert(h(v2) != h(v2_other));
        assert(h(v3) == h(v3));
        assert(h(v3) != h(v3_other));
        assert(h(v0) != h(v1));
        assert(h(v0) != h(v2));
        assert(h(v0) != h(v3));
        assert(h(v1) != h(v2));
        assert(h(v1) != h(v3));
        assert(h(v2) != h(v3));
    }
}
void test_hash_monostate()
{
    using H = std::hash<std::monostate>;
    const H h{};
    std::monostate m1{};
    const std::monostate m2{};
    assert(h(m1) == h(m1));
    assert(h(m2) == h(m2));
    assert(h(m1) == h(m2));
    {
        ASSERT_SAME_TYPE(decltype(h(m1)), std::size_t);
        ASSERT_NOEXCEPT(h(m1));
        static_assert(std::is_copy_constructible<H>::value, "");
    }
}
void test_hash_variant_duplicate_elements()
{
    using V = std::variant<std::monostate, std::monostate>;
    using H = std::hash<V>;
    H h{};
    const V v1(std::in_place_index<0>);
    const V v2(std::in_place_index<1>);
    assert(h(v1) == h(v1));
    assert(h(v2) == h(v2));
}
int run_test()
{
    test_hash_variant();
    test_hash_monostate();
    test_hash_variant_duplicate_elements();
    return 0;
}
} // namespace hash

namespace helpers {
namespace variant_alternative {
template <class V, size_t I, class E>
void test()
{
    static_assert(std::is_same_v<typename std::variant_alternative<I, V>::type, E>, "");
    static_assert(std::is_same_v<typename std::variant_alternative<I, const V>::type, const E>, "");
    static_assert(std::is_same_v<typename std::variant_alternative<I, volatile V>::type, volatile E>, "");
    static_assert(std::is_same_v<typename std::variant_alternative<I, const volatile V>::type, const volatile E>, "");
    static_assert(std::is_same_v<std::variant_alternative_t<I, V>, E>, "");
    static_assert(std::is_same_v<std::variant_alternative_t<I, const V>, const E>, "");
    static_assert(std::is_same_v<std::variant_alternative_t<I, volatile V>, volatile E>, "");
    static_assert(std::is_same_v<std::variant_alternative_t<I, const volatile V>, const volatile E>, "");
}
int run_test()
{
    {
        using V = std::variant<int, void*, const void*, long double>;
        test<V, 0, int>();
        test<V, 1, void*>();
        test<V, 2, const void*>();
        test<V, 3, long double>();
    }
    return 0;
}
} // namespace variant_alternative

namespace variant_size {
template <class V, size_t E>
void test()
{
    static_assert(std::variant_size<V>::value == E, "");
    static_assert(std::variant_size<const V>::value == E, "");
    static_assert(std::variant_size<volatile V>::value == E, "");
    static_assert(std::variant_size<const volatile V>::value == E, "");
    static_assert(std::variant_size_v<V> == E, "");
    static_assert(std::variant_size_v<const V> == E, "");
    static_assert(std::variant_size_v<volatile V> == E, "");
    static_assert(std::variant_size_v<const volatile V> == E, "");
    static_assert(std::is_base_of<std::integral_constant<std::size_t, E>, std::variant_size<V>>::value, "");
};
int run_test()
{
    test<std::variant<>, 0>();
    test<std::variant<void*>, 1>();
    test<std::variant<long, long, void*, double>, 4>();

    return 0;
}
} // namespace variant_size
} // namespace helpers

namespace monostate {
namespace properties {
int run_test()
{
    using M = std::monostate;
    static_assert(std::is_trivially_default_constructible<M>::value, "");
    static_assert(std::is_trivially_copy_constructible<M>::value, "");
    static_assert(std::is_trivially_copy_assignable<M>::value, "");
    static_assert(std::is_trivially_destructible<M>::value, "");
    constexpr M m{};
    ((void)m);

    return 0;
}
} // namespace properties
namespace relops {
int run_test()
{
    using M = std::monostate;
    constexpr M m1{};
    constexpr M m2{};
    {
        static_assert((m1 < m2) == false, "");
        ASSERT_NOEXCEPT(m1 < m2);
    }
    {
        static_assert((m1 > m2) == false, "");
        ASSERT_NOEXCEPT(m1 > m2);
    }
    {
        static_assert((m1 <= m2) == true, "");
        ASSERT_NOEXCEPT(m1 <= m2);
    }
    {
        static_assert((m1 >= m2) == true, "");
        ASSERT_NOEXCEPT(m1 >= m2);
    }
    {
        static_assert((m1 == m2) == true, "");
        ASSERT_NOEXCEPT(m1 == m2);
    }
    {
        static_assert((m1 != m2) == false, "");
        ASSERT_NOEXCEPT(m1 != m2);
    }

    return 0;
}
} // namespace relops
} // namespace monostate

namespace relops {
struct MyBool
{
    bool value;
    constexpr explicit MyBool(bool v)
        : value(v) {}
    constexpr operator bool() const noexcept { return value; }
};

struct ComparesToMyBool
{
    int value = 0;
};
inline constexpr MyBool operator==(const ComparesToMyBool& l, const ComparesToMyBool& r) noexcept
{
    return MyBool(l.value == r.value);
}
inline constexpr MyBool operator!=(const ComparesToMyBool& l, const ComparesToMyBool& r) noexcept
{
    return MyBool(l.value != r.value);
}
inline constexpr MyBool operator<(const ComparesToMyBool& l, const ComparesToMyBool& r) noexcept
{
    return MyBool(l.value < r.value);
}
inline constexpr MyBool operator<=(const ComparesToMyBool& l, const ComparesToMyBool& r) noexcept
{
    return MyBool(l.value <= r.value);
}
inline constexpr MyBool operator>(const ComparesToMyBool& l, const ComparesToMyBool& r) noexcept
{
    return MyBool(l.value > r.value);
}
inline constexpr MyBool operator>=(const ComparesToMyBool& l, const ComparesToMyBool& r) noexcept
{
    return MyBool(l.value >= r.value);
}

template <class T1, class T2>
void test_equality_basic()
{
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<0>, T1{42});
        constexpr V v2(std::in_place_index<0>, T1{42});
        static_assert(v1 == v2, "");
        static_assert(v2 == v1, "");
        static_assert(!(v1 != v2), "");
        static_assert(!(v2 != v1), "");
    }
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<0>, T1{42});
        constexpr V v2(std::in_place_index<0>, T1{43});
        static_assert(!(v1 == v2), "");
        static_assert(!(v2 == v1), "");
        static_assert(v1 != v2, "");
        static_assert(v2 != v1, "");
    }
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<0>, T1{42});
        constexpr V v2(std::in_place_index<1>, T2{42});
        static_assert(!(v1 == v2), "");
        static_assert(!(v2 == v1), "");
        static_assert(v1 != v2, "");
        static_assert(v2 != v1, "");
    }
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<1>, T2{42});
        constexpr V v2(std::in_place_index<1>, T2{42});
        static_assert(v1 == v2, "");
        static_assert(v2 == v1, "");
        static_assert(!(v1 != v2), "");
        static_assert(!(v2 != v1), "");
    }
}

void test_equality()
{
    test_equality_basic<int, long>();
    test_equality_basic<ComparesToMyBool, int>();
    test_equality_basic<int, ComparesToMyBool>();
    test_equality_basic<ComparesToMyBool, ComparesToMyBool>();
}

template <class Var>
constexpr bool test_less(const Var& l, const Var& r, bool expect_less, bool expect_greater)
{
    static_assert(std::is_same_v<decltype(l < r), bool>, "");
    static_assert(std::is_same_v<decltype(l <= r), bool>, "");
    static_assert(std::is_same_v<decltype(l > r), bool>, "");
    static_assert(std::is_same_v<decltype(l >= r), bool>, "");

    return ((l < r) == expect_less) && (!(l >= r) == expect_less) && ((l > r) == expect_greater) && (!(l <= r) == expect_greater);
}

template <class T1, class T2>
void test_relational_basic()
{
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<0>, T1{1});
        constexpr V v2(std::in_place_index<0>, T1{1});
        static_assert(test_less(v1, v2, false, false), "");
    }
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<0>, T1{0});
        constexpr V v2(std::in_place_index<0>, T1{1});
        static_assert(test_less(v1, v2, true, false), "");
    }
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<0>, T1{1});
        constexpr V v2(std::in_place_index<0>, T1{0});
        static_assert(test_less(v1, v2, false, true), "");
    }
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<0>, T1{0});
        constexpr V v2(std::in_place_index<1>, T2{0});
        static_assert(test_less(v1, v2, true, false), "");
    }
    {
        using V = std::variant<T1, T2>;
        constexpr V v1(std::in_place_index<1>, T2{0});
        constexpr V v2(std::in_place_index<0>, T1{0});
        static_assert(test_less(v1, v2, false, true), "");
    }
}

void test_relational()
{
    test_relational_basic<int, long>();
    test_relational_basic<ComparesToMyBool, int>();
    test_relational_basic<int, ComparesToMyBool>();
    test_relational_basic<ComparesToMyBool, ComparesToMyBool>();
}

int run_test()
{
    test_equality();
    test_relational();
    return 0;
}
} // namespace relops

namespace npos {
int run_test()
{
    static_assert(std::variant_npos == static_cast<std::size_t>(-1), "");
    return 0;
}
} // namespace npos

namespace assign {
namespace conv {
int run_test()
{
    static_assert(!std::is_assignable<std::variant<int, int>, int>::value, "");
    static_assert(!std::is_assignable<std::variant<long, long long>, int>::value, "");
    return 0;
}
} // namespace conv

namespace copy {
struct NoCopy
{
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = default;
};

struct CopyOnly
{
    CopyOnly(const CopyOnly&) = default;
    CopyOnly(CopyOnly&&)      = delete;
    CopyOnly& operator=(const CopyOnly&) = default;
    CopyOnly& operator=(CopyOnly&&) = delete;
};

struct MoveOnly
{
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&)      = default;
    MoveOnly& operator=(const MoveOnly&) = default;
};

struct MoveOnlyNT
{
    MoveOnlyNT(const MoveOnlyNT&) = delete;
    MoveOnlyNT(MoveOnlyNT&&) {}
    MoveOnlyNT& operator=(const MoveOnlyNT&) = default;
};

struct CopyAssign
{
    static int alive;
    static int copy_construct;
    static int copy_assign;
    static int move_construct;
    static int move_assign;
    static void reset()
    {
        copy_construct = copy_assign = move_construct = move_assign = alive = 0;
    }
    CopyAssign(int v)
        : value(v) { ++alive; }
    CopyAssign(const CopyAssign& o)
        : value(o.value)
    {
        ++alive;
        ++copy_construct;
    }
    CopyAssign(CopyAssign&& o) noexcept
        : value(o.value)
    {
        o.value = -1;
        ++alive;
        ++move_construct;
    }
    CopyAssign& operator=(const CopyAssign& o)
    {
        value = o.value;
        ++copy_assign;
        return *this;
    }
    CopyAssign& operator=(CopyAssign&& o) noexcept
    {
        value   = o.value;
        o.value = -1;
        ++move_assign;
        return *this;
    }
    ~CopyAssign() { --alive; }
    int value;
};

int CopyAssign::alive          = 0;
int CopyAssign::copy_construct = 0;
int CopyAssign::copy_assign    = 0;
int CopyAssign::move_construct = 0;
int CopyAssign::move_assign    = 0;

struct CopyMaybeThrows
{
    CopyMaybeThrows(const CopyMaybeThrows&);
    CopyMaybeThrows& operator=(const CopyMaybeThrows&);
};
struct CopyDoesThrow
{
    CopyDoesThrow(const CopyDoesThrow&) noexcept(false);
    CopyDoesThrow& operator=(const CopyDoesThrow&) noexcept(false);
};

struct NTCopyAssign
{
    constexpr NTCopyAssign(int v)
        : value(v) {}
    NTCopyAssign(const NTCopyAssign&) = default;
    NTCopyAssign(NTCopyAssign&&)      = default;
    NTCopyAssign& operator            =(const NTCopyAssign& that)
    {
        value = that.value;
        return *this;
    };
    NTCopyAssign& operator=(NTCopyAssign&&) = delete;
    int value;
};

static_assert(!std::is_trivially_copy_assignable<NTCopyAssign>::value, "");
static_assert(std::is_copy_assignable<NTCopyAssign>::value, "");

struct TCopyAssign
{
    constexpr TCopyAssign(int v)
        : value(v) {}
    TCopyAssign(const TCopyAssign&) = default;
    TCopyAssign(TCopyAssign&&)      = default;
    TCopyAssign& operator=(const TCopyAssign&) = default;
    TCopyAssign& operator=(TCopyAssign&&) = delete;
    int value;
};

static_assert(std::is_trivially_copy_assignable<TCopyAssign>::value, "");

struct TCopyAssignNTMoveAssign
{
    constexpr TCopyAssignNTMoveAssign(int v)
        : value(v) {}
    TCopyAssignNTMoveAssign(const TCopyAssignNTMoveAssign&) = default;
    TCopyAssignNTMoveAssign(TCopyAssignNTMoveAssign&&)      = default;
    TCopyAssignNTMoveAssign& operator=(const TCopyAssignNTMoveAssign&) = default;
    TCopyAssignNTMoveAssign& operator                                  =(TCopyAssignNTMoveAssign&& that)
    {
        value      = that.value;
        that.value = -1;
        return *this;
    }
    int value;
};

static_assert(std::is_trivially_copy_assignable_v<TCopyAssignNTMoveAssign>, "");

void test_copy_assignment_not_noexcept()
{
    {
        using V = std::variant<CopyMaybeThrows>;
        static_assert(!std::is_nothrow_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, CopyDoesThrow>;
        static_assert(!std::is_nothrow_copy_assignable<V>::value, "");
    }
}

void test_copy_assignment_sfinae()
{
    {
        using V = std::variant<int, long>;
        static_assert(std::is_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, CopyOnly>;
        static_assert(std::is_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, NoCopy>;
        static_assert(!std::is_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnly>;
        static_assert(!std::is_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnlyNT>;
        static_assert(!std::is_copy_assignable<V>::value, "");
    }

    {
        using V = std::variant<int, long>;
        static_assert(std::is_trivially_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, NTCopyAssign>;
        static_assert(!std::is_trivially_copy_assignable<V>::value, "");
        static_assert(std::is_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, TCopyAssign>;
        static_assert(std::is_trivially_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, TCopyAssignNTMoveAssign>;
        static_assert(std::is_trivially_copy_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, CopyOnly>;
        static_assert(std::is_trivially_copy_assignable<V>::value, "");
    }
}

template <typename T>
struct Result
{
    size_t index;
    T value;
};

void test_copy_assignment_same_index()
{
    {
        using V = std::variant<int>;
        V v1(43);
        V v2(42);
        V& vref = (v1 = v2);
        assert(&vref == &v1);
        assert(v1.index() == 0);
        assert(std::get<0>(v1) == 42);
    }
    {
        using V = std::variant<int, long, unsigned>;
        V v1(43l);
        V v2(42l);
        V& vref = (v1 = v2);
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, CopyAssign, unsigned>;

        V v1(std::in_place_type<CopyAssign>, 43);
        V v2(std::in_place_type<CopyAssign>, 42);
        CopyAssign::reset();
        V& vref = (v1 = v2);
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1).value == 42);
        assert(CopyAssign::copy_construct == 0);
        assert(CopyAssign::move_construct == 0);
        assert(CopyAssign::copy_assign == 1);
    }

    {
        struct
        {
            constexpr Result<int> operator()() const
            {
                using V = std::variant<int>;
                V v(43);
                V v2(42);
                v = v2;
                return {v.index(), std::get<0>(v)};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 0, "");
        static_assert(result.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<long> operator()() const
            {
                using V = std::variant<int, long, unsigned>;
                V v(43l);
                V v2(42l);
                v = v2;
                return {v.index(), std::get<1>(v)};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42l, "");
    }
    {
        struct
        {
            constexpr Result<int> operator()() const
            {
                using V = std::variant<int, TCopyAssign, unsigned>;
                V v(std::in_place_type<TCopyAssign>, 43);
                V v2(std::in_place_type<TCopyAssign>, 42);
                v = v2;
                return {v.index(), std::get<1>(v).value};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<int> operator()() const
            {
                using V = std::variant<int, TCopyAssignNTMoveAssign, unsigned>;
                V v(std::in_place_type<TCopyAssignNTMoveAssign>, 43);
                V v2(std::in_place_type<TCopyAssignNTMoveAssign>, 42);
                v = v2;
                return {v.index(), std::get<1>(v).value};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42, "");
    }
}

void test_copy_assignment_different_index()
{
    {
        using V = std::variant<int, long, unsigned>;
        V v1(43);
        V v2(42l);
        V& vref = (v1 = v2);
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, CopyAssign, unsigned>;
        CopyAssign::reset();
        V v1(std::in_place_type<unsigned>, 43u);
        V v2(std::in_place_type<CopyAssign>, 42);
        assert(CopyAssign::copy_construct == 0);
        assert(CopyAssign::move_construct == 0);
        assert(CopyAssign::alive == 1);
        V& vref = (v1 = v2);
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1).value == 42);
        assert(CopyAssign::alive == 2);
        assert(CopyAssign::copy_construct == 1);
        assert(CopyAssign::move_construct == 1);
        assert(CopyAssign::copy_assign == 0);
    }

    {
        struct
        {
            constexpr Result<long> operator()() const
            {
                using V = std::variant<int, long, unsigned>;
                V v(43);
                V v2(42l);
                v = v2;
                return {v.index(), std::get<1>(v)};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42l, "");
    }
    {
        struct
        {
            constexpr Result<int> operator()() const
            {
                using V = std::variant<int, TCopyAssign, unsigned>;
                V v(std::in_place_type<unsigned>, 43u);
                V v2(std::in_place_type<TCopyAssign>, 42);
                v = v2;
                return {v.index(), std::get<1>(v).value};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42, "");
    }
}

template <size_t NewIdx, class ValueType>
constexpr bool test_constexpr_assign_imp(
    std::variant<long, void*, int>&& v,
    ValueType&& new_value)
{
    const std::variant<long, void*, int> cp(std::forward<ValueType>(new_value));
    v = cp;
    return v.index() == NewIdx && std::get<NewIdx>(v) == std::get<NewIdx>(cp);
}

void test_constexpr_copy_assignment()
{
    using V = std::variant<long, void*, int>;

    static_assert(std::is_trivially_copyable<V>::value, "");
    static_assert(std::is_trivially_copy_assignable<V>::value, "");
    static_assert(test_constexpr_assign_imp<0>(V(42l), 101l), "");
    static_assert(test_constexpr_assign_imp<0>(V(nullptr), 101l), "");
    static_assert(test_constexpr_assign_imp<1>(V(42l), nullptr), "");
    static_assert(test_constexpr_assign_imp<2>(V(42l), 101), "");
}

int run_test()
{
    test_copy_assignment_same_index();
    test_copy_assignment_different_index();
    test_copy_assignment_sfinae();
    test_copy_assignment_not_noexcept();
    test_constexpr_copy_assignment();

    return 0;
}
} // namespace copy

namespace move {
struct NoCopy
{
    NoCopy(const NoCopy&) = delete;
    NoCopy& operator=(const NoCopy&) = default;
};

struct CopyOnly
{
    CopyOnly(const CopyOnly&) = default;
    CopyOnly(CopyOnly&&)      = delete;
    CopyOnly& operator=(const CopyOnly&) = default;
    CopyOnly& operator=(CopyOnly&&) = delete;
};

struct MoveOnly
{
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&)      = default;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly& operator=(MoveOnly&&) = default;
};

struct MoveOnlyNT
{
    MoveOnlyNT(const MoveOnlyNT&) = delete;
    MoveOnlyNT(MoveOnlyNT&&) {}
    MoveOnlyNT& operator=(const MoveOnlyNT&) = delete;
    MoveOnlyNT& operator=(MoveOnlyNT&&) = default;
};

struct MoveOnlyOddNothrow
{
    MoveOnlyOddNothrow(MoveOnlyOddNothrow&&) noexcept(false) {}
    MoveOnlyOddNothrow(const MoveOnlyOddNothrow&) = delete;
    MoveOnlyOddNothrow& operator=(MoveOnlyOddNothrow&&) noexcept = default;
    MoveOnlyOddNothrow& operator=(const MoveOnlyOddNothrow&) = delete;
};

struct MoveAssignOnly
{
    MoveAssignOnly(MoveAssignOnly&&) = delete;
    MoveAssignOnly& operator=(MoveAssignOnly&&) = default;
};

struct MoveAssign
{
    static int move_construct;
    static int move_assign;
    static void reset() { move_construct = move_assign = 0; }
    MoveAssign(int v)
        : value(v) {}
    MoveAssign(MoveAssign&& o)
        : value(o.value)
    {
        ++move_construct;
        o.value = -1;
    }
    MoveAssign& operator=(MoveAssign&& o)
    {
        value = o.value;
        ++move_assign;
        o.value = -1;
        return *this;
    }
    int value;
};

int MoveAssign::move_construct = 0;
int MoveAssign::move_assign    = 0;

struct NTMoveAssign
{
    constexpr NTMoveAssign(int v)
        : value(v) {}
    NTMoveAssign(const NTMoveAssign&) = default;
    NTMoveAssign(NTMoveAssign&&)      = default;
    NTMoveAssign& operator=(const NTMoveAssign& that) = default;
    NTMoveAssign& operator                            =(NTMoveAssign&& that)
    {
        value      = that.value;
        that.value = -1;
        return *this;
    };
    int value;
};

static_assert(!std::is_trivially_move_assignable<NTMoveAssign>::value, "");
static_assert(std::is_move_assignable<NTMoveAssign>::value, "");

struct TMoveAssign
{
    constexpr TMoveAssign(int v)
        : value(v) {}
    TMoveAssign(const TMoveAssign&) = delete;
    TMoveAssign(TMoveAssign&&)      = default;
    TMoveAssign& operator=(const TMoveAssign&) = delete;
    TMoveAssign& operator=(TMoveAssign&&) = default;
    int value;
};

static_assert(std::is_trivially_move_assignable<TMoveAssign>::value, "");

struct TMoveAssignNTCopyAssign
{
    constexpr TMoveAssignNTCopyAssign(int v)
        : value(v) {}
    TMoveAssignNTCopyAssign(const TMoveAssignNTCopyAssign&) = default;
    TMoveAssignNTCopyAssign(TMoveAssignNTCopyAssign&&)      = default;
    TMoveAssignNTCopyAssign& operator                       =(const TMoveAssignNTCopyAssign& that)
    {
        value = that.value;
        return *this;
    }
    TMoveAssignNTCopyAssign& operator=(TMoveAssignNTCopyAssign&&) = default;
    int value;
};

static_assert(std::is_trivially_move_assignable_v<TMoveAssignNTCopyAssign>, "");

struct TrivialCopyNontrivialMove
{
    TrivialCopyNontrivialMove(TrivialCopyNontrivialMove const&) = default;
    TrivialCopyNontrivialMove(TrivialCopyNontrivialMove&&) noexcept {}
    TrivialCopyNontrivialMove& operator=(TrivialCopyNontrivialMove const&) = default;
    TrivialCopyNontrivialMove& operator                                    =(TrivialCopyNontrivialMove&&) noexcept
    {
        return *this;
    }
};

static_assert(std::is_trivially_copy_assignable_v<TrivialCopyNontrivialMove>, "");
static_assert(!std::is_trivially_move_assignable_v<TrivialCopyNontrivialMove>, "");

void test_move_assignment_noexcept()
{
    {
        using V = std::variant<int>;
        static_assert(std::is_nothrow_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<MoveOnly>;
        static_assert(std::is_nothrow_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, long>;
        static_assert(std::is_nothrow_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnly>;
        static_assert(std::is_nothrow_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<MoveOnlyNT>;
        static_assert(!std::is_nothrow_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<MoveOnlyOddNothrow>;
        static_assert(!std::is_nothrow_move_assignable<V>::value, "");
    }
}

void test_move_assignment_sfinae()
{
    {
        using V = std::variant<int, long>;
        static_assert(std::is_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, CopyOnly>;
        static_assert(std::is_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, NoCopy>;
        static_assert(!std::is_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnly>;
        static_assert(std::is_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnlyNT>;
        static_assert(std::is_move_assignable<V>::value, "");
    }
    {
        // variant only provides move assignment when the types also provide
        // a move constructor.
        using V = std::variant<int, MoveAssignOnly>;
        static_assert(!std::is_move_assignable<V>::value, "");
    }

    // Make sure we properly propagate triviality (see P0602R4).
#if TEST_STD_VER > 17
    {
        using V = std::variant<int, long>;
        static_assert(std::is_trivially_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, NTMoveAssign>;
        static_assert(!std::is_trivially_move_assignable<V>::value, "");
        static_assert(std::is_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, TMoveAssign>;
        static_assert(std::is_trivially_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, TMoveAssignNTCopyAssign>;
        static_assert(std::is_trivially_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, TrivialCopyNontrivialMove>;
        static_assert(!std::is_trivially_move_assignable<V>::value, "");
    }
    {
        using V = std::variant<int, CopyOnly>;
        static_assert(std::is_trivially_move_assignable<V>::value, "");
    }
#endif // > C++17
}

template <typename T>
struct Result
{
    size_t index;
    T value;
};

void test_move_assignment_same_index()
{
    {
        using V = std::variant<int>;
        V v1(43);
        V v2(42);
        V& vref = (v1 = std::move(v2));
        assert(&vref == &v1);
        assert(v1.index() == 0);
        assert(std::get<0>(v1) == 42);
    }
    {
        using V = std::variant<int, long, unsigned>;
        V v1(43l);
        V v2(42l);
        V& vref = (v1 = std::move(v2));
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, MoveAssign, unsigned>;
        V v1(std::in_place_type<MoveAssign>, 43);
        V v2(std::in_place_type<MoveAssign>, 42);
        MoveAssign::reset();
        V& vref = (v1 = std::move(v2));
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1).value == 42);
        assert(MoveAssign::move_construct == 0);
        assert(MoveAssign::move_assign == 1);
    }
    {
        struct
        {
            constexpr Result<int> operator()() const
            {
                using V = std::variant<int>;
                V v(43);
                V v2(42);
                v = std::move(v2);
                return {v.index(), std::get<0>(v)};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 0, "");
        static_assert(result.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<long> operator()() const
            {
                using V = std::variant<int, long, unsigned>;
                V v(43l);
                V v2(42l);
                v = std::move(v2);
                return {v.index(), std::get<1>(v)};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42l, "");
    }
    {
        struct
        {
            constexpr Result<int> operator()() const
            {
                using V = std::variant<int, TMoveAssign, unsigned>;
                V v(std::in_place_type<TMoveAssign>, 43);
                V v2(std::in_place_type<TMoveAssign>, 42);
                v = std::move(v2);
                return {v.index(), std::get<1>(v).value};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42, "");
    }
}

void test_move_assignment_different_index()
{
    {
        using V = std::variant<int, long, unsigned>;
        V v1(43);
        V v2(42l);
        V& vref = (v1 = std::move(v2));
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, MoveAssign, unsigned>;
        V v1(std::in_place_type<unsigned>, 43u);
        V v2(std::in_place_type<MoveAssign>, 42);
        MoveAssign::reset();
        V& vref = (v1 = std::move(v2));
        assert(&vref == &v1);
        assert(v1.index() == 1);
        assert(std::get<1>(v1).value == 42);
        assert(MoveAssign::move_construct == 1);
        assert(MoveAssign::move_assign == 0);
    }
    {
        struct
        {
            constexpr Result<long> operator()() const
            {
                using V = std::variant<int, long, unsigned>;
                V v(43);
                V v2(42l);
                v = std::move(v2);
                return {v.index(), std::get<1>(v)};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42l, "");
    }
    {
        struct
        {
            constexpr Result<long> operator()() const
            {
                using V = std::variant<int, TMoveAssign, unsigned>;
                V v(std::in_place_type<unsigned>, 43u);
                V v2(std::in_place_type<TMoveAssign>, 42);
                v = std::move(v2);
                return {v.index(), std::get<1>(v).value};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42, "");
    }
}

template <size_t NewIdx, class ValueType>
constexpr bool test_constexpr_assign_imp(
    std::variant<long, void*, int>&& v,
    ValueType&& new_value)
{
    std::variant<long, void*, int> v2(
        std::forward<ValueType>(new_value));
    const auto cp = v2;
    v             = std::move(v2);
    return v.index() == NewIdx && std::get<NewIdx>(v) == std::get<NewIdx>(cp);
}

void test_constexpr_move_assignment()
{
    using V = std::variant<long, void*, int>;
    static_assert(std::is_trivially_copyable<V>::value, "");
    static_assert(std::is_trivially_move_assignable<V>::value, "");
    static_assert(test_constexpr_assign_imp<0>(V(42l), 101l), "");
    static_assert(test_constexpr_assign_imp<0>(V(nullptr), 101l), "");
    static_assert(test_constexpr_assign_imp<1>(V(42l), nullptr), "");
    static_assert(test_constexpr_assign_imp<2>(V(42l), 101), "");
}

int run_test()
{
    test_move_assignment_same_index();
    test_move_assignment_different_index();
    test_move_assignment_sfinae();
    test_move_assignment_noexcept();
    test_constexpr_move_assignment();

    return 0;
}
} // namespace move

namespace T {
namespace MetaHelpers {

struct Dummy
{
    Dummy() = default;
};

struct ThrowsCtorT
{
    ThrowsCtorT(int) noexcept(false) {}
    ThrowsCtorT& operator=(int) noexcept { return *this; }
};

struct ThrowsAssignT
{
    ThrowsAssignT(int) noexcept {}
    ThrowsAssignT& operator=(int) noexcept(false) { return *this; }
};

struct NoThrowT
{
    NoThrowT(int) noexcept {}
    NoThrowT& operator=(int) noexcept { return *this; }
};
} // namespace MetaHelpers
namespace RuntimeHelpers {
struct ThrowsCtorT
{
    int value;
    ThrowsCtorT()
        : value(0) {}
    ThrowsCtorT(int) noexcept(false) { throw 42; }
    ThrowsCtorT& operator=(int v) noexcept
    {
        value = v;
        return *this;
    }
};

struct MoveCrashes
{
    int value;
    MoveCrashes(int v = 0) noexcept
        : value{v} {}
    MoveCrashes(MoveCrashes&&) noexcept { assert(false); }
    MoveCrashes& operator=(MoveCrashes&&) noexcept
    {
        assert(false);
        return *this;
    }
    MoveCrashes& operator=(int v) noexcept
    {
        value = v;
        return *this;
    }
};

struct ThrowsCtorTandMove
{
    int value;
    ThrowsCtorTandMove()
        : value(0) {}
    ThrowsCtorTandMove(int) noexcept(false) { throw 42; }
    ThrowsCtorTandMove(ThrowsCtorTandMove&&) noexcept(false) { assert(false); }
    ThrowsCtorTandMove& operator=(int v) noexcept
    {
        value = v;
        return *this;
    }
};

struct ThrowsAssignT
{
    int value;
    ThrowsAssignT()
        : value(0) {}
    ThrowsAssignT(int v) noexcept
        : value(v) {}
    ThrowsAssignT& operator=(int) noexcept(false) { throw 42; }
};

struct NoThrowT
{
    int value;
    NoThrowT()
        : value(0) {}
    NoThrowT(int v) noexcept
        : value(v) {}
    NoThrowT& operator=(int v) noexcept
    {
        value = v;
        return *this;
    }
};

} // namespace RuntimeHelpers
void test_T_assignment_noexcept()
{
    using namespace MetaHelpers;
    {
        using V = std::variant<Dummy, NoThrowT>;
        static_assert(std::is_nothrow_assignable<V, int>::value, "");
    }
    {
        using V = std::variant<Dummy, ThrowsCtorT>;
        static_assert(!std::is_nothrow_assignable<V, int>::value, "");
    }
    {
        using V = std::variant<Dummy, ThrowsAssignT>;
        static_assert(!std::is_nothrow_assignable<V, int>::value, "");
    }
}

void test_T_assignment_sfinae()
{
    {
        using V = std::variant<long, long long>;
        static_assert(!std::is_assignable<V, int>::value, "ambiguous");
    }
    {
        using V = std::variant<std::string, std::string>;
        static_assert(!std::is_assignable<V, const char*>::value, "ambiguous");
    }
    {
        using V = std::variant<std::string, void*>;
        static_assert(!std::is_assignable<V, int>::value, "no matching operator=");
    }

    {
        struct X
        {
        };
        struct Y
        {
            operator X();
        };
        using V = std::variant<X>;
        static_assert(std::is_assignable<V, Y>::value,
                      "regression on user-defined conversions in operator=");
    }
}

void test_T_assignment_basic()
{
    {
        std::variant<int> v(43);
        v = 42;
        assert(v.index() == 0);
        assert(std::get<0>(v) == 42);
    }
    {
        std::variant<int, long> v(43l);
        v = 42;
        assert(v.index() == 0);
        assert(std::get<0>(v) == 42);
        v = 43l;
        assert(v.index() == 1);
        assert(std::get<1>(v) == 43);
    }

    {
        std::variant<bool volatile, int> v = 42;
        v                                  = false;
        assert(v.index() == 0);
        assert(!std::get<0>(v));
        bool lvt = true;
        v        = lvt;
        assert(v.index() == 0);
        assert(std::get<0>(v));
    }
}

void test_T_assignment_performs_construction()
{
    using namespace RuntimeHelpers;
    {
        using V = std::variant<std::string, ThrowsCtorT>;
        V v(std::in_place_type<std::string>, "hello");
        try
        {
            v = 42;
            assert(false);
        } catch (...)
        { /* ... */
        }
        assert(v.index() == 0);
        assert(std::get<0>(v) == "hello");
    }
    {
        using V = std::variant<ThrowsAssignT, std::string>;
        V v(std::in_place_type<std::string>, "hello");
        v = 42;
        assert(v.index() == 0);
        assert(std::get<0>(v).value == 42);
    }
}

void test_T_assignment_performs_assignment()
{
    using namespace RuntimeHelpers;
    {
        using V = std::variant<ThrowsCtorT>;
        V v;
        v = 42;
        assert(v.index() == 0);
        assert(std::get<0>(v).value == 42);
    }
    {
        using V = std::variant<ThrowsCtorT, std::string>;
        V v;
        v = 42;
        assert(v.index() == 0);
        assert(std::get<0>(v).value == 42);
    }
    {
        using V = std::variant<ThrowsAssignT>;
        V v(100);
        try
        {
            v = 42;
            assert(false);
        } catch (...)
        { /* ... */
        }
        assert(v.index() == 0);
        assert(std::get<0>(v).value == 100);
    }
    {
        using V = std::variant<std::string, ThrowsAssignT>;
        V v(100);
        try
        {
            v = 42;
            assert(false);
        } catch (...)
        { /* ... */
        }
        assert(v.index() == 1);
        assert(std::get<1>(v).value == 100);
    }
}

int run_test()
{
    test_T_assignment_basic();
    test_T_assignment_performs_construction();
    test_T_assignment_performs_assignment();
    test_T_assignment_noexcept();
    test_T_assignment_sfinae();

    return 0;
}
} // namespace T
} // namespace assign

namespace ctor {
namespace conv {
int run_test()
{
    static_assert(!std::is_constructible<std::variant<int, int>, int>::value, "");
    static_assert(!std::is_constructible<std::variant<long, long long>, int>::value, "");
    return 0;
}
} // namespace conv

namespace copy {
struct NonT
{
    NonT(int v)
        : value(v) {}
    NonT(const NonT& o)
        : value(o.value) {}
    int value;
};
static_assert(!std::is_trivially_copy_constructible<NonT>::value, "");

struct NoCopy
{
    NoCopy(const NoCopy&) = delete;
};

struct MoveOnly
{
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&)      = default;
};

struct MoveOnlyNT
{
    MoveOnlyNT(const MoveOnlyNT&) = delete;
    MoveOnlyNT(MoveOnlyNT&&) {}
};

struct NTCopy
{
    constexpr NTCopy(int v)
        : value(v) {}
    NTCopy(const NTCopy& that)
        : value(that.value) {}
    NTCopy(NTCopy&&) = delete;
    int value;
};

static_assert(!std::is_trivially_copy_constructible<NTCopy>::value, "");
static_assert(std::is_copy_constructible<NTCopy>::value, "");

struct TCopy
{
    constexpr TCopy(int v)
        : value(v) {}
    TCopy(TCopy const&) = default;
    TCopy(TCopy&&)      = delete;
    int value;
};

static_assert(std::is_trivially_copy_constructible<TCopy>::value, "");

struct TCopyNTMove
{
    constexpr TCopyNTMove(int v)
        : value(v) {}
    TCopyNTMove(const TCopyNTMove&) = default;
    TCopyNTMove(TCopyNTMove&& that)
        : value(that.value) { that.value = -1; }
    int value;
};

static_assert(std::is_trivially_copy_constructible<TCopyNTMove>::value, "");

struct MakeEmptyT
{
    static int alive;
    MakeEmptyT() { ++alive; }
    MakeEmptyT(const MakeEmptyT&)
    {
        ++alive;
        // Don't throw from the copy constructor since variant's assignment
        // operator performs a copy before committing to the assignment.
    }
    MakeEmptyT(MakeEmptyT&&) { throw 42; }
    MakeEmptyT& operator=(const MakeEmptyT&) { throw 42; }
    MakeEmptyT& operator=(MakeEmptyT&&) { throw 42; }
    ~MakeEmptyT() { --alive; }
};

int MakeEmptyT::alive = 0;

template <class Variant>
void makeEmpty(Variant& v)
{
    Variant v2(std::in_place_type<MakeEmptyT>);
    try
    {
        v = std::move(v2);
        assert(false);
    } catch (...)
    {
        assert(v.valueless_by_exception());
    }
}

void test_copy_ctor_sfinae()
{
    {
        using V = std::variant<int, long>;
        static_assert(std::is_copy_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, NoCopy>;
        static_assert(!std::is_copy_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnly>;
        static_assert(!std::is_copy_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnlyNT>;
        static_assert(!std::is_copy_constructible<V>::value, "");
    }

    {
        using V = std::variant<int, long>;
        static_assert(std::is_trivially_copy_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, NTCopy>;
        static_assert(!std::is_trivially_copy_constructible<V>::value, "");
        static_assert(std::is_copy_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, TCopy>;
        static_assert(std::is_trivially_copy_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, TCopyNTMove>;
        static_assert(std::is_trivially_copy_constructible<V>::value, "");
    }
}

void test_copy_ctor_basic()
{
    {
        std::variant<int> v(std::in_place_index<0>, 42);
        std::variant<int> v2 = v;
        assert(v2.index() == 0);
        assert(std::get<0>(v2) == 42);
    }
    {
        std::variant<int, long> v(std::in_place_index<1>, 42);
        std::variant<int, long> v2 = v;
        assert(v2.index() == 1);
        assert(std::get<1>(v2) == 42);
    }
    {
        std::variant<NonT> v(std::in_place_index<0>, 42);
        assert(v.index() == 0);
        std::variant<NonT> v2(v);
        assert(v2.index() == 0);
        assert(std::get<0>(v2).value == 42);
    }
    {
        std::variant<int, NonT> v(std::in_place_index<1>, 42);
        assert(v.index() == 1);
        std::variant<int, NonT> v2(v);
        assert(v2.index() == 1);
        assert(std::get<1>(v2).value == 42);
    }

    {
        constexpr std::variant<int> v(std::in_place_index<0>, 42);
        static_assert(v.index() == 0, "");
        constexpr std::variant<int> v2 = v;
        static_assert(v2.index() == 0, "");
        static_assert(std::get<0>(v2) == 42, "");
    }
    {
        constexpr std::variant<int, long> v(std::in_place_index<1>, 42);
        static_assert(v.index() == 1, "");
        constexpr std::variant<int, long> v2 = v;
        static_assert(v2.index() == 1, "");
        static_assert(std::get<1>(v2) == 42, "");
    }
    {
        constexpr std::variant<TCopy> v(std::in_place_index<0>, 42);
        static_assert(v.index() == 0, "");
        constexpr std::variant<TCopy> v2(v);
        static_assert(v2.index() == 0, "");
        static_assert(std::get<0>(v2).value == 42, "");
    }
    {
        constexpr std::variant<int, TCopy> v(std::in_place_index<1>, 42);
        static_assert(v.index() == 1, "");
        constexpr std::variant<int, TCopy> v2(v);
        static_assert(v2.index() == 1, "");
        static_assert(std::get<1>(v2).value == 42, "");
    }
    {
        constexpr std::variant<TCopyNTMove> v(std::in_place_index<0>, 42);
        static_assert(v.index() == 0, "");
        constexpr std::variant<TCopyNTMove> v2(v);
        static_assert(v2.index() == 0, "");
        static_assert(std::get<0>(v2).value == 42, "");
    }
    {
        constexpr std::variant<int, TCopyNTMove> v(std::in_place_index<1>, 42);
        static_assert(v.index() == 1, "");
        constexpr std::variant<int, TCopyNTMove> v2(v);
        static_assert(v2.index() == 1, "");
        static_assert(std::get<1>(v2).value == 42, "");
    }
}

void test_copy_ctor_valueless_by_exception()
{
    using V = std::variant<int, MakeEmptyT>;
    V v1;
    makeEmpty(v1);
    const V& cv1 = v1;
    V v(cv1);
    assert(v.valueless_by_exception());
}

template <size_t Idx>
constexpr bool test_constexpr_copy_ctor_imp(std::variant<long, void*, const int> const& v)
{
    auto v2 = v;
    return v2.index() == v.index() && v2.index() == Idx && std::get<Idx>(v2) == std::get<Idx>(v);
}

void test_constexpr_copy_ctor()
{
    using V = std::variant<long, void*, const int>;

    static_assert(std::is_trivially_destructible<V>::value, "");
    static_assert(std::is_trivially_copy_constructible<V>::value, "");
    static_assert(std::is_trivially_move_constructible<V>::value, "");
    static_assert(!std::is_copy_assignable<V>::value, "");
    static_assert(!std::is_move_assignable<V>::value, "");

    static_assert(std::is_trivially_copyable<V>::value, "");

    static_assert(test_constexpr_copy_ctor_imp<0>(V(42l)), "");
    static_assert(test_constexpr_copy_ctor_imp<1>(V(nullptr)), "");
    static_assert(test_constexpr_copy_ctor_imp<2>(V(101)), "");
}

int run_test()
{
    test_copy_ctor_basic();
    test_copy_ctor_valueless_by_exception();
    test_copy_ctor_sfinae();
    test_constexpr_copy_ctor();
    return 0;
}
} // namespace copy

namespace default_ctor {
struct NonDefaultConstructible
{
    constexpr NonDefaultConstructible(int) {}
};

struct NotNoexcept
{
    NotNoexcept() noexcept(false) {}
};

struct DefaultCtorThrows
{
    DefaultCtorThrows() { throw 42; }
};

void test_default_ctor_sfinae()
{
    {
        using V = std::variant<std::monostate, int>;
        static_assert(std::is_default_constructible<V>::value, "");
    }
    {
        using V = std::variant<NonDefaultConstructible, int>;
        static_assert(!std::is_default_constructible<V>::value, "");
    }
}

void test_default_ctor_noexcept()
{
    {
        using V = std::variant<int>;
        static_assert(std::is_nothrow_default_constructible<V>::value, "");
    }
    {
        using V = std::variant<NotNoexcept>;
        static_assert(!std::is_nothrow_default_constructible<V>::value, "");
    }
}

void test_default_ctor_throws()
{
    using V = std::variant<DefaultCtorThrows, int>;
    try
    {
        V v;
        assert(false);
    } catch (const int& ex)
    {
        assert(ex == 42);
    } catch (...)
    {
        assert(false);
    }
}

void test_default_ctor_basic()
{
    {
        std::variant<int> v;
        assert(v.index() == 0);
        assert(std::get<0>(v) == 0);
    }
    {
        std::variant<int, long> v;
        assert(v.index() == 0);
        assert(std::get<0>(v) == 0);
    }
    {
        std::variant<int, NonDefaultConstructible> v;
        assert(v.index() == 0);
        assert(std::get<0>(v) == 0);
    }
    {
        using V = std::variant<int, long>;
        constexpr V v;
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v) == 0, "");
    }
    {
        using V = std::variant<int, long>;
        constexpr V v;
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v) == 0, "");
    }
    {
        using V = std::variant<int, NonDefaultConstructible>;
        constexpr V v;
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v) == 0, "");
    }
}

int run_test()
{
    test_default_ctor_basic();
    test_default_ctor_sfinae();
    test_default_ctor_noexcept();
    test_default_ctor_throws();

    return 0;
}
} // namespace default_ctor

namespace in_place_index_args {
void test_ctor_sfinae()
{
    {
        using V = std::variant<int>;
        static_assert(std::is_constructible<V, std::in_place_index_t<0>, int>::value, "");
    }
    {
        using V = std::variant<int, long, long long>;
        static_assert(std::is_constructible<V, std::in_place_index_t<1>, int>::value, "");
    }
    {
        using V = std::variant<int, long, int*>;
        static_assert(std::is_constructible<V, std::in_place_index_t<2>, int*>::value, "");
    }
    {
        using V = std::variant<int, long, int*>;
        static_assert(!std::is_constructible<V, std::in_place_index_t<0>, int*>::value, "");
    }
    {
        using V = std::variant<int, long, int*>;
        static_assert(!std::is_constructible<V, std::in_place_index_t<3>, int>::value, "");
    }
}

void test_ctor_basic()
{
    {
        constexpr std::variant<int> v(std::in_place_index<0>, 42);
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v) == 42, "");
    }
    {
        constexpr std::variant<int, long, long> v(std::in_place_index<1>, 42);
        static_assert(v.index() == 1, "");
        static_assert(std::get<1>(v) == 42, "");
    }
    {
        constexpr std::variant<int, const int, long> v(std::in_place_index<1>, 42);
        static_assert(v.index() == 1, "");
        static_assert(std::get<1>(v) == 42, "");
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_index<0>, x);
        assert(v.index() == 0);
        assert(std::get<0>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_index<1>, x);
        assert(v.index() == 1);
        assert(std::get<1>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_index<2>, x);
        assert(v.index() == 2);
        assert(std::get<2>(v) == x);
    }
}

int run_test()
{
    test_ctor_basic();
    test_ctor_sfinae();

    return 0;
}
} // namespace in_place_index_args

namespace in_place_index_init_list_args {
struct InitList
{
    std::size_t size;
    constexpr InitList(std::initializer_list<int> il)
        : size(il.size()) {}
};

struct InitListArg
{
    std::size_t size;
    int value;
    constexpr InitListArg(std::initializer_list<int> il, int v)
        : size(il.size()), value(v) {}
};

void test_ctor_sfinae()
{
    using IL = std::initializer_list<int>;
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(std::is_constructible<V, std::in_place_index_t<0>, IL>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_index_t<0>, IL, int>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_index_t<1>, IL>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(std::is_constructible<V, std::in_place_index_t<1>, IL, int>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(
            !std::is_constructible<V, std::in_place_index_t<2>, IL>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_index_t<3>, IL>::value, "");
    }
}

void test_ctor_basic()
{
    {
        constexpr std::variant<InitList, InitListArg, InitList> v(
            std::in_place_index<0>, {1, 2, 3});
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v).size == 3, "");
    }
    {
        constexpr std::variant<InitList, InitListArg, InitList> v(
            std::in_place_index<2>, {1, 2, 3});
        static_assert(v.index() == 2, "");
        static_assert(std::get<2>(v).size == 3, "");
    }
    {
        constexpr std::variant<InitList, InitListArg, InitListArg> v(
            std::in_place_index<1>, {1, 2, 3, 4}, 42);
        static_assert(v.index() == 1, "");
        static_assert(std::get<1>(v).size == 4, "");
        static_assert(std::get<1>(v).value == 42, "");
    }
}

int run_test()
{
    test_ctor_basic();
    test_ctor_sfinae();

    return 0;
}
} // namespace in_place_index_init_list_args

namespace in_place_type_args {
void test_ctor_sfinae()
{
    {
        using V = std::variant<int>;
        static_assert(std::is_constructible<V, std::in_place_type_t<int>, int>::value, "");
    }
    {
        using V = std::variant<int, long, long long>;
        static_assert(std::is_constructible<V, std::in_place_type_t<long>, int>::value, "");
    }
    {
        using V = std::variant<int, long, int*>;
        static_assert(std::is_constructible<V, std::in_place_type_t<int*>, int*>::value, "");
    }
    {
        using V = std::variant<int, long, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<int>, int>::value, "");
    }
    {
        using V = std::variant<int, long, int*>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<int>, int*>::value, "");
    }
    {
        using V = std::variant<int, long, int*>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<long long>, int>::value, "");
    }
}

void test_ctor_basic()
{
    {
        constexpr std::variant<int> v(std::in_place_type<int>, 42);
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v) == 42, "");
    }
    {
        constexpr std::variant<int, long> v(std::in_place_type<long>, 42);
        static_assert(v.index() == 1, "");
        static_assert(std::get<1>(v) == 42, "");
    }
    {
        constexpr std::variant<int, const int, long> v(std::in_place_type<const int>, 42);
        static_assert(v.index() == 1, "");
        static_assert(std::get<1>(v) == 42, "");
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_type<const int>, x);
        assert(v.index() == 0);
        assert(std::get<0>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_type<volatile int>, x);
        assert(v.index() == 1);
        assert(std::get<1>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_type<int>, x);
        assert(v.index() == 2);
        assert(std::get<2>(v) == x);
    }
}

int run_test()
{
    test_ctor_basic();
    test_ctor_sfinae();

    return 0;
}
} // namespace in_place_type_args

namespace in_place_type_init_list_args {
struct InitList
{
    std::size_t size;
    constexpr InitList(std::initializer_list<int> il)
        : size(il.size()) {}
};

struct InitListArg
{
    std::size_t size;
    int value;
    constexpr InitListArg(std::initializer_list<int> il, int v)
        : size(il.size()), value(v) {}
};

void test_ctor_sfinae()
{
    using IL = std::initializer_list<int>;
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(std::is_constructible<V, std::in_place_type_t<InitList>, IL>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<InitList>, IL, int>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<InitListArg>, IL>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(std::is_constructible<V, std::in_place_type_t<InitListArg>, IL, int>::value, "");
    }
    {
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<int>, IL>::value, "");
    }
    {
        using V = std::variant<InitListArg, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<InitListArg>, IL, int>::value, "");
    }
}

void test_ctor_basic()
{
    {
        constexpr std::variant<InitList, InitListArg> v(std::in_place_type<InitList>, {1, 2, 3});
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v).size == 3, "");
    }
    {
        constexpr std::variant<InitList, InitListArg> v(std::in_place_type<InitListArg>, {1, 2, 3, 4}, 42);
        static_assert(v.index() == 1, "");
        static_assert(std::get<1>(v).size == 4, "");
        static_assert(std::get<1>(v).value == 42, "");
    }
}

int run_test()
{
    test_ctor_basic();
    test_ctor_sfinae();

    return 0;
}
} // namespace in_place_type_init_list_args

namespace move {
struct ThrowsMove
{
    ThrowsMove(ThrowsMove&&) noexcept(false) {}
};

struct NoCopy
{
    NoCopy(const NoCopy&) = delete;
};

struct MoveOnly
{
    int value;
    MoveOnly(int v)
        : value(v) {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&)      = default;
};

struct MoveOnlyNT
{
    int value;
    MoveOnlyNT(int v)
        : value(v) {}
    MoveOnlyNT(const MoveOnlyNT&) = delete;
    MoveOnlyNT(MoveOnlyNT&& other)
        : value(other.value) { other.value = -1; }
};

struct NTMove
{
    constexpr NTMove(int v)
        : value(v) {}
    NTMove(const NTMove&) = delete;
    NTMove(NTMove&& that)
        : value(that.value) { that.value = -1; }
    int value;
};

static_assert(!std::is_trivially_move_constructible<NTMove>::value, "");
static_assert(std::is_move_constructible<NTMove>::value, "");

struct TMove
{
    constexpr TMove(int v)
        : value(v) {}
    TMove(const TMove&) = delete;
    TMove(TMove&&)      = default;
    int value;
};

static_assert(std::is_trivially_move_constructible<TMove>::value, "");

struct TMoveNTCopy
{
    constexpr TMoveNTCopy(int v)
        : value(v) {}
    TMoveNTCopy(const TMoveNTCopy& that)
        : value(that.value) {}
    TMoveNTCopy(TMoveNTCopy&&) = default;
    int value;
};

static_assert(std::is_trivially_move_constructible<TMoveNTCopy>::value, "");

struct MakeEmptyT
{
    static int alive;
    MakeEmptyT() { ++alive; }
    MakeEmptyT(const MakeEmptyT&)
    {
        ++alive;
    }
    MakeEmptyT(MakeEmptyT&&) { throw 42; }
    MakeEmptyT& operator=(const MakeEmptyT&) { throw 42; }
    MakeEmptyT& operator=(MakeEmptyT&&) { throw 42; }
    ~MakeEmptyT() { --alive; }
};

int MakeEmptyT::alive = 0;

template <class Variant>
void makeEmpty(Variant& v)
{
    Variant v2(std::in_place_type<MakeEmptyT>);
    try
    {
        v = std::move(v2);
        assert(false);
    } catch (...)
    {
        assert(v.valueless_by_exception());
    }
}

void test_move_noexcept()
{
    {
        using V = std::variant<int, long>;
        static_assert(std::is_nothrow_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnly>;
        static_assert(std::is_nothrow_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnlyNT>;
        static_assert(!std::is_nothrow_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, ThrowsMove>;
        static_assert(!std::is_nothrow_move_constructible<V>::value, "");
    }
}

void test_move_ctor_sfinae()
{
    {
        using V = std::variant<int, long>;
        static_assert(std::is_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnly>;
        static_assert(std::is_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, MoveOnlyNT>;
        static_assert(std::is_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, NoCopy>;
        static_assert(!std::is_move_constructible<V>::value, "");
    }

    {
        using V = std::variant<int, long>;
        static_assert(std::is_trivially_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, NTMove>;
        static_assert(!std::is_trivially_move_constructible<V>::value, "");
        static_assert(std::is_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, TMove>;
        static_assert(std::is_trivially_move_constructible<V>::value, "");
    }
    {
        using V = std::variant<int, TMoveNTCopy>;
        static_assert(std::is_trivially_move_constructible<V>::value, "");
    }
}

template <typename T>
struct Result
{
    size_t index;
    T value;
};

void test_move_ctor_basic()
{
    {
        std::variant<int> v(std::in_place_index<0>, 42);
        std::variant<int> v2 = std::move(v);
        assert(v2.index() == 0);
        assert(std::get<0>(v2) == 42);
    }
    {
        std::variant<int, long> v(std::in_place_index<1>, 42);
        std::variant<int, long> v2 = std::move(v);
        assert(v2.index() == 1);
        assert(std::get<1>(v2) == 42);
    }
    {
        std::variant<MoveOnly> v(std::in_place_index<0>, 42);
        assert(v.index() == 0);
        std::variant<MoveOnly> v2(std::move(v));
        assert(v2.index() == 0);
        assert(std::get<0>(v2).value == 42);
    }
    {
        std::variant<int, MoveOnly> v(std::in_place_index<1>, 42);
        assert(v.index() == 1);
        std::variant<int, MoveOnly> v2(std::move(v));
        assert(v2.index() == 1);
        assert(std::get<1>(v2).value == 42);
    }
    {
        std::variant<MoveOnlyNT> v(std::in_place_index<0>, 42);
        assert(v.index() == 0);
        std::variant<MoveOnlyNT> v2(std::move(v));
        assert(v2.index() == 0);
        assert(std::get<0>(v).value == -1);
        assert(std::get<0>(v2).value == 42);
    }
    {
        std::variant<int, MoveOnlyNT> v(std::in_place_index<1>, 42);
        assert(v.index() == 1);
        std::variant<int, MoveOnlyNT> v2(std::move(v));
        assert(v2.index() == 1);
        assert(std::get<1>(v).value == -1);
        assert(std::get<1>(v2).value == 42);
    }

    {
        struct
        {
            constexpr Result<int> operator()() const
            {
                std::variant<int> v(std::in_place_index<0>, 42);
                std::variant<int> v2 = std::move(v);
                return {v2.index(), std::get<0>(std::move(v2))};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 0, "");
        static_assert(result.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<long> operator()() const
            {
                std::variant<int, long> v(std::in_place_index<1>, 42);
                std::variant<int, long> v2 = std::move(v);
                return {v2.index(), std::get<1>(std::move(v2))};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<TMove> operator()() const
            {
                std::variant<TMove> v(std::in_place_index<0>, 42);
                std::variant<TMove> v2(std::move(v));
                return {v2.index(), std::get<0>(std::move(v2))};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 0, "");
        static_assert(result.value.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<TMove> operator()() const
            {
                std::variant<int, TMove> v(std::in_place_index<1>, 42);
                std::variant<int, TMove> v2(std::move(v));
                return {v2.index(), std::get<1>(std::move(v2))};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<TMoveNTCopy> operator()() const
            {
                std::variant<TMoveNTCopy> v(std::in_place_index<0>, 42);
                std::variant<TMoveNTCopy> v2(std::move(v));
                return {v2.index(), std::get<0>(std::move(v2))};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 0, "");
        static_assert(result.value.value == 42, "");
    }
    {
        struct
        {
            constexpr Result<TMoveNTCopy> operator()() const
            {
                std::variant<int, TMoveNTCopy> v(std::in_place_index<1>, 42);
                std::variant<int, TMoveNTCopy> v2(std::move(v));
                return {v2.index(), std::get<1>(std::move(v2))};
            }
        } test;
        constexpr auto result = test();
        static_assert(result.index == 1, "");
        static_assert(result.value.value == 42, "");
    }
}

void test_move_ctor_valueless_by_exception()
{
    using V = std::variant<int, MakeEmptyT>;
    V v1;
    makeEmpty(v1);
    V v(std::move(v1));
    assert(v.valueless_by_exception());
}

template <size_t Idx>
constexpr bool test_constexpr_ctor_imp(std::variant<long, void*, const int> const& v)
{
    auto copy = v;
    auto v2   = std::move(copy);
    return v2.index() == v.index() && v2.index() == Idx && std::get<Idx>(v2) == std::get<Idx>(v);
}

void test_constexpr_move_ctor()
{
    using V = std::variant<long, void*, const int>;
    static_assert(std::is_trivially_destructible<V>::value, "");
    static_assert(std::is_trivially_copy_constructible<V>::value, "");
    static_assert(std::is_trivially_move_constructible<V>::value, "");
    static_assert(!std::is_copy_assignable<V>::value, "");
    static_assert(!std::is_move_assignable<V>::value, "");
    static_assert(std::is_trivially_copyable<V>::value, "");
    static_assert(std::is_trivially_move_constructible<V>::value, "");
    static_assert(test_constexpr_ctor_imp<0>(V(42l)), "");
    static_assert(test_constexpr_ctor_imp<1>(V(nullptr)), "");
    static_assert(test_constexpr_ctor_imp<2>(V(101)), "");
}

int run_test()
{
    test_move_ctor_basic();
    test_move_ctor_valueless_by_exception();
    test_move_noexcept();
    test_move_ctor_sfinae();
    test_constexpr_move_ctor();

    return 0;
}
} // namespace ctor::move

namespace T {
struct Dummy
{
    Dummy() = default;
};

struct ThrowsT
{
    ThrowsT(int) noexcept(false) {}
};

struct NoThrowT
{
    NoThrowT(int) noexcept(true) {}
};

struct AnyConstructible
{
    template <typename T>
    AnyConstructible(T&&)
    {
    }
};
struct NoConstructible
{
    NoConstructible() = delete;
};
template <class T>
struct RValueConvertibleFrom
{
    RValueConvertibleFrom(T&&) {}
};

void test_T_ctor_noexcept()
{
    {
        using V = std::variant<Dummy, NoThrowT>;
        static_assert(std::is_nothrow_constructible<V, int>::value, "");
    }
    {
        using V = std::variant<Dummy, ThrowsT>;
        static_assert(!std::is_nothrow_constructible<V, int>::value, "");
    }
}

void test_T_ctor_sfinae()
{
    {
        using V = std::variant<long, long long>;
        static_assert(!std::is_constructible<V, int>::value, "ambiguous");
    }
    {
        using V = std::variant<std::string, std::string>;
        static_assert(!std::is_constructible<V, const char*>::value, "ambiguous");
    }
    {
        using V = std::variant<std::string, void*>;
        static_assert(!std::is_constructible<V, int>::value,
                      "no matching constructor");
    }
    {
        struct X
        {
        };
        struct Y
        {
            operator X();
        };
        using V = std::variant<X>;
        static_assert(std::is_constructible<V, Y>::value,
                      "regression on user-defined conversions in constructor");
    }
    {
        using V = std::variant<AnyConstructible, NoConstructible>;
        static_assert(
            !std::is_constructible<V, std::in_place_type_t<NoConstructible>>::value,
            "no matching constructor");
        static_assert(!std::is_constructible<V, std::in_place_index_t<1>>::value,
                      "no matching constructor");
    }
}

void test_T_ctor_basic()
{
    {
        constexpr std::variant<int> v(42);
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v) == 42, "");
    }
    {
        constexpr std::variant<int, long> v(42l);
        static_assert(v.index() == 1, "");
        static_assert(std::get<1>(v) == 42, "");
    }
    {
        std::variant<bool volatile const, int> v = true;
        assert(v.index() == 0);
        assert(std::get<0>(v));
    }
    {
        std::variant<RValueConvertibleFrom<int>> v1 = 42;
        assert(v1.index() == 0);

        int x                                                         = 42;
        std::variant<RValueConvertibleFrom<int>, AnyConstructible> v2 = x;
        assert(v2.index() == 1);
    }
}

struct FailOnAnything
{
    template <class T>
    constexpr FailOnAnything(T)
    {
        static_assert(!std::is_same<T, T>::value, "");
    }
};

void test_no_narrowing_check_for_class_types()
{
    using V = std::variant<int, FailOnAnything>;
    V v(42);
    assert(v.index() == 0);
    assert(std::get<0>(v) == 42);
}

struct Bar
{
};
struct Baz
{
};
void test_construction_with_repeated_types()
{
    using V = std::variant<int, Bar, Baz, int, Baz, int, int>;
    static_assert(!std::is_constructible<V, int>::value, "");
    static_assert(!std::is_constructible<V, Baz>::value, "");
    static_assert(std::is_constructible<V, Bar>::value, "");
}

int run_test()
{
    test_T_ctor_basic();
    test_T_ctor_noexcept();
    test_T_ctor_sfinae();
    test_no_narrowing_check_for_class_types();
    test_construction_with_repeated_types();
    return 0;
}
} // namespace ctor::T
} // namespace ctor

namespace dtor {
struct NonTDtor
{
    static int count;
    NonTDtor() = default;
    ~NonTDtor() { ++count; }
};
int NonTDtor::count = 0;
static_assert(!std::is_trivially_destructible<NonTDtor>::value, "");

struct NonTDtor1
{
    static int count;
    NonTDtor1() = default;
    ~NonTDtor1() { ++count; }
};
int NonTDtor1::count = 0;
static_assert(!std::is_trivially_destructible<NonTDtor1>::value, "");

struct TDtor
{
    TDtor(const TDtor&) {}
    ~TDtor() = default;
};
static_assert(!std::is_trivially_copy_constructible<TDtor>::value, "");
static_assert(std::is_trivially_destructible<TDtor>::value, "");

int run_test()
{
    {
        using V = std::variant<int, long, TDtor>;
        static_assert(std::is_trivially_destructible<V>::value, "");
    }
    {
        using V = std::variant<NonTDtor, int, NonTDtor1>;
        static_assert(!std::is_trivially_destructible<V>::value, "");
        {
            V v(std::in_place_index<0>);
            assert(NonTDtor::count == 0);
            assert(NonTDtor1::count == 0);
        }
        assert(NonTDtor::count == 1);
        assert(NonTDtor1::count == 0);
        NonTDtor::count = 0;
        {
            V v(std::in_place_index<1>);
        }
        assert(NonTDtor::count == 0);
        assert(NonTDtor1::count == 0);
        {
            V v(std::in_place_index<2>);
            assert(NonTDtor::count == 0);
            assert(NonTDtor1::count == 0);
        }
        assert(NonTDtor::count == 0);
        assert(NonTDtor1::count == 1);
    }

    return 0;
}
} // namespace dtor

namespace emplace {
namespace index {
template <class Var, size_t I, class... Args>
constexpr auto test_emplace_exists_imp(int) -> decltype(std::declval<Var>().template emplace<I>(std::declval<Args>()...), true)
{
    return true;
}

template <class, size_t, class...>
constexpr auto test_emplace_exists_imp(long) -> bool
{
    return false;
}

template <class Var, size_t I, class... Args>
constexpr bool emplace_exists()
{
    return test_emplace_exists_imp<Var, I, Args...>(0);
}

void test_emplace_sfinae()
{
    {
        using V = std::variant<int, void*, const void*>;
        static_assert(emplace_exists<V, 0>(), "");
        static_assert(emplace_exists<V, 0, int>(), "");
        static_assert(!emplace_exists<V, 0, decltype(nullptr)>(),
                      "cannot construct");
        static_assert(emplace_exists<V, 1, decltype(nullptr)>(), "");
        static_assert(emplace_exists<V, 1, int*>(), "");
        static_assert(!emplace_exists<V, 1, const int*>(), "");
        static_assert(!emplace_exists<V, 1, int>(), "cannot construct");
        static_assert(emplace_exists<V, 2, const int*>(), "");
        static_assert(emplace_exists<V, 2, int*>(), "");
        static_assert(!emplace_exists<V, 3>(), "cannot construct");
    }
}

void test_basic()
{
    {
        using V = std::variant<int>;
        V v(42);
        auto& ref1 = v.emplace<0>();
        static_assert(std::is_same_v<int&, decltype(ref1)>, "");
        assert(std::get<0>(v) == 0);
        assert(&ref1 == &std::get<0>(v));
        auto& ref2 = v.emplace<0>(42);
        static_assert(std::is_same_v<int&, decltype(ref2)>, "");
        assert(std::get<0>(v) == 42);
        assert(&ref2 == &std::get<0>(v));
    }
}

int run_test()
{
    test_basic();
    test_emplace_sfinae();
    return 0;
}
}

namespace index_init_list_args {
struct InitList
{
    std::size_t size;
    constexpr InitList(std::initializer_list<int> il)
        : size(il.size()) {}
};

struct InitListArg
{
    std::size_t size;
    int value;
    constexpr InitListArg(std::initializer_list<int> il, int v)
        : size(il.size()), value(v) {}
};

template <class Var, size_t I, class... Args>
constexpr auto test_emplace_exists_imp(int) -> decltype(std::declval<Var>().template emplace<I>(std::declval<Args>()...), true)
{
    return true;
}

template <class, size_t, class...>
constexpr auto test_emplace_exists_imp(long) -> bool
{
    return false;
}

template <class Var, size_t I, class... Args>
constexpr bool emplace_exists()
{
    return test_emplace_exists_imp<Var, I, Args...>(0);
}

void test_basic()
{
    using V = std::variant<int, InitList, InitListArg>;
    V v;
    auto& ref1 = v.emplace<1>({1, 2, 3});
    static_assert(std::is_same_v<InitList&, decltype(ref1)>, "");
    assert(std::get<1>(v).size == 3);
    assert(&ref1 == &std::get<1>(v));
    auto& ref2 = v.emplace<2>({1, 2, 3, 4}, 42);
    static_assert(std::is_same_v<InitListArg&, decltype(ref2)>, "");
    assert(std::get<2>(v).size == 4);
    assert(std::get<2>(v).value == 42);
    assert(&ref2 == &std::get<2>(v));
    auto& ref3 = v.emplace<1>({1});
    static_assert(std::is_same_v<InitList&, decltype(ref3)>, "");
    assert(std::get<1>(v).size == 1);
    assert(&ref3 == &std::get<1>(v));
}

int run_test()
{
    test_basic();
    return 0;
}
} // namespace emplace::index_init_list_args
}

int run_all_test()
{
    bad_variant_access::run_test();
    get_if::index::run_test();
    get_if::type::run_test();
    get::index::run_test();
    get::type::run_test();
    holds_alternative::run_test();
    hash::run_test();
    helpers::variant_alternative::run_test();
    helpers::variant_size::run_test();
    monostate::properties::run_test();
    monostate::relops::run_test();
    relops::run_test();
    npos::run_test();
    assign::conv::run_test();
    assign::copy::run_test();
    assign::move::run_test();
    assign::T::run_test();
    ctor::conv::run_test();
    ctor::copy::run_test();
    ctor::default_ctor::run_test();
    ctor::in_place_index_args::run_test();
    ctor::in_place_index_init_list_args::run_test();
    ctor::in_place_type_args::run_test();
    ctor::in_place_type_init_list_args::run_test();
    ctor::move::run_test();
    ctor::T::run_test();
    dtor::run_test();

    emplace::index::run_test();
    emplace::index_init_list_args::run_test();

    
    return 0;
}

} // namespace VariantTest