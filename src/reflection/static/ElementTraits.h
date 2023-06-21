#pragma once
#ifndef _ELEMENT_TRAITS_H_
#define _ELEMENT_TRAITS_H_
#include "ElementCategory.h"
namespace screw {
namespace reflection {
namespace detail {
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits
{
    using type             = typename TypeInfo::type; // 元素所属类型
    using type_info        = TypeInfo;                // 元素所属类型信息
    using name_type        = NameType;                // 元素名称类型
    using element_category = PropertyTag;             // 元素类别-属性
    using element_type     = ValueType;               // 元素类型-属性类型
    using value_type       = ValueType;               // 元素指向的值类型

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (*)(Args...)>
{
    using type             = typename TypeInfo::type; // 元素所属类型
    using type_info        = TypeInfo;                // 元素所属类型信息
    using name_type        = NameType;                // 元素名称类型
    using element_category = MemberFunctionStaticTag; // 元素类别-静态成员函数
    using element_type     = Return (*)(Args...);     // 元素类型-成员函数类型
    using value_type       = element_type;            // 元素指向的值类型

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...)>
{
    using type             = typename TypeInfo::type;             // 元素所属类型
    using type_info        = TypeInfo;                            // 元素所属类型信息
    using name_type        = NameType;                            // 元素名称类型
    using element_category = MemberFunctionTag;                   // 元素类别-普通成员函数
    using element_type     = Return (TypeInfo::type::*)(Args...); // 元素类型-成员函数类型
    using value_type       = element_type;                        // 元素指向的值类型

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) const>
{
    using type             = typename TypeInfo::type;                   // 元素所属类型
    using type_info        = TypeInfo;                                  // 元素所属类型信息
    using name_type        = NameType;                                  // 元素名称类型
    using element_category = MemberFunctionConstTag;                    // 元素类别-成员函数const限定
    using element_type     = Return (TypeInfo::type::*)(Args...) const; // 元素类型-成员函数类型
    using value_type       = element_type;                              // 元素指向的值类型

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...)&>
{
    using type             = typename TypeInfo::type;               // 元素所属类型
    using type_info        = TypeInfo;                              // 元素所属类型信息
    using name_type        = NameType;                              // 元素名称类型
    using element_category = MemberFunctionLvalueTag;               // 元素类别-成员函数左值限定
    using element_type     = Return (TypeInfo::type::*)(Args...) &; // 元素类型-成员函数类型
    using value_type       = element_type;                          // 元素指向的值类型

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) &&>
{
    using type             = typename TypeInfo::type;                // 元素所属类型
    using type_info        = TypeInfo;                               // 元素所属类型信息
    using name_type        = NameType;                               // 元素名称类型
    using element_category = MemberFunctionRvalueTag;                // 元素类别-成员函数右值限定
    using element_type     = Return (TypeInfo::type::*)(Args...) &&; // 元素类型-成员函数类型
    using value_type       = element_type;                           // 元素指向的值类型

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) const&>
{
    using type             = typename TypeInfo::type;                    // 元素所属类型
    using type_info        = TypeInfo;                                   // 元素所属类型信息
    using name_type        = NameType;                                   // 元素名称类型
    using element_category = MemberFunctionConstLvalueTag;               // 元素类别-成员函数const左值限定
    using element_type     = Return (TypeInfo::type::*)(Args...) const&; // 元素类型-成员函数类型
    using value_type       = element_type;                               // 元素指向的值类型

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) const&&>
{
    using type             = typename TypeInfo::type;                     // 元素所属类型
    using type_info        = TypeInfo;                                    // 元素所属类型信息
    using name_type        = NameType;                                    // 元素名称类型
    using element_category = MemberFunctionConstRvalueTag;                // 元素类别-成员函数const右值限定
    using element_type     = Return (TypeInfo::type::*)(Args...) const&&; // 元素类型-成员函数类型
    using value_type       = element_type;                                // 元素指向的值类型

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, ValueType*>
{
    using type             = typename TypeInfo::type; // 元素所属类型
    using type_info        = TypeInfo;                // 元素所属类型信息
    using name_type        = NameType;                // 元素名称类型
    using element_category = MemberVariableStaticTag; // 元素类别-静态成员变量
    using element_type     = ValueType*;              // 成员变量类型
    using value_type       = ValueType;               // 元素指向的值类型

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, const ValueType*>
{
    using type             = typename TypeInfo::type;      // 元素所属类型
    using type_info        = TypeInfo;                     // 元素所属类型信息
    using name_type        = NameType;                     // 元素名称类型
    using element_category = MemberVariableStaticConstTag; // 元素类别-静态成员变量
    using element_type     = const ValueType*;             // 成员变量类型
    using value_type       = const ValueType;              // 元素指向的值类型

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, ValueType(TypeInfo::type::*)>
{
    using type             = typename TypeInfo::type;      // 元素所属类型
    using type_info        = TypeInfo;                     // 元素所属类型信息
    using name_type        = NameType;                     // 素名称类型
    using element_category = MemberVariableTag;            // 元素类别-成员变量
    using element_type     = ValueType(TypeInfo::type::*); // 成员变量类型
    using value_type       = ValueType;                    // 元素指向的值类型

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, const ValueType(TypeInfo::type::*)>
{
    using type             = typename TypeInfo::type;            // 元素所属类型
    using type_info        = TypeInfo;                           // 元素所属类型信息
    using name_type        = NameType;                           // 素名称类型
    using element_category = MemberVariableConstTag;             // 元素类别-成员变量
    using element_type     = const ValueType(TypeInfo::type::*); // 成员变量类型
    using value_type       = const ValueType;                    // 元素指向的值类型

    using return_type = void;
    using args_type   = std::meta_list<>;
};
}}}    // namespace screw::reflection::detail
#endif // !_ELEMENT_TRAITS_H_
