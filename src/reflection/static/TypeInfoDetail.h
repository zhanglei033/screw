#pragma once
#ifndef _TYPE_INFO_DETAIL_H_
#define _TYPE_INFO_DETAIL_H_
#include "StringType.h"
namespace screw {
namespace reflection {
namespace detail {
struct ElementTag
{
};
struct PropertyTag : ElementTag
{
};
struct MemberVariableTag : ElementTag
{
};
struct MemberVariableStaticTag : private MemberVariableTag
{
};
struct MemberFunctionTag : ElementTag
{
};
struct MemberFunctionStaticTag : private MemberFunctionTag
{
};
struct MemberFunctionConstTag : MemberFunctionTag
{
};
struct MemberFunctionLvalueTag : MemberFunctionTag
{
};
struct MemberFunctionRvalueTag : MemberFunctionTag
{
};
struct MemberFunctionConstLvalueTag : MemberFunctionConstTag, MemberFunctionLvalueTag
{
};
struct MemberFunctionConstRvalueTag : MemberFunctionConstTag, MemberFunctionRvalueTag
{
};

template <class C, class T, class NameT>
struct ElementTraits
{
    using class_type       = C;           // Ԫ��������
    using name_type        = NameT;       // Ԫ����������
    using element_category = PropertyTag; // Ԫ�����-����
    using element_type     = T;           // Ԫ������-��������
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (*)(Args...), NameT>
{
    using class_type       = C;                       // Ԫ��������
    using name_type        = NameT;                   // Ԫ����������
    using element_category = MemberFunctionStaticTag; // Ԫ�����-��̬��Ա����
    using element_type     = Return (*)(Args...);     // Ԫ������-��Ա��������
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...), NameT>
{
    using class_type       = C;                      // Ԫ��������
    using name_type        = NameT;                  // Ԫ����������
    using element_category = MemberFunctionTag;      // Ԫ�����-��ͨ��Ա����
    using element_type     = Return (C::*)(Args...); // Ԫ������-��Ա��������
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...) const, NameT>
{
    using class_type       = C;                            // Ԫ��������
    using name_type        = NameT;                        // Ԫ����������
    using element_category = MemberFunctionConstTag;       // Ԫ�����-��Ա����const�޶�
    using element_type     = Return (C::*)(Args...) const; // Ԫ������-��Ա��������
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...)&, NameT>
{
    using class_type       = C;                        // Ԫ��������
    using name_type        = NameT;                    // Ԫ����������
    using element_category = MemberFunctionLvalueTag;  // Ԫ�����-��Ա������ֵ�޶�
    using element_type     = Return (C::*)(Args...) &; // Ԫ������-��Ա��������
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...)&&, NameT>
{
    using class_type       = C;                         // Ԫ��������
    using name_type        = NameT;                     // Ԫ����������
    using element_category = MemberFunctionRvalueTag;   // Ԫ�����-��Ա������ֵ�޶�
    using element_type     = Return (C::*)(Args...) &&; // Ԫ������-��Ա��������
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...) const&, NameT>
{
    using class_type       = C;                             // Ԫ��������
    using name_type        = NameT;                         // Ԫ����������
    using element_category = MemberFunctionConstLvalueTag;  // Ԫ�����-��Ա����const��ֵ�޶�
    using element_type     = Return (C::*)(Args...) const&; // Ԫ������-��Ա��������
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...) const&&, NameT>
{
    using class_type       = C;                              // Ԫ��������
    using name_type        = NameT;                          // Ԫ����������
    using element_category = MemberFunctionConstRvalueTag;   // Ԫ�����-��Ա����const��ֵ�޶�
    using element_type     = Return (C::*)(Args...) const&&; // Ԫ������-��Ա��������
};
template <class C, class T, class NameT>
struct ElementTraits<C, T*, NameT>
{
    using class_type       = C;                       // Ԫ��������
    using name_type        = NameT;                   // Ԫ����������
    using element_category = MemberVariableStaticTag; // Ԫ�����-��̬��Ա����
    using element_type     = T*;                      // ��Ա��������
};
template <class C, class T, class NameT>
struct ElementTraits<C, T(C::*), NameT>
{
    using class_type       = C;                 // Ԫ��������
    using name_type        = NameT;             // ����������
    using element_category = MemberVariableTag; // Ԫ�����-��Ա����
    using element_type     = T(C::*);           // ��Ա��������
};

struct ElementValueBase
{
    DECL_CONSTEXPR11 std::string_view GetName() const
    {
        return StringTName<STRT("")>::value;
    }
};

template <class C, class T, class NameT>
struct ElementValue
{
    using traits_type  = ElementTraits<C, T, NameT>;
    using name_type    = typename traits_type::name_type;
    using element_type = typename traits_type::element_type;

