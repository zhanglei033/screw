#pragma once
#ifndef _ITERATOR_BASE_H_
#define _ITERATOR_BASE_H_
#include "Assert.h"
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
    using reference         = const value_type&;
    using const_reference   = const value_type&;
    using pointer           = const value_type*;
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
    DECL_NODISCARD constexpr reference operator*() const noexcept { return AsDerived().Dereference(); }

    DECL_NODISCARD constexpr pointer operator->() const noexcept { return std::addressof(operator*()); }

    constexpr derived_type& operator++() noexcept
    {
        auto& derived = AsDerived();
        derived.Increment(1);
        return derived;
    }

    constexpr derived_type operator++(int) noexcept
    {
        auto& derived = AsDerived();
        auto temp     = derived;
        derived.Increment(1);
        return temp;
    }

    constexpr derived_type& operator--() noexcept
    {
        auto& derived = AsDerived();
        derived.Decrement(1);
        return derived;
    }

    derived_type operator--(int) noexcept
    {
        auto& derived = AsDerived();
        auto temp     = derived;
        derived.Decrement(1);
        return temp;
    }

    constexpr derived_type& operator+=(const difference_type size) noexcept
    {
        auto& derived = AsDerived();
        derived.Increment(size);
        return derived;
    }

    DECL_NODISCARD constexpr derived_type operator+(const difference_type size) const noexcept
    {
        auto temp = AsDerived();
        temp.Increment(size);
        return temp;
    }

    constexpr derived_type& operator-=(const difference_type size) noexcept
    {
        auto& derived = AsDerived();
        derived.Decrement(size);
        return derived;
    }

    DECL_NODISCARD constexpr derived_type operator-(const difference_type size) const noexcept
    {
        derived_type temp = AsDerived();
        temp.Decrement(size);
        return temp;
    }

    DECL_NODISCARD constexpr difference_type operator-(const derived_type& r) const noexcept
    {
        auto& derived = AsDerived();
        return derived.Distance(r);
    }

    DECL_NODISCARD friend constexpr bool operator<(const derived_type& l, const derived_type& r) noexcept { return LessThan(l, r); }

    DECL_NODISCARD friend constexpr bool operator<=(const derived_type& l, const derived_type& r) noexcept { return LessThan(l, r) || Equals(l, r); }

    DECL_NODISCARD friend constexpr bool operator==(const derived_type& l, const derived_type& r) noexcept { return Equals(l, r); }

    DECL_NODISCARD friend constexpr bool operator!=(const derived_type& l, const derived_type& r) noexcept { return !Equals(l, r); }

    DECL_NODISCARD friend constexpr bool operator>(const derived_type& l, const derived_type& r) noexcept { return !LessThan(l, r) && !Equals(l, r); }

    DECL_NODISCARD friend constexpr bool operator>=(const derived_type& l, const derived_type& r) noexcept { return !LessThan(l, r); }

private:
    DECL_NODISCARD derived_type& AsDerived() noexcept { return static_cast<derived_type&>(*this); }

    DECL_NODISCARD const derived_type& AsDerived() const noexcept { return static_cast<const derived_type&>(*this); }

    DECL_NODISCARD static constexpr bool Equals(const derived_type& l, const derived_type& r) noexcept { return l.Equals(r); }

    DECL_NODISCARD static constexpr bool LessThan(const derived_type& l, const derived_type& r) noexcept { return l.LessThan(r); }
};

