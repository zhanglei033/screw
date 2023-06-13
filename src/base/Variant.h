#pragma once
#ifndef _VARIANT_H_
#define _VARIANT_H_
#include "Platform.h"
#if STD_HAS_CXX17
#include <variant>
#else
#include "Assert.h"
#include "STL.h"
#include <limits>
#include <tuple>
namespace std {

#pragma region variant
template <class... Types>
class variant;
#pragma endregion

#pragma region bad_variant_access
class bad_variant_access : public exception
{
public:
    bad_variant_access() DECL_NOEXCEPT = default;

    DECL_NODISCARD const char* what() const DECL_NOEXCEPT override
    {
        return "bad variant access";
    }
};

#define throw_bad_variant_access() ThrowException(false, "bad variant access")

#pragma endregion

#pragma region variant helper
DECL_INLINE_VAR DECL_CONSTEXPR11 size_t variant_npos = ~size_t{0};

template <size_t Idx, class List>
struct variant_init_overload_impl;
template <size_t Idx, template <class...> class List, class First>
struct variant_init_overload_impl<Idx, List<First>>
{
    template <class InitializerType>
    auto operator()(First, InitializerType&&) -> std::meta_list<integral_constant<size_t, Idx>, First>;
};
template <size_t Idx, template <class...> class List, class First, class... Rest>
struct variant_init_overload_impl<Idx, List<First, Rest...>> : variant_init_overload_impl<Idx + 1, List<Rest...>>
{
    using variant_init_overload_impl<Idx + 1, List<Rest...>>::operator();
    template <class InitializerType>
    auto operator()(First, InitializerType&&) -> std::meta_list<integral_constant<size_t, Idx>, First>;
};

template <class List>
using variant_init_overload = variant_init_overload_impl<0, List>;

template <class Enable, class T, class List>
struct variant_init_best_match
{
    using type = std::meta_list<integral_constant<size_t, variant_npos>, void>;
};
template <class T, class List>
struct variant_init_best_match<void_t<decltype(variant_init_overload<List>{}(std::declval<T>(), std::declval<T>()))>, T, List>
{
    using type = decltype(variant_init_overload<List>{}(std::declval<T>(), std::declval<T>()));
};

template <class T, class... Types>
struct variant_init_helper_impl
{
    using first_match_type  = typename variant_init_best_match<void, T, std::meta_list<Types...>>::type;
    using extract_type      = std::meta_list_extract_type_t<meta_list<Types...>, meta_list_at_t<first_match_type, 1>>;
    using second_match_type = std::conditional_t<meta_list_at_t<first_match_type, 0>::value == variant_npos || sizeof...(Types) <= 1,
                                                 std::meta_list<integral_constant<size_t, variant_npos>, void>,
                                                 typename variant_init_best_match<void, T, extract_type>::type>;

    static DECL_CONSTEXPR11 bool is_valid = !is_same_v<meta_list_at_t<first_match_type, 1>, meta_list_at_t<second_match_type, 1>>;
};

template <class IsValid, class T, class... Types>
struct variant_init_helper
{
    using type = std::meta_list<integral_constant<size_t, variant_npos>, void>;
};

template <class T, class... Types>
struct variant_init_helper<enable_if_t<variant_init_helper_impl<T, Types...>::is_valid>, T, Types...>
{
    using type = typename variant_init_helper_impl<T, Types...>::first_match_type;
};

template <class T, class... Types>
using variant_init_t = meta_list_at_t<typename variant_init_helper<void, T, Types...>::type, 1>;
template <class T, class... Types>
using variant_initidx = meta_list_at_t<typename variant_init_helper<void, T, Types...>::type, 0>;
#pragma endregion

#pragma region variant impl
template <size_t Idx, class Storage>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) variant_raw_get(Storage&& obj) DECL_NOEXCEPT
{
    if constexpr (Idx == 0)
    {
        return static_cast<Storage&&>(obj).get();
    }
    else if constexpr (Idx == 1)
    {
        return static_cast<Storage&&>(obj).m_tail.get();
    }
    else if constexpr (Idx == 2)
    {
        return static_cast<Storage&&>(obj).m_tail.m_tail.get();
    }
    else if constexpr (Idx == 3)
    {
        return static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.get();
    }
    else if constexpr (Idx == 4)
    {
        return static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.get();
    }
    else if constexpr (Idx == 5)
    {
        return static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.m_tail.get();
    }
    else if constexpr (Idx == 6)
    {
        return static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.get();
    }
    else if constexpr (Idx == 7)
    {
        return static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.get();
    }
    else if constexpr (Idx < 16)
    {
        return variant_raw_get<Idx - 8>(static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail);
    }
    else if constexpr (Idx < 32)
    {
        return variant_raw_get<Idx - 16>(static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail);
    }
    else if constexpr (Idx < 64)
    {
        return variant_raw_get<Idx - 32>(static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail);
    }
    else
    {
        return variant_raw_get<Idx - 64>(static_cast<Storage&&>(obj).m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail.m_tail);
    }
}

template <class T, size_t Idx>
struct variant_tagged
{
    static DECL_CONSTEXPR11 size_t index = Idx;
    T value;
};

template <class get_storage, size_t Idx>
using variant_tagged_ref_type = variant_tagged<decltype(variant_raw_get<Idx>(std::declval<get_storage>()))&&, Idx>;

template <class Func, class get_storage>
using variant_raw_visit_type = decltype(std::declval<Func>()(std::declval<variant_tagged_ref_type<get_storage, 0>>()));

template <class Func, class get_storage, class Indices = make_index_sequence<remove_reference_t<get_storage>::size>>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool variant_raw_visit_noexcept = false;

template <class Func, class get_storage, size_t... Idxs>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool variant_raw_visit_noexcept<Func, get_storage, index_sequence<Idxs...>> =
    conjunction_v<is_nothrow_invocable<Func, variant_tagged<get_storage&&, variant_npos>>,
                  is_nothrow_invocable<Func, variant_tagged_ref_type<get_storage, Idxs>>...>;

template <class Func, class get_storage>
DECL_NODISCARD DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> variant_raw_visit_valueless(Func&& func, get_storage&& obj) DECL_NOEXCEPT(is_nothrow_invocable_v<Func, variant_tagged<get_storage&&, variant_npos>>)
{
    return static_cast<Func&&>(func)(variant_tagged<get_storage&&, variant_npos>{static_cast<get_storage&&>(obj)});
}

template <size_t Idx, class Func, class get_storage>
DECL_NODISCARD DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> variant_raw_visit_dispatch(Func&& func, get_storage&& obj) DECL_NOEXCEPT(is_nothrow_invocable_v<Func, variant_tagged_ref_type<get_storage, Idx>>)
{
    return static_cast<Func&&>(func)(variant_tagged_ref_type<get_storage, Idx>{variant_raw_get<Idx>(static_cast<get_storage&&>(obj))});
}

template <class Func, class get_storage, class Indices = make_index_sequence<remove_reference_t<get_storage>::size>>
struct variant_raw_dispatch_table;

template <class Func, class get_storage, size_t... Idxs>
struct variant_raw_dispatch_table<Func, get_storage, index_sequence<Idxs...>>
{
    using dispatch_type                               = variant_raw_visit_type<Func, get_storage> (*)(Func&&, get_storage&&) DECL_NOEXCEPT(variant_raw_visit_noexcept<Func, get_storage>);
    static DECL_CONSTEXPR11 dispatch_type dispatchs[] = {&variant_raw_visit_valueless<Func, get_storage>, &variant_raw_visit_dispatch<Idxs, Func, get_storage>...};
};

#define VARIANT_VISIT_STAMP_CASE(n)                                                                                                             \
    case (n) + 1:                                                                                                                               \
        if constexpr ((n) < size)                                                                                                               \
        {                                                                                                                                       \
            return static_cast<Func&&>(func)(variant_tagged_ref_type<get_storage, (n)>{variant_raw_get<(n)>(static_cast<get_storage&&>(obj))}); \
        }                                                                                                                                       \
        std::unreachable()

#define VARIANT_VISIT_STAMP(stamper, n)                                                                                 \
    DECL_CONSTEXPR11 size_t size = remove_reference_t<get_storage>::size;                                               \
    static_assert(((n) == 4 || size > (n) / 4) && size <= (n));                                                         \
    switch (idx)                                                                                                        \
    {                                                                                                                   \
    case 0:                                                                                                             \
        return static_cast<Func&&>(func)(variant_tagged<get_storage&&, variant_npos>{static_cast<get_storage&&>(obj)}); \
        stamper(0, VARIANT_VISIT_STAMP_CASE);                                                                           \
    default:                                                                                                            \
        std::unreachable();                                                                                             \
    }

