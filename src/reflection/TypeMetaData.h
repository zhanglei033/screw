#pragma once
#ifndef _TYPE_META_DATA_H_
#define _TYPE_META_DATA_H_
#include "base/StringView.h"

#include "reflection/static/TypeName.h"
#include <string>
namespace screw {
namespace reflection {
namespace detail {
struct TypeMetaData
{
    TypeMetaData* m_rawData     = nullptr;
    TypeMetaData* m_wrappedData = nullptr;
    TypeMetaData* m_arrayData   = nullptr;

    std::string m_name;
    std::string_view m_typeName;

    static TypeMetaData& CreateInvalid() DECL_NOEXCEPT
    {
        static TypeMetaData instance;
        instance.m_rawData     = &instance;
        instance.m_wrappedData = &instance;
        instance.m_arrayData   = &instance;
        return instance;
    }

    static TypeMetaData* CreateInvalidPtr() DECL_NOEXCEPT
    {
        return std::addressof(CreateInvalid());
    }

    template <typename T>
    static std::unique_ptr<TypeMetaData> CreateMetaData()
    {
        return std::unique_ptr<TypeMetaData>(
            new TypeMetaData{
                CreateInvalidPtr(),
                CreateInvalidPtr(),
                CreateInvalidPtr(),
                detail::Nameof<T>().ToString(),
                detail::Nameof<T>().ToStringView()});
    }
};

}
}}     // namespace screw::reflection::detail
#endif // !_TYPE_META_DATA_H_
