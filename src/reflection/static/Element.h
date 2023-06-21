#pragma once
#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include "ElementTraits.h"
#include "StringType.h"
#include <type_traits>
namespace screw {
namespace reflection {
template <class Element>
struct ElementInfo;
namespace detail {

template <class ElementInfo,
          class ValueType,
          ValueType Elem,
          bool IsMember   = ElementInfo::is_member_v,
          bool IsStatic   = ElementInfo::is_member_static_v,
          bool IsFunction = ElementInfo::is_member_function_v,
          bool IsVariable = ElementInfo::is_member_variable_v>
struct ElementInvoke;

template <class ElementInfo, class ValueType, ValueType Elem>
struct ElementInvoke<ElementInfo, ValueType, Elem, false, false, false, false> // property
{
    using traits_type                       = typename ElementInfo::traits_type;
    using element_type                      = typename traits_type::element_type;
    DECL_STATIC_CONSTEXPR element_type elem = Elem;
    DECL_STATIC_CONSTEXPR auto Invoke() DECL_NOEXCEPT -> decltype(elem)
    {
        return elem;
    }
};

template <class ElementInfo, class ValueType, ValueType Elem>
struct ElementInvoke<ElementInfo, ValueType, Elem, true, false, true, false> // member function
{
    using traits_type                       = typename ElementInfo::traits_type;
    using element_type                      = typename traits_type::element_type;
    using traits_type                       = typename ElementInfo::traits_type;
    using element_type                      = typename traits_type::element_type;
    DECL_STATIC_CONSTEXPR element_type elem = Elem;
    template <class FirstArg, class... RestArgs>
    DECL_STATIC_CONSTEXPR auto Invoke(FirstArg&& first, RestArgs&&... rest)
        DECL_NOEXCEPT(DECL_NOEXCEPT((std::forward<FirstArg>(first).*elem)(std::forward<RestArgs>(rest)...)))
            -> decltype((std::forward<FirstArg>(first).*elem)(std::forward<RestArgs>(rest)...))
    {
        return (std::forward<FirstArg>(first).*elem)(std::forward<RestArgs>(rest)...);
    }
};

template <class ElementInfo, class ValueType, ValueType Elem>
struct ElementInvoke<ElementInfo, ValueType, Elem, true, false, false, true> // member variable
{
    using traits_type                       = typename ElementInfo::traits_type;
    using element_type                      = typename traits_type::element_type;
    DECL_STATIC_CONSTEXPR element_type elem = Elem;
    template <class FirstArg, class... RestArgs>
    DECL_STATIC_CONSTEXPR auto Invoke(FirstArg&& first) DECL_NOEXCEPT -> decltype(std::forward<FirstArg>(first).*elem)
    {
        return std::forward<FirstArg>(first).*elem;
    }
};

template <class ElementInfo, class ValueType, ValueType Elem>
struct ElementInvoke<ElementInfo, ValueType, Elem, true, true, true, false> // staic member function
{
    using traits_type                       = typename ElementInfo::traits_type;
    using element_type                      = typename traits_type::element_type;
    DECL_STATIC_CONSTEXPR element_type elem = Elem;
    template <class... RestArgs>
    DECL_STATIC_CONSTEXPR auto Invoke(RestArgs&&... rest)
        DECL_NOEXCEPT(DECL_NOEXCEPT(std::forward<element_type>(elem)(std::forward<RestArgs>(rest)...)))
            -> decltype(std::forward<element_type>(elem)(std::forward<RestArgs>(rest)...))
    {
        return std::forward<element_type>(elem)(std::forward<RestArgs>(rest)...);
    }
};

template <class ElementInfo, class ValueType, ValueType Elem>
struct ElementInvoke<ElementInfo, ValueType, Elem, true, true, false, true> // staic member variable
{
    using traits_type                       = typename ElementInfo::traits_type;
    using element_type                      = typename traits_type::element_type;
    DECL_STATIC_CONSTEXPR element_type elem = Elem;
    DECL_STATIC_CONSTEXPR auto Invoke() DECL_NOEXCEPT -> decltype(*elem)
    {
        return *elem;
    }
};

template <class TypeInfo, class NameType, class ValueType, ValueType Elem>
struct ElementCore : ElementInvoke<ElementInfo<ElementTraits<TypeInfo, NameType, ValueType>>, ValueType, Elem>
{
public:
    using traits_type      = ElementTraits<TypeInfo, NameType, ValueType>;
    using element_info     = ElementInfo<traits_type>;
    using type             = typename traits_type::type;
    using type_info        = typename traits_type::type_info;
    using name_type        = typename traits_type::name_type;
    using element_category = typename traits_type::element_category;
    using element_type     = typename traits_type::element_type;
    using value_type       = typename traits_type::value_type;

    DECL_STATIC_CONSTEXPR auto& Name() DECL_NOEXCEPT { return StringTypeName<name_type>::value; }
};

template <class TypeInfo, class NameType, class ValueType>
struct StringElementCore
{
public:
    using traits_type      = ElementTraits<TypeInfo, NameType, ValueType>;
    using element_info     = ElementInfo<traits_type>;
    using type             = typename traits_type::type;
    using type_info        = typename traits_type::type_info;
    using name_type        = typename traits_type::name_type;
    using element_category = typename traits_type::element_category;
    using element_type     = typename traits_type::element_type;
    using value_type       = typename traits_type::value_type;

    DECL_STATIC_CONSTEXPR auto Invoke() DECL_NOEXCEPT
    {
        static_assert(element_info::is_property_v, "must be property, static member variable, static member function");
        return StringTypeName<element_type>::value;
    }

    DECL_STATIC_CONSTEXPR auto& Name() DECL_NOEXCEPT { return StringTypeName<name_type>::value; }
};
template <class... Elements>
struct ElementList;

template <class ElementCore>
struct Property : ElementCore
{
    using element_core = ElementCore;
    using traits_type  = typename ElementCore::traits_type;
};

template <class ElementCore, class... Propertys>
struct Member : ElementCore
{
    using element_core   = ElementCore;
    using traits_type    = typename ElementCore::traits_type;
    using propertys_type = ElementList<Propertys...>;

    template <class NameType, size_t>
    DECL_STATIC_CONSTEXPR auto FindProperty(NameType name)
    {
        return propertys_type::Find(name);
    }
};

struct EmptyElement
{
};
} // namespace detail
}}     // namespace screw::reflection
#endif // !_ELEMENT_H_
