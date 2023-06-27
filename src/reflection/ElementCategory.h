#pragma once
#ifndef _ELEMENT_CATEGORY_H_
#define _ELEMENT_CATEGORY_H_

namespace screw {
namespace reflection {
namespace detail {
struct ElementTag
{
};
struct PropertyTag : ElementTag
{
};
struct MemberTag : ElementTag
{
};
struct MemberStaticTag : MemberTag
{
};
struct MemberConstTag : MemberTag
{
};
struct MemberVariableTag : MemberTag
{
};
struct MemberVariableConstTag : MemberVariableTag, MemberConstTag
{
};
struct MemberVariableStaticTag : MemberVariableTag, MemberStaticTag
{
};
struct MemberVariableStaticConstTag : MemberVariableStaticTag, MemberVariableConstTag
{
};
struct MemberFunctionTag : MemberTag
{
};
struct MemberFunctionStaticTag : MemberFunctionTag, MemberStaticTag
{
};
struct MemberFunctionConstTag : MemberFunctionTag, MemberConstTag
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

}}} // namespace screw::reflection::detail

#endif // !_ELEMENT_CATEGORY_H_
