#include "base/Variant.h"
#include "googletest/gtest/gtest.h"

#include <cassert>
#include <chrono>
#include <exception>
#include <functional>
#include <string>
#include <type_traits>
#define ASSERT_NOT_NOEXCEPT(v)          static_assert(!noexcept(v), "must be not noexcept");
#define ASSERT_NOEXCEPT(v)              static_assert(noexcept(v), "must be noexcept");
#define ASSERT_SAME_TYPE(t1, t2)        static_assert(std::is_same_v<t1, t2>, "must be same type");
#define LIBCPP_STATIC_ASSERT(expr, msg) static_assert(!expr, msg)

#define TestAssert(expr)                EXPECT_EQ(expr, true);

TEST(variant_test, bad_variant_access)
{
    static_assert(std::is_base_of<std::exception, std::bad_variant_access>::value, "");
    static_assert(noexcept(std::bad_variant_access{}), "must be noexcept");
    static_assert(noexcept(std::bad_variant_access{}.what()), "must be noexcept");
    std::bad_variant_access ex;
    EXPECT_EQ(ex.what() != nullptr, true);
}

TEST(variant_test, test_const_get_if_index)
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

TEST(variant_test, test_get_if_index)
{
    {
        using V = std::variant<int>;
        V* v    = nullptr;
        TestAssert(std::get_if<0>(v) == nullptr);
    }
    {
        using V = std::variant<int, long>;
        V v(42);
        ASSERT_NOEXCEPT(std::get_if<0>(&v));
        ASSERT_SAME_TYPE(decltype(std::get_if<0>(&v)), int*);
        TestAssert(*std::get_if<0>(&v) == 42);
        TestAssert(std::get_if<1>(&v) == nullptr);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get_if<1>(&v)), const long*);
        TestAssert(*std::get_if<1>(&v) == 42);
        TestAssert(std::get_if<0>(&v) == nullptr);
    }
}

TEST(variant_test, test_const_get_if_type)
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

TEST(variant_test, test_get_if_type)
{
    {
        using V = std::variant<int>;
        V* v    = nullptr;
        TestAssert(std::get_if<int>(v) == nullptr);
    }
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOEXCEPT(std::get_if<int>(&v));
        ASSERT_SAME_TYPE(decltype(std::get_if<int>(&v)), int*);
        TestAssert(*std::get_if<int>(&v) == 42);
        TestAssert(std::get_if<const long>(&v) == nullptr);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get_if<const long>(&v)), const long*);
        TestAssert(*std::get_if<const long>(&v) == 42);
        TestAssert(std::get_if<int>(&v) == nullptr);
    }
}

TEST(variant_test, test_const_lvalue_get_index)
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
        TestAssert(std::get<0>(v) == 42);
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
        TestAssert(std::get<1>(v) == 42);
    }
}

TEST(variant_test, test_lvalue_get_index)
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(v));
        ASSERT_SAME_TYPE(decltype(std::get<0>(v)), int&);
        TestAssert(std::get<0>(v) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<1>(v)), const long&);
        TestAssert(std::get<1>(v) == 42);
    }
}

TEST(variant_test, test_rvalue_get_index)
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<0>(std::move(v))), int&&);
        TestAssert(std::get<0>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<1>(std::move(v))), const long&&);
        TestAssert(std::get<1>(std::move(v)) == 42);
    }
}

TEST(variant_test, test_const_rvalue_get_index)
{
    {
        using V = std::variant<int, const long>;
        const V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<0>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<0>(std::move(v))), const int&&);
        TestAssert(std::get<0>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        const V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<1>(std::move(v))), const long&&);
        TestAssert(std::get<1>(std::move(v)) == 42);
    }
}

TEST(variant_test, test_const_lvalue_get_type)
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
        TestAssert(std::get<int>(v) == 42);
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
        TestAssert(std::get<const long>(v) == 42);
    }
}

TEST(variant_test, test_lvalue_get_type)
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(v));
        ASSERT_SAME_TYPE(decltype(std::get<int>(v)), int&);
        TestAssert(std::get<int>(v) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<const long>(v)), const long&);
        TestAssert(std::get<const long>(v) == 42);
    }
}
TEST(variant_test, test_rvalue_get_type)
{
    {
        using V = std::variant<int, const long>;
        V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<int>(std::move(v))), int&&);
        TestAssert(std::get<int>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<const long>(std::move(v))),
                         const long&&);
        TestAssert(std::get<const long>(std::move(v)) == 42);
    }
}
TEST(variant_test, test_const_rvalue_get_type)
{
    {
        using V = std::variant<int, const long>;
        const V v(42);
        ASSERT_NOT_NOEXCEPT(std::get<int>(std::move(v)));
        ASSERT_SAME_TYPE(decltype(std::get<int>(std::move(v))), const int&&);
        TestAssert(std::get<int>(std::move(v)) == 42);
    }
    {
        using V = std::variant<int, const long>;
        const V v(42l);
        ASSERT_SAME_TYPE(decltype(std::get<const long>(std::move(v))),
                         const long&&);
        TestAssert(std::get<const long>(std::move(v)) == 42);
    }
}

TEST(variant_test, test_holds_alternative)
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
}

