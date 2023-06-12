#pragma once
#ifndef _COMMON_ALGOROTHMS_H_
#define _COMMON_ALGOROTHMS_H_
#include "Platform.h"
#include <memory>

namespace screw {
// boyer_moore
template <class T>
DECL_NODISCARD DECL_CONSTEXPR11 static size_t find(const T* curData, size_t curSize, size_t offset, const T* data, size_t size) DECL_NOEXCEPT
{
    if (offset > curSize || size + offset > curSize || size == 0)
    {
        return SIZE_MAX;
    }
    auto replacedLastPos = size - 1;
    auto back            = data[replacedLastPos];

    size_t skip = 0;

    auto cur = curData + offset;
    auto end = curData + curSize - replacedLastPos;

    while (cur < end)
    {
        while (cur[replacedLastPos] != back)
        {
            if (++cur == end)
            {
                return SIZE_MAX;
            }
        }
        size_t i = 0;
        while (true)
        {
            if (cur[i] != data[i])
            {
                if (skip == 0)
                {
                    skip = 1;
                    while (skip <= replacedLastPos && data[replacedLastPos - skip] != back)
                    {
                        ++skip;
                    }
                }
                cur += skip;
                break;
            }
            if (++i == size)
            {
                return cur - curData;
            }
        }
    }
    return SIZE_MAX;
}

template <class T>
DECL_NODISCARD DECL_CONSTEXPR11 static size_t rfind(const T* curData, size_t curSize, size_t offset, const T* data, const size_t size) DECL_NOEXCEPT
{
    if (offset > curSize || size + offset > curSize || size == 0)
    {
        return SIZE_MAX;
    }

    auto replacedLastPos = size - 1;
    auto back            = data[replacedLastPos];
    auto front           = data[0];

    size_t skip = 0;

    auto cur = curData + curSize - offset - size;
    auto end = curData;

    while (cur >= end)
    {
        while (cur[0] != front)
        {
            if (cur-- == end)
            {
                return SIZE_MAX;
            }
        }
        size_t i = replacedLastPos;
        while (true)
        {
            if (cur[i] != data[i])
            {
                if (skip == 0)
                {
                    skip = 1;
                    while (skip >= replacedLastPos && data[replacedLastPos - skip] != front)
                    {
                        ++skip;
                    }
                }
                cur -= skip;
                break;
            }
            if (++i == size)
            {
                return cur - curData;
            }
        }
    }
    return SIZE_MAX;
}

template <class T>
DECL_NODISCARD DECL_CONSTEXPR11 static size_t find_first_of(const T* curData, size_t curSize, size_t offset, const T* data, size_t size) DECL_NOEXCEPT
{
    if (offset > curSize || size == 0)
    {
        return SIZE_MAX;
    }
    auto cur = curData + offset;
    auto end = curData + curSize;

    while (cur != end)
    {
        if (std::char_traits<T>::find(data, size, *cur) != nullptr)
        {
            return cur - curData;
        }
        cur++;
    }

    return SIZE_MAX;
}

template <class T>
DECL_NODISCARD DECL_CONSTEXPR11 static size_t find_last_of(const T* curData, size_t curSize, size_t offset, const T* data, size_t size) DECL_NOEXCEPT
{
    if (offset > curSize || size == 0)
    {
        return SIZE_MAX;
    }

    auto cur = curData + curSize - offset - 1;
    auto end = curData;

    while (cur >= end)
    {
        if (std::char_traits<T>::find(data, size, *cur) != nullptr)
        {
            return cur - curData;
        }
        cur--;
    }

    return SIZE_MAX;
}

template <class T>
DECL_NODISCARD DECL_CONSTEXPR11 static size_t find_first_not_of(const T* curData, size_t curSize, size_t offset, const T* data, size_t size) DECL_NOEXCEPT
{
    if (offset > curSize || size == 0)
    {
        return SIZE_MAX;
    }
    auto cur = curData + offset;
    auto end = curData + curSize;

    while (cur != end)
    {
        if (std::char_traits<T>::find(data, size, *cur) == nullptr)
        {
            return cur - curData;
        }
        cur++;
    }

    return cur - curData;
}

template <class T>
DECL_NODISCARD DECL_CONSTEXPR11 static size_t find_last_not_of(const T* curData, size_t curSize, size_t offset, const T* data, size_t size) DECL_NOEXCEPT
{
    if (offset > curSize || size == 0)
    {
        return SIZE_MAX;
    }

    auto cur = curData + curSize - offset - 1;
    auto end = curData;

    while (cur >= end)
    {
        if (std::char_traits<T>::find(data, size, *cur) == nullptr)
        {
            return cur - curData;
        }
        cur--;
    }

    return cur - curData;
}

}
#endif // !_COMMON_ALGOROTHMS_H_
