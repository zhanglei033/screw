#include "Minimal.h"
namespace screw {
namespace algo {
namespace Minimal {
static size_t DiffPrefixSize(const uint8* data1, size_t size1, const uint8* data2, size_t size2) DECL_NOEXCEPT
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
static size_t DiffSuffixSize(const uint8* data1, size_t size1, const uint8* data2, size_t size2) DECL_NOEXCEPT
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

static void DiffBisectSplit(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, size_t x, size_t y, std::list<DiffInfo>& diffs) DECL_NOEXCEPT
{
    DiffMain(data1, x, data2, y, offset, diffs);
    DiffMain(data1 + x, size1 - x, data2 + y, size2 - y, offset, diffs);
}

static void DiffBisect(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs) DECL_NOEXCEPT
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

static void DiffCompute1(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs) DECL_NOEXCEPT
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

static void DiffCompute2(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs) DECL_NOEXCEPT
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

static void DiffCompute(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs) DECL_NOEXCEPT
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

static void DiffMain(const uint8* data1, size_t size1, const uint8* data2, size_t size2, size_t& offset, std::list<DiffInfo>& diffs) DECL_NOEXCEPT
{
    if (equal(data1, size1, data2, size2))
        return;

    size_t prefixSize = DiffPrefixSize(data1, size1, data2, size2);
    size_t suffixSize = DiffSuffixSize(data1 + prefixSize, size1 - prefixSize, data2 + prefixSize, size2 - prefixSize);

    offset += prefixSize;
    DiffCompute(data1 + prefixSize, size1 - prefixSize - suffixSize, data2 + prefixSize, size2 - prefixSize - suffixSize, offset, diffs);
    offset += suffixSize;
}

static bool MergeDelete(std::list<DiffInfo>::iterator& cur, std::list<DiffInfo>::iterator& next) DECL_NOEXCEPT
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

static bool MergeInsert(std::list<DiffInfo>::iterator& cur, std::list<DiffInfo>::iterator& next) DECL_NOEXCEPT
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

static void MergeList(std::list<DiffInfo>& diffs) DECL_NOEXCEPT
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
}}}