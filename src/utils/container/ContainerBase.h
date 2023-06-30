#pragma once
#include "base/Platform.h"
namespace screw {
namespace data_structure {
template <class VT, class ST, class DT, class P, class CP, class R, class CR>
struct DefaultContainerTypes
{
    using value_type      = VT;
    using size_type       = ST;
    using difference_type = DT;
    using pointer         = P;
    using const_pointer   = CP;
    using reference       = R;
    using const_reference = CR;
};

template <class KT, class VT, class ST, class DT, class P, class CP, class R, class CR, class N, class C>
struct TreeContainerTypes
{
    using key_type        = KT;
    using value_type      = VT;
    using size_type       = ST;
    using difference_type = DT;
    using pointer         = P;
    using const_pointer   = CP;
    using reference       = R;
    using const_reference = CR;
    using node_type       = N;
    using node_pointer    = typename node_type::node_pointer;
    using comparator_type = C;
};
struct element_copy_construct_tag
{
};

struct element_move_construct_tag
{
};

template <class Tag>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_element_copy_v = std::is_same_v<Tag, element_copy_construct_tag>;
template <class Tag>
DECL_INLINE_VAR DECL_CONSTEXPR11 bool is_element_move_v = std::is_same_v<Tag, element_move_construct_tag>;
}} // namespace screw::data_structure