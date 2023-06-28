#pragma once
#ifndef _MINIMAL_H_
#define _MINIMAL_H_
#include "base/Algorithms.h"
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
#include "base/Variant.h"
#include <list>
#include <vector>
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

    bool IsDelete() const DECL_NOEXCEPT
    {
        return m_info.index() == 0;
    }

    size_t& Index() DECL_NOEXCEPT
    {
        return IsDelete() ? std::get<DeleteInfo>(m_info).m_index : std::get<InsertInfo>(m_info).m_index;
    }
    size_t Index() const DECL_NOEXCEPT
    {
        return IsDelete() ? std::get<DeleteInfo>(m_info).m_index : std::get<InsertInfo>(m_info).m_index;
    }

    size_t& Size() DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(IsDelete(), "variant must be DeleteInfo");
#endif
        return std::get<DeleteInfo>(m_info).m_size;
    }
    size_t Size() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(IsDelete(), "variant must be DeleteInfo");
#endif
        return std::get<DeleteInfo>(m_info).m_size;
    }

    std::string& Data() DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(!IsDelete(), "variant must be InsertInfo");
#endif
        return std::get<InsertInfo>(m_info).m_data;
    }
    const std::string& Data() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(!IsDelete(), "variant must be InsertInfo");
#endif
        return std::get<InsertInfo>(m_info).m_data;
    }
};

size_t DiffPrefixSize(const uint8* data1, size_t size1, const uint8* data2, size_t size2)
{
    size_t size = std::min(size1, size2);
    for (size_t i = 0; i < size; i++)
    {
        if (data1[i] != data2[i])
        {
            return i;
        }
    }
    return size;
}
size_t DiffSuffixSize(const uint8* data1, size_t size1, const uint8* data2, size_t size2)
{
    size_t size = std::min(size1, size2);
    for (size_t i = 1; i <= size; i++)
    {
        if (data1[size1 - i] != data2[size2 - i])
        {
            return i - 1;
        }
    }
    return size;
}

static void DiffMain(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs);

static void DiffBisectSplit(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, size_t x, size_t y, std::list<DiffInfo>& diffs)
{
    DiffMain(data1, x, data2, y, offset, diffs);
    DiffMain(data1 + x, size1 - x, data2 + y, size2 - y, offset, diffs);
}

static void DiffBisect(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs)
{
    const size_t len1     = size1;
    const size_t len2     = size2;
    const size_t max_d    = (len1 + len2 + 1) / 2;
    const size_t v_offset = max_d;
    const size_t v_length = 2 * max_d;

    std::vector<size_t> v1, v2;
    v1.resize(v_length, -1);
    v2.resize(v_length, -1);
    v1[v_offset + 1] = 0;
    v2[v_offset + 1] = 0;

    const size_t delta = len1 - len2;
    const bool front   = (delta % 2 != 0);

    size_t k1start = 0;
    size_t k1end   = 0;
    size_t k2start = 0;
    size_t k2end   = 0;
    for (size_t d = 0; d < max_d; d++)
    {
        for (size_t k1 = k1start - d; k1 <= d - k1end; k1 += 2)
        {
            const size_t k1_offset = v_offset + k1;
            size_t x1;
            if (k1 == 0 - d || (k1 != d && v1[k1_offset - 1] < v1[k1_offset + 1]))
            {
                x1 = v1[k1_offset + 1];
            }
            else
            {
                x1 = v1[k1_offset - 1] + 1;
            }
            size_t y1 = x1 - k1;
            while (x1 < len1 && y1 < len2 && data1[x1] == data2[y1])
            {
                x1++;
                y1++;
            }
            v1[k1_offset] = x1;
            if (x1 > len1)
            {
                k1end += 2;
            }
            else if (y1 > len2)
            {
                k1start += 2;
            }
            else if (front)
            {
                size_t k2_offset = v_offset + delta - k1;
                if (k2_offset >= 0 && k2_offset < v_length && v2[k2_offset] != -1)
                {
                    size_t x2 = len1 - v2[k2_offset];
                    if (x1 >= x2)
                    {
                        DiffBisectSplit(data1, size1, data2, size2, offset, x1, y1, diffs);
                        return;
                    }
                }
            }
        }

        for (size_t k2 = k2start - d; k2 <= d - k2end; k2 += 2)
        {
            const size_t k2_offset = v_offset + k2;
            size_t x2;
            if (k2 == 0 - d || (k2 != d && v2[k2_offset - 1] < v2[k2_offset + 1]))
            {
                x2 = v2[k2_offset + 1];
            }
            else
            {
                x2 = v2[k2_offset - 1] + 1;
            }
            size_t y2 = x2 - k2;
            while (x2 < len1 && y2 < len2 && data1[len1 - x2 - 1] == data2[len2 - y2 - 1])
            {
                x2++;
                y2++;
            }
            v2[k2_offset] = x2;
            if (x2 > len1)
            {
                k2end += 2;
            }
            else if (y2 > len2)
            {
                k2start += 2;
            }
            else if (!front)
            {
                size_t k1_offset = v_offset + delta - k2;
                if (k1_offset >= 0 && k1_offset < v_length && v1[k1_offset] != -1)
                {
                    size_t x1 = v1[k1_offset];
                    size_t y1 = v_offset + x1 - k1_offset;
                    x2        = len1 - x2;
                    if (x1 >= x2)
                    {
                        DiffBisectSplit(data1, size1, data2, size2, offset, x1, y1, diffs);
                        return;
                    }
                }
            }
        }
    }
    diffs.push_back(DiffInfo(offset, size1));
    diffs.push_back(DiffInfo(offset, data2, size2));
    offset += size2;
}

