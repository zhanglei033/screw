#pragma once
#ifndef _STL_H_
#define _STL_H_
#include "Platform.h"
#include "TypesDef.h"
#include <iterator>
#include <type_traits>
#include <utility>
// std的一些向前兼容和扩展

namespace std {
#pragma region compatible CXX17
#if STD_HAS_CXX17 == 0
#pragma region in_place_type
template <class>
struct in_place_type_t
{
    explicit in_place_type_t() = default;
};
template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 in_place_type_t<T> in_place_type{};

template <size_t>
struct in_place_index_t
{
    explicit in_place_index_t() = default;
};

template <size_t Idx>
DECL_INLINE_VAR DECL_CONSTEXPR11 in_place_index_t<Idx> in_place_index{};

template <class>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_in_place_index_specialization = false;
template <size_t Idx>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_in_place_index_specialization<in_place_index_t<Idx>> = true;
#pragma endregion

#pragma region destroy at
#if COMPILER_TYPE_TAG != COMPILER_MSVC_TAG
template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_array_v = is_array<T>::value;
#endif

template <class T>
DECL_CONSTEXPR11 void destroy_at(const T* localtion) DECL_NOEXCEPT
{
    if constexpr (is_array_v<T>)
    {
        auto first = std::begin(*localtion);
        auto last  = std::end(*localtion);
        for (; first != last; ++first)
        {
            std::destroy_at(first);
        }
    }
    else
    {
        localtion->~T();
    }
}

#pragma endregion

#pragma region invoke traits
template <class VoidType, class Callable>
struct invoke_traits_zero
{
    using is_invocable_t         = false_type;
    using is_nothrow_invocable_t = false_type;
    template <class R>
    using is_invocable_r_t = false_type;
    template <class R>
    using is_nothrow_invocable_r_t = false_type;
};

template <class VoidType, class... Types>
struct invoke_traits_nonzero
{
    using is_invocable_t         = false_type;
    using is_nothrow_invocable_t = false_type;
    template <class R>
    using is_invocable_r_t = false_type;
    template <class _R>
    using is_nothrow_invocable_r_t = false_type;
};

template <class Callable, class... Args>
using select_invoke_traits = conditional_t<sizeof...(Args) == 0, invoke_traits_zero<void, Callable>, invoke_traits_nonzero<void, Callable, Args...>>;

template <class Callable, class... Args>
struct invoke_result : select_invoke_traits<Callable, Args...>
{
};

template <class Callable, class... Args>
using invoke_result_t = typename select_invoke_traits<Callable, Args...>::type;

template <class R, class Callable, class... Args>
using is_invocable_r_t = typename select_invoke_traits<Callable, Args...>::template is_invocable_r_t<R>;

template <class Callable, class... Args>
struct is_invocable : select_invoke_traits<Callable, Args...>::is_invocable_t
{
};

template <class Callable, class... Args>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_invocable_v = select_invoke_traits<Callable, Args...>::is_invocable_t::value;

template <class Callable, class... Args>
struct is_nothrow_invocable : select_invoke_traits<Callable, Args...>::is_nothrow_invocable_t
{
};

template <class Callable, class... Args>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_nothrow_invocable_v = select_invoke_traits<Callable, Args...>::is_nothrow_invocable_t::value;

template <class R, class Callable, class... Args>
struct is_invocable_r : is_invocable_r_t<R, Callable, Args...>
{
};

template <class R, class Callable, class... Args>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_invocable_r_v = is_invocable_r_t<R, Callable, Args...>::value;

template <class R, class Callable, class... Args>
struct is_nothrow_invocable_r : select_invoke_traits<Callable, Args...>::template is_nothrow_invocable_r_t<R>
{
};
template <class R, class Callable, class... Args>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_nothrow_invocable_r_v = select_invoke_traits<Callable, Args...>::template is_nothrow_invocable_r_t<R>::value;
#pragma endregion

#pragma region swap
template <class T>
struct is_nothrow_swappable;

template <class T1, class T2, class = void>
struct swappable_with_helper : false_type
{
};

template <class T1, class T2>
struct swappable_with_helper<T1, T2, void_t<decltype(swap(std::declval<T1>(), std::declval<T2>()))>> : true_type
{
};

#if COMPILER_TYPE_TAG != COMPILER_MSVC_TAG
template <bool FirstValue, class First, class... Rest>
struct conjunction_impl
{
    using type = First;
};

template <class True, class Next, class... Rest>
struct conjunction_impl<true, True, Next, Rest...>
{
    using type = typename conjunction_impl<Next::value, Next, Rest...>::type;
};

template <class... Traits>
struct conjunction : true_type
{
};
template <class First, class... Rest>
struct conjunction<First, Rest...> : conjunction_impl<First::value, First, Rest...>::type
{
};

template <class... Traits>
DECL_INLINE_VAR constexpr bool conjunction_v = conjunction<Traits...>::value;

template <bool Val>
using bool_constant = integral_constant<bool, Val>;
#endif

#if COMPILER_TYPE_TAG == COMPILER_MSVC_TAG
template <class T1, class T2>
struct is_swappable_with : bool_constant<conjunction_v<swappable_with_helper<T1, T2>, swappable_with_helper<T2, T1>>>
{
};
template <class T>
struct is_swappable : is_swappable_with<add_lvalue_reference_t<T>, add_lvalue_reference_t<T>>::type
{
};

template <class T1, class T2>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_swappable_with_v = conjunction_v<swappable_with_helper<T1, T2>, swappable_with_helper<T2, T1>>;

template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_swappable_v = is_swappable<T>::value;

#endif

template <class T1, class T2>
struct swap_cannot_throw : bool_constant<DECL_NOEXCEPT(swap(std::declval<T1>(), std::declval<T2>())) //
                                             && DECL_NOEXCEPT(swap(std::declval<T2>(), std::declval<T1>()))>
{
};

#if COMPILER_TYPE_TAG == COMPILER_MSVC_TAG
template <class T1, class T2>
struct is_nothrow_swappable_with : bool_constant<conjunction_v<is_swappable_with<T1, T2>, swap_cannot_throw<T1, T2>>>
{
};

template <class T1, class T2>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T1, T2>::value;
#endif

template <class _Ty>
struct is_nothrow_swappable_impl : is_nothrow_swappable_with<add_lvalue_reference_t<_Ty>, add_lvalue_reference_t<_Ty>>::type
{
};
#if COMPILER_TYPE_TAG == COMPILER_MSVC_TAG
template <class T>
struct is_nothrow_swappable : is_nothrow_swappable_impl<T>::type
{
};
template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;
#endif
#pragma endregion

template <class T>
struct identity
{
    using type = T;
};
template <class T>
using identity_t = typename identity<T>::type;

struct monostate
{
};
#endif
#pragma endregion

#pragma region compatible CXX20
#if STD_HAS_CXX20 == 0
#pragma region type traits
template <class T>
struct remove_cvref // 萃取类型：移除类型的const和volatile的修饰符以及引用
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;
#pragma endregion

#pragma region construst at
template <class Iter>
DECL_NODISCARD DECL_CONSTEXPR11 void* voidptr_iter(Iter iter) DECL_NOEXCEPT;
template <class T, class... Types, class = std::void_t<decltype(::new (std::declval<void*>()) T(std::declval<Types>()...))>>
DECL_CONSTEXPR11 T* construct_at(const T* localtion, Types&&... Args) DECL_NOEXCEPT(DECL_NOEXCEPT(::new (std::voidptr_iter(localtion)) T(std::forward<Types>(Args)...)))
{
    return ::new (std::voidptr_iter(localtion)) T(std::forward<Types>(Args)...);
}
#pragma endregion
#endif
#pragma endregion

#pragma region compatible CXX23
#if STD_HAS_CXX23 == 0
inline void unreachable()
{
#ifdef __GNUC__ // GCC, Clang, ICC
    __builtin_unreachable();
#elif defined(_MSC_VER) // MSVC
    __assume(false);
#endif
}
#endif
#pragma endregion

#pragma region 扩展
#pragma region iterator
template <class Iter>
DECL_NODISCARD DECL_CONSTEXPR11 void* voidptr_iter(Iter iter) DECL_NOEXCEPT
{
    if constexpr (is_pointer<Iter>::value)
    {
        return const_cast<void*>(static_cast<const volatile void*>(iter));
    }
    else
    {
        return const_cast<void*>(static_cast<const volatile void*>(std::addressof(*iter)));
    }
}
#pragma endregion

#pragma region meta

template <class... Types>
struct meta_list
{
};

struct meta_nil
{
};

template <class List>
struct meta_list_empty;

template <template <class...> class List, class... Types>
struct meta_list_empty<List<Types...>>
{
    using type = List<>;
};

template <class List>
struct meta_list_size;
template <template <class...> class List, class... Types>
struct meta_list_size<List<Types...>> : integral_constant<size_t, sizeof...(Types)>
{
};
template <class List>
DECL_INLINE_VAR DECL_CONSTEXPR11 auto meta_list_size_v = meta_list_size<List>::value;

template <class List>
struct meta_list_front;
template <template <class...> class List, class First, class... Rest>
struct meta_list_front<List<First, Rest...>>
{
    using type = First;
};
template <class List>
using meta_list_front_t = typename meta_list_front<List>::type;

template <class List>
struct meta_list_pop_front;
template <template <class...> class List, class First, class... Rest>
struct meta_list_pop_front<List<First, Rest...>>
{
    using type = List<Rest...>;
};
template <class List>
using meta_list_pop_front_t = typename meta_list_pop_front<List>::type;

template <class List, class T>
struct meta_list_push_front;
template <template <class...> class List, class... Types, class T>
struct meta_list_push_front<List<Types...>, T>
{
    using type = List<T, Types...>;
};
template <class List, class T>
using meta_list_push_front_t = typename meta_list_push_front<List, T>::type;

template <class List, class T>
struct meta_list_push_back;
template <template <class...> class List, class... Types, class T>
struct meta_list_push_back<List<Types...>, T>
{
    using type = List<Types..., T>;
};
template <class List, class T>
using meta_list_push_back_t = typename meta_list_push_back<List, T>::type;

template <class List, size_t Idx, size_t Cur>
struct meta_list_at_impl
{
    using type = void;
};
template <template <class...> class List, class First, class... Rest, size_t Idx, size_t Cur>
struct meta_list_at_impl<List<First, Rest...>, Idx, Cur>
{
    using type = std::conditional_t<Idx == Cur,
                                    First,
                                    typename meta_list_at_impl<List<Rest...>, Idx, Cur + 1>::type>;
};

template <class List, size_t Idx, class = void>
struct meta_list_at;
template <template <class...> class List, class... Types, size_t Idx>
struct meta_list_at<List<Types...>, Idx, std::enable_if_t<(Idx < sizeof...(Types))>>
{
    using type = typename meta_list_at_impl<List<Types...>, Idx, 0>::type;
};
template <class List, size_t Idx>
using meta_list_at_t = typename meta_list_at<List, Idx>::type;

template <class VoidType, template <class...> class Func, class... Args>
struct meta_func_helper;
template <template <class...> class Func, class... Args>
struct meta_func_helper<void_t<Func<Args...>>, Func, Args...>
{
    using type = Func<Args...>;
};
template <template <class...> class Func>
struct meta_func
{
    template <class... Args>
    using invoke_type = typename meta_func_helper<void, Func, Args...>::type;
};

template <class MetaFunc, class... Args>
using meta_func_invoke = typename MetaFunc::template invoke_type<Args...>;

template <class MetaFunc, class... Args>
struct meta_func_bind
{
    template <class... Types>
    using invoke_type = meta_func_invoke<MetaFunc, Types..., Args...>;
};

template <class MetaFunc, class List>
struct meta_apply;
template <class MetaFunc, template <class...> class List, class... Types>
struct meta_apply<MetaFunc, List<Types...>>
{
    using type = meta_func_invoke<MetaFunc, Types...>;
};
template <class MetaFunc, class T, T... Idxs>
struct meta_apply<MetaFunc, integer_sequence<T, Idxs...>>
{
    using type = meta_func_invoke<MetaFunc, integral_constant<T, Idxs>...>;
};
template <class MetaFunc, class List>
using meta_apply_t = typename meta_apply<MetaFunc, List>::type;

template <class MetaFunc, class List>
struct meta_transform;
template <template <class...> class List, class MetaFunc, class... Types>
struct meta_transform<MetaFunc, List<Types...>>
{
    using type = List<meta_func_invoke<MetaFunc, Types>...>;
};
template <class MetaFunc, class List>
using meta_transform_t = typename meta_transform<MetaFunc, List>::type;

template <class T, size_t>
using meta_repeat_n_helper = T;
template <class, class, template <class...> class>
struct meta_repeat_n;
template <size_t Count, class T, template <class...> class Continue>
using meta_repeat_n_t = typename meta_repeat_n<T, make_index_sequence<Count>, Continue>::type;
template <class T, size_t... Idxs, template <class...> class Continue>
struct meta_repeat_n<T, index_sequence<Idxs...>, Continue>
{
    using type = Continue<meta_repeat_n_helper<T, Idxs>...>;
};
DECL_INLINE_VAR DECL_CONSTEXPR11 auto meta_list_npos = ~size_t{0};
DECL_INLINE_VAR DECL_CONSTEXPR11 size_t meta_list_find(const bool* const ptr, const size_t count, size_t idx = 0)
{
    for (; idx < count; ++idx)
    {
        if (ptr[idx])
        {
            return idx;
        }
    }
    return meta_list_npos;
}

template <class List, class T>
struct meta_list_find_index
{
    using type = integral_constant<size_t, meta_list_npos>;
};

#if COMPILER_TYPE_TAG != COMPILER_MSVC_TAG
template <class T1, class T2>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_same_v = is_same<T1, T2>::value;
#endif

template <template <class...> class List, class First, class... Rest, class T>
struct meta_list_find_index<List<First, Rest...>, T>
{
    static constexpr bool bools[] = {is_same_v<First, T>, is_same_v<Rest, T>...};
    using type                    = integral_constant<size_t, meta_list_find(bools, 1 + sizeof...(Rest))>;
};

template <class List, class T>
using meta_list_find_index_t = typename meta_list_find_index<List, T>::type;

DECL_NODISCARD DECL_CONSTEXPR11 size_t meta_list_find_unique(const bool* const ptr, const size_t count, const size_t first)
{
    return first != meta_list_npos && meta_list_find(ptr, count, first + 1) == meta_list_npos ? first : meta_list_npos;
}

DECL_NODISCARD DECL_CONSTEXPR11 size_t meta_list_find_unique(const bool* const ptr, const size_t count)
{
    return meta_list_find_unique(ptr, count, meta_list_find(ptr, count));
}

template <class List, class T>
struct meta_list_find_unique_index
{
    using type = integral_constant<size_t, meta_list_npos>;
};

template <template <class...> class List, class First, class... Rest, class T>
struct meta_list_find_unique_index<List<First, Rest...>, T>
{
    using type = integral_constant<size_t, meta_list_find_unique(meta_list_find_index<List<First, Rest...>, T>::bools, 1 + sizeof...(Rest))>;
};

template <class List, class T>
using meta_list_find_unique_index_t = typename meta_list_find_unique_index<List, T>::type;

template <class List, class T>
struct meta_list_count : integral_constant<size_t, 0>
{
};
template <template <class...> class List, class First, class... Rest, class T>
struct meta_list_count<List<First, Rest...>, T> : integral_constant<size_t, meta_list_count<List<Rest...>, T>::value + (std::is_same_v<T, First> ? 1 : 0)>
{
};
template <class List, class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 auto meta_list_count_v = meta_list_count<List, T>::value;

template <class List, size_t ExIdx, size_t CurIdx, class = void>
struct meta_list_extract_idx
{
    using type = typename meta_list_empty<List>::type;
};

template <template <class...> class List, class First, class... Rest, size_t ExIdx, size_t CurIdx>
struct meta_list_extract_idx<List<First, Rest...>, ExIdx, CurIdx, enable_if_t<(sizeof...(Rest) + 1 > ExIdx)>>
{
    using rest_type = typename meta_list_extract_idx<List<Rest...>, ExIdx, CurIdx + 1>::type;

