#pragma once
#ifndef _TYPE_INFO_H_
#define _TYPE_INFO_H_
#include "ElementList.h"
#include "StringType.h"
#include "TypeName.h"
#include "reflection/ReflBase.h"
namespace screw {
namespace reflection {

template <class Element>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_empty_element_v = std::is_same_v<Element, detail::EmptyElement>;

template <class Element>
DECL_STATIC_CONSTEXPR bool is_empty_element(Element)
{
    return is_empty_element_v<Element>;
}
template <class Traits>
struct ElementInfo
{
    using traits_type      = typename Traits;
    using type             = typename traits_type::type;
    using type_info        = typename traits_type::type_info;
    using name_type        = typename traits_type::name_type;
    using element_category = typename traits_type::element_category;
    using element_type     = typename traits_type::element_type;
    using value_type       = typename traits_type::value_type;

    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_property_v                     = std::is_base_of<detail::PropertyTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_v                       = std::is_base_of<detail::MemberTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_static_v                = std::is_base_of<detail::MemberStaticTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_const_v                 = std::is_base_of<detail::MemberConstTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_variable_v              = std::is_base_of<detail::MemberVariableTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_variable_static_v       = std::is_base_of<detail::MemberVariableStaticTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_variable_const_v        = std::is_base_of<detail::MemberVariableConstTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_variable_const_static_v = std::is_base_of<detail::MemberVariableStaticConstTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_function_v              = std::is_base_of<detail::MemberFunctionTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_function_static_v       = std::is_base_of<detail::MemberFunctionStaticTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_function_const_v        = std::is_base_of<detail::MemberFunctionConstTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_function_lvalue_v       = std::is_base_of<detail::MemberFunctionLvalueTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_function_rvalue_v       = std::is_base_of<detail::MemberFunctionRvalueTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_function_const_lvalue_v = std::is_base_of<detail::MemberFunctionConstLvalueTag, element_category>::value;
    DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_member_function_const_rvalue_v = std::is_base_of<detail::MemberFunctionConstRvalueTag, element_category>::value;
};

template <class T>
struct TypeInfo;

template <class Type, class... Bases>
struct TypeInfoBase
{
    using type            = Type;
    using type_info       = TypeInfo<type>;
    using bases_type_info = std::meta_list<TypeInfo<Bases>...>;

    DECL_STATIC_CONSTEXPR auto type_name = detail::Nameof<type>();
    DECL_STATIC_CONSTEXPR auto type_id   = __COUNTER__;

    template <class NameType, class ValueType, ValueType Elem>
    DECL_STATIC_CONSTEXPR auto MakeProperty() DECL_NOEXCEPT
    {
        using ElementCore = detail::ElementCore<type_info, NameType, ValueType, Elem>;
        return detail::Property<ElementCore>{};
    }
    template <class NameType, class StringType>
    DECL_STATIC_CONSTEXPR auto MakeProperty() DECL_NOEXCEPT
    {
        using ElementCore = detail::StringElementCore<type_info, NameType, StringType>;
        return detail::Property<ElementCore>{};
    }
    template <class... Propertys>
    DECL_STATIC_CONSTEXPR auto MakePropertys(Propertys... Property) DECL_NOEXCEPT
    {
        return detail::ElementList<Propertys...>{};
    }
    template <class NameType, class ValueType, ValueType Elem, class... Propertys>
    DECL_STATIC_CONSTEXPR auto MakeMember(Propertys... Property) DECL_NOEXCEPT
    {
        using ElementCore = detail::ElementCore<type_info, NameType, ValueType, Elem>;
        return detail::Member<ElementCore, Propertys...>{};
    }
    template <class... Members>
    DECL_STATIC_CONSTEXPR auto MakeMembers(Members... Member) DECL_NOEXCEPT
    {
        return detail::ElementList<Members...>{};
    }

    template <class Callable>
    DECL_STATIC_CONSTEXPR void ForEachElement(Callable&& caller)
    {
        ForEachProperty(std::forward<Callable>(caller));
        ForEachMember(std::forward<Callable>(caller));
    }

    template <class Callable>
    DECL_STATIC_CONSTEXPR void ForEachProperty(Callable&& caller)
    {
        type_info::propertys.ForEach(std::forward<Callable>(caller));
    }

    template <class Callable>
    DECL_STATIC_CONSTEXPR void ForEachMember(Callable&& caller)
    {
        type_info::members.ForEach(std::forward<Callable>(caller));
    }

    template <class T, class Callable, class = std::enable_if_t<std::is_same_v<std::remove_cvref_t<T>, type>>>
    DECL_STATIC_CONSTEXPR void ForEachMemberVar(T&& obj, Callable&& caller)
    {
        type_info::members.ForEachMemberVar(std::forward<T>(obj), std::forward<Callable>(caller));
    }

    template <class NameType>
    DECL_STATIC_CONSTEXPR auto FindElement(NameType name)
    {
        auto elem1 = FindProperty(name);
        if constexpr (!is_empty_element(elem1))
        {
            return elem1;
        }
        return FindMember(name);
    }
    template <class NameType>
    DECL_STATIC_CONSTEXPR auto FindProperty(NameType name)
    {
        return type_info::propertys.Find(name);
    }
    template <class NameType>
    DECL_STATIC_CONSTEXPR auto FindMember(NameType name)
    {
        return type_info::members.Find(name);
    }
};

template <class T>
struct TypeInfo : TypeInfoBase<T>
{
    DECL_STATIC_CONSTEXPR auto propertys = TypeInfoBase<T>::MakePropertys();
    DECL_STATIC_CONSTEXPR auto members   = TypeInfoBase<T>::MakeMembers();
};
}
} // namespace screw::reflection
#endif // !_TYPE_INFO_H_