static void DiffCompute1(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs)
{
    size_t index = find(data1, size1, 0, data2, size2);
    if (index != SIZE_MAX)
    {
        diffs.push_back(DiffInfo(offset, index));
        offset += size2;
        diffs.push_back(DiffInfo(offset, size1 - index - size2));
        return;
    }

    if (size2 == 1)
    {
        diffs.push_back(DiffInfo(offset, size1));
        diffs.push_back(DiffInfo(offset, data2, size2));
        offset += size2;
        return;
    }
    DiffBisect(data1, size1, data2, size2, offset, diffs);
}

static void DiffCompute2(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs)
{
    size_t index = find(data2, size2, 0, data1, size1);
    if (index != SIZE_MAX)
    {
        diffs.push_back(DiffInfo(offset, data2, index));
        offset += index + size1;
        diffs.push_back(DiffInfo(offset, data2 + index + size1, size2 - index - size1));
        offset += size2 - index - size1;
    }

    if (size1 == 1)
    {
        diffs.push_back(DiffInfo(offset, size1));
        diffs.push_back(DiffInfo(offset, data2, size2));
        offset += size2;
        return;
    }
    DiffBisect(data1, size1, data2, size2, offset, diffs);
}

static void DiffCompute(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs)
{
    if (size1 == 0)
    {
        diffs.push_back(DiffInfo(offset, data2, size2));
        offset += size2;
        return;
    }
    if (size2 == 0)
    {
        diffs.push_back(DiffInfo(offset, size1));
        return;
    }

    if (size1 > size2)
    {
        DiffCompute1(data1, size1, data2, size2, offset, diffs);
    }
    else
    {
        DiffCompute2(data1, size1, data2, size2, offset, diffs);
    }
}

static void DiffMain(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs)
{
    if (equal(data1, size1, data2, size2))
        return;

    size_t prefixSize = DiffPrefixSize(data1, size1, data2, size2);
    size_t suffixSize = DiffSuffixSize(data1 + prefixSize, size1 - prefixSize, data2 + prefixSize, size2 - prefixSize);

    offset += prefixSize;
    DiffCompute(data1 + prefixSize, size1 - prefixSize - suffixSize, data2 + prefixSize, size2 - prefixSize - suffixSize, offset, diffs);
    offset += suffixSize;
}

void MergeList(std::list<DiffInfo>& diffs)
{
    auto cur  = diffs.begin();
    auto next = std::next(cur);
    for (; next != diffs.end();)
    {
        if (MergeDelete(cur, next) || MergeInsert(cur, next))
        {
            diffs.erase(next);
            next = std::next(cur);
            continue;
        }
        next = std::next(++cur);
    }
}
bool MergeDelete(std::list<DiffInfo>::iterator& cur, std::list<DiffInfo>::iterator& next)
{
    if (cur->IsDelete() &&            //
        next->IsDelete() &&           //
        cur->Index() == next->Index() //
    )
    {
        cur->Size() += next->Size();

        return true;
    }
    return false;
}
bool MergeInsert(std::list<DiffInfo>::iterator& cur, std::list<DiffInfo>::iterator& next)
{
    if (!cur->IsDelete() &&                                //
        !next->IsDelete() &&                               //
        cur->Index() + cur->Data().size() == next->Index() //
    )
    {
        cur->Data() += next->Data();
        return true;
    }
    return false;
}

template <class InputType,
          class = std::enable_if_t<std::is_same_v<InputType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<InputType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >>
static std::list<DiffInfo> Compare(InputType&& input1, InputType&& input2)
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
