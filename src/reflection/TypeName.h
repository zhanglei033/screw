#pragma once
#ifndef _TYPE_NAME_H_
#define _TYPE_NAME_H_
#include "base/Platform.h"
#include "base/StaticString.h"

namespace screw {
namespace reflection {
namespace detail {

/////////////////////////////////////////////////////////////////////////////////
#if (COMPILER_TYPE_TAG == COMPILER_MSVC_TAG)
#if STD_HAS_CXX11
//auto __cdecl screw::reflection::detail::Nameof<TYPE>(void) noexcept
DECL_STATIC_CONSTEXPR std::size_t TypeNameBeginPos = sizeof("auto __cdecl screw::reflection::detail::Nameof<") - 1;
DECL_STATIC_CONSTEXPR std::size_t TypeNameEndPos   = sizeof(">(void) noexcept") - 1;
#else
// auto __cdecl screw::reflection::detail::Nameof<[T]>(void)
DECL_STATIC_CONSTEXPR std::size_t TypeNameBeginPos = sizeof("auto __cdecl screw::reflection::detail::Nameof<") - 1;
DECL_STATIC_CONSTEXPR std::size_t TypeNameEndPos   = sizeof(">(void)") - 1;
#endif
#define DECL_FUNC_NAME __FUNCSIG__

#elif COMPILER_TYPE_TAG == COMPILER_GNUC_TAG
// auto screw::reflection::detail::Nameof() [with T = TYPE]
DECL_STATIC_CONSTEXPR std::size_t TypeNameBeginPos = sizeof("auto screw::reflection::detail::Nameof() [with T = ") - 1;
DECL_STATIC_CONSTEXPR std::size_t TypeNameEndPos   = sizeof("]") - 1;
#define DECL_FUNC_NAME __PRETTY_FUNCTION__
#elif COMPILER_TYPE_TAG == COMPILER_CLANG_TAG || COMPILER_TYPE_TAG == COMPILER_APPLECLANG_TAG
// auto screw::reflection::detail::Nameof() [T = TYPE]
DECL_STATIC_CONSTEXPR std::size_t TypeNameBeginPos = sizeof("auto screw::reflection::detail::Nameof() [T = ") - 1;
DECL_STATIC_CONSTEXPR std::size_t TypeNameEndPos   = sizeof("]") - 1;
#define DECL_FUNC_NAME __PRETTY_FUNCTION__
#else
#error "compiler not supported"
#endif

template <typename T>
static constexpr auto Nameof() DECL_NOEXCEPT
{
    constexpr auto fullName = screw::StatiString::Create(DECL_FUNC_NAME);
    return fullName.Substr<TypeNameBeginPos, TypeNameEndPos>();
}

}}} // namespace screw::reflection::detail

#endif // !_TYPE_NAME_H_
