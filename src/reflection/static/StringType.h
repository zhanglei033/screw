#pragma once
#ifndef _STRING_TYPE_H_
#define _STRING_TYPE_H_
#include "reflection/ReflBase.h"

namespace screw {
namespace reflection {
namespace detail {
DECL_INLINE_VAR DECL_CONSTEXPR11 char StringTypeEnd = '\0';

template <char... C>
struct StringType
{
};
template <class StringType, char Back>
struct StringTypePushBack;
template <template <char...> class StringType, char... C, char Back>
struct StringTypePushBack<StringType<C...>, Back>
{
    using type = std::conditional_t<Back == StringTypeEnd, StringType<C...>, StringType<C..., Back>>;
};
template <class StringType, char Back>
using StringTypePushBackType = typename StringTypePushBack<StringType, Back>::type;

template <class StringType, char... C>
struct StringTypeCreateImpl
{
    using type = StringType;
};
template <class StringType, char First, char... Rest>
struct StringTypeCreateImpl<StringType, First, Rest...>
{
    using type = std::conditional_t<First == StringTypeEnd,
                                    StringType,
                                    std::conditional_t<sizeof...(Rest) == 0,
                                                       StringTypePushBackType<StringType, First>,
                                                       typename StringTypeCreateImpl<StringTypePushBackType<StringType, First>, Rest...>::type>>;
};
template <char... C>
struct StringTypeCreate
{
    using type = typename StringTypeCreateImpl<StringType<>, C...>::type;
};

template <class StringType>
struct StringTypeLen;

template <template <char...> class StringType, char... C>
struct StringTypeLen<StringType<C...>> : std::integral_constant<size_t, sizeof...(C)>
{
};

template <class StringType>
struct StringTypeName;

template <template <char...> class StringType, char... C>
struct StringTypeName<StringType<C...>>
{
    DECL_STATIC_CONSTEXPR auto value = StatiString::Create({C...});
};

template <std::size_t Idx, std::size_t N>
constexpr char GetChar(const char (&str)[N])
{
    return str[Idx < N ? Idx : N - 1];
}

#define GET_CHAR(x, y, str)         screw::reflection::detail::GetChar<0x##x##y>(str)
#define EXPAND_STR_LEVEL1_1(x, str) GET_CHAR(x, 0, str), GET_CHAR(x, 1, str), GET_CHAR(x, 2, str), GET_CHAR(x, 3, str)
#define EXPAND_STR_LEVEL1_2(x, str) GET_CHAR(x, 4, str), GET_CHAR(x, 5, str), GET_CHAR(x, 6, str), GET_CHAR(x, 7, str)
#define EXPAND_STR_LEVEL1_3(x, str) GET_CHAR(x, 8, str), GET_CHAR(x, 9, str), GET_CHAR(x, A, str), GET_CHAR(x, B, str)
#define EXPAND_STR_LEVEL1_4(x, str) GET_CHAR(x, C, str), GET_CHAR(x, D, str), GET_CHAR(x, E, str), GET_CHAR(x, F, str)
#define EXPAND_STR_LEVEL1(x, str)   EXPAND_STR_LEVEL1_1(x, str), EXPAND_STR_LEVEL1_2(x, str), EXPAND_STR_LEVEL1_3(x, str), EXPAND_STR_LEVEL1_4(x, str)
#define EXPAND_STR_16(str)          EXPAND_STR_LEVEL1(0, str)

#define EXPAND_STR_LEVEL2_1(str)    EXPAND_STR_LEVEL1(0, str), EXPAND_STR_LEVEL1(1, str), EXPAND_STR_LEVEL1(2, str), EXPAND_STR_LEVEL1(3, str)
#define EXPAND_STR_LEVEL2_2(str)    EXPAND_STR_LEVEL1(4, str), EXPAND_STR_LEVEL1(5, str), EXPAND_STR_LEVEL1(6, str), EXPAND_STR_LEVEL1(7, str)
#define EXPAND_STR_LEVEL2_3(str)    EXPAND_STR_LEVEL1(8, str), EXPAND_STR_LEVEL1(9, str), EXPAND_STR_LEVEL1(A, str), EXPAND_STR_LEVEL1(B, str)
#define EXPAND_STR_LEVEL2_4(str)    EXPAND_STR_LEVEL1(C, str), EXPAND_STR_LEVEL1(D, str), EXPAND_STR_LEVEL1(E, str), EXPAND_STR_LEVEL1(F, str)
#define EXPAND_STR_LEVEL2(str)      EXPAND_STR_LEVEL2_1(str), EXPAND_STR_LEVEL2_2(str), EXPAND_STR_LEVEL2_3(str), EXPAND_STR_LEVEL2_4(str)
#define EXPAND_STR_256(str)         EXPAND_STR_LEVEL2(str)

#define STRT(str)                   screw::reflection::detail::StringTypeCreate<EXPAND_STR_256(str)>::type
}}}    // namespace screw::reflection::detail
#endif // !_STRING_TYPE_H_
