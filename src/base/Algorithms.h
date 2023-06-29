#pragma once
#ifndef _ALGOROTHMS_H_
#define _ALGOROTHMS_H_
#include "Platform.h"
#include "STL.h"
#include <memory>
namespace screw {
namespace algo {
// 因为insert算法只进行插入操作，不进行任何的内存分配行为
// 所以使用者调用时，需要保证有足够的内存进行插入
template <class T, class Traits = std::char_traits<T>>
DECL_STATIC_CONSTEXPR void insert(T* curData,             // 当前操作的数据
                                  const size_t curSize,   // 数据大小
                                  const size_t insertPos, // 需要插入的位置
                                  const T* insertData,    // 插入的数据
                                  const size_t insertSize // 插入的大小
                                  ) DECL_NOEXCEPT
{
    if (insertPos == curSize) // 插入位置是在操作的数据的末尾，没有重叠部分直接拷贝即可
    {
        Traits::copy(curSize + curSize, insertData, insertSize);
    }
    else // 存在重叠部分，需要先将重叠部分往后移，然后再进行拷贝
    {
        auto insertDataPos = curData + insertPos;
        Traits::move(insertDataPos + insertSize, insertDataPos, curSize - insertPos);
        Traits::copy(insertDataPos, insertData, insertSize);
    }
}

template <class Iter, class Traits = std::char_traits<typename std::iterator_traits<Iter>::value_type>>
DECL_STATIC_CONSTEXPR void insert(Iter first, // 第一个
                                  Iter last,        // 最后一个
                                  Iter insertPos,   // 插入位置
                                  Iter insertFirst, // 插入的第一个
                                  Iter insertLast   // 插入的最后一个
                                  ) DECL_NOEXCEPT
{
    if constexpr (std::is_random_iter_v<Iter>)
    {
        insert(std::get_unwrapped(first),
               std::distance(first, last),
               std::distance(first, insertPos),
               std::get_unwrapped(insertFirst),
               std::distance(insertFirst, insertLast));
    }
    else
    {
        auto insertSize = std::distance(insertFirst, insertLast);
        auto newLastPos = std::next(last, insertSize);
        while (insertPos != last)
        {
            Traits::assign(*newLastPos--, *last--);
        }

        while (insertFirst != insertLast)
        {
            Traits::assign(*insertPos++, *insertFirst++);
        }
    }
}

// erase只做数据清除，将指定区间的数据清除
template <class T, class Traits = std::char_traits<T>>
DECL_STATIC_CONSTEXPR void erase(T* curData,            // 当前操作的数据
                                 const size_t curSize,  // 数据大小
                                 const size_t erasePos, // 清除位置
                                 const size_t eraseSize // 清除大小
                                 ) DECL_NOEXCEPT
{
    if (eraseSize >= curSize - erasePos) // erasePos后面全部清空
    {
        Traits::assign(curData + erasePos, curSize - erasePos, Traits::to_char_type(0));
    }
    else // 未清除的区间前移，然后将剩下的清除
    {
        auto eraseDataPos = curData + erasePos;
        Traits::move(eraseDataPos, eraseDataPos + eraseSize, (curSize - erasePos - eraseSize));
        Traits::assign(curData + curSize - eraseSize, eraseSize, Traits::to_char_type(0));
    }
}

template <class Iter, class Traits = std::char_traits<typename std::iterator_traits<Iter>::value_type>>
DECL_STATIC_CONSTEXPR void erase(Iter first,      // 第一个
                                 Iter last,       // 最后一个
                                 Iter eraseFirst, // 清除的第一个
                                 Iter eraseLast   // 清除的最后一个
                                 ) DECL_NOEXCEPT
{
    if constexpr (std::is_random_iter_v<Iter>)
    {
        erase(std::get_unwrapped(first),
              std::distance(first, last),
              std::distance(first, eraseFirst),
              std::distance(eraseFirst, eraseLast));
    }
    else
    {
        while (eraseLast != last)
        {
            Traits::assign(*eraseFirst++, *eraseLast++);
        }
        while (eraseFirst != last)
        {
            Traits::assign(*eraseFirst++, Traits::to_char_type(0));
        }
    }
}

// replace 只做替换操作
template <class T, class Traits = std::char_traits<T>>
DECL_STATIC_CONSTEXPR void repalce(T* curData,                // 当前操作的数据
                                   const size_t curSize,      // 数据大小
                                   const size_t replacedPos,  // 需要替换的位置
                                   const size_t replacedSize, // 需要替换大小
                                   const T* newData,          // 新的数据
                                   const size_t newSize       // 新数据大小
                                   ) DECL_NOEXCEPT
{
    if (replacedSize == newSize)
    {
        Traits::copy(curData + replacedPos, newData, newSize);
    }
    else
    {
        T* replacedData = curData + replacedPos;
        Traits::move(replacedData + newSize, replacedData + replacedSize, curSize - replacedPos - replacedSize);
        Traits::copy(replacedData, newData, newSize);
    }
}

template <class Iter, class Traits = std::char_traits<typename std::iterator_traits<Iter>::value_type>>
DECL_STATIC_CONSTEXPR void repalce(Iter first,         // 第一个
                                   Iter last,          // 最后一个
                                   Iter replacedFirst, // 需要替换的第一个
                                   Iter replacedLast,  // 需要替换的最后一个
                                   Iter newDataFirst,  // 新的数据第一个
                                   Iter newDataLast    // 新的数据最后一个
                                   ) DECL_NOEXCEPT
{
    if constexpr (std::is_random_iter_v<Iter>)
    {
        repalce(std::get_unwrapped(first),
                std::distance(first, last),
                std::distance(first, replacedFirst),
                std::distance(replacedFirst, replacedLast),
                std::get_unwrapped(newDataFirst),
                std::distance(newDataFirst, newDataLast));
    }
    else
    {
        auto replacedSize = std::distance(replacedFirst, replacedLast);
        auto newDataSize  = std::distance(newDataFirst, newDataLast);

        if (newDataSize > replacedSize)
        {
            auto newLastPos           = std::next(last, newDataSize - replacedSize);
            auto replaceCompletionPos = std::next(replacedFirst, newDataSize);
            while (replaceCompletionPos != newLastPos)
            {
                Traits::assign(*newLastPos--, *last--);
            }
        }
        else
        {
            auto newLastPos           = std::prev(last, replacedSize - newDataSize);
            auto replaceCompletionPos = std::next(replacedFirst, newDataSize);
            while (replaceCompletionPos != newLastPos)
            {
                Traits::assign(*replaceCompletionPos--, *replacedLast--);
            }
        }

        while (newDataFirst != newDataLast)
        {
            Traits::assign(*replacedFirst++, *newDataFirst++);
        }
    }
}

// boyer_moore
template <class T>
DECL_NODISCARD DECL_STATIC_CONSTEXPR size_t find(const T* curData,
                                                 size_t curSize,
                                                 size_t offset,
                                                 const T* data,
                                                 size_t size) DECL_NOEXCEPT
{
    if (offset > curSize || size + offset > curSize)
    {
        return SIZE_MAX;
    }
    if (size == 0)
    {
        return offset;
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
DECL_NODISCARD DECL_STATIC_CONSTEXPR size_t rfind(const T* curData,
                                                  size_t curSize,
                                                  size_t offset,
                                                  const T* data,
                                                  const size_t size) DECL_NOEXCEPT
{
    offset = std::min(offset, curSize);
    if (size == 0)
    {
        return offset;
    }

    auto replacedLastPos = size - 1;
    auto back            = data[replacedLastPos];
    auto front           = data[0];

    size_t skip = 0;

    auto cur = curData + offset;
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
DECL_NODISCARD DECL_STATIC_CONSTEXPR size_t find_first_of(const T* curData,
                                                          size_t curSize,
                                                          size_t offset,
                                                          const T* data,
                                                          size_t size) DECL_NOEXCEPT
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
DECL_NODISCARD DECL_STATIC_CONSTEXPR size_t find_last_of(const T* curData,
                                                         size_t curSize,
                                                         size_t offset,
                                                         const T* data,
                                                         size_t size) DECL_NOEXCEPT
{
    offset = std::min(offset, curSize - 1);
    if (size == 0)
    {
        return offset;
    }

    auto cur = curData + offset;
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
DECL_NODISCARD DECL_STATIC_CONSTEXPR size_t find_first_not_of(const T* curData,
                                                              size_t curSize,
                                                              size_t offset,
                                                              const T* data,
                                                              size_t size) DECL_NOEXCEPT
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

    return SIZE_MAX;
}

template <class T>
DECL_NODISCARD DECL_STATIC_CONSTEXPR size_t find_last_not_of(const T* curData,
                                                             size_t curSize,
                                                             size_t offset,
                                                             const T* data,
                                                             size_t size) DECL_NOEXCEPT
{
    offset = std::min(offset, curSize - 1);
    if (size == 0)
    {
        return offset;
    }

    auto cur = curData + offset;
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

// func(const T*, size_t) 将截取的每一段数据传出
template <class T, class Func>
DECL_STATIC_CONSTEXPR void split(const T* curData,
                                 size_t curSize,
                                 const T* data,
                                 size_t size,
                                 Func&& func) DECL_NOEXCEPT
{
    if (!curData || !data)
        return;

    size_t offset  = find(curData, curSize, 0, data, size);
    size_t lastPos = 0;
    while (true)
    {
        if (offset == SIZE_MAX)
        {
            func(curData + lastPos, curSize - lastPos);
            return;
        }

        func(curData + lastPos, offset - lastPos);
        lastPos = offset + size;
        offset  = find(curData, curSize, lastPos, data, size);
    }
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR int compare(const T* curData,
                                                 size_t curSize,
                                                 const T* data,
                                                 size_t size) DECL_NOEXCEPT
{
    const int ret = Traits::compare(curData, data, std::min(curSize, size));
    return ret != 0 ? ret :
                      (curSize > size ? 1 :
                                        (curSize < size ? -1 :
                                                          0));
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool equal(const T* curData,
                                                size_t curSize,
                                                const T* data,
                                                size_t size) DECL_NOEXCEPT
{
    return curSize == size && Traits::compare(curData, data, size) == 0;
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool not_equal(const T* curData,
                                                    size_t curSize,
                                                    const T* data,
                                                    size_t size) DECL_NOEXCEPT
{
    return curSize != size || Traits::compare(curData, data, size) != 0;
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool less(const T* curData,
                                               size_t curSize,
                                               const T* data,
                                               size_t size) DECL_NOEXCEPT
{
    return Traits::compare(curData, data, std::min(curSize, size)) < 0;
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool less_or_equal(const T* curData,
                                                        size_t curSize,
                                                        const T* data,
                                                        size_t size) DECL_NOEXCEPT
{
    auto ret = Traits::compare(curData, data, std::min(curSize, size));
    return ret < 0 || (curSize == size && ret == 0);
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool greater(const T* curData,
                                                  size_t curSize,
                                                  const T* data,
                                                  size_t size) DECL_NOEXCEPT
{
    return Traits::compare(curData, data, std::min(curSize, size)) > 0;
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool greater_or_equal(const T* curData,
                                                           size_t curSize,
                                                           const T* data,
                                                           size_t size) DECL_NOEXCEPT
{
    auto ret = Traits::compare(curData, data, std::min(curSize, size));
    return ret > 0 || (curSize == size && ret == 0);
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool starts_with(const T* curData,
                                                      size_t curSize,
                                                      const T* data,
                                                      size_t size) DECL_NOEXCEPT
{
    return size == 0 || (curSize >= size && Traits::compare(curData, data, size) == 0);
}

template <class T, class Traits = std::char_traits<T>>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool ends_with(const T* curData,
                                                    size_t curSize,
                                                    const T* data,
                                                    size_t size) DECL_NOEXCEPT
{
    return size == 0 || (curSize >= size && Traits ::compare(curData + curSize - size, data, size) == 0);
}

template <class T>
DECL_NODISCARD DECL_STATIC_CONSTEXPR bool contains(const T* curData,
                                                   size_t curSize,
                                                   const T* data,
                                                   size_t size) DECL_NOEXCEPT
{
    return find(curData, curSize, 0, data, size) != SIZE_MAX;
}

template <class T>
constexpr static void reverse(T* curData, size_t curSize) noexcept
{
    if (curSize == 0)
        return;

    size_t left  = 0;
    size_t right = curSize - 1;
    while (left < right)
        std::swap(curData[left++], curData[right--]);
}
}}     // namespace screw::algo
#endif // !_ALGOROTHMS_H_
