#pragma once
#include "base/IteratorBase.h"
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
#include <memory>
namespace screw {
namespace data_structure {
template <class T>
struct RBTreeNodeTraits
{
    using value_type = T;

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& Parent(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->m_parent;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& Left(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->m_left;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& Right(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->m_right;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& Sibling(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->IsLeft() ? node->m_right : node->m_left;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& Uncle(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return Sibling(node->m_parent);
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& Grandparent(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->m_parent->m_parent;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool IsLeft(NodePtr&& node) DECL_NOEXCEPT(DECL_NOEXCEPT(node->IsLeft()))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->IsLeft();
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool IsRight(NodePtr&& node) DECL_NOEXCEPT(DECL_NOEXCEPT(node->IsRight()))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->IsRight();
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& SetLeft(NodePtr&& node, NodePtr&& left) DECL_NOEXCEPT(DECL_NOEXCEPT(node->SetLeft(left)))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->SetLeft(left);
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& SetRight(NodePtr&& node, NodePtr&& right) DECL_NOEXCEPT(DECL_NOEXCEPT(node->SetRight(right)))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->SetRight(right);
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR NodePtr&& Max(NodePtr&& node) DECL_NOEXCEPT(DECL_NOEXCEPT(node->Max()))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->Max();
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR NodePtr&& Min(NodePtr&& node) DECL_NOEXCEPT(DECL_NOEXCEPT(node->Min()))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->Min();
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR NodePtr&& Next(NodePtr&& node) DECL_NOEXCEPT(DECL_NOEXCEPT(node->Next()))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->Next();
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR NodePtr&& Prev(NodePtr&& node) DECL_NOEXCEPT(DECL_NOEXCEPT(node->Prev()))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->Prev();
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR void Lrotate(NodePtr&& node, NodePtr&& head) DECL_NOEXCEPT(DECL_NOEXCEPT(node->Lrotate(head)))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        node->Lrotate(head);
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR void Rrotate(NodePtr&& node, NodePtr&& head) DECL_NOEXCEPT(DECL_NOEXCEPT(node->Rrotate(head)))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        node->Rrotate(head);
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR value_type&& Value(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG

        return node->m_value;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool IsNull(NodePtr&& node) DECL_NOEXCEPT(DECL_NOEXCEPT(node->IsNull()))
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->IsNull();
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool IsRed(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return node->m_isRed;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool IsBlack(NodePtr&& node) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        return !node->m_isRed;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool SetColor(NodePtr&& node, bool isRed) DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(node != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        node->m_isRed = isRed;
    }

    template <class NodePtr>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR void SwapColor(NodePtr&& node1, NodePtr&& node2) DECL_NOEXCEPT(DECL_NOEXCEPT(std::swap(node1->m_isRed, node2->m_isRed)))
    {
#ifndef NDEBUG
        ASSERT(node1 != nullptr && node2 != nullptr, "node pointer must be not nullptr");
#endif // !NDEBUG
        std::swap(node1->m_isRed, node2->m_isRed);
    }
};

template <class T, class Comparator, class Allocator>
struct RBTreeNodeOnlyKeyTraits : RBTreeNodeTraits<T>
{
    using key_type       = T;
    using value_type     = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using key_comparator = Comparator;
    struct comparator_type
    {
        DECL_STATIC_CONSTEXPR key_comparator comparator;
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const key_type& l, const key_type& r) DECL_NOEXCEPT { return comparator(l, r); }
    };

    template <class Key, class... Args>
    DECL_STATIC_CONSTEXPR bool is_extractable_v = false;
    template <>
    DECL_STATIC_CONSTEXPR bool is_extractable_v<key_type> = true;

    struct key_extractor
    {
        using key_type = T;
        DECL_NODISCARD DECL_STATIC_CONSTEXPR key_type& Extract(key_type& key) DECL_NOEXCEPT
        {
            return key;
        }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR const key_type& Extract(const key_type& key) DECL_NOEXCEPT
        {
            return key;
        }
    };
};

template <class KT, class T, class Comparator, class Allocator>
struct RBTreeNodeWithValueTraits : RBTreeNodeTraits<std::pair<const KT, T>>
{
    using key_type       = KT;
    using value_type     = std::pair<const KT, T>;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using key_comparator = Comparator;
    struct comparator_type
    {
        DECL_STATIC_CONSTEXPR key_comparator comparator;
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const key_type& l, const key_type& r) DECL_NOEXCEPT { return comparator(l, r); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const key_type& l, const value_type& r) DECL_NOEXCEPT { return comparator(l, r.first); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const value_type& l, const key_type& r) DECL_NOEXCEPT { return comparator(l.first, r); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const value_type& l, const value_type& r) DECL_NOEXCEPT { return comparator(l.first, r.first); }
    };
    template <class Key, class... Args>
    DECL_STATIC_CONSTEXPR bool is_extractable_v = false;
    template <>
    DECL_STATIC_CONSTEXPR bool is_extractable_v<key_type, value_type> = true;

    struct key_extractor
    {
        using key_type = T;
        DECL_NODISCARD DECL_STATIC_CONSTEXPR key_type& Extract(key_type& key, T&) DECL_NOEXCEPT
        {
            return key;
        }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR const key_type& Extract(const key_type& key, T&) DECL_NOEXCEPT
        {
            return key;
        }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR key_type& Extract(key_type& key, const T&) DECL_NOEXCEPT
        {
            return key;
        }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR const key_type& Extract(const key_type& key, const T&) DECL_NOEXCEPT
        {
            return key;
        }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR const key_type& Extract(const value_type& value) DECL_NOEXCEPT
        {
            return value.first;
        }
    };
};

template <class ComparatorType>
struct ComparatorTraits
{
    using comparator_type = ComparatorType;

    template <template <class...> class Comparator, class KT>
    DECL_STATIC_CONSTEXPR bool is_less = std::is_same_v<std::less<KT>, Comparator<KT>>;

    template <class LT,
              class RT,
              class = std::enable_if_t<std::is_same_v<bool, decltype(comparator_type::Compare(std::declval<LT>(), std::declval<RT>()))> ||     //
                                       std::is_same_v<bool, decltype(comparator_type::compare(std::declval<LT>(), std::declval<RT>()))> ||     //
                                       std::is_same_v<bool, decltype(std::declval<comparator_type>()(std::declval<LT>(), std::declval<RT>()))> //
                                       >>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(LT&& l, RT&& r) DECL_NOEXCEPT
    {
        if constexpr (std::is_same_v<bool, decltype(comparator_type::Compare(std::forward<LT>(l), std::forward<RT>(r)))>)
        {
            return comparator_type::Compare(std::forward<LT>(l), std::forward<RT>(r));
        }
        else if constexpr (std::is_same_v<bool, decltype(comparator_type::compare(std::forward<LT>(l), std::forward<RT>(r)))>)
        {
            return comparator_type::compare(std::forward<LT>(l), std::forward<RT>(r));
        }
        else
        {
            return comparator_type()(std::forward<LT>(l), std::forward<RT>(r));
        }
    }
};

template <class KeyExtractorType>
struct KeyExtractorTraits
{
    using key_extractor_type = KeyExtractorType;
    using key_type           = typename key_extractor_type::key_type;
    template <class... Args,
              class = std::enable_if_t<
                  std::is_same_v<key_type, std::remove_cvref_t<decltype(key_extractor_type::Extract(std::declval<Args>()...))>> ||     //
                  std::is_same_v<key_type, std::remove_cvref_t<decltype(key_extractor_type::extract(std::declval<Args>()...))>> ||     //
                  std::is_same_v<key_type, std::remove_cvref_t<decltype(std::declval<key_extractor_type>()(std::declval<Args>()...))>> //
                  >>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR key_type&& Extract(Args&&... args) DECL_NOEXCEPT
    {
        if constexpr (std::is_same_v<key_type, std::remove_cvref_t<decltype(key_extractor_type::Extract(std::forward<Args>(args)...))>>)
        {
            return key_extractor_type::Extract(std::forward<Args>(args)...);
        }
        else if constexpr (std::is_same_v<key_type, std::remove_cvref_t<decltype(key_extractor_type::extract(std::forward<Args>(args)...))>>)
        {
            return key_extractor_type::extract(std::forward<Args>(args)...);
        }
        else
        {
            return key_extractor_type()(std::forward<Args>(args)...);
        }
    }
};

template <class Traits>
struct RBTreeNode
{
    using node_traits           = Traits;
    using key_type              = typename Traits::key_type;
    using value_type            = typename Traits::value_type;
    using comparator_type       = typename Traits::comparator_type;
    using allocator_type        = typename Traits::allocator_type;
    using node_allocator_type   = typename std::allocator_traits<allocator_type>::template rebind_alloc<RBTreeNode<Traits>>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using pointer               = typename node_allocator_traits::pointer;

    pointer m_left   = nullptr; // 左节点
    pointer m_right  = nullptr; // 右节点
    pointer m_parent = nullptr; // 父节点
    bool m_isRed     = false;   // 是红色节点
    bool m_isNull    = false;   // 是空节点
    value_type m_value;         // 节点所存储的值

    RBTreeNode()                  = default;
    RBTreeNode(const RBTreeNode&) = delete;
    RBTreeNode& operator=(const RBTreeNode&) = delete;

    template <class T, class = std::enable_if_t<std::is_same_v<T, value_type> || std::is_same_v<T, key_type>>>
    DECL_NODISCARD bool operator<(const T& value) const DECL_NOEXCEPT
    {
        return comparator_type::Compare(m_value, value);
    }
    DECL_NODISCARD bool operator<(const RBTreeNode& node) const DECL_NOEXCEPT
    {
        return comparator_type::Compare(m_value, node.m_value);
    }
    DECL_NODISCARD bool operator<(const pointer& nodePtr) const DECL_NOEXCEPT
    {
        return comparator_type::Compare(m_value, nodePtr->m_value);
    }

    // 生成Head节点
    template <class Allocator, class = std::enable_if_t<typename Allocator::value_type, RBTreeNode>>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR pointer GenerateHeadNode(Allocator& alloc) DECL_NOEXCEPT
    {
        static_assert(std::is_same_v<typename Allocator::value_type, RBTreeNode>, "generating nodes using incorrect allocator");

        pointer node = alloc.allocate(1);
        std::construct_at(node->m_left, node);
        std::construct_at(node->m_right, node);
        std::construct_at(node->m_parent, node);
        node->m_isRed  = false;
        node->m_isNull = true;
        return node;
    }

    // 生成普通节点
    template <class Allocator, class... Args, class = std::enable_if_t<typename Allocator::value_type, RBTreeNode>>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR pointer GenerateNode(Allocator& alloc, pointer head, Args&&... args) DECL_NOEXCEPT
    {
        static_assert(std::is_same_v<typename Allocator::value_type, RBTreeNode>, "generating nodes using incorrect allocator");

        pointer node = alloc.allocate(1);
        std::allocator_traits<Allocator>::construct(alloc, std::addressof(node->m_value), std::forward<Args>(args)...);
        std::construct_at(node->m_left, head);
        std::construct_at(node->m_right, head);
        std::construct_at(node->m_parent, head);
        node->m_isRed  = true;
        node->m_isNull = false;
        return node;
    }

    // 释放节点所占资源
    template <class Allocator, class = std::enable_if_t<typename Allocator::value_type, RBTreeNode>>
    DECL_STATIC_CONSTEXPR void FreeNode(Allocator& alloc, pointer node) DECL_NOEXCEPT
    {
        static_assert(std::is_same_v<typename Allocator::value_type, RBTreeNode>, "Free nodes using incorrect allocator");
        std::destroy_at(node->m_left);
        std::destroy_at(node->m_right);
        std::destroy_at(node->m_parent);
        std::allocator_traits<Allocator>::deallocate(alloc, node, 1);
    }

    // 销毁这个节点
    template <class Allocator, class = std::enable_if_t<typename Allocator::value_type, RBTreeNode>>
    DECL_STATIC_CONSTEXPR void DestroyNode(Allocator& alloc, pointer node) DECL_NOEXCEPT
    {
        static_assert(std::is_same_v<typename Allocator::value_type, RBTreeNode>, "Free nodes using incorrect allocator");
        std::allocator_traits<Allocator>::destroy(alloc, std::addressof(node->m_value));
        FreeNode(alloc, node);
    }

    // 获取节点存放的值
    DECL_NODISCARD value_type& GetValue() DECL_NOEXCEPT { return m_value; }
    DECL_NODISCARD const value_type& GetValue() const DECL_NOEXCEPT { return m_value; }

    // 获取兄弟节点
    DECL_NODISCARD pointer Sibling() DECL_NOEXCEPT { return IsLeft() ? m_right : m_left; }
    DECL_NODISCARD pointer Sibling() const DECL_NOEXCEPT { return IsLeft() ? m_right : m_left; }

    // 获取叔叔父节点
    DECL_NODISCARD pointer Uncle() DECL_NOEXCEPT { return m_parent->Sibling(); }
    DECL_NODISCARD pointer Uncle() const DECL_NOEXCEPT { return m_parent->Sibling(); }

    // 获取祖父节点
    DECL_NODISCARD pointer Grandparent() DECL_NOEXCEPT { return m_parent->m_isNull ? nullptr : m_parent->m_parent; }
    DECL_NODISCARD pointer Grandparent() const DECL_NOEXCEPT { return m_parent->m_isNull ? nullptr : m_parent->m_parent; }

    // 判断当前节点是否是root节点
    // root的父节点是head节点，head节点的父节点是root节点
    // DECL_NODISCARD bool IsRoot() const DECL_NOEXCEPT { return m_parent->m_parent == *this; }
    // root的父节点是head节点，head节点是空节点
    DECL_NODISCARD bool IsRoot() const DECL_NOEXCEPT { return m_parent->m_isNull; }

    // 判断当前节点是否是左节点
    DECL_NODISCARD bool IsLeft() const DECL_NOEXCEPT { return m_parent->m_left == *this; }

    // 判断当前节点是否是右节点
    DECL_NODISCARD bool IsRight() const DECL_NOEXCEPT { return m_parent->m_right == *this; }

    // 判断当前节点是否是空节点
    DECL_NODISCARD bool IsNull() const DECL_NOEXCEPT { return m_isNull; }

    // 设置左节点
    void SetLeft(pointer left) const DECL_NOEXCEPT
    {
        m_left         = left;
        left->m_parent = *this;
    }

    // 设置右节点
    void SetRight(pointer right) const DECL_NOEXCEPT
    {
        m_right         = right;
        right->m_parent = *this;
    }

    // 获取当前节点的最大节点
    DECL_NODISCARD pointer Max() const DECL_NOEXCEPT
    {
        pointer max = this;
        while (!max->m_right->m_isNull)
        {
            max = max->m_right;
        }
        return max;
    }

    // 获取当前节点的最小节点
    DECL_NODISCARD pointer Min() const DECL_NOEXCEPT
    {
        pointer min = this;
        while (!min->m_left->m_isNull)
        {
            min = min->m_left;
        }
        return min;
    }

    // 获取下一个节点
    DECL_NODISCARD pointer Next() const DECL_NOEXCEPT
    {
        // 右子树非空时，返回右子树的最小值
        if (!m_right->m_isNull)
            return m_right->Min();

        // 右子树为空，向上查找第一个左节点，返回这个节点的父节点
        const pointer cur = this;
        while (!cur->m_parent->m_isNull)
        {
            if (cur->IsLeft())
                return cur->m_parent;

            cur = cur->m_parent;
        }

        return nullptr;
    }

    // 获取上一个节点
    DECL_NODISCARD pointer Prev() const DECL_NOEXCEPT
    {
        // 左子树非空时，返回右子树的最小值
        if (!m_left->m_isNull)
            return m_left->Max();

        // 左子树为空，向上查找第一个右节点，返回这个节点的父节点
        const pointer cur = this;
        while (!cur->m_parent->m_isNull)
        {
            if (cur->IsRight())
                return cur->m_parent;

            cur = cur->m_parent;
        }

        return nullptr;
    }

    void Lrotate(pointer head) DECL_NOEXCEPT
    {
        pointer right = m_right;
        SetRight(right->m_left);
        if (IsRoot())
        {
            head->m_parent = right;
            right->SetParent(m_parent);
        }
        else
        {
            IsLeft() ? m_parent->SetLeft(right) :
                       m_parent->SetRight(right);
        }
        right->SetLeft(this);
    }

    void Rrotate(pointer head) DECL_NOEXCEPT
    {
        pointer left = m_left;
        SetRight(left->m_right);
        if (IsRoot())
        {
            head->m_parent = left;
            left->SetParent(m_parent);
        }
        else
        {
            IsLeft() ? m_parent->SetLeft(left) :
                       m_parent->SetRight(left);
        }
        left->SetRight(this);
    }
};

template <class Derived, class Container, bool IsConst>
class BPTreeIteratorBase : public IteratorBase<
                               std::conditional_t<IsConst,
                                                  ConstIteratorTraits<Derived, typename Container::value_type, std::bidirectional_iterator_tag>,
                                                  IteratorTraits<Derived, typename Container::value_type, std::bidirectional_iterator_tag>>>
{
};
// 迭代器
template <class Container, bool IsConst>
class BPTreeIterator : public BPTreeIteratorBase<BPTreeIterator<Container, IsConst>, Container, IsConst>
{
public:
    using base_type = BPTreeIteratorBase<BPTreeIterator<Container, IsConst>, Container, IsConst>;

    using value_type        = typename base_type::value_type;
    using size_type         = typename base_type::size_type;
    using reference         = typename base_type::reference;
    using const_reference   = typename base_type::const_reference;
    using pointer           = typename base_type::pointer;
    using const_pointer     = typename base_type::const_pointer;
    using difference_type   = typename base_type::difference_type;
    using iterator_category = typename base_type::iterator_category;

    using node_type         = typename Container::node_type;
    using comparator_type   = typename Container::comparator_type;
    using node_pointer      = typename Container::node_pointer;
    using node_traits       = typename Container::node_traits;
    using comparator_traits = ComparatorTraits<comparator_type>;

#ifndef NDEBUG
    constexpr BPTreeIterator() DECL_NOEXCEPT
        : m_iter(nullptr),
          m_cont(nullptr)
    {
    }
    constexpr explicit BPTreeIterator(node_pointer iter, Container* cont) DECL_NOEXCEPT
        : m_iter(iter),
          m_cont(cont)
    {
    }
    constexpr BPTreeIterator(const BPTreeIterator<Container, true>& iter) DECL_NOEXCEPT
        : m_iter(iter.m_iter),
          m_cont(iter.m_cont) {}

    constexpr BPTreeIterator& operator=(const const BPTreeIterator<Container, true>& iter) DECL_NOEXCEPT
    {
        m_iter = iter.m_iter;
        m_cont = iter.m_cont;
        return *this;
    }
#else
    constexpr BPTreeIterator() DECL_NOEXCEPT
        : m_iter(nullptr)
    {
    }
    constexpr explicit BPTreeIterator(node_pointer iter, Container* cont) DECL_NOEXCEPT
        : m_iter(iter)
    {
    }
    constexpr BPTreeIterator(const BPTreeIterator<Container, true>& iter) DECL_NOEXCEPT
        : m_iter(iter.m_iter) {}

    constexpr BPTreeIterator& operator=(const const BPTreeIterator<Container, true>& iter) DECL_NOEXCEPT
    {
        m_iter = iter.m_iter;
        return *this;
    }
#endif

    DECL_NODISCARD constexpr reference operator[](const difference_type offset) const DECL_NOEXCEPT
    {
        return *(*this + offset);
    }
    DECL_NODISCARD constexpr pointer Unwrapped() DECL_NOEXCEPT
    {
        return std::addressof(Dereference());
    }
    DECL_NODISCARD constexpr reference Dereference() const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
        ASSERT(node_traits::IsNull(m_iter), "Iterator is last");
#endif
        return node_traits::Value(m_iter);
    }
    constexpr void Increment(const difference_type n) DECL_NOEXCEPT
    {
        switch (n)
        {
        case 0: return;
        case 1: {
#ifndef NDEBUG
            ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
#endif
            node_pointer next = node_traits::Next(m_iter);
#ifndef NDEBUG
            ASSERT(next != nullptr, "Iterator out of bounds");
#endif
            m_iter = next;
            return;
        }
        case -1: {
#ifndef NDEBUG
            ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
#endif
            node_pointer prev = node_traits::Prev(m_iter);
#ifndef NDEBUG
            ASSERT(prev != nullptr, "Iterator out of bounds");
#endif
            m_iter = prev;
            return;
        }
        default: {
            if (n > 0)
            {
                Increment(1);
                Increment(n - 1);
            }
            else
            {
                Increment(-1);
                Increment(n + 1);
            }
        }
        }
    }
    constexpr void Decrement(const difference_type n) DECL_NOEXCEPT
    {
        switch (n)
        {
        case 0: return;
        case 1: {
#ifndef NDEBUG
            ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
#endif
            node_pointer prev = node_traits::Prev(m_iter);
#ifndef NDEBUG
            ASSERT(prev != nullptr, "Iterator out of bounds");
#endif
            m_iter = prev;
            return;
        }
        case -1: {
#ifndef NDEBUG
            ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
#endif
            node_pointer next = node_traits::Next(m_iter);
#ifndef NDEBUG
            ASSERT(next != nullptr, "Iterator out of bounds");
#endif
            m_iter = next;
            return;
        }
        default: {
            if (n > 0)
            {
                Decrement(1);
                Decrement(n - 1);
            }
            else
            {
                Decrement(-1);
                Decrement(n + 1);
            }
        }
        }
    }
    DECL_NODISCARD constexpr bool Equals(const BPTreeIterator& r) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
        ASSERT(m_cont == r.m_cont, "Iterators incompatible");
#endif
        return m_iter == r.m_iter;
    }
    DECL_NODISCARD constexpr bool LessThan(const BPTreeIterator& r) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
        ASSERT(m_cont == r.m_cont, "Iterators incompatible");
#endif
        return !(comparator_traits::is_less ^ comparator_traits::Compare(node_traits::Value(m_iter), node_traits::Value(r.m_iter)));
    };

    DECL_NODISCARD constexpr difference_type Distance(const BPTreeIterator& r) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
        ASSERT(m_cont == r.m_cont, "Iterators incompatible");
        ASSERT(LessThan(r) || Equals(r), "transposed pointer range");
#endif
        auto begin             = m_iter;
        auto end               = r.m_iter;
        difference_type offset = 0;
        for (; begin != end; begin.Increment(1))
        {
            ++offset;
        }
        return offset;
    }

    node_pointer m_iter;
#ifndef NDEBUG
    Container* m_cont;
#endif
};

template <class Types>
struct RBTreeNodeContainer
{
    using key_type        = typename Types::key_type;
    using value_type      = typename Types::value_type;
    using size_type       = typename Types::size_type;
    using difference_type = typename Types::difference_type;
    using pointer         = typename Types::pointer;
    using const_pointer   = typename Types::const_pointer;
    using reference       = typename Types::reference;
    using const_reference = typename Types::const_reference;
    using node_type       = typename Types::node_type;
    using node_pointer    = typename Types::node_pointer;
    using comparator_type = typename Types::comparator_type;
    using node_traits     = typename node_type::node_traits;

    using iterator       = BPTreeIterator<RBTreeNodeContainer, false>;
    using const_iterator = BPTreeIterator<RBTreeNodeContainer, true>;

    // 这棵树Root节点
    DECL_NODISCARD constexpr node_pointer Root() DECL_NOEXCEPT { return node_traits::Parent(m_head); }
    constexpr void SetRoot(node_pointer node) DECL_NOEXCEPT { node_traits::Parent(m_head) = node; }
    DECL_NODISCARD constexpr bool IsRoot(node_pointer node) DECL_NOEXCEPT { return node == node_traits::Parent(m_head); }

    // 这棵树最大的节点
    DECL_NODISCARD constexpr node_pointer Max() DECL_NOEXCEPT { return node_traits::Right(m_head); }
    constexpr void SetMax(node_pointer node) DECL_NOEXCEPT { node_traits::Right(m_head) = node; }
    DECL_NODISCARD constexpr bool IsMax(node_pointer node) DECL_NOEXCEPT { return node == node_traits::Right(m_head); }

    // 这棵树最小的节点
    DECL_NODISCARD constexpr node_pointer Min() DECL_NOEXCEPT { return node_traits::Left(m_head); }
    constexpr void SetMin(node_pointer node) DECL_NOEXCEPT { node_traits::Left(m_head) = node; }
    DECL_NODISCARD constexpr bool IsMin(node_pointer node) DECL_NOEXCEPT { return node == node_traits::Left(m_head); }

    // 删除一个节点
    constexpr node_pointer EraseNode(node_pointer erasedNode) DECL_NOEXCEPT
    {
        node_pointer fixNode       = nullptr;
        node_pointer fixNodeParent = nullptr;
        node_pointer curNode       = nullptr;

        FixEraseNode(erasedNode, fixNode, fixNodeParent, curNode);
        m_size--;
        return erasedNode;
    }

    constexpr void FixEraseNode(node_pointer& erasedNode, node_pointer& fixNode, node_pointer& fixNodeParent, node_pointer& curNode) DECL_NOEXCEPT
    {
        if (!node_traits::IsNull(node_traits::Left(erasedNode)) && !node_traits::IsNull(node_traits::Right(erasedNode))) // 有两个子树
        {
            FixEraseNode1(erasedNode, fixNode, fixNodeParent, curNode);
        }
        else // 最多一个子树
        {
            FixEraseNode2(erasedNode, fixNode, fixNodeParent, curNode);
        }
    }

    constexpr void FixEraseNode1(node_pointer& erasedNode, node_pointer& fixNode, node_pointer& fixNodeParent, node_pointer& curNode) DECL_NOEXCEPT
    {
        curNode = node_traits::Next(erasedNode); // 删除节点有两个子节点时，将删除节点的下一个节点，即它右子树的最小节点作为当前操作的节点
        fixNode = node_traits::Right(curNode);   // 需要修复的节点为操作节点的右节点

        node_traits::SetLeft(curNode, node_traits::Left(erasedNode)); // 将删除节点的左子树设置为curNode的左子树

        if (curNode == node_traits::Right(erasedNode)) // 当curNode为删除节点的右节点，此时删除节点的右节点没有左节点，修复节点的父节点仍为curNode
        {
            fixNodeParent = curNode;
        }
        else // 当curNode不为删除节点的右节点，此时删除节点的右节点存在左节点，
        {
            fixNodeParent = node_traits::Parent(curNode);                   // 修复节点的父节点为curNode的父节点
            node_traits::SetLeft(fixNodeParent, fixNode);                   //  将fixNode设置为fixNodeParent的左节点
            node_traits::SetRight(curNode, node_traits::Right(erasedNode)); // 将删除节点的右节点设置为curNode的右节点
        }
        // 当删除节点是root节点时，将curNode设置为root节点
        if (IsRoot(erasedNode))
        {
            SetRoot(curNode);
        }
        else
        {
            // 当删除节点是左节点时，设置curNode为删除节点的父节点的左节点，反之则设置为右节点
            node_traits::IsLeft(erasedNode) ? node_traits::SetLeft(node_traits::Parent(erasedNode), curNode) :
                                              node_traits::SetRight(node_traits::Parent(erasedNode), curNode);
        }
        // 将curNode和erasedNode的颜色交换
        node_traits::SwapColor(curNode, erasedNode);
    }

    constexpr void FixEraseNode2(node_pointer& erasedNode, node_pointer& fixNode, node_pointer& fixNodeParent, node_pointer& curNode) DECL_NOEXCEPT
    {
        curNode = erasedNode; // 当前操作位置的节点

        fixNode       = node_traits::IsNull(node_traits::Left(erasedNode)) ? node_traits::Right(erasedNode) : node_traits::Left(erasedNode); // 需要修复的节点
        fixNodeParent = node_traits::Parent(erasedNode);                                                                                     // 删除节点的父节点，也是修复后fixNode的父节点

        if (IsRoot(erasedNode)) // 当删除节点是root节点时，将需要修复的节点设置为root节点
        {
            SetRoot(fixNode); // 因为此时删除后整个树最多只有一个节点，所以整个树的root，最大，最小节点都是fixNode
            SetMax(fixNode);
            SetMin(fixNode);
        }
        else
        {
            // 当删除节点是左节点时，设置fixNode为删除节点的父节点的左节点，反之则设置为右节点
            node_traits::IsLeft(erasedNode) ? node_traits::SetLeft(node_traits::Parent(erasedNode), fixNode) :
                                              node_traits::SetRight(node_traits::Parent(erasedNode), fixNode);

            // 当删除节点是这棵树的最大或者最小节点时，将删除后的最大和最小节点重新赋值
            if (IsMax(erasedNode))
                SetMax(node_traits::IsNull(fixNode) ? fixNodeParent : node_traits::Max(fixNode));
            else if (IsMin(erasedNode))
                SetMin(node_traits::IsNull(fixNode) ? fixNodeParent : node_traits::Min(fixNode));
        }
    }

    constexpr void FixBlackHeight(node_pointer& erasedNode, node_pointer& fixNode, node_pointer& fixNodeParent, node_pointer& curNode) DECL_NOEXCEPT
    {
        // 如果删除节点是红色，黑高不变不用修复
        if (node_traits::IsRed(erasedNode))
            return;

        // 如果删除节点是黑色的，删除后会使删除节点后的所有节点的黑高-1，所以还需要对整体的黑高进行平衡
        // 从fixNode向上进行黑高平衡，直到fixNode是root节点或者fixNode是红色
        for (; !IsRoot(fixNode) && node_traits::IsBlack(fixNode); fixNodeParent = node_traits::Parent(fixNode))
        {
            // fixNode是fixNodeParent的左节点时
            if (fixNode == node_traits::Left(fixNodeParent))
            {
                FixBlackHeightL(erasedNode, fixNode, fixNodeParent, curNode);
            }
            else
            {
                FixBlackHeightR(erasedNode, fixNode, fixNodeParent, curNode);
            }
        }
        node_traits::SetColor(fixNode, false);
    }

    constexpr void FixBlackHeightL(node_pointer& erasedNode, node_pointer& fixNode, node_pointer& fixNodeParent, node_pointer& curNode) DECL_NOEXCEPT
    {
        curNode = node_traits::Right(fixNodeParent); // 当前操作节点指向fixNodeParent的右节点
        if (node_traits::IsRed(curNode))             // 当curNode是红色时，fixNodeParent变红，curNode变黑，然后左旋fixNodeParent
        {
            node_traits::SetColor(curNode, false);
            node_traits::SetColor(fixNodeParent, true);
            node_traits::Lrotate(fixNodeParent, m_head);
            curNode = node_traits::Right(fixNodeParent); // 此时将curNode指向调整后的fixNodeParent的右节点
        }

        if (node_traits::IsNull(curNode))
        {
            fixNode = fixNodeParent;
        }
        else if (node_traits::IsBlack(node_traits::Left(curNode)) && node_traits::IsBlack(node_traits::Right(curNode)))
        {
            node_traits::SetColor(curNode, true);
            fixNode = fixNodeParent;
        }
        else
        {
            if (node_traits::IsBlack(node_traits::Right(curNode)))
            {
                node_traits::SetColor(node_traits::Left(curNode), false);
                node_traits::SetColor(curNode, true);
                node_traits::Rrotate(curNode, m_head);
                curNode = node_traits::Right(fixNodeParent);
            }

            node_traits::SetColor(curNode, node_traits::IsRed(fixNodeParent));
            node_traits::SetColor(fixNodeParent, false);
            node_traits::SetColor(node_traits::Right(curNode), false);
            node_traits::Lrotate(fixNodeParent, m_head);
            break;
        }
    }

    constexpr void FixBlackHeightR(node_pointer& erasedNode, node_pointer& fixNode, node_pointer& fixNodeParent, node_pointer& curNode) DECL_NOEXCEPT
    {
        curNode = node_traits::Left(fixNodeParent);
        if (node_traits::IsRed(curNode))
        {
            node_traits::SetColor(curNode, false);
            node_traits::SetColor(fixNodeParent, true);
            node_traits::Rrotate(fixNodeParent, m_head);
            curNode = node_traits::Left(fixNodeParent);
        }

        if (node_traits::IsNull(curNode))
        {
            fixNode = fixNodeParent;
        }
        else if (node_traits::IsBlack(node_traits::Right(curNode)) && node_traits::IsBlack(node_traits::Left(curNode)))
        {
            node_traits::SetColor(curNode, true);
            fixNode = fixNodeParent;
        }
        else
        {
            if (node_traits::IsRed(node_traits::Left(curNode)))
            {
                node_traits::SetColor(node_traits::Right(curNode), false);
                node_traits::SetColor(curNode, true);
                node_traits::Lrotate(curNode, m_head);
                curNode = node_traits::Left(fixNodeParent);
            }

            node_traits::SetColor(curNode, node_traits::IsRed(fixNodeParent));
            node_traits::SetColor(fixNodeParent, false);
            node_traits::SetColor(node_traits::Left(curNode), false);

            node_traits::Rrotate(fixNodeParent, m_head);
            break;
        }
    }

    constexpr node_pointer InsertNode(node_pointer parent, bool isRight, const node_pointer newNode) DECL_NOEXCEPT
    {
        ++m_size;
        node_traits::Parent(newNode) = parent;
        if (parent == m_head) // 插入root节点
        {
            node_traits::Left(m_head)   = newNode;
            node_traits::Parent(m_head) = newNode;
            node_traits::Right(m_head)  = newNode;
            node_traits::SetColor(newNode, false);
            return newNode;
        }

        if (isRight) // 插入到右边
        {
            node_traits::Right(parent) = newNode;
            if (IsMax(parent)) SetMax(newNode); // 更新最大值
        }
        else
        {
            node_traits::Left(parent) = newNode;
            if (IsMin(parent)) SetMin(newNode); // 更新最小值
        }

        for (node_pointer curNode = newNode; node_traits::IsRed(node_traits::Parent(curNode));)
        {
            if (node_traits::IsLeft(node_traits::Parent(curNode)))
            {
                node_pointer uncleNode = node_traits::Right(node_traits::Parent(node_traits::Parent(curNode)));
                if (node_traits::IsRed(uncleNode))
                {
                    node_traits::SetColor(node_traits::Parent(curNode), false);
                    node_traits::SetColor(uncleNode, false);
                    node_traits::SetColor(node_traits::Parent(node_traits::Parent(curNode)), true);
                    curNode = node_traits::Parent(node_traits::Parent(curNode));
                }
                else
                {
                    if (node_traits::IsRight(curNode))
                    {
                        curNode = node_traits::Parent(curNode);
                        node_traits::Lrotate(curNode, m_head);
                    }
                    node_traits::SetColor(node_traits::Parent(curNode), false);
                    node_traits::SetColor(node_traits::Parent(node_traits::Parent(curNode)), true);
                    node_traits::Rrotate(node_traits::Parent(node_traits::Parent(curNode)), m_head);
                }
            }
            else
            {
                node_pointer uncleNode = node_traits::Left(node_traits::Parent(node_traits::Parent(curNode)));
                if (node_traits::IsRed(uncleNode))
                {
                    node_traits::SetColor(node_traits::Parent(curNode), false);
                    node_traits::SetColor(uncleNode, false);
                    node_traits::SetColor(node_traits::Parent(node_traits::Parent(curNode)), true);
                    curNode = node_traits::Parent(node_traits::Parent(curNode));
                }
                else
                {
                    if (node_traits::IsLeft(curNode))
                    {
                        curNode = node_traits::Parent(curNode);
                        node_traits::Rrotate(curNode, m_head);
                    }
                    node_traits::SetColor(node_traits::Parent(curNode), false);
                    node_traits::Parent(node_traits::SetColor(node_traits::Parent(curNode)), true);
                    node_traits::Lrotate(node_traits::Parent(node_traits::Parent(curNode)), m_head);
                }
            }
        }

        node_traits::SetColor(node_traits::Parent(m_head), false);
        return newNode;
    }

    DECL_NODISCARD node_pointer Find(const key_type& key)
    {
        node_pointer result = nullptr;
        node_pointer cur    = Root();
        while (!node_traits::IsNull(cur))
        {
            if (comparator_type::compare(key, node_traits::Value(cur)))
            {
                cur = node_traits::Left(cur);
            }
            else if (comparator_type::compare(node_traits::Value(cur), key))
            {
                cur = node_traits::Right(cur);
            }

            result = cur;
            break;
        }
        return result;
    }

    DECL_NODISCARD std::pair<node_pointer, bool> FindInsertWhere(const key_type& key)
    {
        node_pointer parent = Root();
        bool isRight        = true;
        node_pointer cur    = Root();
        while (!node_traits::IsNull(cur))
        {
            if (comparator_type::compare(key, node_traits::Value(cur)))
            {
                parent  = cur;
                isRight = false;
                cur     = node_traits::Left(cur);
            }
            else if (comparator_type::compare(node_traits::Value(cur), key))
            {
                parent  = cur;
                isRight = true;
                cur     = node_traits::Right(cur);
            }

            return std::make_pair(nullptr, isRight); // 重复插入了
        }
        return std::make_pair(parent, isRight);
    }

    template <class NodeAlty>
    void EraseTree(NodeAlty& alloc, node_pointer node) DECL_NOEXCEPT
    {
        while (!node_traits::IsNull(node))
        {
            EraseTree(alloc, node_traits::Right(node));
            NodeAlty::value_type::Freenode(alloc, std::exchange(node, node_traits::Left(node)));
        }
    }

    template <class NodeAlty>
    void EraseHead(NodeAlty& alloc) DECL_NOEXCEPT
    {
        EraseTree(alloc, node_traits::Parent(m_head));
        NodeAlty::value_type::FreeNode(alloc, m_head);
    }

    node_pointer m_head;
    size_type m_size;
};

}} // namespace screw::data_structure