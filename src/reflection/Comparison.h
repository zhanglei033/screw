#pragma once
#ifndef _COMPARISON_H_
#define _COMPARISON_H_
#include "base/StringView.h"
namespace screw {
namespace reflection {
template <typename T>
using is_comparable = std::integral_constant<bool, std::is_same<T, std::string>::value || std::is_same<T, std::string_view>::value || std::is_arithmetic<T>::value || std::is_enum<T>::value || std::is_same<T, std::nullptr_t>::value || std::is_pointer<T>::value>;

template <typename T>
DECL_INLINE_VAR DECL_CONSTEXPR11 auto is_comparable_v = is_comparable<T>::value;

}} // namespace screw::reflection

#endif // !_COMPARISON_H_
