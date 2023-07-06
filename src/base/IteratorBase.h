#pragma once
#ifndef _ITERATOR_BASE_H_
#define _ITERATOR_BASE_H_
#include "Assert.h"
#include "Platform.h"
#include "STL.h"
#include "TypesDef.h"
namespace screw {
template <class Derived, class Elem, class Category>
class IteratorTraits
{
public:
    using derived_type      = Derived;
    using value_type        = Elem;
    using size_type         = size_t;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using pointer           = value_type*;
    using const_pointer     = const value_type*;
    using difference_type   = ptrdiff_t;
    using iterator_category = Category;
};

template <class Derived, class Elem, class Category>
class ConstIteratorTraits
{
public:
    using derived_type      = Derived;
    using value_type        = Elem;
    using size_type         = size_t;
    using reference         = const value_type&;
    using const_reference   = const value_type&;
    using pointer           = const value_type*;
    using const_pointer     = const value_type*;
    using difference_type   = ptrdiff_t;
    using iterator_category = Category;
};

// derived_type 需要满足：
// 1.Dereference()：返回迭代器当前指向元素的引用
// 2.Increment(difference_type n)：迭代器增加n（建议在Debug下，做越界检查）
// 3.Decrement(difference_type n)：迭代器减少n（建议在Debug下，做越界检查）
// 4.Equals(const derived_type&)：返回迭代器是否相等（建议在Debug下，做兼容性检查，迭代器是否指向同一个容器）
// 5.LessThan(const derived_type&)：返回迭代器是否小于（建议在Debug下，做兼容性检查，迭代器是否指向同一个容器）
// 6.Distance(const derived_type&)：返回两个迭代器之间的距离（建议在Debug下，做兼容性检查，迭代器是否指向同一个容器）
template <class Traits>
class IteratorBase
{
public:
    using derived_type      = typename Traits::derived_type;
    using value_type        = typename Traits::value_type;
    using size_type         = typename Traits::size_type;
    using reference         = typename Traits::reference;
    using const_reference   = typename Traits::const_reference;
    using pointer           = typename Traits::pointer;
    using const_pointer     = typename Traits::const_pointer;
    using difference_type   = typename Traits::difference_type;
    using iterator_category = typename Traits::iterator_category;

public:
    DECL_NODISCARD constexpr reference operator*() const DECL_NOEXCEPT { return AsDerived().Dereference(); }

    DECL_NODISCARD constexpr pointer operator->() const DECL_NOEXCEPT { return std::addressof(operator*()); }

    constexpr derived_type& operator++() DECL_NOEXCEPT
    {
        auto& derived = AsDerived();
        derived.Increment(1);
        return derived;
    }

    constexpr derived_type operator++(int) DECL_NOEXCEPT
    {
        auto& derived = AsDerived();
        auto temp     = derived;
        derived.Increment(1);
        return temp;
    }

    constexpr derived_type& operator--() DECL_NOEXCEPT
    {
        auto& derived = AsDerived();
        derived.Decrement(1);
        return derived;
    }

    derived_type operator--(int) DECL_NOEXCEPT
    {
        auto& derived = AsDerived();
        auto temp     = derived;
        derived.Decrement(1);
        return temp;
    }

    constexpr derived_type& operator+=(const difference_type size) DECL_NOEXCEPT
    {
        auto& derived = AsDerived();
        derived.Increment(size);
        return derived;
    }

    DECL_NODISCARD constexpr derived_type operator+(const difference_type size) const DECL_NOEXCEPT
    {
        auto temp = AsDerived();
        temp.Increment(size);
        return temp;
    }

    constexpr derived_type& operator-=(const difference_type size) DECL_NOEXCEPT
    {
        auto& derived = AsDerived();
        derived.Decrement(size);
        return derived;
    }

    DECL_NODISCARD constexpr derived_type operator-(const difference_type size) const DECL_NOEXCEPT
    {
        derived_type temp = AsDerived();
        temp.Decrement(size);
        return temp;
    }

    DECL_NODISCARD constexpr difference_type operator-(const derived_type& r) const DECL_NOEXCEPT
    {
        auto& derived = AsDerived();
        return derived.Distance(r);
    }

    DECL_NODISCARD friend constexpr bool operator<(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return LessThan(l, r); }
    DECL_NODISCARD friend constexpr bool operator>(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return LessThan(r, l); }

    DECL_NODISCARD friend constexpr bool operator<=(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return !(l > r); }
    DECL_NODISCARD friend constexpr bool operator>=(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return !(l < r); }

    DECL_NODISCARD friend constexpr bool operator==(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return Equals(l, r); }

    DECL_NODISCARD friend constexpr bool operator!=(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return !Equals(l, r); }

private:
    DECL_NODISCARD derived_type& AsDerived() DECL_NOEXCEPT { return static_cast<derived_type&>(*this); }

    DECL_NODISCARD const derived_type& AsDerived() const DECL_NOEXCEPT { return static_cast<const derived_type&>(*this); }

    DECL_NODISCARD static constexpr bool Equals(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return l.Equals(r); }

