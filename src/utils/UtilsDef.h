#pragma once
#ifndef _UTILSDEF_H_
#define _UTILSDEF_H_
namespace screw {
namespace utils {
// 常用类型缩写
using int8   = char;
using uint8  = unsigned char;
using int16  = short;
using uint16 = unsigned short;
using int32  = int;
using uint32 = unsigned int;
using int64  = long long;
using uint64 = unsigned long long;
}
} // namespace screw::utils

// C++语言标准
#ifdef __cplusplus
#if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
#define STD_LANG _MSVC_LANG
#else
#define STD_LANG __cplusplus
#endif
#else
#define STD_LANG 0L
#endif // __cplusplus

#ifndef STD_HAS_CXX11
#if (__cplusplus >= 201103L) || (_MSC_VER > 1900)
#define STD_HAS_CXX11 1
#else
#define STD_HAS_CXX11 0
#endif
#endif // STD_HAS_CXX17

#ifndef STD_HAS_CXX17
#if STD_LANG > 201402L
#define STD_HAS_CXX17 1
#else
#define STD_HAS_CXX17 0
#endif
#endif // STD_HAS_CXX17

#ifndef STD_HAS_CXX20
#if STD_HAS_CXX17 && STD_LANG > 201703L
#define STD_HAS_CXX20 1
#else
#define STD_HAS_CXX20 0
#endif
#endif // STD_HAS_CXX20

#if STD_HAS_CXX17
#define DECL_CONSTEXPR17 constexpr // C++17才支持的constexpr函数
#define DECL_INLINE_VAR inline     // 内联变量需要C++17
#else
#define DECL_CONSTEXPR17 inline
#define DECL_INLINE_VAR
#endif

#if STD_HAS_CXX20
#define DECL_CONSTEXPR20 constexpr
#else
#define DECL_CONSTEXPR20 inline
#endif

#if defined(__cplusplus) && (__cplusplus >= 201703)
#define DECL_NODISCARD [[nodiscard]]
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) // includes clang, icc, and clang-cl
#define DECL_NODISCARD __attribute__((warn_unused_result))
#elif defined(_HAS_NODISCARD)
#define DECL_NODISCARD _NODISCARD
#elif (_MSC_VER >= 1700)
#define DECL_NODISCARD _Check_return_
#else
#define DECL_NODISCARD
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
#if defined(STATIC_LIB)
#define DECL_EXPORT
#elif defined(SHARED_LIB)
#define DECL_EXPORT __declspec(dllexport)
#else
#define DECL_EXPORT __declspec(dllimport)
#endif
#endif


#endif // !_UTILSDEF_H_