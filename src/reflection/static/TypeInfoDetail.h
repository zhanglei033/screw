#pragma once
#ifndef _TYPE_INFO_DETAIL_H_
#define _TYPE_INFO_DETAIL_H_
#include "StringType.h"
namespace screw {
namespace reflection {
namespace detail {
// ValueType是ElementValue的特化
// PropertysType是PropertyList的特化
//template <class ValueType, class PropertysType>
//struct MemberVariable : ValueType
//{
//    using traits_type    = typename ValueType::traits_type;
//    using propertys_Type = PropertysType;
//
//    DECL_CONSTEXPR11 MemberVariable(ValueType value, PropertysType propertys) DECL_NOEXCEPT
//        : ValueType{value},
//          m_propertys{propertys} {}
//
//    PropertysType m_propertys;
//};
//
//template <class... Variables>
//struct VariableList : ElementList<Variables...>
//{
//    DECL_CONSTEXPR11 VariableList(Variables... variables)
//        : ElementList<Variables...>{variables...} {}
//};
//
//// ValueType是ElementValue的特化
//// PropertysType是PropertyList的特化
//template <class ValueType, class PropertysType>
//struct MemberMethod : ValueType
//{
//    using traits_type    = typename ValueType::traits_type;
//    using propertys_Type = PropertysType;
//
//    DECL_CONSTEXPR11 MemberMethod(ValueType value, PropertysType propertys) DECL_NOEXCEPT
//        : ValueType{value},
//          m_propertys{propertys} {}
//
//    PropertysType m_propertys;
//};
//
//template <class... Methods>
//struct MethodList : ElementList<Methods...>
//{
//    DECL_CONSTEXPR11 MethodList(Methods... methods)
//        : ElementList<Methods...>{methods...} {}
//};
//
//// ValueType是ElementValue的特化
//template <class ValueType>
//struct Property : ValueType
//{
//    using traits_type = typename ValueType::traits_type;
//
//    DECL_CONSTEXPR11 Property(ValueType value) DECL_NOEXCEPT
//        : ValueType{value} {}
//};
//
//template <class... Propertys>
//struct PropertyList : ElementList<Propertys...>
//{
//    DECL_CONSTEXPR11 PropertyList(Propertys... propertys)
//        : ElementList<Propertys...>{propertys...} {}
//};

}
}}     // namespace screw::reflection::detail
#endif // _TYPE_INFO_DETAIL_H_
