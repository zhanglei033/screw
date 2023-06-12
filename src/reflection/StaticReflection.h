#pragma once
#ifndef _STATIC_REFLECTION_H_
#define _STATIC_REFLECTION_H_
#include "ReflectionDef.h"
#include "StaticString.h"
namespace screw {
namespace srefl {
template <class Name>
struct Nameof
{
    constexpr Nameof(Name) {}
    static constexpr typename std::tuple_size<Name>::value_type size = std::tuple_size_v<Name>;
    static constexpr StaticString<size> name                         = get_static_string(Name);
};

template <class T, class Name>
struct ValueType : public Nameof<Name>
{
    using value_type      = std::remove_cv_t<std::remove_reference_t<T>>;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;

    using Nameof<Name>::Nameof;
};

template <class... Elems>
struct ElemList
{
    constexpr ElemList(Elems... es)
        : elems{es...} {}

    std::tuple<Elems...> elems;
    static constexpr std::size_t size = sizeof...(Elems);
};

template <class T, class Name>
struct AttributeValue : ValueType<T, Name>
{
    AttributeValue(typename ValueType<T, Name>::value_type v, Name)
        : value(v) {}

    typename ValueType<T, Name>::value_type value;
};

template <class T, class Name>
struct Attribute : AttributeValue<T, Name>
{
    using AttributeValue<T, Name>::AttributeValue;
};
template <class Name>
struct Attribute<void, Name> : Nameof<Name>
{
    using Nameof<Name>::Nameof;
};

template <class... Attributes>
struct AttributeList : public ElemList<Attributes...>
{
    using ElemList<Attributes...>::ElemList;
};


template <class T, class Name, class MemberTraits>
struct MemberValue : ValueType<T, Name>
{
    using value_type = std::conditional_t<
        is_member_var_v<MemberTraits> || is_static_member_var_v<MemberTraits>,
        typename ValueType<T, Name>::reference,
        typename ValueType<T, Name>::const_reference>;
    using member_category = typename MemberTraits::member_category;

    MemberValue(value_type v, Name)
        : value(v) {}

    value_type value;
};

template <class T, class Name, class AttributeList = AttributeList<>>
struct Member : MemberValue<T, Name, MemberTraits<T>>
{
    constexpr Member(T v, Name, AttributeList as = {})
        : MemberValue<T, Name, MemberTraits<T>>(v), attrs(as) {}

    AttributeList attrs;
};

template <class... Members>
struct MemberList : ElemList<Members...>
{
    using ElemList<Members...>::ElemList;
};

template <typename T>
struct TypeInfo
{
};
template <class Derived, class... Bases>
struct TypeInfoBase
{
    using derived_type = Derived;
    using base_type    = std::tuple<Bases...>;
};

}}     // namespace screw::srefl
#endif // !_STATIC_REFLECTION_H_
