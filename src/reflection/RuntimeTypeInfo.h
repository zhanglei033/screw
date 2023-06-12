#pragma once
#ifndef _RUNTIME_TYPE_INFO_H_
#define _RUNTIME_TYPE_INFO_H_
#include "base/Platform.h"
namespace screw {
namespace reflection {
class DECL_EXPORT RuntimeTypeInfo
{
public:
    using type_id_type = uintptr_t;

    RuntimeTypeInfo(const RuntimeTypeInfo& other) DECL_NOEXCEPT;
};
}}     // namespace screw::reflection
#endif // !_RUNTIME_TYPE_INFO_H_
