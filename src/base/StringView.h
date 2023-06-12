#pragma once
#ifndef _STRING_VIEW_H_
#define _STRING_VIEW_H_
#include "Platform.h"
#if STD_HAS_CXX17
#include <string>
#else
#include "CommonAlgorithms.h"
#include "IteratorBase.h"
#include "TypesDef.h"
#include <string>

namespace std {
template <class Elem, class Traits>
class basic_string_view
{
public:
    static_assert(is_same_v<Elem, typename Traits::char_type>,
                  "Bad char_traits for basic_string_view; "
                  "N4659 24.4.2 [string.view.template]/1 \"the type traits::char_type shall name the same type as charT.\"");

    static_assert(!is_array_v<Elem> && is_trivial_v<Elem> && is_standard_layout_v<Elem>,
                  "The character type of basic_string_view must be a non-array trivial standard-layout type. See N4861 "
                  "[strings.general]/1.");

    using traits_type            = Traits;
    using value_type             = Elem;
    using pointer                = Elem*;
    using const_pointer          = const Elem*;
    using reference              = Elem&;
    using const_reference        = const Elem&;
    using const_iterator         = screw::RandomIterator<Elem, basic_string_view<Elem, Traits>, true>;
    using iterator               = const_iterator;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using reverse_iterator       = const_reverse_iterator;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;

    static DECL_CONSTEXPR11 auto npos{static_cast<size_type>(-1)};

    DECL_CONSTEXPR11 basic_string_view() DECL_NOEXCEPT
        : m_data(nullptr),
          m_size(0) {}

    DECL_CONSTEXPR11 basic_string_view(const basic_string_view&) DECL_NOEXCEPT = default;

    DECL_CONSTEXPR11 basic_string_view(const const_pointer data) DECL_NOEXCEPT
        : m_data(data),
          m_size(Traits::length(data)) {}

    DECL_CONSTEXPR11 basic_string_view(const const_pointer data, const size_type size) DECL_NOEXCEPT
        : m_data(data),
          m_size(size) {}

    template <class Allocator = std::allocator<Elem>>
    DECL_CONSTEXPR11 basic_string_view(const std::basic_string<Elem, Traits, Allocator>& str) DECL_NOEXCEPT
        : m_data(str.data()),
          m_size(str.size())
    {
    }

    DECL_NODISCARD DECL_CONSTEXPR11 basic_string_view& operator=(const basic_string_view&) DECL_NOEXCEPT = default;