TEST(variant_test, test_hash_variant)
{
    {
        using V = std::variant<int, long, int>;
        using H = std::hash<V>;
        const V v(std::in_place_index<0>, 42);
        const V v_copy = v;
        V v2(std::in_place_index<0>, 100);
        const H h{};
        TestAssert(h(v) == h(v));
        TestAssert(h(v) != h(v2));
        TestAssert(h(v) == h(v_copy));
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
        TestAssert(h(v0) == h(v0));
        TestAssert(h(v0) == h(v0_other));
        TestAssert(h(v1) == h(v1));
        TestAssert(h(v1) != h(v1_other));
        TestAssert(h(v2) == h(v2));
        TestAssert(h(v2) != h(v2_other));
        TestAssert(h(v3) == h(v3));
        TestAssert(h(v3) != h(v3_other));
        TestAssert(h(v0) != h(v1));
        TestAssert(h(v0) != h(v2));
        TestAssert(h(v0) != h(v3));
        TestAssert(h(v1) != h(v2));
        TestAssert(h(v1) != h(v3));
        TestAssert(h(v2) != h(v3));
    }
}

TEST(variant_test, test_hash_monostate)
{
    using H = std::hash<std::monostate>;
    const H h{};
    std::monostate m1{};
    const std::monostate m2{};
    TestAssert(h(m1) == h(m1));
    TestAssert(h(m2) == h(m2));
    TestAssert(h(m1) == h(m2));
    {
        ASSERT_SAME_TYPE(decltype(h(m1)), std::size_t);
        ASSERT_NOEXCEPT(h(m1));
        static_assert(std::is_copy_constructible<H>::value, "");
    }
}

TEST(variant_test, test_hash_variant_duplicate_elements)
{
    using V = std::variant<std::monostate, std::monostate>;
    using H = std::hash<V>;
    H h{};
    const V v1(std::in_place_index<0>);
    const V v2(std::in_place_index<1>);
    TestAssert(h(v1) == h(v1));
    TestAssert(h(v2) == h(v2));
}

template <class V, size_t I, class E>
void test_helpers_variant_alternative_impl()
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

TEST(variant_test, test_helpers_variant_alternative)
{
    using V = std::variant<int, void*, const void*, long double>;
    test_helpers_variant_alternative_impl<V, 0, int>();
    test_helpers_variant_alternative_impl<V, 1, void*>();
    test_helpers_variant_alternative_impl<V, 2, const void*>();
    test_helpers_variant_alternative_impl<V, 3, long double>();
}

template <class V, size_t E>
void test_helpers_variant_size_impl()
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
TEST(variant_test, test_helpers_variant_size)
{
    test_helpers_variant_size_impl<std::variant<>, 0>();
    test_helpers_variant_size_impl<std::variant<void*>, 1>();
    test_helpers_variant_size_impl<std::variant<long, long, void*, double>, 4>();
}

TEST(variant_test, test_monostate_properties)
{
    using M = std::monostate;
    static_assert(std::is_trivially_default_constructible<M>::value, "");
    static_assert(std::is_trivially_copy_constructible<M>::value, "");
    static_assert(std::is_trivially_copy_assignable<M>::value, "");
    static_assert(std::is_trivially_destructible<M>::value, "");
    constexpr M m{};
    ((void)m);
}

TEST(variant_test, test_monostate_relops)
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
}

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

} // namespace relops
TEST(variant_test, test_relops)
{
    using namespace relops;
    test_equality_basic<int, long>();
    test_equality_basic<ComparesToMyBool, int>();
    test_equality_basic<int, ComparesToMyBool>();
    test_equality_basic<ComparesToMyBool, ComparesToMyBool>();

    test_relational_basic<int, long>();
    test_relational_basic<ComparesToMyBool, int>();
    test_relational_basic<int, ComparesToMyBool>();
    test_relational_basic<ComparesToMyBool, ComparesToMyBool>();
}
TEST(variant_test, test_npos)
{
    static_assert(std::variant_npos == static_cast<std::size_t>(-1), "");
}
TEST(variant_test, test_assign_conv)
{
    static_assert(!std::is_assignable<std::variant<int, int>, int>::value, "");
    static_assert(!std::is_assignable<std::variant<long, long long>, int>::value, "");
}
namespace assign_copy {
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
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 0);
        TestAssert(std::get<0>(v1) == 42);
    }
    {
        using V = std::variant<int, long, unsigned>;
        V v1(43l);
        V v2(42l);
        V& vref = (v1 = v2);
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, CopyAssign, unsigned>;

        V v1(std::in_place_type<CopyAssign>, 43);
        V v2(std::in_place_type<CopyAssign>, 42);
        CopyAssign::reset();
        V& vref = (v1 = v2);
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1).value == 42);
        TestAssert(CopyAssign::copy_construct == 0);
        TestAssert(CopyAssign::move_construct == 0);
        TestAssert(CopyAssign::copy_assign == 1);
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
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, CopyAssign, unsigned>;
        CopyAssign::reset();
        V v1(std::in_place_type<unsigned>, 43u);
        V v2(std::in_place_type<CopyAssign>, 42);
        TestAssert(CopyAssign::copy_construct == 0);
        TestAssert(CopyAssign::move_construct == 0);
        TestAssert(CopyAssign::alive == 1);
        V& vref = (v1 = v2);
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1).value == 42);
        TestAssert(CopyAssign::alive == 2);
        TestAssert(CopyAssign::copy_construct == 1);
        TestAssert(CopyAssign::move_construct == 1);
        TestAssert(CopyAssign::copy_assign == 0);
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
} // namespace assign_copy