    DECL_CONSTEXPR11 ElementValue(element_type value) DECL_NOEXCEPT
        : m_value(value) {}
    DECL_CONSTEXPR11 ElementValue(const ElementValue& value) DECL_NOEXCEPT
        : m_value(value.m_value) {}

    DECL_STATIC_CONSTEXPR auto m_name = StringTName<name_type>::value;
    element_type m_value;
};

// Element Traits Extract
template <class Element>
struct ElementTraitsExtract
{
    using traits_type      = typename Element::traits_type;
    using class_type       = typename traits_type::class_type;
    using name_type        = typename traits_type::name_type;
    using element_category = typename traits_type::element_category;
    using element_type     = typename traits_type::element_type;
};

template <class... Elements>
struct ElementList
{
    using element_list = std::tuple<Elements...>;

    DECL_STATIC_CONSTEXPR size_t m_size = std::tuple_size_v<element_list>;
    element_list m_elements;

    template <class StringT>
    DECL_STATIC_CONSTEXPR bool is_contains_v = std::is_any_of_v<StringT, typename ElementTraitsExtract<Elements>::name_type...>;

    constexpr ElementList(Elements... elements)
        : m_elements{elements...} {}

    template <class Visitor>
    DECL_CONSTEXPR11 auto ForEach(Visitor visitor) const DECL_NOEXCEPT
    {
        return ForEach<0>(visitor);
    }

private:
    template <size_t Idx, class Visitor>
    DECL_CONSTEXPR11 auto ForEach(Visitor visitor) const DECL_NOEXCEPT
    {
        if constexpr (Idx == m_size)
        {
            return visitor;
        }
        else
        {
            // visitor(ElementTraitsFacade<std::tuple_element_t<Idx, element_list>>(std::get<Idx>(m_elements)));
            visitor(std::get<Idx>(m_elements));
            return ForEach<Idx + 1>(visitor);
        }
    }
};

// ValueType��ElementValue���ػ�
// PropertysType��PropertyList���ػ�
template <class ValueType, class PropertysType>
struct MemberVariable : ValueType
{
    using traits_type = typename ValueType::traits_type;

    DECL_CONSTEXPR11 MemberVariable(ValueType value, PropertysType propertys) DECL_NOEXCEPT
        : ValueType{value},
          m_propertys{propertys} {}

    PropertysType m_propertys;
};

template <class... Variables>
struct VariableList : ElementList<Variables...>
{
    DECL_CONSTEXPR11 VariableList(Variables... variables)
        : ElementList<Variables...>{variables...} {}
};
//
// template <class C, class T, class NameT, class PropertyList>
// struct Method : ElementTraits<C, T, NameT>
//{
//     PropertyList propertys;
//     DECL_CONSTEXPR11 Method(T v)
//         : ElementTraits<C, T, NameT>(v), propertys{} {}
//     DECL_CONSTEXPR11 Method(T v, PropertyList ps)
//         : ElementTraits<C, T, NameT>(v), propertys{ps} {}
// };
//
// template <class... Methods>
// struct MethodList : TraitList<Methods...>
//{
//     constexpr MethodList()
//         : TraitList<Methods...>{} {}
//     constexpr MethodList(Methods&&... ms)
//         : TraitList<Methods...>{ms...} {}
// };
//
// template <class C, class T, class NameT>
// struct Property : ElementTraits<C, T, NameT>
//{
//     DECL_CONSTEXPR11 Property(T v)
//         : ElementTraits<C, T, NameT>(v) {}
// };
//
template <class... Propertys>
struct PropertyList : ElementList<Propertys...>
{
    DECL_CONSTEXPR11 PropertyList(Propertys... propertys)
        : ElementList<Propertys...>{propertys...} {}
};

}
}}     // namespace screw::reflection::detail
#endif // _TYPE_INFO_DETAIL_H_
