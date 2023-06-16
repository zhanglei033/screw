#pragma once
#ifndef _REFL_BASE_H_
#define _REFL_BASE_H_
#include "base/Platform.h"
#include "base/STL.h"
#include "base/StaticString.h"
#include "base/StringView.h"
#include "base/TypesDef.h"
#include "base/Variant.h"

#include <list>
namespace screw {
namespace reflection {
class RuntimeTypeInfo;

namespace detail {
struct TypeMetaData;
}

}} // namespace screw::reflection

#endif // !_REFL_BASE_H_