TEST(variant_test, test_assign_copy)
{
    using namespace assign_copy;
    test_copy_assignment_same_index();
    test_copy_assignment_different_index();
    test_copy_assignment_sfinae();
    test_copy_assignment_not_noexcept();
    test_constexpr_copy_assignment();
}

namespace assign_move {
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
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 0);
        TestAssert(std::get<0>(v1) == 42);
    }
    {
        using V = std::variant<int, long, unsigned>;
        V v1(43l);
        V v2(42l);
        V& vref = (v1 = std::move(v2));
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, MoveAssign, unsigned>;
        V v1(std::in_place_type<MoveAssign>, 43);
        V v2(std::in_place_type<MoveAssign>, 42);
        MoveAssign::reset();
        V& vref = (v1 = std::move(v2));
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1).value == 42);
        TestAssert(MoveAssign::move_construct == 0);
        TestAssert(MoveAssign::move_assign == 1);
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
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1) == 42);
    }
    {
        using V = std::variant<int, MoveAssign, unsigned>;
        V v1(std::in_place_type<unsigned>, 43u);
        V v2(std::in_place_type<MoveAssign>, 42);
        MoveAssign::reset();
        V& vref = (v1 = std::move(v2));
        TestAssert(&vref == &v1);
        TestAssert(v1.index() == 1);
        TestAssert(std::get<1>(v1).value == 42);
        TestAssert(MoveAssign::move_construct == 1);
        TestAssert(MoveAssign::move_assign == 0);
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
} // namespace assign_move

TEST(variant_test, test_assign_move)
{
    using namespace assign_move;
    test_move_assignment_same_index();
    test_move_assignment_different_index();
    test_move_assignment_sfinae();
    test_move_assignment_noexcept();
    test_constexpr_move_assignment();
}

namespace template_test {
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
    MoveCrashes(MoveCrashes&&) noexcept { TestAssert(false); }
    MoveCrashes& operator=(MoveCrashes&&) noexcept
    {
        TestAssert(false);
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
    ThrowsCtorTandMove(ThrowsCtorTandMove&&) noexcept(false) { TestAssert(false); }
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
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == 42);
    }
    {
        std::variant<int, long> v(43l);
        v = 42;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == 42);
        v = 43l;
        TestAssert(v.index() == 1);
        TestAssert(std::get<1>(v) == 43);
    }

    {
        std::variant<bool volatile, int> v = 42;
        v                                  = false;
        TestAssert(v.index() == 0);
        TestAssert(!std::get<0>(v));
        bool lvt = true;
        v        = lvt;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v));
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
            TestAssert(false);
        } catch (...)
        { /* ... */
        }
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == "hello");
    }
    {
        using V = std::variant<ThrowsAssignT, std::string>;
        V v(std::in_place_type<std::string>, "hello");
        v = 42;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v).value == 42);
    }
}

void test_T_assignment_performs_assignment()
{
    using namespace RuntimeHelpers;
    {
        using V = std::variant<ThrowsCtorT>;
        V v;
        v = 42;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v).value == 42);
    }
    {
        using V = std::variant<ThrowsCtorT, std::string>;
        V v;
        v = 42;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v).value == 42);
    }
    {
        using V = std::variant<ThrowsAssignT>;
        V v(100);
        try
        {
            v = 42;
            TestAssert(false);
        } catch (...)
        { /* ... */
        }
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v).value == 100);
    }
    {
        using V = std::variant<std::string, ThrowsAssignT>;
        V v(100);
        try
        {
            v = 42;
            TestAssert(false);
        } catch (...)
        { /* ... */
        }
        TestAssert(v.index() == 1);
        TestAssert(std::get<1>(v).value == 100);
    }
}
} // namespace template_test

TEST(variant_test, test_template_test)
{
    using namespace template_test;
    test_T_assignment_basic();
    test_T_assignment_performs_construction();
    test_T_assignment_performs_assignment();
    test_T_assignment_noexcept();
    test_T_assignment_sfinae();
}

TEST(variant_test, test_ctor_conv)
{
    static_assert(!std::is_constructible<std::variant<int, int>, int>::value, "");
    static_assert(!std::is_constructible<std::variant<long, long long>, int>::value, "");
}

namespace ctor_copy {

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
        TestAssert(false);
    } catch (...)
    {
        TestAssert(v.valueless_by_exception());
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
        TestAssert(v2.index() == 0);
        TestAssert(std::get<0>(v2) == 42);
    }
    {
        std::variant<int, long> v(std::in_place_index<1>, 42);
        std::variant<int, long> v2 = v;
        TestAssert(v2.index() == 1);
        TestAssert(std::get<1>(v2) == 42);
    }
    {
        std::variant<NonT> v(std::in_place_index<0>, 42);
        TestAssert(v.index() == 0);
        std::variant<NonT> v2(v);
        TestAssert(v2.index() == 0);
        TestAssert(std::get<0>(v2).value == 42);
    }
    {
        std::variant<int, NonT> v(std::in_place_index<1>, 42);
        TestAssert(v.index() == 1);
        std::variant<int, NonT> v2(v);
        TestAssert(v2.index() == 1);
        TestAssert(std::get<1>(v2).value == 42);
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
    TestAssert(v.valueless_by_exception());
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
} // namespace ctor_copy