    using type = std::conditional_t<ExIdx == CurIdx, rest_type, meta_list_push_front_t<rest_type, First>>;
};

template <class List, size_t ExIdx>
using meta_list_extract_idx_t = typename meta_list_extract_idx<List, ExIdx, 0>::type;

template <class List, class T>
struct meta_list_extract_type
{
    using type = meta_list_extract_idx_t<List, meta_list_find_index_t<List, T>::value>;
};

template <class List, class T>
using meta_list_extract_type_t = typename meta_list_extract_type<List, T>::type;

template <class>
struct meta_as_list_impl;
template <template <class...> class List, class... Types>
struct meta_as_list_impl<List<Types...>>
{
    using type = meta_list<Types...>;
};
template <class T, T... Idxs>
struct meta_as_list_impl<integer_sequence<T, Idxs...>>
{
    using type = meta_list<integral_constant<T, Idxs>...>;
};
template <class T>
using meta_as_list = typename meta_as_list_impl<T>::type;

template <class List>
struct meta_as_integer_sequence_impl;
template <template <class...> class List, class T, T... Idxs>
struct meta_as_integer_sequence_impl<List<integral_constant<T, Idxs>...>>
{
    using type = integer_sequence<T, Idxs...>;
};
template <class List>
using meta_as_integer_sequence = typename meta_as_integer_sequence_impl<List>::type;

template <class...>
struct meta_concat_impl;
template <class... Types>
using meta_concat = typename meta_concat_impl<Types...>::type;

template <template <class...> class List>
struct meta_concat_impl<List<>>
{
    using type = List<>;
};

template <template <class...> class List, class... Items1>
struct meta_concat_impl<List<Items1...>>
{
    using type = List<Items1...>;
};

template <template <class...> class List, class... Items1, class... Items2>
struct meta_concat_impl<List<Items1...>, List<Items2...>>
{
    using type = List<Items1..., Items2...>;
};

template <template <class...> class List, class... Items1, class... Items2, class... Items3>
struct meta_concat_impl<List<Items1...>, List<Items2...>, List<Items3...>>
{
    using type = List<Items1..., Items2..., Items3...>;
};

template <template <class...> class List, class... Items1, class... Items2, class... Items3, class... Rest>
struct meta_concat_impl<List<Items1...>, List<Items2...>, List<Items3...>, Rest...>
{
    using type = meta_concat<List<Items1..., Items2..., Items3...>, Rest...>;
};

template <class ListOfLists>
using meta_join = meta_apply<meta_func<meta_concat>, ListOfLists>;

template <class>
struct meta_cartesian_product_impl;
template <class ListOfLists>
using meta_cartesian_product = typename meta_cartesian_product_impl<ListOfLists>::type;

template <template <class...> class List>
struct meta_cartesian_product_impl<List<>>
{
    using type = List<>;
};

template <template <class...> class List1, template <class...> class List2, class... Items>
struct meta_cartesian_product_impl<List1<List2<Items...>>>
{
    using type = List1<List2<Items>...>;
};

template <template <class...> class List1, class... Items, template <class...> class List2, class... Lists>
struct meta_cartesian_product_impl<List1<List2<Items...>, Lists...>>
{
    using type = meta_join<List1<meta_transform<meta_func_bind<meta_func<meta_list_push_front>, Items>, meta_cartesian_product<List1<Lists...>>>...>>;
};

#pragma endregion

#pragma region construct or destroy in place
template <class T, class... Args>
DECL_CONSTEXPR11 void construct_in_place(T& obj, Args&&... args) DECL_NOEXCEPT(is_nothrow_constructible<T, Args...>::value)
{
#if _HAS_CXX20
    if (std::is_constant_evaluated())
    {
        std::construct_at(std::addressof(obj), std::forward<Args>(args)...);
    }
    else
#endif
    {
        ::new (std::voidptr_iter(std::addressof(obj))) T(std::forward<Args>(args)...);
    }
}

template <class T>
DECL_CONSTEXPR11 void destroy_in_place(const T& obj) DECL_NOEXCEPT
{
    if constexpr (is_array_v<T>)
    {
        auto first = std::begin(obj);
        auto last  = std::end(obj);
        for (; first != last; ++first)
        {
            std::destroy_in_place(*first);
        }
    }
    else
    {
        obj.~T();
    }
}
#pragma endregion

#pragma region template specialization
template <class Type, template <class...> class Template>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_specialization_v = false;
template <template <class...> class Template, class... Types>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_specialization_v<Template<Types...>, Template> = true;
#pragma endregion

#pragma region iterator
template <class Iter>
using iter_category_t = typename std::iterator_traits<Iter>::iterator_category;

template <class T, class = void>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_iterator_v = false;

template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_iterator_v<T, void_t<iter_category_t<T>>> = true;

#if COMPILER_TYPE_TAG != COMPILER_MSVC_TAG
template <class T1, class T2>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_convertible_v = is_convertible<T1, T2>::value;
#endif

template <class _Iter>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_random_iter_v = is_convertible_v<iter_category_t<_Iter>, random_access_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_bidirectional_iter_v = is_convertible_v<iter_category_t<_Iter>, bidirectional_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_forward_iter_v = is_convertible_v<iter_category_t<_Iter>, forward_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_output_iter_v = is_convertible_v<iter_category_t<_Iter>, output_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_input_iter_v = is_convertible_v<iter_category_t<_Iter>, input_iterator_tag>;

template <class Iter, class = void>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_unwrappable_iter_v = false;
template <class Iter>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_unwrappable_iter_v<Iter, std::void_t<decltype(std::declval<Iter>().Unwrapped())>> = true;

template <class Iter>
DECL_NODISCARD DECL_CONSTEXPR11 auto get_unwrapped(Iter&& iter) DECL_NOEXCEPT
{
    if constexpr (std::is_iterator_v<Iter>)
    {
        if constexpr (std::is_pointer<std::decay_t<Iter>>::value)
        {
            return iter;
        }
        else if constexpr (is_unwrappable_iter_v<Iter>)
        {
            return iter.Unwrapped();
        }
        else
        {
            return std::addressof(*iter);
        }
    }
    else
    {
        return static_cast<Iter&&>(iter);
    }
}

#pragma endregion

#pragma region tuple
template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_tuple_v = false;
template <class First, class... Rest>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_tuple_v<std::tuple<First, Rest...>> = true;

template <class T>
struct tuple_rest
{
};
template <class First, class... Rest>
struct tuple_rest<std::tuple<First, Rest...>>
{
    using type = std::tuple<Rest...>;
};
template <class T, class = std::enable_if_t<is_tuple_v<T>>>
using tuple_rest_t = typename tuple_rest<T>::type;

template <class T, class First>
struct tuple_push_front
{
};
template <class... Types, class First>
struct tuple_push_front<std::tuple<Types...>, First>
{
    using type = conditional_t<is_void<First>::value, tuple<Types...>, std::tuple<First, Types...>>;
};
template <class T, class First, class = enable_if_t<is_tuple_v<T>>>
using tuple_push_front_t = typename tuple_push_front<T, First>::type;

template <class T, class Last>
struct tuple_push_back
{
};
template <class... Types, class Last>
struct tuple_push_back<std::tuple<Types...>, Last>
{
    using type = conditional_t<is_void<Last>::value, tuple<Types...>, std::tuple<Types..., Last>>;
};
template <class T, class Last, class = enable_if_t<is_tuple_v<T>>>
using tuple_push_back_t = typename tuple_push_back<T, Last>::type;
#pragma endregion

#pragma region swap
template <class, class = void>
struct has_adl_swap : false_type
{
};
template <class T>
struct has_adl_swap<T, void_t<decltype(std::swap(std::declval<T&>(), std::declval<T&>()))>> : true_type
{
};
#if COMPILER_TYPE_TAG != COMPILER_MSVC_TAG
template <class Trait>
struct negation : bool_constant<!static_cast<bool>(Trait::value)>
{
};
template <class Trait>
DECL_INLINE_VAR constexpr bool negation_v = negation<Trait>::value;
#endif

template <class T>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_trivially_swappable_v = conjunction_v<is_trivially_destructible<T>,
                                                                               is_trivially_move_constructible<T>,
                                                                               is_trivially_move_assignable<T>,
                                                                               negation<has_adl_swap<T>>>;
#if STD_HAS_CXX17
template <>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_trivially_swappable_v<std::byte> = true;
#endif

template <class T>
struct is_trivially_swappable : bool_constant<is_trivially_swappable_v<T>>
{
};
template <class T>
DECL_CONSTEXPR11 void swap_adl(T& l, T& r) DECL_NOEXCEPT(is_nothrow_swappable<T>::value)
{
    swap(l, r);
}
#pragma endregion

#pragma region constructible_types
template <class Tuple, class T>
struct constructible_types
{
    using type = std::tuple<>;
};
template <class T>
struct constructible_types<std::tuple<>, T>
{
    using type = std::tuple<>;
};

template <class First, class... Rest, class T>
struct constructible_types<std::tuple<First, Rest...>, T>
{
    using cur_type      = typename constructible_types<std::tuple<Rest...>, T>::type;
    using cur_push_type = std::conditional_t<std::is_constructible<First, T>::value, First, void>;
    using type          = typename std::tuple_push_back<cur_type, cur_push_type>::type;
};

template <class Tuple, class T, class = enable_if_t<is_tuple_v<T>>>
using constructible_types_t = typename constructible_types<Tuple, T>::type;
#pragma endregion

#pragma region hash

#pragma endregion

#pragma region other
#if COMPILER_TYPE_TAG != COMPILER_MSVC_TAG
template <bool FirstValue, class First, class... Rest>
struct disjunction_impl
{
    using type = First;
};

template <class False, class Next, class... Rest>
struct disjunction_impl<false, False, Next, Rest...>
{
    using type = typename disjunction_impl<Next::value, Next, Rest...>::type;
};

template <class... Traits>
struct disjunction : false_type
{
};

template <class First, class... Rest>
struct disjunction<First, Rest...> : disjunction_impl<First::value, First, Rest...>::type
{
};

template <class... Traits>
DECL_INLINE_VAR constexpr bool disjunction_v = disjunction<Traits...>::value;
#endif

template <class T, class... Types>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_any_of_v = disjunction<is_same<T, Types>...>::value;

template <class T, class... Types>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_only_of_v = std::meta_list_count_v<std::meta_list<Types...>, T> == 1;
#pragma endregion

#pragma endregion

} // namespace std
#endif // !_STL_H_