    DECL_NODISCARD DECL_CONSTEXPR11 const_reference operator[](const size_type idx) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_size > idx, "string_view subscript out of range");
#endif
        return m_data[idx];
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_iterator begin() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        return const_iterator(m_data, this);
#else
        return const_iterator(m_data);
#endif
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_iterator end() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        return const_iterator(m_data + m_size, this);
#else
        return const_iterator(m_data + m_size);
#endif
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_iterator cbegin() const DECL_NOEXCEPT
    {
        return begin();
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_iterator cend() const DECL_NOEXCEPT
    {
        return end();
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_reverse_iterator rbegin() const DECL_NOEXCEPT
    {
        return const_reverse_iterator{end()};
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_reverse_iterator rend() const DECL_NOEXCEPT
    {
        return const_reverse_iterator{begin()};
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_reverse_iterator crbegin() const DECL_NOEXCEPT
    {
        return rbegin();
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_reverse_iterator crend() const DECL_NOEXCEPT
    {
        return rend();
    }

    DECL_CONSTEXPR11 const_pointer unchecked_begin() const DECL_NOEXCEPT
    {
        return m_data;
    }

    DECL_CONSTEXPR11 const_pointer unchecked_end() const DECL_NOEXCEPT
    {
        return m_data + m_size;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type size() const DECL_NOEXCEPT
    {
        return m_size;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type length() const DECL_NOEXCEPT
    {
        return m_size;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool empty() const DECL_NOEXCEPT
    {
        return m_size == 0;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_pointer data() const DECL_NOEXCEPT
    {
        return m_data;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type max_size() const DECL_NOEXCEPT
    {
        return (std::min)(static_cast<size_t>(PTRDIFF_MAX), static_cast<size_t>(-1) / sizeof(Elem));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_reference at(const size_type idx) const
    {
#ifndef NDEBUG
        ASSERT(m_size > idx, "string_view subscript out of range");
#endif
        return m_data[idx];
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_reference front() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_size != 0, "cannot call front on empty string_view");
#endif
        return m_data[0];
    }

    DECL_NODISCARD DECL_CONSTEXPR11 const_reference back() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_size != 0, "cannot call front on empty string_view");
#endif
        return m_data[m_size - 1];
    }

    DECL_CONSTEXPR11 void remove_prefix(const size_type count) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_size >= count, "cannot remove prefix longer than total size");
#endif
        m_data += count;
        m_size -= count;
    }

    DECL_CONSTEXPR11 void remove_suffix(const size_type count) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_size >= count, "cannot remove suffix longer than total size");
#endif
        m_size -= count;
    }

    DECL_CONSTEXPR11 void swap(basic_string_view& other) DECL_NOEXCEPT
    {
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }

    DECL_CONSTEXPR20 size_type copy(const_pointer ptr, size_type count, const size_type offset = 0) const
    {
#ifndef NDEBUG
        ASSERT(m_size > offset, "string_view subscript out of range");
#endif
        count = (std::min)(count, m_size - offset);
        Traits::copy(ptr, m_data + offset, count);
        return count;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 basic_string_view substr(const size_type offset = 0, size_type count = npos) const
    {
#ifndef NDEBUG
        ASSERT(m_size > offset, "string_view subscript out of range");
#endif
        count = (std::min)(count, m_size - offset);
        return basic_string_view(m_data + offset, count);
    }

    DECL_CONSTEXPR11 bool equal(const basic_string_view other) const DECL_NOEXCEPT
    {
        return Traits::compare(m_data, m_size, other.m_data, std::min(m_size, other.m_size)) == 0;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 int compare(const basic_string_view other) const DECL_NOEXCEPT
    {
        const int ret = Traits::compare(m_data, m_size, other.m_data, std::min(m_size, other.m_size));
        return (ret != 0 ? ret : (m_size == other.m_size ? 0 : m_size < other.m_size ? -1 :
                                                                                       1));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 int compare(const size_type offset, const size_type size, const basic_string_view other) const
    {
        return substr(offset, size).compare(other);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 int compare(const size_type offset, const size_type size, const basic_string_view other, const size_type otherOffset, const size_type otherSize) const
    {
        return substr(offset, size).compare(other.substr(otherOffset, otherSize));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 int compare(const_pointer ptr) const
    {
        return compare(basic_string_view(ptr));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 int compare(const size_type offset, const size_type size, const_pointer ptr) const
    {
        return substr(offset, size).compare(basic_string_view(ptr));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 int compare(const size_type offset, const size_type size, const_pointer ptr, const size_type len) const
    {
        return substr(offset, size).compare(basic_string_view(ptr, len));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool starts_with(const basic_string_view other) const DECL_NOEXCEPT
    {
        return m_size >= other.m_size && Traits::compare(m_data, other.m_data, other.m_size) == 0;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool starts_with(const Elem other) const DECL_NOEXCEPT
    {
        return !empty() && Traits::eq(front(), other);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool starts_with(const_pointer other) const DECL_NOEXCEPT
    {
        return starts_with(basic_string_view(other));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool ends_with(const basic_string_view other) const DECL_NOEXCEPT
    {
        return m_size >= other.m_size && Traits::compare(m_data + (m_size - other.m_size), other.m_data, other.m_size) == 0;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool ends_with(const Elem other) const DECL_NOEXCEPT
    {
        return !empty() && Traits::eq(back(), other);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool ends_with(const_pointer other) const DECL_NOEXCEPT
    {
        return ends_with(basic_string_view(other));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool contains(const basic_string_view other) const DECL_NOEXCEPT
    {
        return find(other) != npos;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool contains(const Elem other) const DECL_NOEXCEPT
    {
        return find(other) != npos;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 bool contains(const_pointer other) const DECL_NOEXCEPT
    {
        return find(other) != npos;
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find(const basic_string_view other, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find(m_data, m_size, offset, other.m_data, other.m_size);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find(const Elem ch, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find(m_data, m_size, offset, &ch, 1);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find(const_pointer ptr, const size_type offset, const size_type count) const DECL_NOEXCEPT
    {
        return screw::find(m_data, m_size, offset, ptr, count);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find(const_pointer ptr, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find(m_data, m_size, offset, ptr, Traits::length(ptr));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type rfind(const basic_string_view other, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::rfind(m_data, m_size, offset, other.m_data, other.m_size);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type rfind(const Elem ch, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::rfind(m_data, m_size, offset, &ch, 1);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type rfind(const_pointer ptr, const size_type offset, const size_type count) const DECL_NOEXCEPT
    {
        return screw::rfind(m_data, m_size, offset, ptr, count);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type rfind(const_pointer ptr, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::rfind(m_data, m_size, offset, ptr, Traits::length(ptr));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_of(const basic_string_view other, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find_first_of(m_data, m_size, offset, other.m_data, other.m_size);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_of(const Elem ch, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find_first_of(m_data, m_size, offset, &ch, 1);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_of(const_pointer ptr, const size_type offset, const size_type count) const DECL_NOEXCEPT
    {
        return screw::find_first_of(m_data, m_size, offset, ptr, count);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_of(const_pointer ptr, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find_first_of(m_data, m_size, offset, ptr, Traits::length(ptr));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_of(const basic_string_view other, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::find_last_of(m_data, m_size, offset, other.m_data, other.m_size);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_of(const Elem ch, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::find_last_of(m_data, m_size, offset, &ch, 1);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_of(const_pointer ptr, const size_type offset, const size_type count) const DECL_NOEXCEPT
    {
        return screw::find_last_of(m_data, m_size, offset, ptr, count);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_of(const_pointer ptr, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::find_last_of(m_data, m_size, offset, ptr, Traits::length(ptr));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_not_of(const basic_string_view other, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find_first_not_of(m_data, m_size, offset, other.m_data, other.m_size);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_not_of(const Elem ch, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find_first_not_of(m_data, m_size, offset, &ch, 1);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_not_of(const_pointer ptr, const size_type offset, const size_type count) const DECL_NOEXCEPT
    {
        return screw::find_first_not_of(m_data, m_size, offset, ptr, count);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_first_not_of(const_pointer ptr, const size_type offset = 0) const DECL_NOEXCEPT
    {
        return screw::find_first_not_of(m_data, m_size, offset, ptr, Traits::length(ptr));
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_not_of(const basic_string_view other, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::find_last_not_of(m_data, m_size, offset, other.m_data, other.m_size);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_not_of(const Elem ch, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::find_last_not_of(m_data, m_size, offset, &ch, 1);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_not_of(const_pointer ptr, const size_type offset, const size_type count) const DECL_NOEXCEPT
    {
        return screw::find_last_not_of(m_data, m_size, offset, ptr, count);
    }

    DECL_NODISCARD DECL_CONSTEXPR11 size_type find_last_not_of(const_pointer ptr, const size_type offset = npos) const DECL_NOEXCEPT
    {
        return screw::find_last_not_of(m_data, m_size, offset, ptr, Traits::length(ptr));
    }

    template <typename Allocator = std::allocator<Elem>>
    std::basic_string<Elem, Traits, Allocator> to_string(const Allocator& alloc = {}) const
    {
        return std::basic_string<Elem, Traits, Allocator>(begin(), end(), alloc);
    }

    const_pointer m_data;
    size_type m_size;
};
using string_view = basic_string_view<char, std::char_traits<char>>;
} // namespace std
#endif
#endif // !_STRING_VIEW_H_