TEST(variant_test, test_ctor_copy)
{
    using namespace ctor_copy;
    test_copy_ctor_basic();
    test_copy_ctor_valueless_by_exception();
    test_copy_ctor_sfinae();
    test_constexpr_copy_ctor();
}

namespace ctor_default_ctor {
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
        TestAssert(false);
    } catch (const int& ex)
    {
        TestAssert(ex == 42);
    } catch (...)
    {
        TestAssert(false);
    }
}

void test_default_ctor_basic()
{
    {
        std::variant<int> v;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == 0);
    }
    {
        std::variant<int, long> v;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == 0);
    }
    {
        std::variant<int, NonDefaultConstructible> v;
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == 0);
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

} // namespace ctor_default_ctor

TEST(variant_test, test_ctor_default_ctor)
{
    using namespace ctor_default_ctor;
    test_default_ctor_basic();
    test_default_ctor_sfinae();
    test_default_ctor_noexcept();
    test_default_ctor_throws();
}

namespace ctor_in_place_index_args {
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
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_index<1>, x);
        TestAssert(v.index() == 1);
        TestAssert(std::get<1>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_index<2>, x);
        TestAssert(v.index() == 2);
        TestAssert(std::get<2>(v) == x);
    }
}
} // namespace ctor_in_place_index_args

TEST(variant_test, test_ctor_in_place_index_args)
{
    using namespace ctor_in_place_index_args;
    test_ctor_basic();
    test_ctor_sfinae();
}

namespace ctor_in_place_index_init_list_args {
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
} // namespace ctor_in_place_index_init_list_args

TEST(variant_test, test_ctor_in_place_index_init_list_args)
{
    using namespace ctor_in_place_index_init_list_args;
    test_ctor_basic();
    test_ctor_sfinae();
}

namespace ctor_in_place_type_args {
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
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_type<volatile int>, x);
        TestAssert(v.index() == 1);
        TestAssert(std::get<1>(v) == x);
    }
    {
        using V = std::variant<const int, volatile int, int>;
        int x   = 42;
        V v(std::in_place_type<int>, x);
        TestAssert(v.index() == 2);
        TestAssert(std::get<2>(v) == x);
    }
}

} // namespace ctor_in_place_type_args

TEST(variant_test, test_ctor_in_place_type_args)
{
    using namespace ctor_in_place_type_args;
    test_ctor_basic();
    test_ctor_sfinae();
}

namespace ctor_in_place_type_init_list_args {
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

} // namespace ctor_in_place_type_init_list_args

TEST(variant_test, test_in_place_type_init_list_args)
{
    using namespace ctor_in_place_type_init_list_args;
    test_ctor_basic();
    test_ctor_sfinae();
}

namespace ctor_move {
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
        TestAssert(false);
    } catch (...)
    {
        TestAssert(v.valueless_by_exception());
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
        TestAssert(v2.index() == 0);
        TestAssert(std::get<0>(v2) == 42);
    }
    {
        std::variant<int, long> v(std::in_place_index<1>, 42);
        std::variant<int, long> v2 = std::move(v);
        TestAssert(v2.index() == 1);
        TestAssert(std::get<1>(v2) == 42);
    }
    {
        std::variant<MoveOnly> v(std::in_place_index<0>, 42);
        TestAssert(v.index() == 0);
        std::variant<MoveOnly> v2(std::move(v));
        TestAssert(v2.index() == 0);
        TestAssert(std::get<0>(v2).value == 42);
    }
    {
        std::variant<int, MoveOnly> v(std::in_place_index<1>, 42);
        TestAssert(v.index() == 1);
        std::variant<int, MoveOnly> v2(std::move(v));
        TestAssert(v2.index() == 1);
        TestAssert(std::get<1>(v2).value == 42);
    }
    {
        std::variant<MoveOnlyNT> v(std::in_place_index<0>, 42);
        TestAssert(v.index() == 0);
        std::variant<MoveOnlyNT> v2(std::move(v));
        TestAssert(v2.index() == 0);
        TestAssert(std::get<0>(v).value == -1);
        TestAssert(std::get<0>(v2).value == 42);
    }
    {
        std::variant<int, MoveOnlyNT> v(std::in_place_index<1>, 42);
        TestAssert(v.index() == 1);
        std::variant<int, MoveOnlyNT> v2(std::move(v));
        TestAssert(v2.index() == 1);
        TestAssert(std::get<1>(v).value == -1);
        TestAssert(std::get<1>(v2).value == 42);
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
    TestAssert(v.valueless_by_exception());
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
} // namespace ctor_move

TEST(variant_test, test_ctor_move)
{
    using namespace ctor_move;
    run_test();
}
namespace ctor_T {
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
        TestAssert(v.index() == 0);
        TestAssert(std::get<0>(v));
    }
    {
        std::variant<RValueConvertibleFrom<int>> v1 = 42;
        TestAssert(v1.index() == 0);

        int x                                                         = 42;
        std::variant<RValueConvertibleFrom<int>, AnyConstructible> v2 = x;
        TestAssert(v2.index() == 1);
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
    TestAssert(v.index() == 0);
    TestAssert(std::get<0>(v) == 42);
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
} // namespace ctor_T

