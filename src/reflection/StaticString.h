#pragma once
#ifndef _STATIC_STRING_H_
#define _STATIC_STRING_H_
#include "ReflectionDef.h"
#include <tuple>
#include <type_traits>
namespace screw {
namespace srefl {
struct CharTypeBase
{
};

template <char C>
struct CharType : CharTypeBase
{
    static constexpr char value = C;
};

template <size_t N, size_t... I>
DECL_NODISCARD constexpr auto make_static_string(const char (&es)[N], std::index_sequence<I...>)
{
    return std::make_tuple(es[I]...);
}
template <size_t N>
DECL_NODISCARD constexpr auto make_static_string(const char (&str)[N])
{
    return make_static_string(str, std::make_index_sequence<N - 1>{});
}


template <size_t N>
struct StaticString
{
    using value_type = char;
    using size_type  = size_t;

    static constexpr size_type num = N;

    template <size_type... I>
    constexpr StaticString(const char (&str)[N], std::index_sequence<I...>)
        : value{str[I]...}
    {
    }
    const value_type value[N];
};



template <class... Strs>
DECL_NODISCARD constexpr auto cat_static_string(Strs&&... strs)
{
    return std::tuple_cat(std::forward<Strs>(strs)...);
}

template <class Str, size_t... I>
DECL_NODISCARD constexpr auto get_static_string(Str str, std::index_sequence<I...>)
{
    constexpr typename std::tuple_size<Str>::value_type size = std::tuple_size_v<Str>;
    return StaticString<size + 1>({std::get<I>(str)..., '\0'}, std::make_index_sequence<size + 1>{});
}

template <class Str>
DECL_NODISCARD constexpr auto get_static_string(Str str)
{
    constexpr typename std::tuple_size<Str>::value_type size = std::tuple_size_v<Str>;
    return get_static_string(str, std::make_index_sequence<size>{});
}
}}     // namespace screw::srefl
#endif // !_STATIC_STRING_H_
