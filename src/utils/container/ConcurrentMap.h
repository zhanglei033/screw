#pragma once
#ifndef _CONCURRENT_MAP_H_
#define _MAP_H_
#include "data structure/RBTree.h"
#include <tuple>
namespace screw {
namespace container {

template <class KeyT, class ValueT, class Comparator, class Allocator>
using RBTreeNodeWithValueTraits = data_structure::RBTreeNodeWithValueTraits<KeyT, ValueT, Comparator, Allocator>;
template <class Traits>
using RBTree = data_structure::RBTree<Traits>;
template <class Iter, class Node>
using InsertHandleResult = data_structure::InsertHandleResult<Iter, Node>;

template <class KeyT, class ValueT, class Comparator = std::less<KeyT>, class Allocator = std::allocator<std::pair<const KeyT, ValueT>>>
class ConcurrentMap : public RBTree<RBTreeNodeWithValueTraits<KeyT, ValueT, Comparator, Allocator>>
{
public:
    static_assert(std::is_same_v<std::pair<const KeyT, ValueT>, typename Allocator::value_type>,
                  " ConcurrentMap<Key, Value, Compare, Allocator> requires that Allocator's value_type match  pair<const Key, Value>"
                  " (See N4659 26.2.1 [container.requirements.general]/16 allocator_type)"
                  " Either fix the allocator value_type or define _ENFORCE_MATCHING_ALLOCATORS=0"
                  " to suppress this diagnostic.");

    using mybase = RBTree<RBTreeNodeWithValueTraits<KeyT, ValueT, Comparator, Allocator>>;

    using key_type               = KeyT;
    using mapped_type            = ValueT;
    using key_compare            = Comparator;
    using value_compare          = typename mybase::value_compare;
    using value_type             = std::pair<const KeyT, ValueT>;
    using allocator_type         = typename mybase::allocator_type;
    using size_type              = typename mybase::size_type;
    using difference_type        = typename mybase::difference_type;
    using pointer                = typename mybase::pointer;
    using const_pointer          = typename mybase::const_pointer;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = typename mybase::iterator;
    using const_iterator         = typename mybase::const_iterator;
    using reverse_iterator       = typename mybase::reverse_iterator;
    using const_reverse_iterator = typename mybase::const_reverse_iterator;

    using node_type             = typename mybase::node_type;
    using node_allocator_type   = typename mybase::node_allocator_type;
    using node_allocator_traits = typename mybase::node_allocator_traits;
    using node_pointer          = typename mybase::node_pointer;
    using node_handle           = typename mybase::node_handle;
    using insert_return_type    = InsertHandleResult<iterator, node_type>;

    using value_comparator_traits = typename mybase::value_comparator_traits;

    using mybase::mybase;

    using mybase::begin;
    using mybase::cbegin;
    using mybase::cend;
    using mybase::clear;
    using mybase::contains;
    using mybase::count;
    using mybase::crbegin;
    using mybase::crend;
    using mybase::emplace;
    using mybase::emplace_hint;
    using mybase::empty;
    using mybase::end;
    using mybase::equal_range;
    using mybase::erase;
    using mybase::extract;
    using mybase::find;
    using mybase::get_allocator;
    using mybase::get_node_allocator;
    using mybase::insert;
    using mybase::key_comp;
    using mybase::length;
    using mybase::lower_bound;
    using mybase::m_ctrl;
    using mybase::max_size;
    using mybase::merge;
    using mybase::rbegin;
    using mybase::rend;
    using mybase::size;
    using mybase::swap;
    using mybase::upper_bound;
    using mybase::value_comp;

    ConcurrentMap()
        : mybase()
    {
    }

    template <class ElemConstructTag = element_copy_construct_tag>
    ConcurrentMap(const ConcurrentMap& other, ElemConstructTag tag = ElemConstructTag{})
        : mybase(other, tag)
    {
    }

    ConcurrentMap(ConcurrentMap&& other)
        : mybase(std::move(other))
    {
    }

    template <class Iter, class ElemConstructTag = element_copy_construct_tag>
    ConcurrentMap(Iter first, Iter last, ElemConstructTag tag = ElemConstructTag{})
        : mybase()
    {
        insert(first, last, tag);
    }