TEST(variant_test, test_ctor_T)
{
    using namespace ctor_T;
    run_test();
}

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
            TestAssert(NonTDtor::count == 0);
            TestAssert(NonTDtor1::count == 0);
        }
        TestAssert(NonTDtor::count == 1);
        TestAssert(NonTDtor1::count == 0);
        NonTDtor::count = 0;
        {
            V v(std::in_place_index<1>);
        }
        TestAssert(NonTDtor::count == 0);
        TestAssert(NonTDtor1::count == 0);
        {
            V v(std::in_place_index<2>);
            TestAssert(NonTDtor::count == 0);
            TestAssert(NonTDtor1::count == 0);
        }
        TestAssert(NonTDtor::count == 0);
        TestAssert(NonTDtor1::count == 1);
    }

    return 0;
}
} // namespace dtor

TEST(variant_test, test_dtor)
{
    using namespace dtor;
    run_test();
}

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
        TestAssert(std::get<0>(v) == 0);
        TestAssert(&ref1 == &std::get<0>(v));
        auto& ref2 = v.emplace<0>(42);
        static_assert(std::is_same_v<int&, decltype(ref2)>, "");
        TestAssert(std::get<0>(v) == 42);
        TestAssert(&ref2 == &std::get<0>(v));
    }
}

int run_test()
{
    test_basic();
    test_emplace_sfinae();
    return 0;
}
} // namespace index

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
    TestAssert(std::get<1>(v).size == 3);
    TestAssert(&ref1 == &std::get<1>(v));
    auto& ref2 = v.emplace<2>({1, 2, 3, 4}, 42);
    static_assert(std::is_same_v<InitListArg&, decltype(ref2)>, "");
    TestAssert(std::get<2>(v).size == 4);
    TestAssert(std::get<2>(v).value == 42);
    TestAssert(&ref2 == &std::get<2>(v));
    auto& ref3 = v.emplace<1>({1});
    static_assert(std::is_same_v<InitList&, decltype(ref3)>, "");
    TestAssert(std::get<1>(v).size == 1);
    TestAssert(&ref3 == &std::get<1>(v));
}

int run_test()
{
    test_basic();
    return 0;
}
} // namespace index_init_list_args

namespace type_args {
template <class Var, class T, class... Args>
constexpr auto test_emplace_exists_imp(int) -> decltype(std::declval<Var>().template emplace<T>(std::declval<Args>()...), true)
{
    return true;
}

template <class, class, class...>
constexpr auto test_emplace_exists_imp(long) -> bool
{
    return false;
}

template <class... Args>
constexpr bool emplace_exists()
{
    return test_emplace_exists_imp<Args...>(0);
}

void test_emplace_sfinae()
{
    {
        using V = std::variant<int, void*, const void*>;
        static_assert(emplace_exists<V, int>(), "");
        static_assert(emplace_exists<V, int, int>(), "");
        static_assert(!emplace_exists<V, int, decltype(nullptr)>(),
                      "cannot construct");
        static_assert(emplace_exists<V, void*, decltype(nullptr)>(), "");
        static_assert(!emplace_exists<V, void*, int>(), "cannot construct");
        static_assert(emplace_exists<V, void*, int*>(), "");
        static_assert(!emplace_exists<V, void*, const int*>(), "");
        static_assert(emplace_exists<V, const void*, const int*>(), "");
        static_assert(emplace_exists<V, const void*, int*>(), "");
    }
}

void test_basic()
{
    {
        using V = std::variant<int>;
        V v(42);
        auto& ref1 = v.emplace<int>();
        static_assert(std::is_same_v<int&, decltype(ref1)>, "");
        TestAssert(std::get<0>(v) == 0);
        TestAssert(&ref1 == &std::get<0>(v));
        auto& ref2 = v.emplace<int>(42);
        static_assert(std::is_same_v<int&, decltype(ref2)>, "");
        TestAssert(std::get<0>(v) == 42);
        TestAssert(&ref2 == &std::get<0>(v));
    }
    {
        using V     = std::variant<int, long, const void*, std::string>;
        const int x = 100;
        V v(std::in_place_type<int>, -1);
        auto& ref1 = v.emplace<long>();
        static_assert(std::is_same_v<long&, decltype(ref1)>, "");
        TestAssert(std::get<1>(v) == 0);
        TestAssert(&ref1 == &std::get<1>(v));
        auto& ref2 = v.emplace<const void*>(&x);
        static_assert(std::is_same_v<const void*&, decltype(ref2)>, "");
        TestAssert(std::get<2>(v) == &x);
        TestAssert(&ref2 == &std::get<2>(v));
        // emplace with multiple args
        auto& ref3 = v.emplace<std::string>(3u, 'a');
        static_assert(std::is_same_v<std::string&, decltype(ref3)>, "");
        TestAssert(std::get<3>(v) == "aaa");
        TestAssert(&ref3 == &std::get<3>(v));
    }
}

int run_test()
{
    test_basic();
    test_emplace_sfinae();

    return 0;
}
} // namespace type_args

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
    { // just init list
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(std::is_constructible<V, std::in_place_type_t<InitList>, IL>::value, "");
    }
    { // too many arguments
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<InitList>, IL, int>::value, "");
    }
    { // too few arguments
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<InitListArg>, IL>::value, "");
    }
    { // init list and arguments
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(std::is_constructible<V, std::in_place_type_t<InitListArg>, IL, int>::value, "");
    }
    { // not constructible from arguments
        using V = std::variant<InitList, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<int>, IL>::value, "");
    }
    { // duplicate types in variant
        using V = std::variant<InitListArg, InitListArg, int>;
        static_assert(!std::is_constructible<V, std::in_place_type_t<InitListArg>, IL, int>::value, "");
    }
}

