#pragma once
#ifndef _RUNTIME_TYPE_DATA_H_
#define _RUNTIME_TYPE_DATA_H_
#include "base/Platform.h"
#include "base/StringView.h"
#include <bitset>

namespace screw {
namespace reflection {
namespace detail {

enum class type_trait_infos : std::size_t
{
    is_class = 0,
    is_enum,
    is_array,
    is_pointer,
    is_arithmetic,
    is_function_pointer,
    is_member_object_pointer,
    is_member_function_pointer,
    is_associative_container,
    is_sequential_container,
    is_template_instantiation,

    TYPE_TRAIT_COUNT
};

enum class type_of_visit : bool
{
    begin_visit_type,
    end_visit_type
};

using type_traits         = std::bitset<static_cast<std::size_t>(type_trait_infos::TYPE_TRAIT_COUNT)>;
using create_variant_func = decltype(&create_invalid_variant_policy::create_variant);
using get_base_types_func = decltype(&base_classes<int>::get_types);
using create_wrapper_func = void (*)(const argument& arg, variant& var);
using get_metadata_func   = std::vector<metadata>& (*)(void);
using get_class_data_func = class_data& (*)(void);
using visit_type_func     = void (*)(type_of_visit, visitor&, const type&);

struct type_data
{
    type_data* raw_type_data;
    type_data* wrapped_type;
    type_data* array_raw_type;

    std::string name;
    std::string_view type_name;

    std::size_t get_sizeof;
    std::size_t get_pointer_dimension;

    create_variant_func create_variant;
    get_base_types_func get_base_types; // FIXME: this info should not be stored, its just temporarily,
                                              // thats why we store it as function pointer

    enumeration_wrapper_base* enum_wrapper;
    get_metadata_func get_metadata;
    create_wrapper_func create_wrapper;
    visit_type_func visit_type;

    bool is_valid;
    DECL_FORCE_INLINE bool type_trait_value(type_trait_infos type_trait) const DECL_NOEXCEPT
    {
        return m_type_traits.test(static_cast<std::size_t>(type_trait));
    }

    type_traits m_type_traits;
    class_data m_class_data;
};

}}}    // namespace screw::reflection::detail
#endif // !_RUNTIME_TYPE_DATA_H_
