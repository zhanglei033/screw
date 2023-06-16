#pragma once
#ifndef _REGISTRATION_MANAGER_H_
#define _REGISTRATION_MANAGER_H_
#include "ReflBase.h"

namespace screw {
namespace reflection {
class RegistrationManager
{
protected:
    RegistrationManager()                     = default;
    RegistrationManager(RegistrationManager&) = delete;
    ~RegistrationManager() { Unregister(); }
    RegistrationManager& operator=(RegistrationManager&) = delete;

public:
    using TypeMetaData = detail::TypeMetaData;

    static DECL_NODISCARD RegistrationManager& Instance()
    {
        static RegistrationManager instance;
        return instance;
    }

    TypeMetaData* AddTypeMetaData(std::unique_ptr<TypeMetaData> metaData)
    {
        m_typeMetaDataList.push_back(std::move(metaData));
        return m_typeMetaDataList.back().get();
    }

    void Unregister()
    {
    }

private:
    std::list<std::unique_ptr<detail::TypeMetaData>> m_typeMetaDataList;
};
}}     // namespace screw::reflection
#endif // !_REGISTRATION_MANAGER_H_
