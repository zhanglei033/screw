#pragma once
#ifndef _MINIMAL_H_
#define _MINIMAL_H_
#include "base/Algorithms.h"
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
#include "base/Variant.h"
#include "base/Assert.h"
#include <list>
#include <vector>
#include <string>
namespace screw {
namespace algo {
namespace Minimal {
struct DiffInfo
{
    struct DeleteInfo
    {
        size_t m_index = 0;
        size_t m_size  = 0;
    };
    struct InsertInfo
    {
        size_t m_index = 0;
        std::string m_data;
    };

    std::variant<DeleteInfo, InsertInfo> m_info;
    DiffInfo() DECL_NOEXCEPT{};
    DiffInfo(size_t index, size_t size) DECL_NOEXCEPT
        : m_info(DeleteInfo{index, size}) {}
    DiffInfo(size_t index, const uint8* data, size_t size) DECL_NOEXCEPT
        : m_info(InsertInfo{index, std::string(reinterpret_cast<const char*>(data), size)}) {}

    DECL_NODISCARD bool IsDelete() const DECL_NOEXCEPT { return m_info.index() == 0; }

    DECL_NODISCARD size_t& Index() DECL_NOEXCEPT { return IsDelete() ? std::get<DeleteInfo>(m_info).m_index : std::get<InsertInfo>(m_info).m_index; }
    DECL_NODISCARD size_t Index() const DECL_NOEXCEPT { return IsDelete() ? std::get<DeleteInfo>(m_info).m_index : std::get<InsertInfo>(m_info).m_index; }

    DECL_NODISCARD size_t& Size() DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(IsDelete(), "variant must be DeleteInfo");
#endif
        return std::get<DeleteInfo>(m_info).m_size;
    }
    DECL_NODISCARD size_t Size() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(IsDelete(), "variant must be DeleteInfo");
#endif
        return std::get<DeleteInfo>(m_info).m_size;
    }

    DECL_NODISCARD std::string& Data() DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(!IsDelete(), "variant must be InsertInfo");
#endif
        return std::get<InsertInfo>(m_info).m_data;
    }
    DECL_NODISCARD const std::string& Data() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(!IsDelete(), "variant must be InsertInfo");
#endif
        return std::get<InsertInfo>(m_info).m_data;
    }
};

static void DiffMain(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs) DECL_NOEXCEPT;

static void MergeList(std::list<DiffInfo>& diffs) DECL_NOEXCEPT;

template <class InputType,
          class = std::enable_if_t<std::is_same_v<InputType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<InputType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >>
DECL_NODISCARD static std::list<DiffInfo> Compare(InputType&& input1, InputType&& input2) DECL_NOEXCEPT
{
    std::list<DiffInfo> diffs;
    size_t offset = 0;
    if constexpr (std::is_same_v<InputType, char*>)
    {
        DiffMain(reinterpret_cast<const uint8*>(input1),
                 std::char_traits<char>::length(input1),
                 reinterpret_cast<const uint8*>(input2),
                 std::char_traits<char>::length(input2),
                 offset,
                 diffs);
    }
    else
    {
        using pointer_type = decltype(input1.data());
        using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        DiffMain(reinterpret_cast<const uint8*>(input1.data()),
                 input1.size() * sizeof(value_type),
                 reinterpret_cast<const uint8*>(input2.data()),
                 input2.size() * sizeof(value_type),
                 offset,
                 diffs);
    }
    MergeList(diffs);
    return diffs;
}

}}}    // namespace screw::algo::Minimal
#endif // !_MINIMAL_H_