void test_ctor_basic()
{
    {
        constexpr std::variant<InitList, InitListArg> v(
            std::in_place_type<InitList>, {1, 2, 3});
        static_assert(v.index() == 0, "");
        static_assert(std::get<0>(v).size == 3, "");
    }
    {
        constexpr std::variant<InitList, InitListArg> v(
            std::in_place_type<InitListArg>, {1, 2, 3, 4}, 42);
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
} // namespace emplace

TEST(variant_test, test_emplace)
{
    emplace::index::run_test();
    emplace::index_init_list_args::run_test();
    emplace::type_args::run_test();
    emplace::in_place_type_init_list_args::run_test();
}

namespace status {
namespace index {
int run_test()
{
    {
        using V = std::variant<int, long>;
        constexpr V v;
        static_assert(v.index() == 0, "");
    }
    {
        using V = std::variant<int, long>;
        V v;
        TestAssert(v.index() == 0);
    }
    {
        using V = std::variant<int, long>;
        constexpr V v(std::in_place_index<1>);
        static_assert(v.index() == 1, "");
    }
    {
        using V = std::variant<int, std::string>;
        V v("abc");
        TestAssert(v.index() == 1);
        v = 42;
        TestAssert(v.index() == 0);
    }
    return 0;
}
} // namespace index
namespace valueless_by_exception {
int run_test()
{
    {
        using V = std::variant<int, long>;
        constexpr V v;
        static_assert(!v.valueless_by_exception(), "");
    }
    {
        using V = std::variant<int, long>;
        V v;
        TestAssert(!v.valueless_by_exception());
    }
    {
        using V = std::variant<int, long, std::string>;
        const V v("abc");
        TestAssert(!v.valueless_by_exception());
    }

    return 0;
}
} // namespace valueless_by_exception
} // namespace status

TEST(variant_test, test_status)
{
    status::index::run_test();
    status::valueless_by_exception::run_test();
}

namespace member_swap {
struct NotSwappable
{
};
void swap(NotSwappable&, NotSwappable&) = delete;

struct NotCopyable
{
    NotCopyable()                   = default;
    NotCopyable(const NotCopyable&) = delete;
    NotCopyable& operator=(const NotCopyable&) = delete;
};

struct NotCopyableWithSwap
{
    NotCopyableWithSwap()                           = default;
    NotCopyableWithSwap(const NotCopyableWithSwap&) = delete;
    NotCopyableWithSwap& operator=(const NotCopyableWithSwap&) = delete;
};
void swap(NotCopyableWithSwap&, NotCopyableWithSwap) {}

struct NotMoveAssignable
{
    NotMoveAssignable()                    = default;
    NotMoveAssignable(NotMoveAssignable&&) = default;
    NotMoveAssignable& operator=(NotMoveAssignable&&) = delete;
};

struct NotMoveAssignableWithSwap
{
    NotMoveAssignableWithSwap()                            = default;
    NotMoveAssignableWithSwap(NotMoveAssignableWithSwap&&) = default;
    NotMoveAssignableWithSwap& operator=(NotMoveAssignableWithSwap&&) = delete;
};
void swap(NotMoveAssignableWithSwap&, NotMoveAssignableWithSwap&) noexcept {}

template <bool Throws>
void do_throw()
{
}

template <>
void do_throw<true>()
{
    throw 42;
}

template <bool NT_Copy, bool NT_Move, bool NT_CopyAssign, bool NT_MoveAssign, bool NT_Swap, bool EnableSwap = true>
struct NothrowTypeImp
{
    static int move_called;
    static int move_assign_called;
    static int swap_called;
    static void reset() { move_called = move_assign_called = swap_called = 0; }
    NothrowTypeImp() = default;
    explicit NothrowTypeImp(int v)
        : value(v) {}
    NothrowTypeImp(const NothrowTypeImp& o) noexcept(NT_Copy)
        : value(o.value)
    {
        TestAssert(false);
    } // never called by test
    NothrowTypeImp(NothrowTypeImp&& o) noexcept(NT_Move)
        : value(o.value)
    {
        ++move_called;
        do_throw<!NT_Move>();
        o.value = -1;
    }
    NothrowTypeImp& operator=(const NothrowTypeImp&) noexcept(NT_CopyAssign)
    {
        TestAssert(false);
        return *this;
    } // never called by the tests
    NothrowTypeImp& operator=(NothrowTypeImp&& o) noexcept(NT_MoveAssign)
    {
        ++move_assign_called;
        do_throw<!NT_MoveAssign>();
        value   = o.value;
        o.value = -1;
        return *this;
    }
    int value;
};
template <bool NT_Copy, bool NT_Move, bool NT_CopyAssign, bool NT_MoveAssign, bool NT_Swap, bool EnableSwap>
int NothrowTypeImp<NT_Copy, NT_Move, NT_CopyAssign, NT_MoveAssign, NT_Swap, EnableSwap>::move_called = 0;
template <bool NT_Copy, bool NT_Move, bool NT_CopyAssign, bool NT_MoveAssign, bool NT_Swap, bool EnableSwap>
int NothrowTypeImp<NT_Copy, NT_Move, NT_CopyAssign, NT_MoveAssign, NT_Swap, EnableSwap>::move_assign_called = 0;
template <bool NT_Copy, bool NT_Move, bool NT_CopyAssign, bool NT_MoveAssign, bool NT_Swap, bool EnableSwap>
int NothrowTypeImp<NT_Copy, NT_Move, NT_CopyAssign, NT_MoveAssign, NT_Swap, EnableSwap>::swap_called = 0;

template <bool NT_Copy, bool NT_Move, bool NT_CopyAssign, bool NT_MoveAssign, bool NT_Swap>
void swap(NothrowTypeImp<NT_Copy, NT_Move, NT_CopyAssign, NT_MoveAssign, NT_Swap, true>& lhs,
          NothrowTypeImp<NT_Copy, NT_Move, NT_CopyAssign, NT_MoveAssign, NT_Swap, true>& rhs) noexcept(NT_Swap)
{
    lhs.swap_called++;
    do_throw<!NT_Swap>();
    int tmp   = lhs.value;
    lhs.value = rhs.value;
    rhs.value = tmp;
}

// throwing copy, nothrow move ctor/assign, no swap provided
using NothrowMoveable = NothrowTypeImp<false, true, false, true, false, false>;
// throwing copy and move assign, nothrow move ctor, no swap provided
using NothrowMoveCtor = NothrowTypeImp<false, true, false, false, false, false>;
// nothrow move ctor, throwing move assignment, swap provided
using NothrowMoveCtorWithThrowingSwap =
    NothrowTypeImp<false, true, false, false, false, true>;
// throwing move ctor, nothrow move assignment, no swap provided
using ThrowingMoveCtor =
    NothrowTypeImp<false, false, false, true, false, false>;
// throwing special members, nothrowing swap
using ThrowingTypeWithNothrowSwap =
    NothrowTypeImp<false, false, false, false, true, true>;
using NothrowTypeWithThrowingSwap =
    NothrowTypeImp<true, true, true, true, false, true>;
// throwing move assign with nothrow move and nothrow swap
using ThrowingMoveAssignNothrowMoveCtorWithSwap =
    NothrowTypeImp<false, true, false, false, true, true>;
// throwing move assign with nothrow move but no swap.
using ThrowingMoveAssignNothrowMoveCtor =
    NothrowTypeImp<false, true, false, false, false, false>;

struct NonThrowingNonNoexceptType
{
    static int move_called;
    static void reset() { move_called = 0; }
    NonThrowingNonNoexceptType() = default;
    NonThrowingNonNoexceptType(int v)
        : value(v) {}
    NonThrowingNonNoexceptType(NonThrowingNonNoexceptType&& o) noexcept(false)
        : value(o.value)
    {
        ++move_called;
        o.value = -1;
    }
    NonThrowingNonNoexceptType&
    operator=(NonThrowingNonNoexceptType&&) noexcept(false)
    {
        TestAssert(false); // never called by the tests.
        return *this;
    }
    int value;
};
int NonThrowingNonNoexceptType::move_called = 0;

struct ThrowsOnSecondMove
{
    int value;
    int move_count;
    ThrowsOnSecondMove(int v)
        : value(v), move_count(0) {}
    ThrowsOnSecondMove(ThrowsOnSecondMove&& o) noexcept(false)
        : value(o.value), move_count(o.move_count + 1)
    {
        if (move_count == 2)
            do_throw<true>();
        o.value = -1;
    }
    ThrowsOnSecondMove& operator=(ThrowsOnSecondMove&&)
    {
        TestAssert(false); // not called by test
        return *this;
    }
};

void test_swap_valueless_by_exception()
{
}

void test_swap_same_alternative()
{
    {
        using T = ThrowingTypeWithNothrowSwap;
        using V = std::variant<T, int>;
        T::reset();
        V v1(std::in_place_index<0>, 42);
        V v2(std::in_place_index<0>, 100);
        v1.swap(v2);
        TestAssert(T::swap_called == 1);
        TestAssert(std::get<0>(v1).value == 100);
        TestAssert(std::get<0>(v2).value == 42);
        swap(v1, v2);
        TestAssert(T::swap_called == 2);
        TestAssert(std::get<0>(v1).value == 42);
        TestAssert(std::get<0>(v2).value == 100);
    }
    {
        using T = NothrowMoveable;
        using V = std::variant<T, int>;
        T::reset();
        V v1(std::in_place_index<0>, 42);
        V v2(std::in_place_index<0>, 100);
        v1.swap(v2);
        TestAssert(T::swap_called == 0);
        TestAssert(T::move_called == 1);
        TestAssert(T::move_assign_called == 2);
        TestAssert(std::get<0>(v1).value == 100);
        TestAssert(std::get<0>(v2).value == 42);
        T::reset();
        swap(v1, v2);
        TestAssert(T::swap_called == 0);
        TestAssert(T::move_called == 1);
        TestAssert(T::move_assign_called == 2);
        TestAssert(std::get<0>(v1).value == 42);
        TestAssert(std::get<0>(v2).value == 100);
    }
}

void test_swap_different_alternatives()
{
    {
        using T = NothrowMoveCtorWithThrowingSwap;
        using V = std::variant<T, int>;
        T::reset();
        V v1(std::in_place_index<0>, 42);
        V v2(std::in_place_index<1>, 100);
        v1.swap(v2);
        TestAssert(T::swap_called == 0);
        // The libc++ implementation double copies the argument, and not
        // the variant swap is called on.
        TestAssert(T::move_called != 1);
        TestAssert(T::move_called <= 2);
        TestAssert(T::move_assign_called == 0);
        TestAssert(std::get<1>(v1) == 100);
        TestAssert(std::get<0>(v2).value == 42);
        T::reset();
        swap(v1, v2);
        TestAssert(T::swap_called == 0);
        TestAssert(T::move_called != 2);
        TestAssert(T::move_called <= 2);
        TestAssert(T::move_assign_called == 0);
        TestAssert(std::get<0>(v1).value == 42);
        TestAssert(std::get<1>(v2) == 100);
    }
}

template <class Var>
constexpr auto has_swap_member_imp(int)
    -> decltype(std::declval<Var&>().swap(std::declval<Var&>()), true)
{
    return true;
}

template <class Var>
constexpr auto has_swap_member_imp(long) -> bool
{
    return false;
}

template <class Var>
constexpr bool has_swap_member()
{
    return has_swap_member_imp<Var>(0);
}
using std::swap;
template <typename T>
struct is_swappable2
{
private:
    template <typename U,
              typename = decltype(swap(std::declval<U&>(),
                                       std::declval<U&>()))>
    inline static std::true_type test(int);

    template <typename U>
    inline static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

void test_swap_sfinae()
{
    {
        using V = std::variant<int, NotSwappable>;
        LIBCPP_STATIC_ASSERT(!has_swap_member<V>(), "");
        static_assert(std::is_swappable_v<V>, "");
    }
#if STD_HAS_CXX17
    {
        using V = std::variant<int, NotCopyable>;
        LIBCPP_STATIC_ASSERT(!has_swap_member<V>(), "");

        static_assert(!std::is_swappable_v<V>, "");
    }
    {
        using V = std::variant<int, NotCopyableWithSwap>;
        LIBCPP_STATIC_ASSERT(!has_swap_member<V>(), "");
        static_assert(!std::is_swappable_v<V>, "");
    }
    {
        using V = std::variant<int, NotMoveAssignable>;
        LIBCPP_STATIC_ASSERT(!has_swap_member<V>(), "");
        static_assert(!std::is_swappable_v<V>, "");
    }
#endif
}

void test_swap_noexcept()
{
    {
        using V = std::variant<int, NothrowMoveable>;
        static_assert(std::is_swappable_v<V> && has_swap_member<V>(), "");
        static_assert(std::is_nothrow_swappable_v<V>, "");
        V v1, v2;
        v1.swap(v2);
        swap(v1, v2);
    }
    {
        using V = std::variant<int, NothrowMoveCtor>;
        static_assert(std::is_swappable_v<V> && has_swap_member<V>(), "");
        static_assert(!std::is_nothrow_swappable_v<V>, "");
        V v1, v2;
        v1.swap(v2);
        swap(v1, v2);
    }
    {
        using V = std::variant<int, ThrowingTypeWithNothrowSwap>;
        static_assert(std::is_swappable_v<V> && has_swap_member<V>(), "");
        static_assert(!std::is_nothrow_swappable_v<V>, "");
        // instantiate swap
        V v1, v2;
        v1.swap(v2);
        swap(v1, v2);
    }
    {
        using V = std::variant<int, ThrowingMoveAssignNothrowMoveCtor>;
        static_assert(std::is_swappable_v<V> && has_swap_member<V>(), "");
        static_assert(!std::is_nothrow_swappable_v<V>, "");
        // instantiate swap
        V v1, v2;
        v1.swap(v2);
        swap(v1, v2);
    }
    {
        using V = std::variant<int, ThrowingMoveAssignNothrowMoveCtorWithSwap>;
        static_assert(std::is_swappable_v<V> && has_swap_member<V>(), "");

        static_assert(std::is_nothrow_swappable_v<V>, "");

        // instantiate swap
        V v1, v2;
        v1.swap(v2);
        swap(v1, v2);
    }
    {
        using V = std::variant<int, NotMoveAssignableWithSwap>;

        static_assert(std::is_swappable_v<V> && has_swap_member<V>(), "");
        static_assert(std::is_nothrow_swappable_v<V>, "");
        // instantiate swap
        V v1, v2;
        v1.swap(v2);
        swap(v1, v2);
    }
    {
        // This variant type does not provide either a member or non-member swap
        // but is still swappable via the generic swap algorithm, since the
        // variant is move constructible and move assignable.
        using V = std::variant<int, NotSwappable>;
        LIBCPP_STATIC_ASSERT(!has_swap_member<V>(), "");
        static_assert(std::is_swappable_v<V>, "");
        static_assert(std::is_nothrow_swappable_v<V>, "");
        V v1, v2;
        swap(v1, v2);
    }
}

int run_test()
{
    test_swap_valueless_by_exception();
    test_swap_same_alternative();
    test_swap_different_alternatives();
    test_swap_sfinae();
    test_swap_noexcept();

    return 0;
}
} // namespace member_swap

TEST(variant_test, test_member_swap)
{
    member_swap::run_test();
}

namespace visit {
namespace robust_against_adl {
struct PrintVisitor1
{
    void operator()(int i)
    {
    }
    void operator()(double f)
    {
    }
    void operator()(const std::string& s)
    {
    }
};

bool test(bool do_it)
{
    std::variant<int, double, std::string> var1(1.1);
    std::visit(PrintVisitor1{}, var1);
    return true;
}

int run_test()
{
    test(true);
    return 0;
}
}
} // namespace visit::robust_against_adl

TEST(variant_test, test_visit)
{
    visit::robust_against_adl::run_test();
}