// 通用的随机访问读写迭代器模板
template <class T, bool IsConst>
class RandomIterator : public IteratorBase<std::conditional_t<IsConst, ConstIteratorTraits<RandomIterator<T, IsConst>, T, std::random_access_iterator_tag>, IteratorTraits<RandomIterator<T, IsConst>, T, std::random_access_iterator_tag>>>
{
public:
    using base_type         = IteratorBase<std::conditional_t<IsConst,
                                                      ConstIteratorTraits<RandomIterator<T, IsConst>, T, std::random_access_iterator_tag>,
                                                      IteratorTraits<RandomIterator<T, IsConst>, T, std::random_access_iterator_tag>>>;
    using derived_type      = typename base_type::derived_type;
    using value_type        = typename base_type::value_type;
    using size_type         = typename base_type::size_type;
    using reference         = typename base_type::reference;
    using const_reference   = typename base_type::const_reference;
    using pointer           = typename base_type::pointer;
    using const_pointer     = typename base_type::const_pointer;
    using difference_type   = typename base_type::difference_type;
    using iterator_category = typename base_type::iterator_category;

#ifndef NDEBUG
    constexpr RandomIterator() noexcept
        : m_data(nullptr), m_size(0), m_offset(0)
    {
    }

    constexpr explicit RandomIterator(pointer data, size_t size, size_t offset) noexcept
        : m_data(data), m_size(size), m_offset(offset) {}

    constexpr RandomIterator(const RandomIterator<T, true>& iter) noexcept
        : m_data(iter.m_data), m_size(iter.m_size), m_offset(iter.m_offset) {}

    constexpr RandomIterator& operator=(const const RandomIterator<T, true>& iter) noexcept
    {
        m_data   = iter.m_data;
        m_size   = iter.m_size;
        m_offset = iter.m_offset;
        return *this;
    }
#else
    constexpr RandomIterator() noexcept
        : m_data(nullptr)
    {
    }
    constexpr explicit RandomIterator(pointer data) noexcept
        : m_data(data) {}

    constexpr RandomIterator(const RandomIterator<T, true>& iter) noexcept
        : m_data(iter.m_data) {}

    constexpr RandomIterator& operator=(const const RandomIterator<T, true>& iter) noexcept
    {
        m_data = iter.m_data;
        return *this;
    }
#endif

    DECL_NODISCARD constexpr reference operator[](const difference_type offset) const noexcept
    {
        return *(*this + offset);
    }
    DECL_NODISCARD constexpr pointer Unwrapped() noexcept
    {
        return m_data;
    }

    DECL_NODISCARD constexpr reference Dereference() const noexcept
    {
#ifndef NDEBUG
        ASSERT(m_data != nullptr, "Iterator not initialized");
        ASSERT(m_offset < m_size, "Iterator is last");
        return m_data[m_offset];
#else
        return *m_data;
#endif
    }

    constexpr void Increment(const difference_type n) noexcept
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

    constexpr void Decrement(const difference_type n) noexcept
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

    DECL_NODISCARD constexpr bool Equals(const RandomIterator& r) const noexcept
    {
#ifndef NDEBUG
        ASSERT(m_data == r.m_data && m_size == r.m_size, "Iterators incompatible");
        return m_offset == r.m_offset;
#else
        return m_data == r.m_data;
#endif
    }

    DECL_NODISCARD constexpr bool LessThan(const RandomIterator& r) const noexcept
    {
#ifndef NDEBUG
        ASSERT(m_data == r.m_data && m_size == r.m_size, "Iterators incompatible");
        return m_offset < r.m_offset;
#else
        return m_data < r.m_data;
#endif
    };

    DECL_NODISCARD constexpr difference_type Distance(const RandomIterator& r) const noexcept
    {
#ifndef NDEBUG
        ASSERT(m_data == r.m_data && m_size == r.m_size, "Iterators incompatible");
        return m_offset - r.m_offset;
#else
        return m_data - r.m_data;
#endif
    }

private:
    pointer m_data;
#ifndef NDEBUG
    size_t m_size = 0;
    size_t m_offset;
#endif
};

template <class T, bool IsConst>
DECL_NODISCARD DECL_CONSTEXPR11 RandomIterator<T, IsConst> operator+(const typename RandomIterator<T, IsConst>::difference_type offset, RandomIterator<T, IsConst> iter) DECL_NOEXCEPT
{
    iter += offset;
    return iter;
}

} // namespace screw
#endif // !_ITERATOR_BASE_H_
