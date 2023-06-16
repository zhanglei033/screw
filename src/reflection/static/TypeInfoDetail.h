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
    using class_type       = C;           // 元素所属类
    using name_type        = NameT;       // 元素名称类型
    using element_category = PropertyTag; // 元素类别-属性
    using element_type     = T;           // 元素类型-属性类型
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (*)(Args...), NameT>
{
    using class_type       = C;                       // 元素所属类
    using name_type        = NameT;                   // 元素名称类型
    using element_category = MemberFunctionStaticTag; // 元素类别-静态成员函数
    using element_type     = Return (*)(Args...);     // 元素类型-成员函数类型
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...), NameT>
{
    using class_type       = C;                      // 元素所属类
    using name_type        = NameT;                  // 元素名称类型
    using element_category = MemberFunctionTag;      // 元素类别-普通成员函数
    using element_type     = Return (C::*)(Args...); // 元素类型-成员函数类型
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...) const, NameT>
{
    using class_type       = C;                            // 元素所属类
    using name_type        = NameT;                        // 元素名称类型
    using element_category = MemberFunctionConstTag;       // 元素类别-成员函数const限定
    using element_type     = Return (C::*)(Args...) const; // 元素类型-成员函数类型
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...)&, NameT>
{
    using class_type       = C;                        // 元素所属类
    using name_type        = NameT;                    // 元素名称类型
    using element_category = MemberFunctionLvalueTag;  // 元素类别-成员函数左值限定
    using element_type     = Return (C::*)(Args...) &; // 元素类型-成员函数类型
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...)&&, NameT>
{
    using class_type       = C;                         // 元素所属类
    using name_type        = NameT;                     // 元素名称类型
    using element_category = MemberFunctionRvalueTag;   // 元素类别-成员函数右值限定
    using element_type     = Return (C::*)(Args...) &&; // 元素类型-成员函数类型
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...) const&, NameT>
{
    using class_type       = C;                             // 元素所属类
    using name_type        = NameT;                         // 元素名称类型
    using element_category = MemberFunctionConstLvalueTag;  // 元素类别-成员函数const左值限定
    using element_type     = Return (C::*)(Args...) const&; // 元素类型-成员函数类型
};
template <class C, class Return, class... Args, class NameT>
struct ElementTraits<C, Return (C::*)(Args...) const&&, NameT>
{
    using class_type       = C;                              // 元素所属类
    using name_type        = NameT;                          // 元素名称类型
    using element_category = MemberFunctionConstRvalueTag;   // 元素类别-成员函数const右值限定
    using element_type     = Return (C::*)(Args...) const&&; // 元素类型-成员函数类型
};
template <class C, class T, class NameT>
struct ElementTraits<C, T*, NameT>
{
    using class_type       = C;                       // 元素所属类
    using name_type        = NameT;                   // 元素名称类型
    using element_category = MemberVariableStaticTag; // 元素类别-静态成员变量
    using element_type     = T*;                      // 成员变量类型
};
template <class C, class T, class NameT>
struct ElementTraits<C, T(C::*), NameT>
{
    using class_type       = C;                 // 元素所属类
    using name_type        = NameT;             // 素名称类型
    using element_category = MemberVariableTag; // 元素类别-成员变量
    using element_type     = T(C::*);           // 成员变量类型
};
template <class C, class T, class NameT>
struct ElementValue
{
    using traits_type  = ElementTraits<C, T, NameT>;
    using element_type = typename traits_type::element_type;
    using name_type    = typename traits_type::name_type;

    constexpr ElementValue(element_type value) DECL_NOEXCEPT
        : m_value(value) {}

    DECL_STATIC_CONSTEXPR auto m_name = StringTName<name_type>::value;
    element_type m_value;
};

// Element
template <class Element>
struct ElementFacade
{
    using traits_type  = typename Element::traits_type;
    using element_type = typename traits_type::element_type;
    using name_type    = typename traits_type::name_type;

    DECL_CONSTEXPR11 ElementFacade(Element elem) DECL_NOEXCEPT
        : m_elem{elem} {}

    DECL_CONSTEXPR11 name_type& GetName() DECL_NOEXCEPT { return m_elem.m_name; }
    DECL_CONSTEXPR11 element_type& GetValue() DECL_NOEXCEPT { return m_elem.m_value; }

    Element m_elem;
};

template <class... Elements>
struct ElementList
{
    using element_list = std::tuple<Elements...>;

    static constexpr size_t m_size = std::tuple_size_v<element_list>;
    element_list m_elements;

    constexpr ElementList(Elements... elements)
        : m_elements{elements...} {}

    template <class Visitor>
    static constexpr auto ForEach(Visitor visitor) DECL_NOEXCEPT
    {
        return ForEach<0>(visitor);
    }
    template <class StringT>
    static constexpr bool Contains(StringT = {}) DECL_NOEXCEPT
    {
        return std::is_any_of_v<StringT, typename Elements::name_type...>;
    }

private:
    template <size_t Idx, class Visitor>
    static constexpr auto ForEach(Visitor visitor) DECL_NOEXCEPT
    {
        if constexpr (Idx == m_size)
        {
            return visitor;
        }
        else
        {
            visitor(ElementFacade<std::tuple_element_t<Idx, element_list>>(std::get<Idx>(m_elements)));
            return ForEach<Idx + 1>(visitor);
        }
    }
};

// ValueType是ElementValue的特化
// PropertysType是PropertyList的特化
template <class ValueType, class PropertysType>
struct MemberVariable
{
    using category = MemberVariableTag;

    DECL_CONSTEXPR11 MemberVariable(ValueType value, PropertysType propertys = {}) DECL_NOEXCEPT
        : m_value{value},
          m_propertys{propertys} {}

    ValueType m_value;
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
// template <class... Propertys>
// struct PropertyList : TraitList<Propertys...>
//{
//     constexpr PropertyList()
//         : TraitList<Propertys...>{} {}
//     constexpr PropertyList(Propertys&&... ps)
//         : TraitList<Propertys...>{ps...} {}
// };

}
}}     // namespace screw::reflection::detail
#endif // _TYPE_INFO_DETAIL_H_
