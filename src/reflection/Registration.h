#pragma once
#ifndef _REGISTRATION_h_
#define _REGISTRATION_h_
#include "base/Platform.h"
#include "base/STL.h"
#include "base/StringView.h"
namespace screw {
namespace reflection {
namespace registration {
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
using access_levels_list = std::meta_list<public_access_tag, protected_access_tag, private_access_tag>;

} // namespace detail
class DECL_EXPORT Registration
{
};
}
}} // namespace screw::reflection::registration

#if COMPILER_TYPE_TAG == COMPILER_MSVC_TAG
#define REFLECTION_REGISTRATION                                              \
    static void rttr_auto_register_reflection_function_();                   \
    namespace {                                                              \
    struct rttr__auto__register__                                            \
    {                                                                        \
        rttr__auto__register__()                                             \
        {                                                                    \
            rttr_auto_register_reflection_function_();                       \
        }                                                                    \
    };                                                                       \
    }                                                                        \
    static const rttr__auto__register__ RTTR_CAT(auto_register__, __LINE__); \
    static void rttr_auto_register_reflection_function_()
#else
#define REFLECTION_REGISTRATION                                                      \
    static void rttr_auto_register_reflection_function_() RTTR_DECLARE_PLUGIN_CTOR;  \
    static void rttr_auto_unregister_reflection_function() RTTR_DECLARE_PLUGIN_DTOR; \
                                                                                     \
    static void rttr_auto_unregister_reflection_function()                           \
    {                                                                                \
        rttr::detail::get_registration_manager().unregister();                       \
    }                                                                                \
    static void rttr_auto_register_reflection_function_()
#endif

#endif // !_REGISTRATION_h_
