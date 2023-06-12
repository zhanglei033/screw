#pragma once
#ifndef _REFLECTION_DEF_H_
#define _REFLECTION_DEF_H_
#ifdef __cplusplus
#if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
#define STD_LANG _MSVC_LANG
#else
#define STD_LANG __cplusplus
#endif
#else
#define STD_LANG 0L
#endif // __cplusplus

#ifndef STD_HAS_CXX17
#if STD_LANG > 201402L
#define STD_HAS_CXX17 1
#else
#define STD_HAS_CXX17 0
#endif
#endif // STD_HAS_CXX17

#if STD_HAS_CXX17
#define DECL_INLINE_VAR inline // 内联变量需要C++17
#else
#define DECL_INLINE_VAR
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

struct NonStaticMemberTag
{
};
struct StaticMemberTag
{
};
struct MemberVariableTag : public NonStaticMemberTag
{
};
struct MemberFunctionTag : public NonStaticMemberTag
{
};
struct MemberEnumTag : public StaticMemberTag
{
};
struct StaticMemberVariableTag : public StaticMemberTag
{
};
struct StaticMemberFunctionTag : public StaticMemberTag
{
};
template <class T>
struct MemberTraits
{
    using member_category = MemberEnumTag;
};
template <class U, class T>
struct MemberTraits<T U::*>
{
    using member_category = std::conditional_t<std::is_function_v<T>, MemberFunctionTag, MemberVariableTag>;
};
template <class T>
struct MemberTraits<T*>
{
    using member_category = std::conditional_t<std::is_function_v<T>, StaticMemberFunctionTag, StaticMemberVariableTag>;
};

template <class Member>
DECL_INLINE_VAR static constexpr bool is_member_var_v = std::is_same_v<typename Member::member_category, MemberVariableTag>;

template <class Member>
DECL_INLINE_VAR static constexpr bool is_static_member_var_v = std::is_same_v<typename Member::member_category, StaticMemberVariableTag>;

template <class Member>
DECL_INLINE_VAR static constexpr bool is_member_func_v = std::is_same_v<typename Member::member_category, MemberFunctionTag>;

template <class Member>
DECL_INLINE_VAR static constexpr bool is_static_member_func_v = std::is_same_v<typename Member::member_category, StaticMemberFunctionTag>;


#endif // !_REFLECTION_DEF_H_
