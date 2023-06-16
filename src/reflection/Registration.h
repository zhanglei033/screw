#pragma once
#ifndef _REGISTRATION_h_
#define _REGISTRATION_h_
#include "base/Platform.h"
#include "base/STL.h"
#include "base/StringView.h"
namespace screw {
namespace reflection {
namespace detail {
struct private_access_tag
{
};
struct protected_access_tag : private_access_tag
{
};
struct public_access_tag : protected_access_tag
{
};
using access_levels_list_type = std::meta_list<public_access_tag, protected_access_tag, private_access_tag>;

} // namespace detail
class DECL_EXPORT Registration
{
public:
    template<class Type>
    class ReflClass
    {
        ReflClass(std::string_view name)
        {

        }
    };
};
}}     // namespace screw::reflection
#endif // !_REGISTRATION_h_