    template <class ElemConstructTag = element_copy_construct_tag>
    ConcurrentMap(std::initializer_list<value_type> list, ElemConstructTag tag = ElemConstructTag{})
        : mybase()
    {
        insert(list, tag);
    }

    ConcurrentMap& operator=(const ConcurrentMap& other)
    {
        mybase::operator=(other);
        return *this;
    }

    ConcurrentMap& operator=(ConcurrentMap&& other)
    {
        mybase::operator=(std::move(other));
        return *this;
    }

    ConcurrentMap& operator=(std::initializer_list<value_type> list)
    {
        mybase::clear();
        insert(list);
        return *this;
    }

    DECL_NODISCARD mapped_type operator[](key_type&& key)
    {
        return try_emplace_impl(std::move(key)).first->Value().second;
    }

    DECL_NODISCARD mapped_type& operator[](const key_type& key)
    {
        return try_emplace_impl(key).first->Value().second;
    }

    DECL_NODISCARD bool operator==(ConcurrentMap& other)
    {
        return m_ctrl.m_cont.GetSize() == other.m_ctrl.m_cont.GetSize() && std::equal(begin(), end(), other.begin());
    }

    DECL_NODISCARD bool operator!=(ConcurrentMap& other)
    {
        return m_ctrl.m_cont.GetSize() != other.m_ctrl.m_cont.GetSize() || !std::equal(begin(), end(), other.begin());
    }

    DECL_NODISCARD bool operator<(ConcurrentMap& other)
    {
        return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
    }
    DECL_NODISCARD bool operator<=(ConcurrentMap& other)
    {
        return !(other < *this);
    }
    DECL_NODISCARD bool operator>(ConcurrentMap& other)
    {
        return other < *this;
    }
    DECL_NODISCARD bool operator>=(ConcurrentMap& other)
    {
        return !(*this < other);
    }

    DECL_NODISCARD mapped_type& at(const key_type& key)
    {
        auto result        = m_ctrl.m_cont.FindLower(key);
        node_pointer bound = result.m_bound;
        ThrowException(!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()), "invalid key");
        return bound->Value().second;
    }

    DECL_NODISCARD const mapped_type& at(const key_type& key) const
    {
        auto result        = m_ctrl.m_cont.FindLower(key);
        node_pointer bound = result.m_bound;
        ThrowException(!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()), "invalid key");
        return bound->Value().second;
    }

    void swap(ConcurrentMap& other) noexcept(noexcept(mybase::swap(other)))
    {
        mybase::swap(other);
    }

    template <class ValueT, class = std::enable_if_t<std::is_constructible_v<value_type, ValueT>>>
    std::pair<iterator, bool> insert(ValueT&& value)
    {
        return emplace(std::forward<ValueT>(value));
    }

    template <class ValueT,
              class Iter,
              class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>,
              class = std::enable_if_t<std::is_constructible_v<value_type, ValueT>>>
    iterator insert(Iter pos, ValueT&& value)
    {
        return emplace_hint(pos, std::forward<ValueT>(value));
    }

    template <class... MappedT>
    std::pair<iterator, bool> try_emplace(const key_type& key, MappedT&&... mappeds)
    {
        auto res = try_emplace_impl(key, std::forward<MappedT>(mappeds)...);
        return {iterator(res.first, &m_ctrl.m_cont), res.second};
    }
    template <class... MappedT>
    std::pair<iterator, bool> try_emplace(key_type&& key, MappedT&&... mappeds)
    {
        auto res = try_emplace_impl(std::forward<key_type>(key), std::forward<MappedT>(mappeds)...);
        return {iterator(res.first, &m_ctrl.m_cont), res.second};
    }