    DECL_NODISCARD static constexpr bool LessThan(const derived_type& l, const derived_type& r) DECL_NOEXCEPT { return l.LessThan(r); }
};

// 通用的随机访问读写迭代器模板
template <class T, bool IsConst>
class RandomIterator : public IteratorBase<std::conditional_t<IsConst,
                                                              ConstIteratorTraits<RandomIterator<T, IsConst>,
                                                                                  T,
                                                                                  std::random_access_iterator_tag>,
                                                              IteratorTraits<RandomIterator<T, IsConst>,
                                                                             T,
                                                                             std::random_access_iterator_tag>>>
{
public:
    using base_type         = IteratorBase<std::conditional_t<IsConst,
                                                      ConstIteratorTraits<RandomIterator<T, IsConst>, T, std::random_access_iterator_tag>,
                                                      IteratorTraits<RandomIterator<T, IsConst>, T, std::random_access_iterator_tag>>>;
    using value_type        = typename base_type::value_type;
    using size_type         = typename base_type::size_type;
    using reference         = typename base_type::reference;
    using const_reference   = typename base_type::const_reference;
    using pointer           = typename base_type::pointer;
    using const_pointer     = typename base_type::const_pointer;
    using difference_type   = typename base_type::difference_type;
    using iterator_category = typename base_type::iterator_category;

#ifndef NDEBUG
    constexpr RandomIterator() DECL_NOEXCEPT
        : m_data(nullptr),
          m_size(0),
          m_offset(0)
    {
    }

    constexpr explicit RandomIterator(pointer data, size_t size, size_t offset) DECL_NOEXCEPT
        : m_data(data),
          m_size(size),
          m_offset(offset) {}

    constexpr RandomIterator(const RandomIterator<T, false>& iter) DECL_NOEXCEPT
        : m_data(iter.m_data),
          m_size(iter.m_size),
          m_offset(iter.m_offset) {}

    constexpr RandomIterator& operator=(const RandomIterator<T, false>& iter) DECL_NOEXCEPT
    {
        m_data   = iter.m_data;
        m_size   = iter.m_size;
        m_offset = iter.m_offset;
        return *this;
    }

#else
    constexpr RandomIterator() DECL_NOEXCEPT
        : m_data(nullptr)
    {
    }
    constexpr explicit RandomIterator(pointer data) DECL_NOEXCEPT
        : m_data(data) {}

    constexpr RandomIterator(const RandomIterator<T, false>& iter) DECL_NOEXCEPT
        : m_data(iter.m_data) {}

    constexpr RandomIterator& operator=(const const RandomIterator<T, false>& iter) DECL_NOEXCEPT
    {
        m_data = iter.m_data;
        return *this;
    }
#endif

    DECL_NODISCARD constexpr reference operator[](const difference_type offset) const DECL_NOEXCEPT
    {
        return *(*this + offset);
    }
    DECL_NODISCARD constexpr pointer Unwrapped() DECL_NOEXCEPT
    {
        return m_data;
    }

    DECL_NODISCARD constexpr reference Dereference() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_data != nullptr, "Iterator not initialized");
        ASSERT(m_offset < m_size, "Iterator is last");
        return m_data[m_offset];
#else
        return *m_data;
#endif
    }

    constexpr void Increment(const difference_type n) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_data != nullptr, "Iterator not initialized");
        if (n > 0)
        {
            ASSERT(m_size - m_offset >= n, "Iterator out of bounds");
        }
        else
        {
            ASSERT(m_offset >= -n, "Iterator out of bounds");
        }
        m_offset += n;
#else
        m_data += n;
#endif
    }

    constexpr void Decrement(const difference_type n) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_data != nullptr, "Iterator not initialized");

        if (n > 0)
        {
            ASSERT(m_offset >= n, "Iterator out of bounds");
        }
        else
        {
            ASSERT(m_size - m_offset >= -n, "Iterator out of bounds");
        }
        m_offset -= n;
#else
        m_data -= n;
#endif
    }

    DECL_NODISCARD constexpr bool Equals(const RandomIterator& r) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_data == r.m_data && m_size == r.m_size, "Iterators incompatible");
        return m_offset == r.m_offset;
#else
        return m_data == r.m_data;
#endif
    }

    DECL_NODISCARD constexpr bool LessThan(const RandomIterator& r) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_data == r.m_data && m_size == r.m_size, "Iterators incompatible");
        return m_offset < r.m_offset;
#else
        return m_data < r.m_data;
#endif
    };

    DECL_NODISCARD constexpr difference_type Distance(const RandomIterator& r) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_data == r.m_data && m_size == r.m_size, "Iterators incompatible");
        return m_offset - r.m_offset;
#else
        return m_data - r.m_data;
#endif
    }

#ifndef NDEBUG
    friend void VerifyIterRange(const RandomIterator& first, const RandomIterator& last) noexcept
    {
        ASSERT(first.m_data == last.m_data && first.m_size == last.m_size, "Iterators incompatible");
        ASSERT(first.m_offset < last.m_offset, "Iterators range error");
    }
#endif

    pointer m_data;
#ifndef NDEBUG
    size_t m_size = 0;
    size_t m_offset;
#endif
};

template <class T>
constexpr void VerifyIterRange(const T* const first, const T* const last) DECL_NOEXCEPT
{
#ifndef NDEBUG
    ASSERT(first <= last, "Iterators incompatible");
#endif
}

template <class Iter, class Sentinel = Iter, class = void>
DECL_INLINE_VAR constexpr bool is_range_verifiable_v = false;

template <class Iter, class Sentinel>
DECL_INLINE_VAR constexpr bool is_range_verifiable_v<Iter, Sentinel, std::void_t<decltype(VerifyIterRange(std::declval<const Iter&>(), std::declval<const Sentinel&>()))>> = true;

template <class Iter, class Sentinel>
constexpr void VerifyIterRangeADL(const Iter& first, const Sentinel& last)
{
    if constexpr (is_range_verifiable_v<Iter, Sentinel>)
    {
        VerifyIterRange(first, last);
    }
}

template <class T, bool IsConst>
DECL_NODISCARD DECL_CONSTEXPR11 RandomIterator<T, IsConst> operator+(const typename RandomIterator<T, IsConst>::difference_type offset, RandomIterator<T, IsConst> iter) DECL_NOEXCEPT
{
    iter += offset;
    return iter;
}

} // namespace screw
#endif // !_ITERATOR_BASE_H_
