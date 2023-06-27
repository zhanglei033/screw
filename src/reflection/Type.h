#pragma once
#ifndef _TYPE_H_
#define _TYPE_H_
#include "ReflBase.h"
namespace screw {
namespace reflection {
template <class T>
struct Type
{
    using type =  T;
};

template <class NameType, class... Args, class = std::enable_if_t<std::is_constructible_v<typename Type<NameType>::type, Args...>>>
auto GetByName(Args&&... args)
{
    using type = typename Type<NameType>::type;
    type obj;
    std::construct_in_place(obj, std::forward<Args>(args)...);
    return obj;
}
}
} // namespace screw::reflection
#endif // !_TYPE_H_