    template <class Iter, class... MappedT, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator try_emplace(Iter pos, const key_type& key, MappedT&&... mappeds)
    {
        auto result = m_ctrl.m_cont.FindHint(GetNodePtr(pos), key);
        if (result.m_isDuplicate)
        {
            return iterator(result.m_parent, &m_ctrl.m_cont);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        node_pointer newNode = m_ctrl.m_cont.GenerateNode(
            get_node_allocator(),
            m_ctrl.m_cont.GetHead(),
            std::piecewise_construct,
            std::forward_as_tuple(std::forward<key_type>(key)),
            std::forward_as_tuple(std::forward<MappedT>(mappeds)...));
        return iterator(m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, newNode), &m_ctrl.m_cont);
    }
    template <class Iter, class... MappedT, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator try_emplace(Iter pos, key_type&& key, MappedT&&... mappeds)
    {
        auto result = m_ctrl.m_cont.FindHint(GetNodePtr(pos), key);
        if (result.m_isDuplicate)
        {
            return iterator(result.m_parent, &m_ctrl.m_cont);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");

        node_pointer newNode = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<key_type>(key), std::forward<MappedT>(mappeds)...);
        return iterator(m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, newNode), &m_ctrl.m_cont);
    }

    template <class MappedT>
    std::pair<iterator, bool> insert_or_assign(const key_type& key, MappedT&& mapped)
    {
        auto result        = m_ctrl.m_cont.FindLower(key);
        node_pointer bound = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()))
        {
            bound->Value().second = std::forward<MappedT>(mapped);

            return std::make_pair(iterator(bound, &m_ctrl.m_cont), false);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        node_pointer newNode = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<key_type>(key), std::forward<MappedT>(mapped));
        return std::make_pair(iterator(m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, newNode), &m_ctrl.m_cont), true);
    }

    template <class MappedT>
    std::pair<iterator, bool> insert_or_assign(key_type&& key, MappedT&& mapped)
    {
        auto result        = m_ctrl.m_cont.FindLower(key);
        node_pointer bound = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()))
        {
            bound->Value().second = std::forward<MappedT>(mapped);

            return std::make_pair(iterator(bound, &m_ctrl.m_cont), false);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");

        node_pointer newNode = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<key_type>(key), std::forward<MappedT>(mapped));
        return std::make_pair(iterator(m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, newNode), &m_ctrl.m_cont), true);
    }

    template <class Iter, class MappedT, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator insert_or_assign(Iter pos, const key_type& key, MappedT&& mapped)
    {
        auto result = m_ctrl.m_cont.FindHint(GetNodePtr(pos), key);
        if (result.m_isDuplicate)
        {
            result.m_parent->Value().second = std::forward<MappedT>(mapped);
            return iterator(result.m_parent, &m_ctrl.m_cont);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        node_pointer newNode = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<key_type>(key), std::forward<MappedT>(mapped));
        return iterator(m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, newNode), &m_ctrl.m_cont);
    }

    template <class Iter, class MappedT, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator insert_or_assign(Iter pos, key_type&& key, MappedT&& mapped)
    {
        auto result = m_ctrl.m_cont.FindHint(GetNodePtr(pos), key);
        if (result.m_isDuplicate)
        {
            result.m_parent->Value().second = std::forward<MappedT>(mapped);
            return iterator(result.m_parent, &m_ctrl.m_cont);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        node_pointer newNode = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<key_type>(key), std::forward<MappedT>(mapped));
        return iterator(m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, newNode), &m_ctrl.m_cont);
    }

private:
    template <class... MappedT>
    std::pair<node_pointer, bool> try_emplace_impl(key_type&& key, MappedT&&... mappeds)
    {
        auto result        = m_ctrl.m_cont.FindLower(key);
        node_pointer bound = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()))
        {
            return std::make_pair(bound, false);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        node_pointer newNode = m_ctrl.m_cont.GenerateNode(
            get_node_allocator(),
            m_ctrl.m_cont.GetHead(),
            std::piecewise_construct,
            std::forward_as_tuple(std::forward<key_type>(key)),
            std::forward_as_tuple(std::forward<MappedT>(mappeds)...));
        return std::make_pair(m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, newNode), true);
    }
};
template <class KeyT, class ValueT, class Comparator, class Allocator>
void swap(ConcurrentMap<KeyT, ValueT, Comparator, Allocator>& l, ConcurrentMap<KeyT, ValueT, Comparator, Allocator>& r) noexcept(noexcept(l.swap(r)))
{
    l.swap(r);
}
}
} // namespace screw::container
#endif // !_MAP_H_