#define VARIANT_STAMP4(n, x) \
    x(n);                    \
    x(n + 1);                \
    x(n + 2);                \
    x(n + 3)

#define VARIANT_STAMP16(n, x) \
    VARIANT_STAMP4(n, x);     \
    VARIANT_STAMP4(n + 4, x); \
    VARIANT_STAMP4(n + 8, x); \
    VARIANT_STAMP4(n + 12, x)

#define VARIANT_STAMP64(n, x)   \
    VARIANT_STAMP16(n, x);      \
    VARIANT_STAMP16(n + 16, x); \
    VARIANT_STAMP16(n + 32, x); \
    VARIANT_STAMP16(n + 48, x)

#define VARIANT_STAMP256(n, x)   \
    VARIANT_STAMP64(n, x);       \
    VARIANT_STAMP64(n + 64, x);  \
    VARIANT_STAMP64(n + 128, x); \
    VARIANT_STAMP64(n + 192, x)

#define VARIANT_STAMP(n, x) x(VARIANT_STAMP##n, n)

template <int Strategy>
struct variant_raw_visit_impl;

template <>
struct variant_raw_visit_impl<-1>
{
    template <class Func, class get_storage>
    DECL_NODISCARD static DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> visit(size_t idx, Func&& func, get_storage&& obj) DECL_NOEXCEPT(variant_raw_visit_noexcept<Func, get_storage>)
    {
        DECL_CONSTEXPR11 size_t size = remove_reference_t<get_storage>::size;
        static_assert(size > 256);
        DECL_CONSTEXPR11 auto& dispatchs = variant_raw_dispatch_table<Func, get_storage>::dispatchs;
        return dispatchs[idx](static_cast<Func&&>(func), static_cast<get_storage&&>(obj));
    }
};

template <>
struct variant_raw_visit_impl<1>
{
    template <class Func, class get_storage>
    DECL_NODISCARD static DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> visit(size_t idx, Func&& func, get_storage&& obj) DECL_NOEXCEPT(variant_raw_visit_noexcept<Func, get_storage>)
    {
        VARIANT_STAMP(4, VARIANT_VISIT_STAMP);
    }
};

template <>
struct variant_raw_visit_impl<2>
{
    template <class Func, class get_storage>
    DECL_NODISCARD static DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> visit(size_t idx, Func&& func, get_storage&& obj) DECL_NOEXCEPT(variant_raw_visit_noexcept<Func, get_storage>)
    {
        VARIANT_STAMP(16, VARIANT_VISIT_STAMP);
    }
};

template <>
struct variant_raw_visit_impl<3>
{
    template <class Func, class get_storage>
    DECL_NODISCARD static DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> visit(size_t idx, Func&& func, get_storage&& obj) DECL_NOEXCEPT(variant_raw_visit_noexcept<Func, get_storage>)
    {
        VARIANT_STAMP(64, VARIANT_VISIT_STAMP);
    }
};

template <>
struct variant_raw_visit_impl<4>
{
    template <class Func, class get_storage>
    DECL_NODISCARD static DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> visit(size_t idx, Func&& func, get_storage&& obj) DECL_NOEXCEPT(variant_raw_visit_noexcept<Func, get_storage>)
    {
        VARIANT_STAMP(256, VARIANT_VISIT_STAMP);
    }
};

#undef VARIANT_VISIT_STAMP
#undef VARIANT_VISIT_STAMP_CASE
#undef VARIANT_STAMP
#undef VARIANT_STAMP4
#undef VARIANT_STAMP16
#undef VARIANT_STAMP64
#undef VARIANT_STAMP256

template <class get_storage, class Func>
DECL_NODISCARD DECL_CONSTEXPR11 variant_raw_visit_type<Func, get_storage> variant_raw_visit(size_t idx, get_storage&& obj, Func&& func) DECL_NOEXCEPT(variant_raw_visit_noexcept<Func, get_storage>)
{
    DECL_CONSTEXPR11 size_t size  = remove_reference_t<get_storage>::size;
    DECL_CONSTEXPR11 int strategy = size <= 4   ? 1 :
                                    size <= 16  ? 2 :
                                    size <= 64  ? 3 :
                                    size <= 256 ? 4 :
                                                  -1;
    return variant_raw_visit_impl<strategy>::visit(idx + 1, static_cast<Func&&>(func), static_cast<get_storage&&>(obj));
}

template <class... Types>
class variant_base;

template <class... Types>
struct VariantConstructVisitor
{
    variant_base<Types...>& self;

    template <class T, size_t Idx>
    DECL_CONSTEXPR11 void operator()(variant_tagged<T, Idx> source) const DECL_NOEXCEPT(disjunction_v<bool_constant<Idx == variant_npos>, is_nothrow_constructible<remove_reference_t<T>, T>>)
    {
        if constexpr (Idx != variant_npos)
        {
            construct_in_place(self.get_storage(), integral_constant<size_t, Idx>{}, static_cast<T&&>(source.value));
            self.set_index(Idx);
        }
    }
};

template <class Target, class... Types>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_variant_should_directly_construct_v = disjunction_v<is_nothrow_constructible<Target, Types...>, negation<is_nothrow_move_constructible<Target>>>;

template <class... Types>
struct variant_assign_visitor
{
    variant_base<Types...>& self;

    template <class T, size_t Idx>
    DECL_CONSTEXPR11 void operator()(variant_tagged<T, Idx> source) const
        DECL_NOEXCEPT(disjunction_v<bool_constant<Idx == variant_npos>,
                                    conjunction<is_nothrow_assignable<remove_cvref_t<T>&, T>,
                                                is_nothrow_constructible<remove_cvref_t<T>, T>>>)
    {
        if constexpr (Idx == variant_npos)
        {
            self.reset();
        }
        else
        {
            if (self.m_index == Idx)
            {
                auto& target = variant_raw_get<Idx>(self.get_storage());
                target       = static_cast<T&&>(source.value);
            }
            else
            {
                if constexpr (is_lvalue_reference_v<T>)
                {
                    if constexpr (is_variant_should_directly_construct_v<remove_cvref_t<T>, T>)
                    {
                        self.reset();
                        construct_in_place(self.get_storage(), integral_constant<size_t, Idx>{}, source.value);
                    }
                    else
                    {
                        auto temp = source.value;
                        self.reset();
                        construct_in_place(self.get_storage(), integral_constant<size_t, Idx>{}, std::move(temp));
                    }
                }
                else
                {
                    self.reset();
                    construct_in_place(self.get_storage(), integral_constant<size_t, Idx>{}, static_cast<T&&>(source.value));
                }

                self.set_index(Idx);
            }
        }
    }
};

#pragma endregion

#pragma region variant facade
/// variant  base type facade
// 非平凡拷贝构造的外壳
template <class Base>
struct NonTrivialCopyConstructionFacade : Base
{
    using Base::Base;

    DECL_CONSTEXPR11 NonTrivialCopyConstructionFacade() = default;
    DECL_CONSTEXPR11 NonTrivialCopyConstructionFacade(const NonTrivialCopyConstructionFacade& other) DECL_NOEXCEPT(DECL_NOEXCEPT(Base::construct_from(static_cast<const Base&>(other))))
    {
        Base::construct_from(static_cast<const Base&>(other));
    }
    DECL_CONSTEXPR11 NonTrivialCopyConstructionFacade(NonTrivialCopyConstructionFacade&&) = default;
    DECL_CONSTEXPR11 NonTrivialCopyConstructionFacade& operator=(const NonTrivialCopyConstructionFacade&) = default;
    DECL_CONSTEXPR11 NonTrivialCopyConstructionFacade& operator=(NonTrivialCopyConstructionFacade&&) = default;
};

// 判断Types: 是否都有拷贝构造函数且非平凡的拷贝构造函数
// true时:    类型为NonTrivialCopyConstructionFacade<Base>
// false时:   类型为Base
template <class Base, class... Types>
using FacadeControlCopyConstruction = conditional_t<
    conjunction_v<is_copy_constructible<Types>..., negation<conjunction<is_trivially_copy_constructible<Types>...>>>,
    NonTrivialCopyConstructionFacade<Base>,
    Base>;

