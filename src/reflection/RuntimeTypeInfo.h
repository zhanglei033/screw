#pragma once
#ifndef _RUNTIME_TYPE_INFO_H_
#define _RUNTIME_TYPE_INFO_H_
#include "ReflBase.h"

#include "Comparison.h"
#include "RegistrationManager.h"
#include "TypeMetaData.h"

namespace screw {
namespace reflection {
class DECL_EXPORT RuntimeTypeInfo
{
public:
    using TypeMetaData = detail::TypeMetaData;

    template<class ClassType>
    class Type
    {
        template<size_t N>
        DECL_CONSTEXPR11 Type(const char (&name)[N])
        {
            auto obj = RuntimeTypeInfo::CreateType<ClassType>();
        }
    };


    RuntimeTypeInfo() DECL_NOEXCEPT : m_metaData(std::addressof(TypeMetaData::CreateInvalid())) {}

    RuntimeTypeInfo(detail::TypeMetaData* data) DECL_NOEXCEPT : m_metaData(data) {}

    /*template <typename T>
    static DECL_NODISCARD DECL_CONSTEXPR11 RuntimeTypeInfo GetType() DECL_NOEXCEPT
    {
        return detail::create_or_get_type<std::remove_cvref_t<T>>();
    }*/

    template <typename T, std::enable_if_t<!is_comparable_v<T>>>
    static DECL_NODISCARD DECL_CONSTEXPR11 RuntimeTypeInfo CreateType() DECL_NOEXCEPT
    {
        static const RuntimeTypeInfo obj = CreateTypeInfo(RegistrationManager::Instance().AddTypeMetaData(TypeMetaData::CreateMetaData<T>()));
        return obj;
    }

private:
    static DECL_NODISCARD RuntimeTypeInfo CreateTypeInfo(TypeMetaData* data) DECL_NOEXCEPT
    {
        return data ? RuntimeTypeInfo(data) : RuntimeTypeInfo();
    }

private:
    TypeMetaData* m_metaData;
};
}}     // namespace screw::reflection
#endif // !_RUNTIME_TYPE_INFO_H_
