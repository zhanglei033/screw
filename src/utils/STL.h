#pragma once
#ifndef _STL_H_
#define _STL_H_
#include "UtilsDef.h"
#include <iterator>
#include <type_traits>
// std的一些向前兼容和扩展
namespace std {
#if STD_HAS_CXX17 == 0
template <class T>
constexpr void destroy_at(const T* localtion) noexcept
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
#endif

#if STD_HAS_CXX20 == 0
template <class T>
struct remove_cvref // 萃取类型：移除类型的const和volatile的修饰符以及引用
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <class T, class... Types, class = std::void_t<decltype(::new (std::declval<void*>()) T(std::declval<Types>()...))>>
constexpr T* construct_at(const T* localtion, Types&&... _Args) noexcept(noexcept(::new (std::NormalizationIter(localtion)) T(std::forward<Types>(_Args)...)))
{
    return ::new (std::NormalizationIter(localtion)) T(std::forward<Types>(_Args)...);
}
#endif

template <class Iter>
using iter_category_t = typename std::iterator_traits<Iter>::iterator_category;

template <class T, class = void>
DECL_INLINE_VAR constexpr bool is_iterator_v = false;

template <class T>
DECL_INLINE_VAR constexpr bool is_iterator_v<T, void_t<iter_category_t<T>>> = true;

template <class _Iter>
DECL_INLINE_VAR constexpr bool is_random_iter_v = is_convertible_v<iter_category_t<_Iter>, random_access_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR constexpr bool is_bidirectional_iter_v = is_convertible_v<iter_category_t<_Iter>, bidirectional_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR constexpr bool is_forward_iter_v = is_convertible_v<iter_category_t<_Iter>, forward_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR constexpr bool is_output_iter_v = is_convertible_v<iter_category_t<_Iter>, output_iterator_tag>;

template <class _Iter>
DECL_INLINE_VAR constexpr bool is_input_iter_v = is_convertible_v<iter_category_t<_Iter>, input_iterator_tag>;


template <class Iter>
DECL_NODISCARD constexpr auto get_unwrapped(Iter&& iter) noexcept
{
    if constexpr (std::is_pointer_v<std::decay_t<Iter>>)
    {
        return iter;
    }
    else
    {
        return std::addressof(*iter);
    }
}
} // namespace std
#endif // !_STL_H_