// 非平凡的移动构造外壳
template <class Base, class... Types>
struct NonTrivialMoveConstructionFacade : FacadeControlCopyConstruction<Base, Types...>
{
    using Mybase = FacadeControlCopyConstruction<Base, Types...>;
    using Mybase::Mybase;

    DECL_CONSTEXPR11 NonTrivialMoveConstructionFacade()                                        = default;
    DECL_CONSTEXPR11 NonTrivialMoveConstructionFacade(const NonTrivialMoveConstructionFacade&) = default;
    DECL_CONSTEXPR11 NonTrivialMoveConstructionFacade(NonTrivialMoveConstructionFacade&& other) DECL_NOEXCEPT(DECL_NOEXCEPT(Mybase::construct_from(static_cast<Base&&>(other))))
    {
        Mybase::construct_from(static_cast<Base&&>(other));
    }
    DECL_CONSTEXPR11 NonTrivialMoveConstructionFacade& operator=(const NonTrivialMoveConstructionFacade&) = default;
    DECL_CONSTEXPR11 NonTrivialMoveConstructionFacade& operator=(NonTrivialMoveConstructionFacade&&) = default;
};

// 判断Types: 是否都有移动构造函数且非平凡的移动构造函数
// true时:    类型为NonTrivialMoveConstructionFacade<Base>
// false时:   类型由FacadeControlCopyConstruction<Base, Types...>推导
template <class Base, class... Types>
using FacadeControlMoveConstruction = conditional_t<
    conjunction_v<is_move_constructible<Types>..., negation<conjunction<is_trivially_move_constructible<Types>...>>>,
    NonTrivialMoveConstructionFacade<Base, Types...>,
    FacadeControlCopyConstruction<Base, Types...>>;

// 非平凡的拷贝赋值外壳
template <class Base, class... Types>
struct NonTrivialCopyAssignFacade : FacadeControlMoveConstruction<Base, Types...>
{
    using Mybase = FacadeControlMoveConstruction<Base, Types...>;
    using Mybase::Mybase;

    DECL_CONSTEXPR11 NonTrivialCopyAssignFacade()                                  = default;
    DECL_CONSTEXPR11 NonTrivialCopyAssignFacade(const NonTrivialCopyAssignFacade&) = default;
    DECL_CONSTEXPR11 NonTrivialCopyAssignFacade(NonTrivialCopyAssignFacade&&)      = default;

    DECL_CONSTEXPR11 NonTrivialCopyAssignFacade& operator=(const NonTrivialCopyAssignFacade& other) DECL_NOEXCEPT(DECL_NOEXCEPT(Mybase::assign_from(static_cast<const Base&>(other))))
    {
        Mybase::assign_from(static_cast<const Base&>(other));
        return *this;
    }
    DECL_CONSTEXPR11 NonTrivialCopyAssignFacade& operator=(NonTrivialCopyAssignFacade&&) = default;
};

// 已删除的拷贝赋值的外壳
template <class Base, class... Types>
struct DeletedCopyAssignFacade : FacadeControlMoveConstruction<Base, Types...>
{
    using Mybase = FacadeControlMoveConstruction<Base, Types...>;
    using Mybase::Mybase;

    DECL_CONSTEXPR11 DeletedCopyAssignFacade()                               = default;
    DECL_CONSTEXPR11 DeletedCopyAssignFacade(const DeletedCopyAssignFacade&) = default;
    DECL_CONSTEXPR11 DeletedCopyAssignFacade(DeletedCopyAssignFacade&&)      = default;
    DECL_CONSTEXPR11 DeletedCopyAssignFacade& operator=(const DeletedCopyAssignFacade&) = delete;
    DECL_CONSTEXPR11 DeletedCopyAssignFacade& operator=(DeletedCopyAssignFacade&&) = default;
};

// 第一步
// 判断Types: 是否都有平凡的析构函数，平凡的拷贝构造，平凡的拷贝赋值
// true时:    类型由FacadeControlMoveConstruction<Base, Types...>推导
// false时:   进入第二步
// 第二步
// 判断Types: 是否都有拷贝构造，拷贝赋值
// true时:    类型为NonTrivialCopyAssignFacade<Base, Types...>
// false时:   类型为DeletedCopyAssignFacade<Base, Types...>
template <class Base, class... Types>
using FacadeControlCopyAssign =
    conditional_t<conjunction_v<is_trivially_destructible<Types>..., is_trivially_copy_constructible<Types>..., is_trivially_copy_assignable<Types>...>,
                  FacadeControlMoveConstruction<Base, Types...>,
                  conditional_t<conjunction_v<is_copy_constructible<Types>..., is_copy_assignable<Types>...>,
                                NonTrivialCopyAssignFacade<Base, Types...>,
                                DeletedCopyAssignFacade<Base, Types...>>>;

// 非平凡的移动赋值外壳
template <class Base, class... Types>
struct NonTrivialMoveAssignFacade : FacadeControlCopyAssign<Base, Types...>
{
    using Mybase = FacadeControlCopyAssign<Base, Types...>;
    using Mybase::Mybase;

    DECL_CONSTEXPR11 NonTrivialMoveAssignFacade()                                  = default;
    DECL_CONSTEXPR11 NonTrivialMoveAssignFacade(const NonTrivialMoveAssignFacade&) = default;
    DECL_CONSTEXPR11 NonTrivialMoveAssignFacade(NonTrivialMoveAssignFacade&&)      = default;
    DECL_CONSTEXPR11 NonTrivialMoveAssignFacade& operator=(const NonTrivialMoveAssignFacade&) = default;

    DECL_CONSTEXPR11 NonTrivialMoveAssignFacade& operator=(NonTrivialMoveAssignFacade&& other) DECL_NOEXCEPT(DECL_NOEXCEPT(Mybase::assign_from(static_cast<Base&&>(other))))
    {
        Mybase::assign_from(static_cast<Base&&>(other));
        return *this;
    }
};

// 已删除的移动赋值外壳
template <class Base, class... Types>
struct DeletedMoveAssignFacade : FacadeControlCopyAssign<Base, Types...>
{
    using Mybase = FacadeControlCopyAssign<Base, Types...>;
    using Mybase::Mybase;

    DECL_CONSTEXPR11 DeletedMoveAssignFacade()                               = default;
    DECL_CONSTEXPR11 DeletedMoveAssignFacade(const DeletedMoveAssignFacade&) = default;
    DECL_CONSTEXPR11 DeletedMoveAssignFacade(DeletedMoveAssignFacade&&)      = default;
    DECL_CONSTEXPR11 DeletedMoveAssignFacade& operator=(const DeletedMoveAssignFacade&) = default;
    DECL_CONSTEXPR11 DeletedMoveAssignFacade& operator=(DeletedMoveAssignFacade&&) = delete;
};

// 第一步
// 判断Types: 是否都有平凡的析构函数，平凡的移动构造，平凡的移动赋值
// true时:    类型由FacadeControlCopyAssign<Base, Types...>推导
// false时:   进入第二步
// 第二步
// 判断Types: 是否都有移动构造，移动赋值
// true时:    类型为NonTrivialMoveAssignFacade<Base, Types...>
// false时:   类型为DeletedMoveAssignFacade<Base, Types...>
template <class Base, class... Types>
using FacadeControlMoveAssign =
    conditional_t<conjunction_v<is_trivially_destructible<Types>..., is_trivially_move_constructible<Types>..., is_trivially_move_assignable<Types>...>,
                  FacadeControlCopyAssign<Base, Types...>,
                  conditional_t<conjunction_v<is_move_constructible<Types>..., is_move_assignable<Types>...>,
                                NonTrivialMoveAssignFacade<Base, Types...>,
                                DeletedMoveAssignFacade<Base, Types...>>>;

template <class Base, class... Types>
using FacadeControl = FacadeControlMoveAssign<Base, Types...>;
#pragma endregion

#pragma region variant storage
template <bool TrivialDestruction, class... Types>
class variant_storage_impl
{
};

template <class... Types>
using variant_storage = variant_storage_impl<conjunction_v<is_trivially_destructible<Types>...>, Types...>;

template <class First, class... Rest>
class variant_storage_impl<true, First, Rest...>
{
public:
    static DECL_CONSTEXPR11 size_t size = 1 + sizeof...(Rest);
    union
    {
        remove_const_t<First> m_head;
        variant_storage<Rest...> m_tail;
    };

    DECL_CONSTEXPR11 variant_storage_impl() DECL_NOEXCEPT {}

    template <class... Types>
    DECL_CONSTEXPR11 explicit variant_storage_impl(integral_constant<size_t, 0>, Types&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<First, Types...>)
        : m_head(static_cast<Types&&>(args)...)
    {
    }

