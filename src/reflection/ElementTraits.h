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
    using type             = typename TypeInfo::type; // Ԫ����������
    using type_info        = TypeInfo;                // Ԫ������������Ϣ
    using name_type        = NameType;                // Ԫ����������
    using element_category = PropertyTag;             // Ԫ�����-����
    using element_type     = ValueType;               // Ԫ������-��������
    using value_type       = ValueType;               // Ԫ��ָ���ֵ����

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (*)(Args...)>
{
    using type             = typename TypeInfo::type; // Ԫ����������
    using type_info        = TypeInfo;                // Ԫ������������Ϣ
    using name_type        = NameType;                // Ԫ����������
    using element_category = MemberFunctionStaticTag; // Ԫ�����-��̬��Ա����
    using element_type     = Return (*)(Args...);     // Ԫ������-��Ա��������
    using value_type       = element_type;            // Ԫ��ָ���ֵ����

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...)>
{
    using type             = typename TypeInfo::type;             // Ԫ����������
    using type_info        = TypeInfo;                            // Ԫ������������Ϣ
    using name_type        = NameType;                            // Ԫ����������
    using element_category = MemberFunctionTag;                   // Ԫ�����-��ͨ��Ա����
    using element_type     = Return (TypeInfo::type::*)(Args...); // Ԫ������-��Ա��������
    using value_type       = element_type;                        // Ԫ��ָ���ֵ����

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) const>
{
    using type             = typename TypeInfo::type;                   // Ԫ����������
    using type_info        = TypeInfo;                                  // Ԫ������������Ϣ
    using name_type        = NameType;                                  // Ԫ����������
    using element_category = MemberFunctionConstTag;                    // Ԫ�����-��Ա����const�޶�
    using element_type     = Return (TypeInfo::type::*)(Args...) const; // Ԫ������-��Ա��������
    using value_type       = element_type;                              // Ԫ��ָ���ֵ����

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...)&>
{
    using type             = typename TypeInfo::type;               // Ԫ����������
    using type_info        = TypeInfo;                              // Ԫ������������Ϣ
    using name_type        = NameType;                              // Ԫ����������
    using element_category = MemberFunctionLvalueTag;               // Ԫ�����-��Ա������ֵ�޶�
    using element_type     = Return (TypeInfo::type::*)(Args...) &; // Ԫ������-��Ա��������
    using value_type       = element_type;                          // Ԫ��ָ���ֵ����

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) &&>
{
    using type             = typename TypeInfo::type;                // Ԫ����������
    using type_info        = TypeInfo;                               // Ԫ������������Ϣ
    using name_type        = NameType;                               // Ԫ����������
    using element_category = MemberFunctionRvalueTag;                // Ԫ�����-��Ա������ֵ�޶�
    using element_type     = Return (TypeInfo::type::*)(Args...) &&; // Ԫ������-��Ա��������
    using value_type       = element_type;                           // Ԫ��ָ���ֵ����

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) const&>
{
    using type             = typename TypeInfo::type;                    // Ԫ����������
    using type_info        = TypeInfo;                                   // Ԫ������������Ϣ
    using name_type        = NameType;                                   // Ԫ����������
    using element_category = MemberFunctionConstLvalueTag;               // Ԫ�����-��Ա����const��ֵ�޶�
    using element_type     = Return (TypeInfo::type::*)(Args...) const&; // Ԫ������-��Ա��������
    using value_type       = element_type;                               // Ԫ��ָ���ֵ����

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class Return, class... Args>
struct ElementTraits<TypeInfo, NameType, Return (TypeInfo::type::*)(Args...) const&&>
{
    using type             = typename TypeInfo::type;                     // Ԫ����������
    using type_info        = TypeInfo;                                    // Ԫ������������Ϣ
    using name_type        = NameType;                                    // Ԫ����������
    using element_category = MemberFunctionConstRvalueTag;                // Ԫ�����-��Ա����const��ֵ�޶�
    using element_type     = Return (TypeInfo::type::*)(Args...) const&&; // Ԫ������-��Ա��������
    using value_type       = element_type;                                // Ԫ��ָ���ֵ����

    using return_type = Return;
    using args_type   = std::meta_list<Args...>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, ValueType*>
{
    using type             = typename TypeInfo::type; // Ԫ����������
    using type_info        = TypeInfo;                // Ԫ������������Ϣ
    using name_type        = NameType;                // Ԫ����������
    using element_category = MemberVariableStaticTag; // Ԫ�����-��̬��Ա����
    using element_type     = ValueType*;              // ��Ա��������
    using value_type       = ValueType;               // Ԫ��ָ���ֵ����

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, const ValueType*>
{
    using type             = typename TypeInfo::type;      // Ԫ����������
    using type_info        = TypeInfo;                     // Ԫ������������Ϣ
    using name_type        = NameType;                     // Ԫ����������
    using element_category = MemberVariableStaticConstTag; // Ԫ�����-��̬��Ա����
    using element_type     = const ValueType*;             // ��Ա��������
    using value_type       = const ValueType;              // Ԫ��ָ���ֵ����

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, ValueType(TypeInfo::type::*)>
{
    using type             = typename TypeInfo::type;      // Ԫ����������
    using type_info        = TypeInfo;                     // Ԫ������������Ϣ
    using name_type        = NameType;                     // ����������
    using element_category = MemberVariableTag;            // Ԫ�����-��Ա����
    using element_type     = ValueType(TypeInfo::type::*); // ��Ա��������
    using value_type       = ValueType;                    // Ԫ��ָ���ֵ����

    using return_type = void;
    using args_type   = std::meta_list<>;
};
template <class TypeInfo, class NameType, class ValueType>
struct ElementTraits<TypeInfo, NameType, const ValueType(TypeInfo::type::*)>
{
    using type             = typename TypeInfo::type;            // Ԫ����������
    using type_info        = TypeInfo;                           // Ԫ������������Ϣ
    using name_type        = NameType;                           // ����������
    using element_category = MemberVariableConstTag;             // Ԫ�����-��Ա����
    using element_type     = const ValueType(TypeInfo::type::*); // ��Ա��������
    using value_type       = const ValueType;                    // Ԫ��ָ���ֵ����

    using return_type = void;
    using args_type   = std::meta_list<>;
};
}}}    // namespace screw::reflection::detail
#endif // !_ELEMENT_TRAITS_H_
