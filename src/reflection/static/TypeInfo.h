#pragma once
#ifndef _TYPE_INFO_H_
#define _TYPE_INFO_H_
#include "StringType.h"
#include "TypeInfoDetail.h"
#include "reflection/ReflBase.h"
namespace screw {
namespace reflection {
template <class T>
struct TypeInfo;
template <class Type, class... Bases>
struct TypeInfoBase
{
    using TypeInfo = TypeInfo<Type>;
    // using bases_type      = detail::MetaListPro<Bases...>;
    // using bases_type_info = detail::MetaListPro<TypeInfo<Bases>...>;

    template <class T, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(T value, NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, T, NameT>(value);
    }
    template <class Return, class... Args, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(Return (*value)(Args...), NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, Return (*)(Args...), NameT>(value);
    }
    template <class Return, class... Args, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(Return (Type::*value)(Args...), NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, Return (*)(Args...), NameT>(value);
    }
    template <class Return, class... Args, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(Return (Type::*value)(Args...) const, NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, Return (*)(Args...) const, NameT>(value);
    }
    template <class Return, class... Args, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(Return (Type::*value)(Args...) &, NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, Return (*)(Args...)&, NameT>(value);
    }
    template <class Return, class... Args, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(Return (Type::*value)(Args...) &&, NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, Return (*)(Args...)&&, NameT>(value);
    }
    template <class Return, class... Args, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(Return (Type::*value)(Args...) const&, NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, Return (*)(Args...) const&, NameT>(value);
    }
    template <class Return, class... Args, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(Return (Type::*value)(Args...) const&&, NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, Return (*)(Args...) const&&, NameT>(value);
    }
    template <class T, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(T* value, NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, T*, NameT>(value);
    }
    template <class T, class NameT>
    DECL_STATIC_CONSTEXPR auto MakeElementValue(T(Type::*value), NameT) DECL_NOEXCEPT
    {
        return detail::ElementValue<Type, T(Type::*), NameT>(value);
    }

    template <class Func>
    static constexpr void ForEachVar(Type&& obj, Func&& func)
    {
        /*   type_info::variables.ForEach([&](auto&& ins) {
               using traits = std::member_traits<Type, typename decltype(ins)::value_type>;
               if constexpr (traits::is_static)
               {
                   func(ins, std::forward<Type>(obj.*(typename decltype(ins)::value_type{})));
               }
               else
               {
                   func(ins, std::forward<Type>(obj.*(typename decltype(ins)::value_type{})));
               }
           });*/
    }
};

template <class T>
struct TypeInfo : TypeInfoBase<T>
{
    // DECL_STATIC_CONSTEXPR auto variables = MakeVariableList();
    // DECL_STATIC_CONSTEXPR auto methods   = MakeVariableList();
    // DECL_STATIC_CONSTEXPR auto propertys = MakeVariableList();
};
}
} // namespace screw::reflection
#endif // !_TYPE_INFO_H_