    template <size_t Idx, class... Types, enable_if_t<(Idx > 0), int> = 0>
    DECL_CONSTEXPR11 explicit variant_storage_impl(integral_constant<size_t, Idx>, Types&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<variant_storage<Rest...>, integral_constant<size_t, Idx - 1>, Types...>)
        : m_tail(integral_constant<size_t, Idx - 1>{}, static_cast<Types&&>(args)...)
    {
    }

    DECL_NODISCARD DECL_CONSTEXPR11 First& get() & DECL_NOEXCEPT
    {
        return m_head;
    }
    DECL_NODISCARD DECL_CONSTEXPR11 const First& get() const& DECL_NOEXCEPT
    {
        return m_head;
    }
    DECL_NODISCARD DECL_CONSTEXPR11 First&& get() && DECL_NOEXCEPT
    {
        return std::move(m_head);
    }
    DECL_NODISCARD DECL_CONSTEXPR11 const First&& get() const&& DECL_NOEXCEPT
    {
        return std::move(m_head);
    }
};

template <class First, class... Rest>
class variant_storage_impl<false, First, Rest...>
{
public:
    static DECL_CONSTEXPR11 size_t size = 1 + sizeof...(Rest);
    union
    {
        remove_const_t<First> m_head;
        variant_storage<Rest...> m_tail;
    };

    DECL_CONSTEXPR20 ~variant_storage_impl() DECL_NOEXCEPT
    {
    }

    DECL_CONSTEXPR11 variant_storage_impl() DECL_NOEXCEPT {}

    template <class... Types>
    DECL_CONSTEXPR11 explicit variant_storage_impl(integral_constant<size_t, 0>, Types&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<First, Types...>)
        : m_head(static_cast<Types&&>(args)...)
    {
    }

    template <size_t Idx, class... Types, enable_if_t<(Idx > 0), int> = 0>
    DECL_CONSTEXPR11 explicit variant_storage_impl(integral_constant<size_t, Idx>, Types&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<variant_storage<Rest...>, integral_constant<size_t, Idx - 1>, Types...>)
        : m_tail(integral_constant<size_t, Idx - 1>{}, static_cast<Types&&>(args)...)
    {
    }

    variant_storage_impl(variant_storage_impl&&)      = default;
    variant_storage_impl(const variant_storage_impl&) = default;
    variant_storage_impl& operator=(variant_storage_impl&&) = default;
    variant_storage_impl& operator=(const variant_storage_impl&) = default;

    DECL_NODISCARD DECL_CONSTEXPR11 First& get() & DECL_NOEXCEPT
    {
        return m_head;
    }
    DECL_NODISCARD DECL_CONSTEXPR11 const First& get() const& DECL_NOEXCEPT
    {
        return m_head;
    }
    DECL_NODISCARD DECL_CONSTEXPR11 First&& get() && DECL_NOEXCEPT
    {
        return std::move(m_head);
    }
    DECL_NODISCARD DECL_CONSTEXPR11 const First&& get() const&& DECL_NOEXCEPT
    {
        return std::move(m_head);
    }
};
#pragma endregion

#pragma region variant base

template <class... Types>
class variant_base : private variant_storage<Types...>
{
public:
    static DECL_CONSTEXPR11 size_t type_count = sizeof...(Types);
    using index_type                          = conditional_t<
        (type_count < static_cast<size_t>((numeric_limits<signed char>::max)())),
        signed char,
        conditional_t<
            (type_count < static_cast<size_t>((numeric_limits<short>::max)())),
            short,
            int>>;

    using type_list = meta_list<Types...>;

    static DECL_CONSTEXPR11 auto invalid_index = static_cast<index_type>(-1);
    index_type m_index;

    using storage_type = variant_storage<Types...>;

    DECL_NODISCARD DECL_CONSTEXPR11 storage_type& get_storage() & DECL_NOEXCEPT
    {
        return *this;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const storage_type& get_storage() const& DECL_NOEXCEPT
    {
        return *this;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 storage_type&& get_storage() && DECL_NOEXCEPT
    {
        return std::move(*this);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const storage_type&& get_storage() const&& DECL_NOEXCEPT
    {
        return std::move(*this);
    }

    DECL_CONSTEXPR11 variant_base() DECL_NOEXCEPT
        : storage_type{},
          m_index{invalid_index} {}

    template <size_t Idx,
              class... Args,
              class = enable_if_t<is_constructible_v<meta_list_at_t<type_list, Idx>, Args...>>>
    DECL_CONSTEXPR11 explicit variant_base(in_place_index_t<Idx>, Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<meta_list_at_t<type_list, Idx>, Args...>)
        : storage_type(integral_constant<size_t, Idx>{}, static_cast<Args&&>(args)...),
          m_index{static_cast<index_type>(Idx)}
    {
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool valueless_by_exception() const DECL_NOEXCEPT
    {
        return m_index < 0;
    }
    DECL_NODISCARD DECL_CONSTEXPR11 size_t index() const DECL_NOEXCEPT
    {
        return static_cast<size_t>(m_index);
    }
    DECL_CONSTEXPR11 void set_index(const size_t idx) DECL_NOEXCEPT
    {
        m_index = static_cast<index_type>(idx);
    }

    template <size_t Idx>
    DECL_CONSTEXPR11 void destory_variant() DECL_NOEXCEPT
    {
        if constexpr (Idx != variant_npos && !is_trivially_destructible_v<meta_list_at_t<type_list, Idx>>)
        {
            destroy_in_place(variant_raw_get<Idx>(get_storage()));
        }
    }

    DECL_CONSTEXPR11 void destory_variant() DECL_NOEXCEPT
    {
        if constexpr (!conjunction_v<is_trivially_destructible<Types>...>)
        {
            variant_raw_visit(index(), get_storage(), [](auto ref) DECL_NOEXCEPT {
                if constexpr (ref.index != variant_npos)
                {
                    destroy_in_place(ref.value);
                }
            });
        }
    }

    DECL_CONSTEXPR11 void reset() DECL_NOEXCEPT
    {
        destory_variant();
        set_index(variant_npos);
    }

    template <size_t Idx>
    DECL_CONSTEXPR11 void reset() DECL_NOEXCEPT
    {
        if constexpr (Idx != variant_npos)
        {
            destory_variant<Idx>();
            set_index(variant_npos);
        }
    }

    DECL_CONSTEXPR11 void construct_from(const variant_base& other) DECL_NOEXCEPT(conjunction_v<is_nothrow_copy_constructible<Types>...>)
    {
        variant_raw_visit(other.index(), other.get_storage(), VariantConstructVisitor<Types...>{*this});
    }

    DECL_CONSTEXPR11 void construct_from(variant_base&& other) DECL_NOEXCEPT(conjunction_v<is_nothrow_move_constructible<Types>...>)
    {
        variant_raw_visit(other.index(), std::move(other).get_storage(), VariantConstructVisitor<Types...>{*this});
    }

    DECL_CONSTEXPR11 void assign_from(const variant_base& other) DECL_NOEXCEPT(conjunction_v<is_nothrow_copy_constructible<Types>..., is_nothrow_copy_assignable<Types>...>)
    {
        variant_raw_visit(other.index(), other.get_storage(), variant_assign_visitor<Types...>{*this});
    }

    DECL_CONSTEXPR11 void assign_from(variant_base&& other) DECL_NOEXCEPT(conjunction_v<is_nothrow_move_constructible<Types>..., is_nothrow_move_assignable<Types>...>)
    {
        variant_raw_visit(other.index(), std::move(other).get_storage(), variant_assign_visitor<Types...>{*this});
    }
};

template <class... Types>
struct VariantDestructionFacadeImpl : variant_base<Types...>
{
    using Mybase = variant_base<Types...>;

    using Mybase::Mybase;

    using Mybase::assign_from;
    using Mybase::construct_from;
    using Mybase::destory_variant;
    using Mybase::get_storage;
    using Mybase::index;
    using Mybase::reset;
    using Mybase::set_index;
    using Mybase::valueless_by_exception;

    DECL_CONSTEXPR20 ~VariantDestructionFacadeImpl() DECL_NOEXCEPT
    {
        destory_variant();
    }

    DECL_CONSTEXPR11 VariantDestructionFacadeImpl()                                    = default;
    DECL_CONSTEXPR11 VariantDestructionFacadeImpl(const VariantDestructionFacadeImpl&) = default;
    DECL_CONSTEXPR11 VariantDestructionFacadeImpl(VariantDestructionFacadeImpl&&)      = default;
    DECL_CONSTEXPR11 VariantDestructionFacadeImpl& operator=(const VariantDestructionFacadeImpl&) = default;
    DECL_CONSTEXPR11 VariantDestructionFacadeImpl& operator=(VariantDestructionFacadeImpl&&) = default;
};

template <class... Types>
using VariantDestructionFacade = conditional_t<conjunction_v<is_trivially_destructible<Types>...>,
                                               variant_base<Types...>,
                                               VariantDestructionFacadeImpl<Types...>>;
#pragma endregion

#pragma region variant other
template <class... Types, enable_if_t<conjunction_v<is_move_constructible<Types>..., is_swappable<Types>...>, int> = 0>
DECL_CONSTEXPR20 void swap(variant<Types...>& l, variant<Types...>& r) noexcept(noexcept(l.swap(r)))
{
    l.swap(r);
}

template <class Oper, class Result, class... Types>
struct variant_relop_visitor
{
    const variant_storage<Types...>& l;

    template <class T, size_t Idx>
    DECL_NODISCARD constexpr Result operator()(variant_tagged<const T&, Idx> r) const DECL_NOEXCEPT(disjunction_v<bool_constant<Idx == variant_npos>, is_nothrow_invocable_r<Result, Oper, const T&, const T&>>)
    {
        if constexpr (Idx != variant_npos)
        {
            return Oper{}(variant_raw_get<Idx>(l), r.value);
        }
        else
        {
            return Oper{}(0, 0);
        }
    }
};
#pragma endregion

#pragma region variant
template <class... Types>
class variant : private FacadeControl<VariantDestructionFacade<Types...>, Types...>
{
public:
    static_assert(conjunction_v<is_object<Types>..., negation<is_array<Types>>..., is_destructible<Types>...>,
                  "variant<Types...> requires all of the Types to meet the Cpp17Destructible requirements "
                  "N4828 [variant.variant]/2.");
    static_assert(sizeof...(Types) > 0,
                  "variant<> (with no template arguments) may not be instantiated (N4835 [variant.variant]/3).");

    using Mybase    = FacadeControl<VariantDestructionFacade<Types...>, Types...>;
    using type_list = meta_list<Types...>;

    // using Mybase::Mybase;

    using Mybase::m_index;

    using Mybase::assign_from;
    using Mybase::construct_from;
    using Mybase::destory_variant;
    using Mybase::get_storage;
    using Mybase::index;
    using Mybase::reset;
    using Mybase::set_index;
    using Mybase::valueless_by_exception;

    template <class First = meta_list_front_t<variant>, enable_if_t<is_default_constructible_v<First>, int> = 0>
    DECL_CONSTEXPR11 variant() DECL_NOEXCEPT(is_nothrow_default_constructible_v<First>)
        : Mybase(in_place_index<0>)
    {
    }

    template <class T, class = enable_if_t<sizeof...(Types) != 0                                       //
                                           && !is_same_v<remove_cvref_t<T>, variant>                   //
                                           && !is_specialization_v<remove_cvref_t<T>, in_place_type_t> //
                                           && !is_in_place_index_specialization<remove_cvref_t<T>>     //
                                           && is_constructible_v<variant_init_t<T, Types...>, T>>>
    DECL_CONSTEXPR11 variant(T&& obj) DECL_NOEXCEPT(is_nothrow_constructible_v<variant_init_t<T, Types...>, T>)
        : Mybase(in_place_index<variant_initidx<T, Types...>::value>, static_cast<T&&>(obj))
    {
    }

    template <class T, class... Args, class Idx = meta_list_find_unique_index_t<variant, T>, enable_if_t<Idx::value != variant_npos && is_constructible_v<T, Args...>, int> = 0>
    DECL_CONSTEXPR11 explicit variant(in_place_type_t<T>, Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<T, Args...>)
        : Mybase(in_place_index<Idx::value>, static_cast<Args&&>(args)...)
    {
    }

    template <class T, class Elem, class... Args, class Idx = meta_list_find_unique_index_t<variant, T>, enable_if_t<Idx::value != variant_npos && is_constructible_v<T, initializer_list<Elem>&, Args...>, int> = 0>
    DECL_CONSTEXPR11 explicit variant(in_place_type_t<T>, initializer_list<Elem> initList, Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<T, initializer_list<Elem>&, Args...>)
        : Mybase(in_place_index<Idx::value>, initList, static_cast<Args&&>(args)...)
    {
    }

    template <size_t Idx, class... Args, enable_if_t<is_constructible_v<meta_list_at_t<type_list, Idx>, Args...>, int> = 0>
    DECL_CONSTEXPR11 explicit variant(in_place_index_t<Idx>, Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<meta_list_at_t<type_list, Idx>, Args...>)
        : Mybase(in_place_index<Idx>, static_cast<Args&&>(args)...)
    {
    }

    template <size_t Idx, class Elem, class... Args, enable_if_t<is_constructible_v<meta_list_at_t<type_list, Idx>, initializer_list<Elem>&, Args...>, int> = 0>
    DECL_CONSTEXPR11 explicit variant(in_place_index_t<Idx>, initializer_list<Elem> initList, Args&&... args) DECL_NOEXCEPT(is_constructible_v<meta_list_at_t<type_list, Idx>, initializer_list<Elem>&, Args...>)
        : Mybase(in_place_index<Idx>, initList, static_cast<Args&&>(args)...)
    {
    }

    template <class T, enable_if_t<!is_same_v<remove_cvref_t<T>, variant>                    //
                                       && is_constructible_v<variant_init_t<T, Types...>, T> //
                                       && is_assignable_v<variant_init_t<T, Types...>&, T>,  //
                                   int> = 0>
    DECL_CONSTEXPR11 variant& operator=(T&& obj) DECL_NOEXCEPT(is_nothrow_assignable_v<variant_init_t<T, Types...>&, T>&& is_nothrow_constructible_v<variant_init_t<T, Types...>, T>)
    {
        constexpr size_t targetIdx = variant_initidx<T, Types...>::value;
        if (index() == targetIdx)
        {
            auto& target = variant_raw_get<targetIdx>(get_storage());
            target       = static_cast<T&&>(obj);
        }
        else
        {
            using target_type = variant_init_t<T, Types...>;
            if constexpr (is_variant_should_directly_construct_v<target_type, T>)
            {
                reset();
                EmplaceValueless<targetIdx>(static_cast<T&&>(obj));
            }
            else
            {
                target_type temp(static_cast<T&&>(obj));
                reset();
                EmplaceValueless<targetIdx>(std::move(temp));
            }
        }
        return *this;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool operator==(const variant& r) const DECL_NOEXCEPT(conjunction_v<is_nothrow_invocable_r<bool, equal_to<>, const Types&, const Types&>...>)
    {
        using visitor       = variant_relop_visitor<equal_to<>, bool, Types...>;
        const size_t rIndex = r.index();
        return index() == rIndex && variant_raw_visit(rIndex, r.get_storage(), visitor{get_storage()});
    }

    template <class... Types>
    DECL_NODISCARD DECL_CONSTEXPR11 bool operator!=(const variant<Types...>& r) const DECL_NOEXCEPT(conjunction_v<is_nothrow_invocable_r<bool, not_equal_to<>, const Types&, const Types&>...>)
    {
        using visitor       = variant_relop_visitor<not_equal_to<>, bool, Types...>;
        const size_t rIndex = r.index();
        return index() != rIndex || variant_raw_visit(rIndex, r.get_storage(), visitor{get_storage()});
    }

    template <class... Types>
    DECL_NODISCARD DECL_CONSTEXPR11 bool operator<(const variant<Types...>& r) const DECL_NOEXCEPT(conjunction_v<is_nothrow_invocable_r<bool, less<>, const Types&, const Types&>...>)
    {
        using visitor        = variant_relop_visitor<less<>, bool, Types...>;
        const size_t lOffset = index() + 1;
        const size_t rOffset = r.index() + 1;
        return lOffset < rOffset || (lOffset == rOffset && variant_raw_visit(rOffset - 1, r.get_storage(), visitor{get_storage()}));
    }

    template <class... Types>
    DECL_NODISCARD DECL_CONSTEXPR11 bool operator>(const variant<Types...>& r) const DECL_NOEXCEPT(conjunction_v<is_nothrow_invocable_r<bool, greater<>, const Types&, const Types&>...>)
    {
        using visitor        = variant_relop_visitor<greater<>, bool, Types...>;
        const size_t lOffset = index() + 1;
        const size_t rOffset = r.index() + 1;
        return lOffset > rOffset || (lOffset == rOffset && variant_raw_visit(rOffset - 1, r.get_storage(), visitor{get_storage()}));
    }

    template <class... Types>
    DECL_NODISCARD DECL_CONSTEXPR11 bool operator<=(const variant<Types...>& r) const DECL_NOEXCEPT(conjunction_v<is_nothrow_invocable_r<bool, less_equal<>, const Types&, const Types&>...>)
    {
        using visitor        = variant_relop_visitor<less_equal<>, bool, Types...>;
        const size_t lOffset = index() + 1;
        const size_t rOffset = r.index() + 1;
        return lOffset < rOffset || (lOffset == rOffset && variant_raw_visit(rOffset - 1, r.get_storage(), visitor{get_storage()}));
    }

    template <class... Types>
    DECL_NODISCARD DECL_CONSTEXPR11 bool operator>=(const variant<Types...>& r) const DECL_NOEXCEPT(conjunction_v<is_nothrow_invocable_r<bool, greater_equal<>, const Types&, const Types&>...>)
    {
        using visitor        = variant_relop_visitor<greater_equal<>, bool, Types...>;
        const size_t lOffset = index() + 1;
        const size_t rOffset = r.index() + 1;
        return lOffset > rOffset || (lOffset == rOffset && variant_raw_visit(rOffset - 1, r.get_storage(), visitor{get_storage()}));
    }

    template <class T, class... Args, size_t Idx = meta_list_find_unique_index_t<variant, T>::value, enable_if_t<Idx != variant_npos && is_constructible_v<T, Args...>, int> = 0>
    DECL_CONSTEXPR11 T& emplace(Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<T, Args...>)
    {
        reset();
        return EmplaceValueless<Idx>(static_cast<Args&&>(args)...);
    }

    template <class T, class Elem, class... Args, size_t Idx = meta_list_find_unique_index_t<variant, T>::value, enable_if_t<Idx != variant_npos && is_constructible_v<T, initializer_list<Elem>&, Args...>, int> = 0>
    DECL_CONSTEXPR11 T& emplace(initializer_list<Elem> initList, Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<T, initializer_list<Elem>&, Args...>)
    {
        reset();
        return EmplaceValueless<Idx>(initList, static_cast<Args&&>(args)...);
    }

    template <size_t Idx, class... Args, enable_if_t<is_constructible_v<meta_list_at_t<type_list, Idx>, Args...>, int> = 0>
    DECL_CONSTEXPR11 meta_list_at_t<type_list, Idx>& emplace(Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<meta_list_at_t<type_list, Idx>, Args...>)
    {
        reset();
        return EmplaceValueless<Idx>(static_cast<Args&&>(args)...);
    }

    template <size_t Idx, class Elem, class... Args, enable_if_t<is_constructible_v<meta_list_at_t<type_list, Idx>, initializer_list<Elem>&, Args...>, int> = 0>
    DECL_CONSTEXPR11 meta_list_at_t<type_list, Idx>& emplace(initializer_list<Elem> initList, Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<meta_list_at_t<type_list, Idx>, initializer_list<Elem>&, Args...>)
    {
        reset();
        return EmplaceValueless<Idx>(initList, static_cast<Args&&>(args)...);
    }

    DECL_CONSTEXPR11 void swap(variant& other) DECL_NOEXCEPT(conjunction_v<is_nothrow_move_constructible<Types>..., is_nothrow_swappable<Types>...>)
    {
        static_assert(conjunction_v<is_move_constructible<Types>...>,
                      "variant<Types...>::swap requires all of the Types... to be move constructible. (N4835 [variant.swap]/1)");
        static_assert(disjunction_v<negation<is_move_constructible<Types>>..., conjunction<is_swappable<Types>...>>,
                      "variant<Types...>::swap requires all of the Types... to be swappable. (N4835 [variant.swap]/1)");

        if constexpr (conjunction_v<_Is_trivially_swappable<Types>...>)
        {
            using variant_base_type = variant_base<Types...>;
            std::swap(static_cast<variant_base_type&>(*this), static_cast<variant_base_type&>(other));
        }
        else if constexpr (sizeof...(Types) < 32)
        {
            variant_raw_visit(index(), get_storage(), [this, &other](auto thisRef) DECL_NOEXCEPT(conjunction_v<is_nothrow_move_constructible<Types>..., is_nothrow_swappable<Types>...>) {
                variant_raw_visit(other.index(), other.get_storage(), [this, &other, thisRef](auto otherRef) DECL_NOEXCEPT(conjunction_v<is_nothrow_move_constructible<Types>..., is_nothrow_swappable<Types>...>) {
                    constexpr size_t otherIndex = otherRef.index;
                    constexpr size_t thisIndex  = thisRef.index;

                    if constexpr (thisIndex == otherIndex)
                    {
                        if constexpr (thisIndex != variant_npos)
                        {
                            swap_adl(thisRef.value, otherRef.value);
                        }
                    }
                    else if constexpr (thisIndex == variant_npos)
                    {
                        EmplaceValueless<otherIndex>(std::move(otherRef.value));
                        other.reset<otherIndex>();
                    }
                    else if constexpr (otherIndex == variant_npos)
                    {
                        other.EmplaceValueless<thisIndex>(std::move(thisRef.value));
                        this->template reset<thisIndex>();
                    }
                    else
                    {
                        auto temp = std::move(thisRef.value);
                        this->template reset<thisIndex>();
                        EmplaceValueless<otherIndex>(std::move(otherRef.value));
                        other.reset<otherIndex>();
                        other.EmplaceValueless<thisIndex>(std::move(temp));
                    }
                });
            });
        }
        else
        {
            if (m_index == other.m_index)
            {
                variant_raw_visit(static_cast<size_t>(m_index), other.get_storage(), [this](auto ref) DECL_NOEXCEPT(conjunction_v<is_nothrow_swappable<Types>...>) {
                    constexpr size_t index = ref.index;
                    if constexpr (index != variant_npos)
                    {
                        swap_adl(variant_raw_get<index>(get_storage()), ref.value);
                    }
                });
            }
            else
            {
                variant temp = std::move(*this);
                EmplaceFrom(std::move(other));
                other.EmplaceFrom(std::move(temp));
            }
        }
    }

private:
    template <size_t Idx, class... Args>
    DECL_CONSTEXPR11 meta_list_at_t<type_list, Idx>& EmplaceValueless(Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible_v<meta_list_at_t<type_list, Idx>, Args...>)
    {
        construct_in_place(get_storage(), integral_constant<size_t, Idx>{}, static_cast<Args&&>(args)...);
        set_index(Idx);
        return variant_raw_get<Idx>(get_storage());
    }

    DECL_CONSTEXPR11 void EmplaceFrom(variant&& other) DECL_NOEXCEPT(conjunction_v<is_nothrow_move_constructible<Types>...>)
    {
        reset();
        variant_raw_visit(other.index(), other.get_storage(), [this](auto ref) DECL_NOEXCEPT(conjunction_v<is_nothrow_move_constructible<Types>...>) {
            constexpr size_t index = ref.index;
            if constexpr (index != variant_npos)
            {
                EmplaceValueless<index>(std::move(ref.value));
            }
        });
    }
};
#pragma endregion

#pragma region variant function
#pragma region get
template <size_t Idx, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(variant<Types...>& var)
{
    static_assert(Idx < sizeof...(Types), "variant index out of bounds");
    if (var.index() == Idx)
    {
        return variant_raw_get<Idx>(var.get_storage());
    }
    throw_bad_variant_access();
}
template <size_t Idx, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(variant<Types...>&& var)
{
    static_assert(Idx < sizeof...(Types), "variant index out of bounds");
    if (var.index() == Idx)
    {
        return variant_raw_get<Idx>(std::move(var).get_storage());
    }
    throw_bad_variant_access();
}
template <size_t Idx, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(const variant<Types...>& var)
{
    static_assert(Idx < sizeof...(Types), "variant index out of bounds");
    if (var.index() == Idx)
    {
        return variant_raw_get<Idx>(var.get_storage());
    }
    throw_bad_variant_access();
}
template <size_t Idx, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(const variant<Types...>&& var)
{
    static_assert(Idx < sizeof...(Types), "variant index out of bounds");
    if (var.index() == Idx)
    {
        return variant_raw_get<Idx>(std::move(var).get_storage());
    }
    throw_bad_variant_access();
}

template <class T, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(variant<Types...>& var)
{
    constexpr size_t Idx = std::meta_list_find_unique_index_t<variant<Types...>, T>::value;
    static_assert(Idx < sizeof...(Types), "get<T>(variant<Types...>&) requires T to occur exactly once in Types. (N4835 [variant.get]/5)");
    return std::get<Idx>(var);
}
template <class T, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(variant<Types...>&& var)
{
    constexpr size_t Idx = std::meta_list_find_unique_index_t<variant<Types...>, T>::value;
    static_assert(Idx < sizeof...(Types), "get<T>(variant<Types...>&&) requires T to occur exactly once in Types. (N4835 [variant.get]/5)");
    return std::get<Idx>(std::move(var));
}
template <class T, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(const variant<Types...>& var)
{
    constexpr size_t Idx = std::meta_list_find_unique_index_t<variant<Types...>, T>::value;
    static_assert(Idx < sizeof...(Types), "get<T>(const variant<Types...>&) requires T to occur exactly once in Types. (N4835 [variant.get]/5)");
    return std::get<Idx>(var);
}
template <class T, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 decltype(auto) get(const variant<Types...>&& var)
{
    constexpr size_t Idx = std::meta_list_find_unique_index_t<variant<Types...>, T>::value;
    static_assert(Idx < sizeof...(Types), "get<T>(const variant<Types...>&&) requires T to occur exactly once in Types. (N4835 [variant.get]/5)");
    return std::get<Idx>(std::move(var));
}
#pragma endregion

#pragma region get_if
template <size_t Idx, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 auto get_if(variant<Types...>* ptr) DECL_NOEXCEPT
{
    static_assert(Idx < sizeof...(Types), "variant index out of bounds");
    return ptr && ptr->index() == Idx ? std::addressof(variant_raw_get<Idx>(ptr->get_storage())) : nullptr;
}
template <size_t Idx, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 auto get_if(const variant<Types...>* ptr) DECL_NOEXCEPT
{
    static_assert(Idx < sizeof...(Types), "variant index out of bounds");
    return ptr && ptr->index() == Idx ? std::addressof(variant_raw_get<Idx>(ptr->get_storage())) : nullptr;
}

template <class T, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 add_pointer_t<T> get_if(variant<Types...>* ptr) DECL_NOEXCEPT
{
    constexpr size_t Idx = meta_list_find_unique_index_t<variant<Types...>, T>::value;
    static_assert(Idx != variant_npos, "get_if<T>(variant<Types...> *) requires T to occur exactly once in Types. (N4835 [variant.get]/9)");
    return std::get_if<Idx>(ptr);
}
template <class T, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 add_pointer_t<const T> get_if(const variant<Types...>* ptr) DECL_NOEXCEPT
{
    constexpr size_t Idx = meta_list_find_unique_index_t<variant<Types...>, T>::value;
    static_assert(Idx != variant_npos, "get_if<T>(const variant<Types...> *) requires T to occur exactly once in Types. (N4835 [variant.get]/9)");
    return std::get_if<Idx>(ptr);
}
#pragma endregion

#pragma region holds_alternative
template <class T, class... Types>
DECL_NODISCARD DECL_CONSTEXPR11 bool holds_alternative(const variant<Types...>& var) DECL_NOEXCEPT
{
    constexpr size_t Idx = std::meta_list_find_unique_index_t<variant<Types...>, T>::value;
    static_assert(Idx != meta_list_npos, "holds_alternative<T>(const variant<Types...>&) requires T to occur exactly "
                                         "once in Types. (N4835 [variant.get]/1)");
    return var.index() == Idx;
}
#pragma endregion

#pragma endregion

#pragma region hash
struct variant_hash
{
    template <class T, size_t Idx>
    DECL_NODISCARD size_t operator()(variant_tagged<const T&, Idx> obj) const DECL_NOEXCEPT(disjunction_v<bool_constant<Idx == variant_npos>, is_nothrow_invocable<hash<T>, const T&>>)
    {
        if constexpr (Idx == variant_npos)
        {
            return 0;
        }
        else
        {
            return hash<T>{}(obj.value);
        }
    }
};

template <class... Types>
struct hash<variant<Types...>> : _Conditionally_enabled_hash<variant<Types...>, conjunction_v<is_default_constructible<hash<remove_const_t<Types>>>...>>
{
    using key_type    = variant<Types...>;
    using result_type = size_t;

    DECL_NODISCARD size_t operator()(const key_type& var) const DECL_NOEXCEPT
    {
        return variant_raw_visit(var.index(), var.get_storage(), variant_hash{});
    }
};

template <>
struct hash<monostate>
{
    using argument_type = monostate;
    using result_type   = size_t;

    DECL_NODISCARD size_t operator()(argument_type) const DECL_NOEXCEPT
    {
        return 1729;
    }
};

DECL_NODISCARD DECL_CONSTEXPR11 bool operator==(monostate, monostate) DECL_NOEXCEPT
{
    return true;
}
DECL_NODISCARD DECL_CONSTEXPR11 bool operator!=(monostate, monostate) DECL_NOEXCEPT
{
    return false;
}
DECL_NODISCARD DECL_CONSTEXPR11 bool operator<(monostate, monostate) DECL_NOEXCEPT
{
    return false;
}
DECL_NODISCARD DECL_CONSTEXPR11 bool operator>(monostate, monostate) DECL_NOEXCEPT
{
    return false;
}
DECL_NODISCARD DECL_CONSTEXPR11 bool operator<=(monostate, monostate) DECL_NOEXCEPT
{
    return true;
}
DECL_NODISCARD DECL_CONSTEXPR11 bool operator>=(monostate, monostate) DECL_NOEXCEPT
{
    return true;
}
#pragma endregion

#pragma region variant_alternative
template <size_t Idx, class T>
struct variant_alternative;
template <size_t Idx, class T>
using variant_alternative_t = typename variant_alternative<Idx, T>::type;
template <size_t Idx, class T>
struct variant_alternative<Idx, const T>
{
    using type = add_const_t<variant_alternative_t<Idx, T>>;
};
template <size_t Idx, class T>
struct variant_alternative<Idx, volatile T>
{
    using type = add_volatile_t<variant_alternative_t<Idx, T>>;
};
template <size_t Idx, class T>
struct variant_alternative<Idx, const volatile T>
{
    using type = add_cv_t<variant_alternative_t<Idx, T>>;
};
template <size_t Idx, class... Types>
struct variant_alternative<Idx, variant<Types...>>
{
    static_assert(Idx < sizeof...(Types), "variant index out of bounds");
    using type = meta_list_at_t<variant<Types...>, Idx>;
};
#pragma endregion

#pragma region variant_size
template <class T>
struct variant_size;
template <class T>
struct variant_size<const T> : variant_size<T>::type
{
};
template <class T>
struct variant_size<volatile T> : variant_size<T>::type
{
};
template <class T>
struct variant_size<const volatile T> : variant_size<T>::type
{
};

template <class... Types>
struct variant_size<variant<Types...>> : integral_constant<size_t, sizeof...(Types)>
{
};
template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 size_t variant_size_v = variant_size<T>::value;
#pragma endregion

#pragma region visit

template <class Callable, class IndexSequence, class... Variants>
struct variant_single_visit_result;

template <class Callable, size_t... Idxs, class... Variants>
struct variant_single_visit_result<Callable, index_sequence<Idxs...>, Variants...>
{
    using type = decltype(std::invoke(std::declval<Callable>(), variant_raw_get<Idxs == 0 ? 0 : Idxs - 1>(std::declval<Variants>().get_storage())...));
};

template <class...>
struct all_same : true_type
{
};
template <class First, class... Rest>
struct all_same<First, Rest...> : bool_constant<conjunction_v<is_same<First, Rest>...>>
{
};

template <class Callable, class ListOfIndexVectors, class... Variants>
struct variant_all_visit_results_same;

template <class Callable, class... IndexVectors, class... Variants>
struct variant_all_visit_results_same<Callable, meta_list<IndexVectors...>, Variants...> : all_same<typename variant_single_visit_result<Callable, IndexVectors, Variants...>::type...>::type
{
};

template <class Callable, class... Types>
using variant_visit_result_t = decltype(std::invoke(std::declval<Callable>(), variant_raw_get<0>(std::declval<Types>().get_storage())...));

template <class... Types>
variant<Types...>& as_variant_impl(variant<Types...>&);
template <class... Types>
const variant<Types...>& as_variant_impl(const variant<Types...>&);
template <class... Types>
variant<Types...>&& as_variant_impl(variant<Types...>&&);
template <class... Types>
const variant<Types...>&& as_variant_impl(const variant<Types...>&&);
template <class T>
using as_variant = decltype(as_variant_impl(std::declval<T>()));

template <class... Variants>
DECL_INLINE_VAR DECL_CONSTEXPR11 size_t variant_total_states_impl = 1;

template <class FirstVariant, class... RestVariant>
DECL_INLINE_VAR DECL_CONSTEXPR11 size_t variant_total_states_impl<FirstVariant, RestVariant...> =
    (variant_size_v<FirstVariant> + 1) * variant_total_states_impl<RestVariant...>;

template <class... Variants>
DECL_INLINE_VAR DECL_CONSTEXPR11 size_t variant_total_states = variant_total_states_impl<Variants...>;

DECL_NODISCARD constexpr size_t variant_visit_index(const size_t acc) noexcept
{
    return acc;
}
template <class FirstTy, class... RestTys>
DECL_NODISCARD constexpr size_t variant_visit_index(size_t acc, const FirstTy& first, const RestTys&... rest) noexcept
{
    acc += (first.index() + 1) * variant_total_states<RestTys...>;
    return variant_visit_index(acc, rest...);
}

template <class>
struct variant_dispatcher;

template <size_t... I>
struct variant_dispatcher<index_sequence<I...>>
{
    template <class Ret, class Callable, class... Types, bool AnyValueless = I == 0>
    DECL_NODISCARD static constexpr Ret Dispatch2(Callable&& obj, Types&&... args)
    {
        if constexpr (AnyValueless)
        {
            throw_bad_variant_access();
        }
        else
        {
            return std::invoke(static_cast<Callable&&>(obj), variant_raw_get<I - 1>(static_cast<Types&&>(args).get_storage())...);
        }
    }
};

template <class Ret, class Ordinals, class Callable, class Variants>
struct variant_dispatch_table;

template <class Ret, class... Ordinals, class Callable, class... Variants>
struct variant_dispatch_table<Ret, meta_list<Ordinals...>, Callable, meta_list<Variants...>>
{
    using Dispatch_Type                    = Ret (*)(Callable&&, Variants&&...);
    static constexpr Dispatch_Type Array[] = {&variant_dispatcher<Ordinals>::template Dispatch2<Ret, Callable, Variants...>...};
};

template <int Strategy>
struct visit_strategy;

template <>
struct visit_strategy<-1>
{
    template <class Ret, class ListOfIndexVectors, class Callable, class... Variants>
    static constexpr Ret visit(size_t idx, Callable&& obj, Variants&&... args)
    {
        constexpr size_t size = variant_total_states<remove_cvref_t<Variants>...>;
        static_assert(size > 256);
        constexpr auto& Array = variant_dispatch_table<Ret, ListOfIndexVectors, Callable, meta_list<Variants...>>::Array;
        return Array[idx](static_cast<Callable&&>(obj), static_cast<Variants&&>(args)...);
    }
};

template <>
struct visit_strategy<0>
{
    template <class Ret, class, class Callable>
    static constexpr Ret visit(size_t, Callable&& obj)
    {
        if constexpr (is_void_v<Ret>)
        {
            return static_cast<void>(static_cast<Callable&&>(obj)());
        }
        else
        {
            return static_cast<Callable&&>(obj)();
        }
    }
};

#define VISIT_CASE(n)                                                                           \
    case (n):                                                                                   \
        if constexpr ((n) < size)                                                               \
        {                                                                                       \
            using Indices = meta_list_at_t<ListOfIndexVectors, (n)>;                            \
            return variant_dispatcher<Indices>::template Dispatch2<Ret, Callable, Variants...>( \
                static_cast<Callable&&>(obj), static_cast<Variants&&>(args)...);                \
        }                                                                                       \
        std::unreachable();

#define VISIT_STAMP(stamper, n)                                                \
    constexpr size_t size = variant_total_states<remove_cvref_t<Variants>...>; \
    static_assert(size > (n) / 4 && size <= (n));                              \
    switch (idx)                                                               \
    {                                                                          \
        stamper(0, VISIT_CASE);                                                \
    default:                                                                   \
        std::unreachable();                                                    \
    }

#define VARIANT_STAMP4(n, x) \
    x(n);                    \
    x(n + 1);                \
    x(n + 2);                \
    x(n + 3)

#define VARIANT_STAMP16(n, x) \
    VARIANT_STAMP4(n, x);     \
    VARIANT_STAMP4(n + 4, x); \
    VARIANT_STAMP4(n + 8, x); \
    VARIANT_STAMP4(n + 12, x)

#define VARIANT_STAMP64(n, x)   \
    VARIANT_STAMP16(n, x);      \
    VARIANT_STAMP16(n + 16, x); \
    VARIANT_STAMP16(n + 32, x); \
    VARIANT_STAMP16(n + 48, x)

#define VARIANT_STAMP256(n, x)   \
    VARIANT_STAMP64(n, x);       \
    VARIANT_STAMP64(n + 64, x);  \
    VARIANT_STAMP64(n + 128, x); \
    VARIANT_STAMP64(n + 192, x)

#define VARIANT_STAMP(n, x) x(VARIANT_STAMP##n, n)

template <>
struct visit_strategy<1>
{
    template <class Ret, class ListOfIndexVectors, class Callable, class... Variants>
    static constexpr Ret visit(size_t idx, Callable&& obj, Variants&&... args)
    {
        VARIANT_STAMP(4, VISIT_STAMP);
    }
};

template <>
struct visit_strategy<2>
{
    template <class Ret, class ListOfIndexVectors, class Callable, class... Variants>
    static constexpr Ret visit(size_t idx, Callable&& obj, Variants&&... args)
    {
        VARIANT_STAMP(16, VISIT_STAMP);
    }
};

template <>
struct visit_strategy<3>
{
    template <class Ret, class ListOfIndexVectors, class Callable, class... Variants>
    static constexpr Ret visit(size_t idx, Callable&& obj, Variants&&... args)
    {
        VARIANT_STAMP(64, VISIT_STAMP);
    }
};

template <>
struct visit_strategy<4>
{
    template <class Ret, class ListOfIndexVectors, class Callable, class... Variants>
    static constexpr Ret visit(size_t idx, Callable&& obj, Variants&&... args)
    {
        VARIANT_STAMP(256, VISIT_STAMP);
    }
};

#undef VISIT_STAMP
#undef VISIT_CASE
#undef VARIANT_STAMP
#undef VARIANT_STAMP4
#undef VARIANT_STAMP16
#undef VARIANT_STAMP64
#undef VARIANT_STAMP256

template <size_t Size, class Ret, class ListOfIndexVectors, class Callable, class... Variants>
constexpr Ret visit_impl(Callable&& obj, Variants&&... args)
{
    constexpr int Strategy = Size == 1 ? 0 : Size <= 4 ? 1 :
                                         Size <= 16    ? 2 :
                                         Size <= 64    ? 3 :
                                         Size <= 256   ? 4 :
                                                         -1;
    return visit_strategy<Strategy>::template visit<Ret, ListOfIndexVectors>(variant_visit_index(0, static_cast<as_variant<Variants>&>(args)...), static_cast<Callable&&>(obj), static_cast<as_variant<Variants>&&>(args)...);
}

template <class Callable, class... Variants, class = void_t<as_variant<Variants>...>>
constexpr variant_visit_result_t<Callable, as_variant<Variants>...> visit(Callable&& obj, Variants&&... args)
{
    constexpr auto Size      = variant_total_states<remove_cvref_t<as_variant<Variants>>...>;
    using ListOfIndexLists   = meta_list<meta_as_list<make_index_sequence<1 + variant_size_v<remove_cvref_t<as_variant<Variants>>>>>...>;
    using ListOfIndexVectors = meta_transform_t<meta_func<meta_as_integer_sequence>, meta_cartesian_product<ListOfIndexLists>>;
    using Ret                = variant_visit_result_t<Callable, as_variant<Variants>...>;
    static_assert(variant_all_visit_results_same<Callable, ListOfIndexVectors, as_variant<Variants>...>::value,
                  "visit() requires the result of all potential invocations to have the same type and value category "
                  "(N4835 [variant.visit]/2).");

    return visit_impl<Size, Ret, ListOfIndexVectors>(static_cast<Callable&&>(obj), static_cast<Variants&&>(args)...);
}

#pragma endregion

} // namespace std
#endif
#endif // !_VARINT_H_
