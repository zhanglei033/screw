#pragma once
#include "base/IteratorBase.h"
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
#include "container/ContainerBase.h"
#include <atomic>
#include <memory>
namespace screw {
namespace data_structure {
template <class Iter, class Node>
struct InsertHandleResult
{
    Iter position;
    bool inserted;
    Node node;
};

template <class Derived, class Container>
struct NodeHandleBase1
{
    using value_type = typename Container::value_type;

    DECL_NODISCARD value_type& value() const DECL_NOEXCEPT
    {
        const auto& self = static_cast<const Derived&>(*this);
        return self.get_pointer()->Value();
    }
};

template <class Derived, class Container>
struct NodeHandleBase2
{
    using key_type    = typename Container::key_type;
    using mapped_type = typename Container::value_type;

    DECL_NODISCARD key_type& key() const DECL_NOEXCEPT
    {
        const auto& self = static_cast<const Derived&>(*this);
        return const_cast<key_type&>(self.get_pointer()->Value().first);
    }
    DECL_NODISCARD mapped_type& mapped() const DECL_NOEXCEPT
    {
        const auto& self = static_cast<const Derived&>(*this);
        return self.get_pointer()->Value().second;
    }
};

template <class Container, class Alloc, template <class...> class Base>
class NodeHandle : public Base<NodeHandle<Container, Alloc, Base>, Container>
{
public:
    using allocator_type = Alloc;

private:
    using node_type             = typename Container::node_type;
    using node_pointer          = typename Container::node_pointer;
    using allocator_traits      = std::allocator_traits<Alloc>;
    using node_allocator_type   = typename std::allocator_traits<allocator_type>::template rebind_alloc<node_type>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;

    node_pointer m_ptr{};
    Container* m_cont;

    void Clear() DECL_NOEXCEPT
    {
        if (m_ptr != nullptr)
        {
            m_cont->DestroyNode(node_allocator_type{}, m_ptr);
            m_ptr = nullptr;
        }
    }

    NodeHandle(const node_pointer ptr, Container* cont) DECL_NOEXCEPT
        : m_ptr{ptr},
          m_cont{cont}
    {
    }

public:
    constexpr NodeHandle() DECL_NOEXCEPT
    {
    }

    ~NodeHandle() DECL_NOEXCEPT
    {
        Clear();
    }

    NodeHandle(NodeHandle&& other) DECL_NOEXCEPT
        : m_ptr{other.m_ptr}
    {
        other.m_ptr = nullptr;
    }

    NodeHandle& operator=(NodeHandle&& other) DECL_NOEXCEPT
    {
        Clear();
        m_ptr       = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }

    node_pointer get_pointer() const DECL_NOEXCEPT
    {
        return m_ptr;
    }

    explicit operator bool() const DECL_NOEXCEPT
    {
        return m_ptr != nullptr;
    }

    DECL_NODISCARD bool empty() const DECL_NOEXCEPT
    {
        return m_ptr == nullptr;
    }

    node_pointer Release() DECL_NOEXCEPT
    {
        return std::exchange(m_ptr, nullptr);
    }

    void swap(NodeHandle& other) DECL_NOEXCEPT
    {
        std::swap(m_ptr, other.m_ptr);
    }
    friend void swap(NodeHandle& l, NodeHandle& r) DECL_NOEXCEPT
    {
        l.swap(r);
    }

    static NodeHandle Make(const node_pointer m_ptr, const allocator_type& allocator)
    {
        return NodeHandle{m_ptr, allocator};
    }
};

template <class T, class Comparator, class Allocator>
struct RBTreeNodeOnlyKeyTraits
{
    using key_type       = T;
    using value_type     = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using key_comparator = Comparator;

    struct value_compare
    {
        using key_compare = key_comparator;
        DECL_STATIC_CONSTEXPR key_comparator comparator;
        DECL_NODISCARD bool operator()(const value_type& l, const value_type& r) const { return comparator(l, r); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const key_type& l, const key_type& r) DECL_NOEXCEPT { return comparator(l, r); }
    };
};

template <class KT, class T, class Comparator, class Allocator>
struct RBTreeNodeWithValueTraits
{
    using key_type       = KT;
    using value_type     = std::pair<const KT, T>;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using key_comparator = Comparator;
    struct value_compare
    {
        using key_compare = key_comparator;
        DECL_STATIC_CONSTEXPR key_comparator comparator{};
        DECL_NODISCARD bool operator()(const key_type& l, const key_type& r) const { return comparator(l, r); }
        DECL_NODISCARD bool operator()(const key_type& l, const value_type& r) const { return comparator(l, r.first); }
        DECL_NODISCARD bool operator()(const value_type& l, const key_type& r) const { return comparator(l.first, r); }
        DECL_NODISCARD bool operator()(const value_type& l, const value_type& r) const { return comparator(l.first, r.first); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const key_type& l, const key_type& r) DECL_NOEXCEPT { return comparator(l, r); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const key_type& l, const value_type& r) DECL_NOEXCEPT { return comparator(l, r.first); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const value_type& l, const key_type& r) DECL_NOEXCEPT { return comparator(l.first, r); }
        DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(const value_type& l, const value_type& r) DECL_NOEXCEPT { return comparator(l.first, r.first); }
    };
};

template <class Key, class... Args>
struct RBTreeKeyExtract
{
    static constexpr bool is_extractable = false;
};

template <class Key>
struct RBTreeKeyExtract<Key, Key>
{
    static constexpr bool is_extractable = true;
    static constexpr const Key& Extract(const Key& key) DECL_NOEXCEPT
    {
        return key;
    }
};

template <class Key, class Value>
struct RBTreeKeyExtract<Key, Key, Value>
{
    static constexpr bool is_extractable = true;
    static constexpr const Key& Extract(const Key& key, const Value&) DECL_NOEXCEPT
    {
        return key;
    }
};

template <class Key, class First, class Second>
struct RBTreeKeyExtract<Key, std::pair<First, Second>>
{
    static constexpr bool is_extractable = std::is_same_v<Key, std::remove_cvref_t<First>>;
    static const Key& Extract(const std::pair<First, Second>& value)
    {
        return value.first;
    }
};

template <class ComparatorType>
struct ComparatorTraits
{
    using value_compare = ComparatorType;
    using key_compare   = typename value_compare::key_compare;

    template <class Comparator>
    struct is_less
    {
        DECL_STATIC_CONSTEXPR bool less    = Comparator()(1, 2);
        DECL_STATIC_CONSTEXPR bool equal   = Comparator()(1, 1);
        DECL_STATIC_CONSTEXPR bool greater = Comparator()(2, 1);

        DECL_STATIC_CONSTEXPR bool value = less && !equal && !greater;
    };

    DECL_STATIC_CONSTEXPR bool is_less_v = is_less<key_compare>::value;

    template <class LT, class RT>
    DECL_NODISCARD DECL_STATIC_CONSTEXPR bool Compare(LT&& l, RT&& r) DECL_NOEXCEPT
    {
        return value_compare::Compare(std::forward<LT>(l), std::forward<RT>(r));
    }
};

template <class Traits>
struct RBTreeNode
{
    using key_type              = typename Traits::key_type;
    using value_type            = typename Traits::value_type;
    using value_compare         = typename Traits::value_compare;
    using allocator_type        = typename Traits::allocator_type;
    using node_allocator_type   = typename std::allocator_traits<allocator_type>::template rebind_alloc<RBTreeNode<Traits>>;
    using node_allocator_traits = std::allocator_traits<node_allocator_type>;
    using node_pointer          = typename node_allocator_traits::pointer;

    enum Color : bool
    {
        Red   = true,
        Black = false
    };

    RBTreeNode()                  = default;
    RBTreeNode(const RBTreeNode&) = delete;
    RBTreeNode& operator=(const RBTreeNode&) = delete;

    // 左节点
    DECL_NODISCARD node_pointer& Left() DECL_NOEXCEPT { return m_left; }
    DECL_NODISCARD const node_pointer& Left() const DECL_NOEXCEPT { return m_left; }

    // 叔父节点
    DECL_NODISCARD node_pointer& Right() DECL_NOEXCEPT { return m_right; }
    DECL_NODISCARD const node_pointer& Right() const DECL_NOEXCEPT { return m_right; }

    // 祖父节点
    DECL_NODISCARD node_pointer& Parent() DECL_NOEXCEPT { return m_parent; }
    DECL_NODISCARD const node_pointer& Parent() const DECL_NOEXCEPT { return m_parent; }

    // 兄弟节点
    DECL_NODISCARD node_pointer& Sibling() DECL_NOEXCEPT { return IsLeft() ? m_right : m_left; }
    DECL_NODISCARD const node_pointer& Sibling() const DECL_NOEXCEPT { return IsLeft() ? m_right : m_left; }

    // 叔父节点
    DECL_NODISCARD node_pointer& Uncle() DECL_NOEXCEPT { return m_parent->Sibling(); }
    DECL_NODISCARD const node_pointer& Uncle() const DECL_NOEXCEPT { return m_parent->Sibling(); }

    // 祖父节点
    DECL_NODISCARD node_pointer& Grandparent() DECL_NOEXCEPT { return m_parent->Parent(); }
    DECL_NODISCARD const node_pointer& Grandparent() const DECL_NOEXCEPT { return m_parent->Parent(); }

    // 判断当前节点是否是root节点
    // root的父节点是head节点，head节点的父节点是root节点
    // DECL_NODISCARD bool IsRoot() const DECL_NOEXCEPT { return m_parent->Parent() == *this; }
    // root的父节点是head节点，head节点是空节点
    DECL_NODISCARD bool IsRoot() const DECL_NOEXCEPT { return m_parent->IsNull(); }

    // 判断当前节点是否是左节点
    DECL_NODISCARD bool IsLeft() const DECL_NOEXCEPT { return m_parent->Left() == this; }

    // 判断当前节点是否是右节点
    DECL_NODISCARD bool IsRight() const DECL_NOEXCEPT { return m_parent->Right() == this; }

    // 判断当前节点是否是空节点
    DECL_NODISCARD bool IsNull() const DECL_NOEXCEPT { return m_isNull; }

    // 判断当前节点是否是红色节点
    DECL_NODISCARD bool IsRed() const DECL_NOEXCEPT { return m_isRed; }

    // 判断当前节点是否是黑色节点
    DECL_NODISCARD bool IsBlack() const DECL_NOEXCEPT { return !m_isRed; }

    // 设置为null节点
    void SetNull() DECL_NOEXCEPT { m_isNull = true; }

    // 设置为非null节点
    void SetNotNull() DECL_NOEXCEPT { m_isNull = false; }


    // 设置为红色节点
    void SetRed() DECL_NOEXCEPT { m_isRed = Red; }

    // 设置为黑色节点
    void SetBlack() DECL_NOEXCEPT { m_isRed = Black; }

    // 交换颜色
    void SwapColor(node_pointer node) DECL_NOEXCEPT { std::swap(m_isRed, node->m_isRed); }

    // 获取引用计数
    DECL_NODISCARD size_t Uses() DECL_NOEXCEPT { return m_uses.load(); }

    // 增加引用计数
    void IncrementRef() DECL_NOEXCEPT { m_uses.fetch_add(1); }

    // 减少引用计数
    void DecrementRef() DECL_NOEXCEPT { m_uses.fetch_sub(1); }

    // 节点值
    value_type& Value() DECL_NOEXCEPT { return m_value; }
    const value_type& Value() const DECL_NOEXCEPT { return m_value; }

    // 设置左节点
    void SetLeft(node_pointer left) DECL_NOEXCEPT
    {
        m_left = left;
        if (!left->IsNull())
        {
            left->Parent() = this;
        }
    }

    // 设置右节点
    void SetRight(node_pointer right) DECL_NOEXCEPT
    {
        m_right = right;
        if (!right->IsNull())
        {
            right->Parent() = this;
        }
    }

    // 获取当前节点的最大节点
    DECL_NODISCARD node_pointer Max() const DECL_NOEXCEPT
    {
        node_pointer max = const_cast<node_pointer>(this);
        while (!max->Right()->IsNull())
        {
            max = max->Right();
        }
        return max;
    }

    // 获取当前节点的最小节点
    DECL_NODISCARD node_pointer Min() const DECL_NOEXCEPT
    {
        node_pointer min = const_cast<node_pointer>(this);
        while (!min->Left()->IsNull())
        {
            min = min->Left();
        }
        return min;
    }

    // 获取下一个节点
    DECL_NODISCARD node_pointer Next() const DECL_NOEXCEPT
    {
        node_pointer curNode = const_cast<node_pointer>(this);
        if (curNode->Right()->IsNull())
        {
            node_pointer node;
            while (!(node = curNode->Parent())->IsNull() && curNode == node->Right())
            {
                curNode = node;
            }
            curNode = node;
        }
        else
        {
            curNode = curNode->Right()->Min();
        }
        return curNode;
    }

    // 获取上一个节点
    DECL_NODISCARD node_pointer Prev() const DECL_NOEXCEPT
    {
        node_pointer curNode = const_cast<node_pointer>(this);
        if (curNode->IsNull())
        {
            curNode = curNode->Right();
        }
        else if (curNode->Left()->IsNull())
        {
            node_pointer node;
            while (!(node = curNode->Parent())->IsNull() && curNode == node->Left())
            {
                curNode = node;
            }

            if (!curNode->IsNull())
            {
                curNode = node;
            }
        }
        else
        {
            curNode = curNode->Left()->Max();
        }
        return curNode;
    }

    void Lrotate(node_pointer head) DECL_NOEXCEPT
    {
        node_pointer right = m_right;
        SetRight(right->Left());
        right->Parent() = m_parent;
        if (this == head->Parent())
        {
            head->Parent() = right;
        }
        else if (IsLeft())
        {
            m_parent->Left() = right;
        }
        else
        {
            m_parent->Right() = right;
        }
        right->SetLeft(this);
    }

    void Rrotate(node_pointer head) DECL_NOEXCEPT
    {
        node_pointer left = m_left;
        SetLeft(left->Right());
        left->Parent() = m_parent;
        if (this == head->Parent())
        {
            head->Parent() = left;
        }
        else if (IsRight())
        {
            m_parent->Right() = left;
        }
        else
        {
            m_parent->Left() = left;
        }
        left->SetRight(this);
    }

private:
    node_pointer m_left       = nullptr; // 左节点
    node_pointer m_right      = nullptr; // 右节点
    node_pointer m_parent     = nullptr; // 父节点
    bool m_isRed              = false;   // 是红色节点
    bool m_isNull             = false;   // 是空节点
    std::atomic_size_t m_uses = 0;       // 节点的引用计数
    value_type m_value{};                // 节点所存储的值
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

    using node_type               = typename Container::node_type;
    using value_compare           = typename Container::value_compare;
    using node_pointer            = typename Container::node_pointer;
    using value_comparator_traits = ComparatorTraits<value_compare>;

#ifndef NDEBUG
    constexpr BPTreeIterator() DECL_NOEXCEPT
        : m_iter(nullptr),
          m_cont(nullptr)
    {
    }
    constexpr explicit BPTreeIterator(node_pointer iter, const Container* cont) DECL_NOEXCEPT
        : m_iter(iter),
          m_cont(cont)
    {
    }
    constexpr BPTreeIterator(const BPTreeIterator<Container, false>& iter) DECL_NOEXCEPT
        : m_iter(iter.m_iter),
          m_cont(iter.m_cont) {}

    constexpr BPTreeIterator& operator=(const BPTreeIterator<Container, false>& iter) DECL_NOEXCEPT
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
    constexpr explicit BPTreeIterator(node_pointer iter, const Container* cont) DECL_NOEXCEPT
        : m_iter(iter)
    {
    }
    constexpr BPTreeIterator(const BPTreeIterator<Container, false>& iter) DECL_NOEXCEPT
        : m_iter(iter.m_iter) {}

    constexpr BPTreeIterator& operator=(const const BPTreeIterator<Container, false>& iter) DECL_NOEXCEPT
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
        ASSERT(!m_iter->IsNull(), "Iterator is last");
#endif
        return m_iter->Value();
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
            node_pointer next = m_iter->Next();
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
            node_pointer prev = m_iter->Prev();
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
            node_pointer prev = m_iter->Prev();
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
            node_pointer next = m_iter->Next();
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
        return m_iter == r.m_iter;
    }
    DECL_NODISCARD constexpr bool LessThan(const BPTreeIterator& r) const DECL_NOEXCEPT
    {
#ifndef NDEBUG
        ASSERT(m_cont != nullptr || m_iter != nullptr, "Iterator not initialized");
        ASSERT(m_cont == r.m_cont, "Iterators incompatible");
#endif
        return !(value_comparator_traits::is_less_v ^ value_comparator_traits::Compare(m_iter->Value(), r.m_iter->Value()));
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

#ifndef NDEBUG
    friend void VerifyIterRange(const BPTreeIterator& first, const BPTreeIterator& last) DECL_NOEXCEPT
    {
        ASSERT(first.LessThan(last), "Iterators range error");
    }
#endif

    node_pointer m_iter;
#ifndef NDEBUG
    const Container* m_cont;
#endif
};

template <class Types>
class RBTreeNodeContainer
{
public:
    using key_type                = typename Types::key_type;
    using value_type              = typename Types::value_type;
    using size_type               = typename Types::size_type;
    using difference_type         = typename Types::difference_type;
    using pointer                 = typename Types::pointer;
    using const_pointer           = typename Types::const_pointer;
    using reference               = typename Types::reference;
    using const_reference         = typename Types::const_reference;
    using node_type               = typename Types::node_type;
    using node_pointer            = typename Types::node_pointer;
    using value_compare           = typename Types::value_compare;
    using value_comparator_traits = ComparatorTraits<value_compare>;
    using node_allocator_type     = typename node_type::node_allocator_type;

    using iterator       = BPTreeIterator<RBTreeNodeContainer<Types>, false>;
    using const_iterator = BPTreeIterator<RBTreeNodeContainer<Types>, true>;

    struct FindResult
    {
        node_pointer m_parent;
        bool m_isRight;
        node_pointer m_bound;
    };

    struct FindHintResult
    {
        node_pointer m_parent;
        bool m_isRight;
        bool m_isDuplicate;
    };

    // 生成Head节点
    DECL_NODISCARD constexpr auto GenerateHeadNode(node_allocator_type& alloc) DECL_NOEXCEPT
    {
        node_pointer node = alloc.allocate(1);
        std::construct_in_place(node->Left(), node);
        std::construct_in_place(node->Right(), node);
        std::construct_in_place(node->Parent(), node);
        node->SetBlack();
        node->SetNull();
        return node;
    }
    // 生成普通节点
    template <class... Args>
    DECL_NODISCARD constexpr auto GenerateNode(node_allocator_type& alloc, node_pointer head, Args&&... args) DECL_NOEXCEPT
    {
        node_pointer node = alloc.allocate(1);
        std::construct_in_place(node->Value(), std::forward<Args>(args)...);
        std::construct_in_place(node->Left(), head);
        std::construct_in_place(node->Right(), head);
        std::construct_in_place(node->Parent(), head);
        node->SetRed();
        node->SetNotNull();
        return node;
    }

    //  head 节点
    DECL_NODISCARD constexpr node_pointer GetHead() const DECL_NOEXCEPT { return m_head; }
    constexpr node_pointer SetHead(node_pointer head) DECL_NOEXCEPT { return m_head = head; }

    constexpr void InitSize() DECL_NOEXCEPT { m_size = 0; }
    DECL_NODISCARD constexpr size_type GetSize() const DECL_NOEXCEPT { return m_size; }
    constexpr size_type SetSize(size_type size) DECL_NOEXCEPT { return m_size = size; }
    constexpr void IncrementSize(size_type n = 1) DECL_NOEXCEPT { m_size += n; }
    constexpr void DecrementSize(size_type n = 1) DECL_NOEXCEPT { m_size -= n; }
    // 这棵树Root节点
    DECL_NODISCARD constexpr node_pointer Root() const DECL_NOEXCEPT
    {
        return m_head->Parent();
    }
    constexpr void SetRoot(node_pointer node) DECL_NOEXCEPT { m_head->Parent() = node; }
    DECL_NODISCARD constexpr bool IsRoot(node_pointer node) const DECL_NOEXCEPT { return node == m_head->Parent(); }

    // 这棵树最大的节点
    DECL_NODISCARD constexpr node_pointer Max() const DECL_NOEXCEPT { return m_head->Right(); }
    constexpr void SetMax(node_pointer node) DECL_NOEXCEPT { m_head->Right() = node; }
    DECL_NODISCARD constexpr bool IsMax(node_pointer node) const DECL_NOEXCEPT { return node == m_head->Right(); }

    // 这棵树最小的节点
    DECL_NODISCARD constexpr node_pointer Min() const DECL_NOEXCEPT { return m_head->Left(); }
    constexpr void SetMin(node_pointer node) DECL_NOEXCEPT { m_head->Left() = node; }
    DECL_NODISCARD constexpr bool IsMin(node_pointer node) const DECL_NOEXCEPT { return node == m_head->Left(); }

    // 提取出一个节点
    constexpr node_pointer ExtractNode(node_pointer erasedNode) DECL_NOEXCEPT
    {
        node_pointer fixNode       = nullptr;
        node_pointer fixNodeParent = nullptr;
        node_pointer curNode       = erasedNode;

        if (curNode->Left()->IsNull()) // 左子树为空
        {
            fixNode = curNode->Right();
        }
        else if (curNode->Right()->IsNull()) // 右子树为空
        {
            fixNode = curNode->Left();
        }
        else // 两个子树，用后续节点替换删除
        {
            curNode = curNode->Next();
            fixNode = curNode->Right();
        }

        if (curNode == erasedNode) // 最多一个子树，重新连接
        {
            fixNodeParent = erasedNode->Parent();
            if (!fixNode->IsNull())
            {
                fixNode->Parent() = fixNodeParent; // 连接父节点
            }

            if (m_head->Parent() == erasedNode)
            {
                m_head->Parent() = fixNode; // root节点
            }
            else if (fixNodeParent->Left() == erasedNode)
            {
                fixNodeParent->Left() = fixNode; // 父节点连接左节点
            }
            else
            {
                fixNodeParent->Right() = fixNode; // 父节点连接右节点
            }

            if (m_head->Left() == erasedNode) // 删除的节点是最小的节点
            {
                m_head->Left() = fixNode->IsNull() ? fixNodeParent : fixNode->Min();
            }
            if (m_head->Right() == erasedNode) // 删除的节点是最大的节点
            {
                m_head->Right() = fixNode->IsNull() ? fixNodeParent : fixNode->Max();
            }
        }
        else // 两个子树 curNode是最终删除的节点
        {
            curNode->SetLeft(erasedNode->Left());

            if (curNode == erasedNode->Right())
            {
                fixNodeParent = curNode;
            }
            else
            {
                fixNodeParent = curNode->Parent();

                fixNodeParent->SetLeft(fixNode->Left());
                curNode->SetLeft(erasedNode->Right());
            }

            if (m_head->Parent() == erasedNode)
            {
                m_head->Parent() = curNode;
            }
            else if (erasedNode->IsLeft())
            {
                erasedNode->Parent()->Left() = curNode;
            }
            else
            {
                erasedNode->Parent()->Right() = curNode;
            }
            curNode->Parent() = erasedNode->Parent();
            curNode->SwapColor(erasedNode);
        }

        if (erasedNode->IsBlack())
        {
            for (; fixNode != m_head->Parent() && fixNode->IsBlack(); fixNodeParent = fixNode->Parent())
            {
                if (fixNode == fixNodeParent->Left())
                {
                    curNode = fixNodeParent->Right();
                    if (curNode->IsRed())
                    {
                        curNode->SetBlack();
                        fixNodeParent->SetRed();
                        fixNodeParent->Lrotate(m_head);
                        curNode = fixNodeParent->Right();
                    }

                    if (curNode->IsNull())
                    {
                        fixNode = fixNodeParent;
                    }
                    else if (curNode->Left()->IsBlack() && curNode->Right()->IsBlack())
                    {
                        curNode->SetRed();
                        fixNode = fixNodeParent;
                    }
                    else
                    {
                        if (curNode->Right()->IsBlack())
                        {
                            curNode->Left()->SetBlack();
                            curNode->SetRed();
                            curNode->Rrotate(m_head);
                            curNode = fixNodeParent->Right();
                        }

                        fixNodeParent->IsRed() ? curNode->SetRed() : curNode->SetBlack();
                        fixNodeParent->SetBlack();
                        curNode->Right()->SetRed();
                        fixNodeParent->Lrotate(m_head);
                        break;
                    }
                }
                else
                {
                    curNode = fixNodeParent->Left();
                    if (curNode->IsRed())
                    {
                        curNode->SetBlack();
                        fixNodeParent->SetRed();
                        fixNodeParent->Rrotate(m_head);
                        curNode = fixNodeParent->Left();
                    }

                    if (curNode->IsNull())
                    {
                        fixNode = fixNodeParent;
                    }
                    else if (curNode->Left()->IsBlack() && curNode->Right()->IsBlack())
                    {
                        curNode->SetRed();
                        fixNode = fixNodeParent;
                    }
                    else
                    {
                        if (!curNode->Left()->IsRed())
                        {
                            curNode->Right()->SetBlack();
                            curNode->SetRed();
                            curNode->Lrotate(m_head);
                            curNode = fixNodeParent->Left();
                        }
                        fixNodeParent->IsRed() ? curNode->SetRed() : curNode->SetBlack();
                        fixNodeParent->SetBlack();
                        curNode->Left()->SetBlack();
                        fixNodeParent->Rrotate(m_head);
                        break;
                    }
                }
            }
            fixNode->SetBlack();
        }

        m_size--;
        return erasedNode;
    }

    constexpr node_pointer InsertNode(node_pointer parent, bool isRight, const node_pointer newNode) DECL_NOEXCEPT
    {
        ++m_size;
        newNode->Parent() = parent;
        if (parent == m_head) // root节点
        {
            m_head->Left()   = newNode;
            m_head->Parent() = newNode;
            m_head->Right()  = newNode;
            newNode->SetBlack();
            return newNode;
        }

        if (isRight)
        {
            parent->Right() = newNode;          // 插入父节点右边
            if (IsMax(parent)) SetMax(newNode); // 如果父节点是max节点时，需要将max节点更新为newNode
        }
        else
        {
            parent->Left() = newNode;           // 插入父节点左边
            if (IsMin(parent)) SetMin(newNode); // 如果父节点是min节点时，需要将min节点更新为newNode
        }

        node_pointer curNode = newNode;
        for (node_pointer parentNode = curNode->Parent(); parentNode->IsRed(); parentNode = curNode->Parent())
        {
            node_pointer grandparentNode = parentNode->Parent();
            bool parentIsLeft            = parentNode->IsLeft();
            node_pointer uncleNode       = parentIsLeft ? grandparentNode->Right() : grandparentNode->Left();
            if (uncleNode->IsRed())
            {
                parentNode->SetBlack();
                uncleNode->SetBlack();
                grandparentNode->SetRed();
                curNode = grandparentNode;
                continue;
            }

            if (parentIsLeft)
            {
                if (curNode->IsRight())
                {
                    curNode = parentNode;
                    curNode->Lrotate(m_head);
                }
                parentNode      = curNode->Parent();
                grandparentNode = parentNode->Parent();
                parentNode->SetBlack();
                grandparentNode->SetRed();
                grandparentNode->Rrotate(m_head);
            }
            else
            {
                if (curNode->IsLeft())
                {
                    curNode = parentNode;
                    curNode->Rrotate(m_head);
                }
                parentNode      = curNode->Parent();
                grandparentNode = parentNode->Parent();
                parentNode->SetBlack();
                grandparentNode->SetRed();
                grandparentNode->Lrotate(m_head);
            }
        }

        m_head->Parent()->SetBlack();
        return newNode;
    }

    DECL_NODISCARD FindResult FindUpper(const key_type& key) const DECL_NOEXCEPT
    {
        FindResult result{m_head->Parent(), true, m_head};
        node_pointer cur = result.m_parent;
        while (!cur->IsNull())
        {
            result.m_parent = cur;
            if (value_comparator_traits::Compare(key, cur->Value()))
            {
                result.m_isRight = false;
                result.m_bound   = cur;
                cur              = cur->Left();
            }
            else
            {
                result.m_isRight = true;
                cur              = cur->Right();
            }
        }
        return result;
    }

    DECL_NODISCARD FindResult FindLower(const key_type& key) const DECL_NOEXCEPT
    {
        FindResult result{m_head->Parent(), true, m_head};
        node_pointer cur = result.m_parent;
        while (!cur->IsNull())
        {
            result.m_parent = cur;
            if (value_comparator_traits::Compare(cur->Value(), key))
            {
                result.m_isRight = true;
                cur              = cur->Right();
            }
            else
            {
                result.m_isRight = false;
                result.m_bound   = cur;
                cur              = cur->Left();
            }
        }
        return result;
    }

    DECL_NODISCARD FindHintResult FindHint(const node_pointer node, const key_type& key) const DECL_NOEXCEPT
    {
        if (node->IsNull())
        {
            if (Root()->IsNull() || value_comparator_traits::Compare(m_head->Right()->Value(), key))
            {
                return FindHintResult{m_head->Right(), true, false};
            }
        }
        else if (m_head->Left() == node)
        {
            if (value_comparator_traits::Compare(key, node->Value()))
            {
                return FindHintResult{node, false, false};
            }
        }
        else if (value_comparator_traits::Compare(key, node->Value()))
        {
            const auto prev = node->Prev();
            if (value_comparator_traits::Compare(prev->Value(), key))
            {
                return prev->Right()->IsNull() ? FindHintResult{prev, true, false} : FindHintResult{node, false, false};
            }
        }
        else if (value_comparator_traits::Compare(node->Value(), key))
        {
            const auto next = node->Next();
            if (next->IsNull() || value_comparator_traits::Compare(key, next->Value()))
            {
                return node->Right()->IsNull() ? FindHintResult{node, true, false} : FindHintResult{next, false, false};
            }
        }
        else
        {
            return FindHintResult{node, true, true};
        }

        auto result        = FindLower(key);
        node_pointer bound = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value())) // 重复插入
        {
            return FindHintResult{bound, false, true};
        }

        return FindHintResult{result.m_parent, result.m_isRight, false};
    }

    DECL_NODISCARD node_pointer Find(const key_type& key) const DECL_NOEXCEPT
    {
        auto result        = FindLower(key);
        node_pointer bound = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value())) // 重复插入
        {
            return bound;
        }
        return m_head;
    }

    void EraseTree(node_allocator_type& alloc, node_pointer node) DECL_NOEXCEPT
    {
        while (!node->IsNull())
        {
            EraseTree(alloc, node->Right());
            FreeNode(alloc, std::exchange(node, node->Left()));
        }
    }

    void EraseHead(node_allocator_type& alloc) DECL_NOEXCEPT
    {
        EraseTree(alloc, m_head->Parent());
        FreeNode(alloc, m_head);
    }

    // 释放节点所占资源
    constexpr void FreeNode(node_allocator_type& alloc, node_pointer node) DECL_NOEXCEPT
    {
        std::destroy_in_place(node->Left());
        std::destroy_in_place(node->Right());
        std::destroy_in_place(node->Parent());
        std::allocator_traits<node_allocator_type>::deallocate(alloc, node, 1);
    }

    // 销毁这个节点
    constexpr void DestroyNode(node_allocator_type& alloc, node_pointer node) DECL_NOEXCEPT
    {
        std::allocator_traits<node_allocator_type>::destroy(alloc, std::addressof(node->Value()));
        FreeNode(alloc, node);
    }

    constexpr void Swap(RBTreeNodeContainer& other) DECL_NOEXCEPT
    {
        std::swap(m_head, other.m_head);
        m_size       = other.m_size;
        other.m_size = 0;
    }

private:
    node_pointer m_head;
    size_type m_size;
};

template <class Types>
struct RBTreeNodeBinaryContainer
{
    using key_type                = typename Types::key_type;
    using value_type              = typename Types::value_type;
    using size_type               = typename Types::size_type;
    using difference_type         = typename Types::difference_type;
    using pointer                 = typename Types::pointer;
    using const_pointer           = typename Types::const_pointer;
    using reference               = typename Types::reference;
    using const_reference         = typename Types::const_reference;
    using node_type               = typename Types::node_type;
    using node_pointer            = typename Types::node_pointer;
    using value_compare           = typename Types::value_compare;
    using value_comparator_traits = ComparatorTraits<value_compare>;
    using node_allocator_type     = typename node_type::node_allocator_type;

    using iterator       = BPTreeIterator<RBTreeNodeContainer<Types>, false>;
    using const_iterator = BPTreeIterator<RBTreeNodeContainer<Types>, true>;

    struct FindResult
    {
        node_pointer m_parent;
        bool m_isRight;
        node_pointer m_bound;
    };

    struct FindHintResult
    {
        node_pointer m_parent;
        bool m_isRight;
        bool m_isDuplicate;
    };
    // 生成Head节点
    DECL_NODISCARD constexpr auto GenerateHeadNode(node_allocator_type& alloc) DECL_NOEXCEPT -> node_pointer
    {
        node_pointer node = reinterpret_cast<node_pointer>(m_buf.data());
        std::construct_in_place(node->Left(), node);
        std::construct_in_place(node->Right(), node);
        std::construct_in_place(node->Parent(), node);
        node->IsRed()  = false;
        node->IsNull() = true;
        return node;
    }
    // 生成普通节点
    template <class... Args>
    DECL_NODISCARD constexpr auto GenerateNode(node_allocator_type& alloc, node_pointer head, Args&&... args) DECL_NOEXCEPT -> node_pointer
    {
        m_buf.insert(sizeof(node_type), '\0');
        node_pointer node = reinterpret_cast<node_pointer>(m_buf.data() - sizeof(node_type));
        std::construct_in_place(node->Value(), std::forward<Args>(args)...);
        std::construct_in_place(node->Left(), head);
        std::construct_in_place(node->Right(), head);
        std::construct_in_place(node->Parent(), head);
        node->IsRed()  = true;
        node->IsNull() = false;
        return node;
    }

    //  head 节点
    // DECL_NODISCARD constexpr node_pointer GetHead() const DECL_NOEXCEPT { return reinterpret_cast<node_pointer>(m_buf.data()); }
    // constexpr node_pointer SetHead(node_pointer head) DECL_NOEXCEPT { return m_head = head; }
    //
    //    constexpr void InitSize() DECL_NOEXCEPT { m_size = 0; }
    //    DECL_NODISCARD constexpr size_type GetSize() const DECL_NOEXCEPT { return m_size; }
    //    constexpr size_type SetSize(size_type size) DECL_NOEXCEPT { return m_size = size; }
    //    constexpr void IncrementSize(size_type n = 1) DECL_NOEXCEPT { m_size += n; }
    //    constexpr void DecrementSize(size_type n = 1) DECL_NOEXCEPT { m_size -= n; }
    //    // 这棵树Root节点
    //    DECL_NODISCARD constexpr node_pointer Root() const DECL_NOEXCEPT
    //    {
    //        return m_head->Parent();
    //    }
    //    constexpr void SetRoot(node_pointer node) DECL_NOEXCEPT { m_head->Parent() = node; }
    //    DECL_NODISCARD constexpr bool IsRoot(node_pointer node) const DECL_NOEXCEPT { return node == m_head->Parent(); }
    //
    //    // 这棵树最大的节点
    //    DECL_NODISCARD constexpr node_pointer Max() const DECL_NOEXCEPT { return m_head->Right(); }
    //    constexpr void SetMax(node_pointer node) DECL_NOEXCEPT { m_head->Right() = node; }
    //    DECL_NODISCARD constexpr bool IsMax(node_pointer node) const DECL_NOEXCEPT { return node == m_head->Right(); }
    //
    //    // 这棵树最小的节点
    //    DECL_NODISCARD constexpr node_pointer Min() const DECL_NOEXCEPT { return m_head->Left(); }
    //    constexpr void SetMin(node_pointer node) DECL_NOEXCEPT { m_head->Left() = node; }
    //    DECL_NODISCARD constexpr bool IsMin(node_pointer node) const DECL_NOEXCEPT { return node == m_head->Left(); }
    //
    //    // 提取出一个节点
    //    constexpr node_pointer ExtractNode(node_pointer erasedNode) DECL_NOEXCEPT
    //    {
    //        node_pointer fixNode       = nullptr;
    //        node_pointer fixNodeParent = nullptr;
    //        node_pointer curNode       = erasedNode;
    //
    //        if (curNode->Left()->IsNull()) // 左子树为空
    //        {
    //            fixNode = curNode->Right();
    //        }
    //        else if (curNode->Right()->IsNull()) // 右子树为空
    //        {
    //            fixNode = curNode->Left();
    //        }
    //        else // 两个子树，用后续节点替换删除
    //        {
    //            curNode = curNode->Next();
    //            fixNode = curNode->Right();
    //        }
    //
    //        if (curNode == erasedNode) // 最多一个子树，重新连接
    //        {
    //            fixNodeParent = erasedNode->Parent();
    //            if (!fixNode->IsNull())
    //            {
    //                fixNode->Parent() = fixNodeParent; // 连接父节点
    //            }
    //
    //            if (m_head->Parent() == erasedNode)
    //            {
    //                m_head->Parent() = fixNode; // root节点
    //            }
    //            else if (fixNodeParent->Left() == erasedNode)
    //            {
    //                fixNodeParent->Left() = fixNode; // 父节点连接左节点
    //            }
    //            else
    //            {
    //                fixNodeParent->Right() = fixNode; // 父节点连接右节点
    //            }
    //
    //            if (m_head->Left() == erasedNode) // 删除的节点是最小的节点
    //            {
    //                m_head->Left() = fixNode->IsNull() ? fixNodeParent : fixNode->Min();
    //            }
    //            if (m_head->Right() == erasedNode) // 删除的节点是最大的节点
    //            {
    //                m_head->Right() = fixNode->IsNull() ? fixNodeParent : fixNode->Max();
    //            }
    //        }
    //        else // 两个子树 curNode是最终删除的节点
    //        {
    //            curNode->SetLeft(erasedNode->Left());
    //
    //            if (curNode == erasedNode->Right())
    //            {
    //                fixNodeParent = curNode;
    //            }
    //            else
    //            {
    //                fixNodeParent = curNode->Parent();
    //
    //                fixNodeParent->SetLeft(fixNode->Left());
    //                curNode->SetLeft(erasedNode->Right());
    //            }
    //
    //            if (m_head->Parent() == erasedNode)
    //            {
    //                m_head->Parent() = curNode;
    //            }
    //            else if (erasedNode->Parent()->Left() == erasedNode)
    //            {
    //                erasedNode->Parent()->Left() = curNode;
    //            }
    //            else
    //            {
    //                erasedNode->Parent()->Right() = curNode;
    //            }
    //            curNode->Parent() = erasedNode->Parent();
    //            std::swap(curNode->IsRed(), erasedNode->IsRed());
    //        }
    //
    //        if (!erasedNode->IsRed())
    //        {
    //            for (; fixNode != m_head->Parent() && !fixNode->IsRed(); fixNodeParent = fixNode->Parent())
    //            {
    //                if (fixNode == fixNodeParent->Left())
    //                {
    //                    curNode = fixNodeParent->Right();
    //                    if (curNode->IsRed())
    //                    {
    //                        curNode->SetBlack();
    //                        fixNodeParent->SetRed();
    //                        fixNodeParent->Lrotate(m_head);
    //                        curNode = fixNodeParent->Right();
    //                    }
    //
    //                    if (curNode->IsNull())
    //                    {
    //                        fixNode = fixNodeParent;
    //                    }
    //                    else if (!curNode->Left()->IsRed() && !curNode->Right()->IsRed())
    //                    {
    //                        curNode->SetRed();
    //                        fixNode          = fixNodeParent;
    //                    }
    //                    else
    //                    {
    //                        if (!curNode->Right()->IsRed())
    //                        {
    //                            curNode->Left()->IsRed() = Black;
    //                            curNode->IsRed()         = Red;
    //                            curNode->Rrotate(m_head);
    //                            curNode = fixNodeParent->Right();
    //                        }
    //
    //                        curNode->IsRed()          = fixNodeParent->IsRed();
    //                        fixNodeParent->IsRed()    = Black;
    //                        curNode->Right()->IsRed() = Black;
    //                        fixNodeParent->Lrotate(m_head);
    //                        break;
    //                    }
    //                }
    //                else
    //                {
    //                    curNode = fixNodeParent->Left();
    //                    if (curNode->IsRed())
    //                    {
    //                        curNode->SetBlack();
    //                        fixNodeParent->SetRed();
    //                        fixNodeParent->Rrotate(m_head);
    //                        curNode = fixNodeParent->Left();
    //                    }
    //
    //                    if (curNode->IsNull())
    //                    {
    //                        fixNode = fixNodeParent;
    //                    }
    //                    else if (!curNode->Left()->IsRed() && !curNode->Right()->IsRed())
    //                    {
    //                        curNode->SetRed();
    //                        fixNode          = fixNodeParent;
    //                    }
    //                    else
    //                    {
    //                        if (!curNode->Left()->IsRed())
    //                        {
    //                            curNode->Right()->IsRed() = Black;
    //                            curNode->IsRed()          = Red;
    //                            curNode->Lrotate(m_head);
    //                            curNode = fixNodeParent->Left();
    //                        }
    //
    //                        curNode->IsRed()         = fixNodeParent->IsRed();
    //                        fixNodeParent->IsRed()   = Black;
    //                        curNode->Left()->IsRed() = Black;
    //                        fixNodeParent->Rrotate(m_head);
    //                        break;
    //                    }
    //                }
    //            }
    //            fixNode->IsRed() = Black;
    //        }
    //
    //        m_size--;
    //        return erasedNode;
    //    }
    //
    //    constexpr node_pointer InsertNode(node_pointer parent, bool isRight, const node_pointer newNode) DECL_NOEXCEPT
    //    {
    //        ++m_size;
    //        newNode->Parent() = parent;
    //        if (parent == m_head) // root节点
    //        {
    //            m_head->Left()   = newNode;
    //            m_head->Parent() = newNode;
    //            m_head->Right()  = newNode;
    //            newNode->IsRed() = Black;
    //            return newNode;
    //        }
    //
    //        if (isRight)
    //        {
    //            parent->Right() = newNode;          // 插入父节点右边
    //            if (IsMax(parent)) SetMax(newNode); // 如果父节点是max节点时，需要将max节点更新为newNode
    //        }
    //        else
    //        {
    //            parent->Left() = newNode;           // 插入父节点左边
    //            if (IsMin(parent)) SetMin(newNode); // 如果父节点是min节点时，需要将min节点更新为newNode
    //        }
    //
    //        node_pointer curNode = newNode;
    //        for (node_pointer parentNode = curNode->Parent(); parentNode->IsRed(); parentNode = curNode->Parent())
    //        {
    //            node_pointer grandparentNode = parentNode->Parent();
    //            bool parentIsLeft            = parentNode->IsLeft();
    //            node_pointer uncleNode       = parentIsLeft ? grandparentNode->Right() : grandparentNode->Left();
    //            if (uncleNode->IsRed())
    //            {
    //                parentNode->IsRed()      = Black;
    //                uncleNode->SetBlack();
    //                grandparentNode->SetRed();
    //                curNode                  = grandparentNode;
    //                continue;
    //            }
    //
    //            if (parentIsLeft)
    //            {
    //                if (curNode->IsRight())
    //                {
    //                    curNode = parentNode;
    //                    curNode->Lrotate(m_head);
    //                }
    //                parentNode               = curNode->Parent();
    //                grandparentNode          = parentNode->Parent();
    //                parentNode->IsRed()      = Black;
    //                grandparentNode->SetRed();
    //                grandparentNode->Rrotate(m_head);
    //            }
    //            else
    //            {
    //                if (curNode->IsLeft())
    //                {
    //                    curNode = parentNode;
    //                    curNode->Rrotate(m_head);
    //                }
    //                parentNode               = curNode->Parent();
    //                grandparentNode          = parentNode->Parent();
    //                parentNode->IsRed()      = Black;
    //                grandparentNode->SetRed();
    //                grandparentNode->Lrotate(m_head);
    //            }
    //        }
    //
    //        m_head->Parent()->IsRed() = Black;
    //        return newNode;
    //    }
    //
    //    DECL_NODISCARD FindResult FindUpper(const key_type& key) const DECL_NOEXCEPT
    //    {
    //        FindResult result{m_head->Parent(), true, m_head};
    //        node_pointer cur = result.m_parent;
    //        while (!cur->IsNull())
    //        {
    //            result.m_parent = cur;
    //            if (value_comparator_traits::Compare(key, cur->Value()))
    //            {
    //                result.m_isRight = false;
    //                result.m_bound   = cur;
    //                cur              = cur->Left();
    //            }
    //            else
    //            {
    //                result.m_isRight = true;
    //                cur              = cur->Right();
    //            }
    //        }
    //        return result;
    //    }
    //
    //    DECL_NODISCARD FindResult FindLower(const key_type& key) const DECL_NOEXCEPT
    //    {
    //        FindResult result{m_head->Parent(), true, m_head};
    //        node_pointer cur = result.m_parent;
    //        while (!cur->IsNull())
    //        {
    //            result.m_parent = cur;
    //            if (value_comparator_traits::Compare(cur->Value(), key))
    //            {
    //                result.m_isRight = true;
    //                cur              = cur->Right();
    //            }
    //            else
    //            {
    //                result.m_isRight = false;
    //                result.m_bound   = cur;
    //                cur              = cur->Left();
    //            }
    //        }
    //        return result;
    //    }
    //
    //    DECL_NODISCARD FindHintResult FindHint(const node_pointer node, const key_type& key) const DECL_NOEXCEPT
    //    {
    //        if (node->IsNull())
    //        {
    //            if (Root()->IsNull() || value_comparator_traits::Compare(m_head->Right()->Value(), key))
    //            {
    //                return FindHintResult{m_head->Right(), true, false};
    //            }
    //        }
    //        else if (m_head->Left() == node)
    //        {
    //            if (value_comparator_traits::Compare(key, node->Value()))
    //            {
    //                return FindHintResult{node, false, false};
    //            }
    //        }
    //        else if (value_comparator_traits::Compare(key, node->Value()))
    //        {
    //            const auto prev = node->Prev();
    //            if (value_comparator_traits::Compare(prev->Value(), key))
    //            {
    //                return prev->Right()->IsNull() ? FindHintResult{prev, true, false} : FindHintResult{node, false, false};
    //            }
    //        }
    //        else if (value_comparator_traits::Compare(node->Value(), key))
    //        {
    //            const auto next = node->Next();
    //            if (next->IsNull() || value_comparator_traits::Compare(key, next->Value()))
    //            {
    //                return node->Right()->IsNull() ? FindHintResult{node, true, false} : FindHintResult{next, false, false};
    //            }
    //        }
    //        else
    //        {
    //            return FindHintResult{node, true, true};
    //        }
    //
    //        auto result        = FindLower(key);
    //        node_pointer bound = result.m_bound;
    //        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value())) // 重复插入
    //        {
    //            return FindHintResult{bound, false, true};
    //        }
    //
    //        return FindHintResult{result.m_parent, result.m_isRight, false};
    //    }
    //
    //    DECL_NODISCARD node_pointer Find(const key_type& key) const DECL_NOEXCEPT
    //    {
    //        auto result        = FindLower(key);
    //        node_pointer bound = result.m_bound;
    //        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value())) // 重复插入
    //        {
    //            return bound;
    //        }
    //        return m_head;
    //    }
    //
    //    void EraseTree(node_allocator_type& alloc, node_pointer node) DECL_NOEXCEPT
    //    {
    //        while (!node->IsNull())
    //        {
    //            EraseTree(alloc, node->Right());
    //            m_ctrl.m_cont.FreeNode(alloc, std::exchange(node, node->Left()));
    //        }
    //    }
    //
    //    void EraseHead(node_allocator_type& alloc) DECL_NOEXCEPT
    //    {
    //        EraseTree(alloc, m_head->Parent());
    //        m_ctrl.m_cont.FreeNode(alloc, m_head);
    //    }
    //
    // 释放节点所占资源
    constexpr void FreeNode(node_allocator_type& alloc, node_pointer node) DECL_NOEXCEPT
    {
        std::destroy_in_place(node->Left());
        std::destroy_in_place(node->Right());
        std::destroy_in_place(node->Parent());

        // auto offset = std::distance(node, buf.data());

        std::allocator_traits<node_allocator_type>::deallocate(alloc, node, 1);
    }

    // 销毁这个节点
    constexpr void DestroyNode(node_allocator_type& alloc, node_pointer node) DECL_NOEXCEPT
    {
        std::allocator_traits<node_allocator_type>::destroy(alloc, std::addressof(node->Value()));
        FreeNode(alloc, node);
    }

    constexpr void Swap(RBTreeNodeBinaryContainer& other) DECL_NOEXCEPT
    {
    }

private:
    std::string m_buf = std::string(sizeof(node_type), '\0');
    size_type m_size;
};

template <class Traits>
class RBTree
{
public:
    using key_type                = typename Traits::key_type;
    using value_type              = typename Traits::value_type;
    using allocator_type          = typename Traits::allocator_type;
    using allocator_traits        = std::allocator_traits<allocator_type>;
    using size_type               = typename allocator_traits::size_type;
    using difference_type         = typename allocator_traits::difference_type;
    using pointer                 = typename allocator_traits::pointer;
    using const_pointer           = typename allocator_traits::const_pointer;
    using reference               = value_type&;
    using const_reference         = const value_type&;
    using key_compare             = typename Traits::key_comparator;
    using value_compare           = typename Traits::value_compare;
    using value_comparator_traits = ComparatorTraits<value_compare>;

    using node_type             = RBTreeNode<Traits>;
    using node_allocator_type   = typename node_type::node_allocator_type;
    using node_allocator_traits = typename node_type::node_allocator_traits;
    using node_pointer          = typename node_type::node_pointer;

    using rbtree_types = container::TreeContainerTypes<key_type,
                                                       value_type,
                                                       size_type,
                                                       difference_type,
                                                       pointer,
                                                       const_pointer,
                                                       reference,
                                                       const_reference,
                                                       node_type,
                                                       value_compare>;
    template <class Types>
    struct ControlBlock final : public node_allocator_type
    {
        using container_type = RBTreeNodeContainer<Types>;
        container_type m_cont{};
    };

    using control_block_type = ControlBlock<rbtree_types>;
    using container_type     = typename control_block_type::container_type;

    using node_handle = NodeHandle<container_type, allocator_type, NodeHandleBase2>;

    using iterator               = typename container_type::iterator;
    using const_iterator         = typename container_type::const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr RBTree() DECL_NOEXCEPT
    {
        m_ctrl.m_cont.SetHead(m_ctrl.m_cont.GenerateHeadNode(get_node_allocator()));
        m_ctrl.m_cont.SetSize(0);
    }

    template <class ElemConstructTag = container::element_copy_construct_tag>
    constexpr RBTree(const RBTree& other, ElemConstructTag tag = ElemConstructTag{}) DECL_NOEXCEPT
    {
        m_ctrl.m_cont.SetHead(m_ctrl.m_cont.GenerateHeadNode(get_node_allocator()));
        CopyRBTree(other, tag);
    }

    constexpr RBTree(RBTree&& other) DECL_NOEXCEPT
    {
        m_ctrl.m_cont.SetHead(m_ctrl.m_cont.GenerateHeadNode(get_node_allocator()));
        m_ctrl.m_cont.Swap(other.m_ctrl.m_cont);
    }

    DECL_CONSTEXPR20 ~RBTree() DECL_NOEXCEPT
    {
        m_ctrl.m_cont.EraseHead(get_node_allocator());
        m_ctrl.m_cont.SetHead(nullptr);
        m_ctrl.m_cont.InitSize();
    }

    RBTree& operator=(const RBTree& r)
    {
        if (this == std::addressof(r))
            return *this;
        clear();
        CopyRBTree(r, container::element_copy_construct_tag{});
        return *this;
    }

    RBTree& operator=(RBTree&& other)
    {
        if (this == std::addressof(other))
            return *this;
        clear();
        m_ctrl.m_cont.Swap(other.m_ctrl.m_cont);
        return *this;
    }

    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        using key_extractor = RBTreeKeyExtract<key_type, std::remove_cvref_t<Args>...>;
        node_pointer newNode;
        node_pointer parentNode;
        bool isRight;
        if constexpr (key_extractor::is_extractable)
        {
            auto& key          = key_extractor::Extract(std::forward<Args>(args)...);
            auto findResult    = m_ctrl.m_cont.FindLower(key);
            node_pointer bound = findResult.m_bound;

            if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value())) // 重复插入
            {
                return {iterator(bound, &m_ctrl.m_cont), false};
            }
            parentNode = findResult.m_parent;
            isRight    = findResult.m_isRight;
            newNode    = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<Args>(args)...);
        }
        else
        {
            newNode            = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<Args>(args)...);
            auto& key          = RBTreeKeyExtract<key_type, std::remove_cvref_t<decltype(newNode->Value())>>::Extract(newNode->Value());
            auto findResult    = m_ctrl.m_cont.FindLower(key);
            node_pointer bound = findResult.m_bound;
            if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value())) // 重复插入
            {
                return {iterator(bound, &m_ctrl.m_cont), false};
            }
            parentNode = findResult.m_parent;
            isRight    = findResult.m_isRight;
        }

        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");

        return {iterator(m_ctrl.m_cont.InsertNode(parentNode, isRight, newNode), &m_ctrl.m_cont), true};
    }

    template <class Iter, class... Args, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator emplace_hint(Iter pos, Args&&... args)
    {
        using key_extractor = RBTreeKeyExtract<key_type, std::remove_cvref_t<Args>...>;
        node_pointer newNode;
        node_pointer parentNode;
        bool isRight;
        if constexpr (key_extractor::is_extractable)
        {
            auto& key       = key_extractor::Extract(std::forward<Args>(args)...);
            auto findResult = m_ctrl.m_cont.FindHint(pos.m_iter, key);
            if (findResult.m_isDuplicate)
            {
                return iterator(findResult.m_parent, &m_ctrl.m_cont);
            }
            parentNode = findResult.m_parent;
            isRight    = findResult.m_isRight;
            newNode    = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<Args>(args)...);
        }
        else
        {
            newNode         = m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::forward<Args>(args)...);
            auto& key       = RBTreeKeyExtract<key_type, std::remove_cvref_t<decltype(newNode->Value())>>::Extract(newNode->Value());
            auto findResult = m_ctrl.m_cont.FindHint(pos.m_iter, key);

            if (findResult.m_isDuplicate)
            {
                return {end(), false};
            }
            parentNode = findResult.m_parent;
            isRight    = findResult.m_isRight;
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        return iterator(m_ctrl.m_cont.InsertNode(parentNode, isRight, newNode), &m_ctrl.m_cont);
    }

    constexpr void clear() DECL_NOEXCEPT
    {
        m_ctrl.m_cont.EraseTree(get_node_allocator(), m_ctrl.m_cont.Root());
        m_ctrl.m_cont.SetRoot(m_ctrl.m_cont.GetHead());
        m_ctrl.m_cont.SetMax(m_ctrl.m_cont.GetHead());
        m_ctrl.m_cont.SetMin(m_ctrl.m_cont.GetHead());
        m_ctrl.m_cont.InitSize();
    }

    DECL_NODISCARD constexpr bool empty() const DECL_NOEXCEPT { return m_ctrl.m_cont.GetSize() == 0; }

    DECL_NODISCARD constexpr size_type size() const DECL_NOEXCEPT { return m_ctrl.m_cont.GetSize(); }

    DECL_NODISCARD constexpr size_type length() const DECL_NOEXCEPT { return m_ctrl.m_cont.GetSize(); }

    DECL_NODISCARD constexpr size_type count() const DECL_NOEXCEPT { return m_ctrl.m_cont.GetSize(); }

    DECL_NODISCARD constexpr size_type max_size() const DECL_NOEXCEPT { return std::min(static_cast<size_type>(std::numeric_limits<difference_type>::max()), node_allocator_traits::max_size(get_allocator())); }

    DECL_NODISCARD constexpr iterator begin() DECL_NOEXCEPT
    {
        return iterator(m_ctrl.m_cont.Min(), &m_ctrl.m_cont);
    }

    DECL_NODISCARD constexpr const_iterator begin() const DECL_NOEXCEPT
    {
        return const_iterator(m_ctrl.m_cont.Min(), &m_ctrl.m_cont);
    }

    DECL_NODISCARD constexpr iterator end() DECL_NOEXCEPT
    {
        return iterator(m_ctrl.m_cont.GetHead(), &m_ctrl.m_cont);
    }

    DECL_NODISCARD constexpr const_iterator end() const DECL_NOEXCEPT
    {
        return iterator(m_ctrl.m_cont.GetHead(), &m_ctrl.m_cont);
    }

    DECL_NODISCARD reverse_iterator rbegin() DECL_NOEXCEPT
    {
        return reverse_iterator(end());
    }

    DECL_NODISCARD const_reverse_iterator rbegin() const DECL_NOEXCEPT
    {
        return const_reverse_iterator(end());
    }

    DECL_NODISCARD reverse_iterator rend() DECL_NOEXCEPT
    {
        return reverse_iterator(begin());
    }

    DECL_NODISCARD const_reverse_iterator rend() const DECL_NOEXCEPT
    {
        return const_reverse_iterator(begin());
    }

    DECL_NODISCARD const_iterator cbegin() const DECL_NOEXCEPT
    {
        return begin();
    }

    DECL_NODISCARD const_iterator cend() const DECL_NOEXCEPT
    {
        return end();
    }

    DECL_NODISCARD const_reverse_iterator crbegin() const DECL_NOEXCEPT
    {
        return rbegin();
    }

    DECL_NODISCARD const_reverse_iterator crend() const DECL_NOEXCEPT
    {
        return rend();
    }

    DECL_NODISCARD const node_allocator_type& get_node_allocator() const DECL_NOEXCEPT
    {
        return m_ctrl;
    }

    DECL_NODISCARD node_allocator_type& get_node_allocator() DECL_NOEXCEPT
    {
        return m_ctrl;
    }

    DECL_NODISCARD allocator_type get_allocator() const DECL_NOEXCEPT
    {
        return static_cast<allocator_type>(get_node_allocator());
    }

    DECL_NODISCARD allocator_type get_allocator() DECL_NOEXCEPT
    {
        return static_cast<allocator_type>(get_node_allocator());
    }

    DECL_NODISCARD key_compare key_comp() const
    {
        return key_compare{};
    }

    DECL_NODISCARD value_compare value_comp() const
    {
        return value_compare{};
    }

    std::pair<iterator, bool> insert(value_type&& value)
    {
        return emplace(std::move(value));
    }
    std::pair<iterator, bool> insert(const value_type& value)
    {
        return emplace(value);
    }
    template <class Iter, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator insert(Iter pos, value_type&& value)
    {
#ifndef NDEBUG
        ASSERT(pos.m_cont == &m_ctrl.m_cont, "Iterators incompatible");
#endif // ! NDBUG
        return emplace_hint(pos, std::move(value));
    }
    template <class Iter, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator insert(Iter pos, const value_type& value)
    {
#ifndef NDEBUG
        ASSERT(pos.m_cont == &m_ctrl.m_cont, "Iterators incompatible");
#endif // ! NDBUG
        return emplace_hint(pos, value);
    }
    template <class Iter, class ElemConstructTag = container::element_copy_construct_tag, class = std::enable_if_t<std::is_same_v<typename std::iterator_traits<Iter>::value_type, value_type>>>
    void insert(Iter first, Iter last, ElemConstructTag tag = ElemConstructTag{})
    {
        VerifyIterRangeADL(first, last);
        for (; first != last; ++first)
        {
            if constexpr (container::is_element_move_v<ElemConstructTag>)
            {
                emplace(std::move(*first));
            }
            else
            {
                emplace(*first);
            }
        }
    }
    template <class ElemConstructTag = container::element_copy_construct_tag>
    void insert(std::initializer_list<value_type> list, ElemConstructTag tag = ElemConstructTag{})
    {
        insert(list.begin(), list.end(), tag);
    }

    template <class Iter, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator erase(Iter pos)
    {
#ifndef NDEBUG
        ASSERT(pos.m_cont == &m_ctrl.m_cont, "Iterators incompatible");
        ASSERT(!pos.m_iter->IsNull(), "cannot erase end iterator");
#endif // ! NDBUG

        auto next              = std::next(pos);
        node_pointer eraseNode = m_ctrl.m_cont.ExtractNode(pos.m_iter);
        m_ctrl.m_cont.FreeNode(get_node_allocator(), eraseNode);
        return iterator(next.m_iter, &m_ctrl.m_cont);
    }
    template <class Iter, class = std::enable_if_t<std::is_any_of_v<Iter, iterator, const_iterator>>>
    iterator erase(Iter first, Iter last)
    {
        VerifyIterRangeADL(first, last);

        if (begin() == first && last.m_iter->IsNull())
        {
            clear();
            return iterator(last.m_iter, &m_ctrl.m_cont);
        }

        while (first != last)
        {
            erase(first++);
        }
        return iterator(last.m_iter, &m_ctrl.m_cont);
    }
    size_type erase(const key_type& key)
    {
        node_pointer node = m_ctrl.m_cont.Find(key);
        if (node->IsNull())
        {
            return 0;
        }
        else
        {
            erase(iterator(node, &m_ctrl.m_cont));
            return 1;
        }
    }

    DECL_NODISCARD iterator find(const key_type& key) DECL_NOEXCEPT
    {
        return iterator(m_ctrl.m_cont.Find(key), &m_ctrl.m_cont);
    }

    DECL_NODISCARD const_iterator find(const key_type& key) const DECL_NOEXCEPT
    {
        return const_iterator(m_ctrl.m_cont.Find(key), &m_ctrl.m_cont);
    }

    DECL_NODISCARD bool contains(const key_type& key) const DECL_NOEXCEPT
    {
        auto result        = m_ctrl.m_cont.FindLower(key);
        node_pointer bound = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()))
        {
            return true;
        }
        return false;
    }

    DECL_NODISCARD bool count(const key_type& key) const DECL_NOEXCEPT
    {
        auto result        = m_ctrl.m_cont.FindLower(key);
        node_pointer bound = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()))
        {
            return 1;
        }
        return 0;
    }

    DECL_NODISCARD iterator lower_bound(const key_type& key) DECL_NOEXCEPT
    {
        auto result = m_ctrl.m_cont.FindLower(key);
        return iterator(result.m_bound, &m_ctrl.m_cont);
    }

    DECL_NODISCARD const_iterator lower_bound(const key_type& key) const DECL_NOEXCEPT
    {
        auto result = m_ctrl.m_cont.FindLower(key);
        return const_iterator(result.m_bound, &m_ctrl.m_cont);
    }

    DECL_NODISCARD iterator upper_bound(const key_type& key) DECL_NOEXCEPT
    {
        auto result = m_ctrl.m_cont.FindUpper(key);
        return iterator(result.m_bound, &m_ctrl.m_cont);
    }

    DECL_NODISCARD const_iterator upper_bound(const key_type& key) const DECL_NOEXCEPT
    {
        auto result = m_ctrl.m_cont.FindUpper(key);
        return const_iterator(result.m_bound, &m_ctrl.m_cont);
    }

    DECL_NODISCARD std::pair<iterator, iterator> equal_range(const key_type& key) DECL_NOEXCEPT
    {
        return {lower_bound(key), upper_bound(key)};
    }

    DECL_NODISCARD std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const DECL_NOEXCEPT
    {
        return {lower_bound(key), upper_bound(key)};
    }

    void swap(RBTree& other) DECL_NOEXCEPT(std::is_nothrow_swappable_v<key_compare>)
    {
        if (this == std::addressof(other))
            return;

        m_ctrl.m_cont.Swap(other.m_ctrl.m_cont);
    }

    DECL_NODISCARD node_handle extract(const const_iterator pos)
    {
#ifndef NDEBUG
        ASSERT(pos.m_cont == &m_ctrl.m_cont, "Iterators incompatible");
        ASSERT(!pos.m_iter->IsNull(), "cannot erase end iterator");
#endif // ! NDBUG
        node_pointer node = m_ctrl.m_cont.ExtractNode(pos.m_iter);
        return node_handle::Make(node, get_allocator());
    }
    node_handle extract(const key_type& key)
    {
        const const_iterator pos = find(key);
        if (pos == end())
        {
            return node_handle{};
        }

        return extract(pos);
    }

    InsertHandleResult<iterator, node_handle> insert(node_handle&& nh)
    {
        if (nh.empty())
        {
            return InsertHandleResult<iterator, node_handle>(end(), false, {});
        }
#ifndef NDEBUG
        ASSERT(nh.get_allocator() == get_allocator(), "node handle allocator incompatible for insert");
#endif // ! NDBUG
        using key_extractor = RBTreeKeyExtract<key_type, std::pair<const key_type, value_type>>;
        const auto& key     = key_extractor::Extract(nh.get_pointer()->Value());
        auto result         = m_ctrl.m_cont.FindLower(key);
        node_pointer bound  = result.m_bound;
        if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()))
        {
            return InsertHandleResult<iterator, node_handle>(iterator(bound, &m_ctrl.m_cont), false, std::move(nh));
        }

        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        const auto inserted = m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, nh.Release());
        return InsertHandleResult<iterator, node_handle>(iterator(inserted, &m_ctrl.m_cont), true, std::move(nh));
    }
    iterator insert(const const_iterator pos, node_handle&& nh)
    {
        if (nh.empty())
        {
            return end();
        }
#ifndef NDEBUG
        ASSERT(pos.m_cont == &m_ctrl.m_cont, "Iterators incompatible");
        ASSERT(nh.get_allocator() == get_allocator(), "node handle allocator incompatible for insert");
#endif // ! NDBUG
        using key_extractor = RBTreeKeyExtract<key_type, std::pair<const key_type, value_type>>;
        const auto& key     = key_extractor::Extract(nh.get_pointer()->Value());
        auto result         = m_ctrl.m_cont.FindHint(pos.m_iter, key);
        if (result.m_isDuplicate)
        {
            return iterator(result.m_parent, &m_ctrl.m_cont);
        }
        ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
        const auto inserted = m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, nh.Release());
        return iterator(inserted, &m_ctrl.m_cont);
    }

    void merge(RBTree& other)
    {
        if (this == std::addressof(other))
            return;

        const auto head     = m_ctrl.m_cont.GetHead();
        auto first          = other.begin();
        using key_extractor = RBTreeKeyExtract<key_type, std::pair<const key_type, value_type>>;
        while (!first.m_iter->IsNull())
        {
            const auto node = first.m_iter;
            ++first;
            const auto& key = key_extractor::Extract(node->Value());

            auto result        = m_ctrl.m_cont.FindLower(key);
            node_pointer bound = result.m_bound;
            if (!bound->IsNull() && !value_comparator_traits::Compare(key, bound->Value()))
            {
                continue;
            }
            ThrowException(max_size() != m_ctrl.m_cont.GetSize(), "too long");
            const auto extracted = other.m_ctrl.m_cont.ExtractNode(node);
            extracted->Left()    = head;
            extracted->Right()   = head;
            extracted->SetRed();

            m_ctrl.m_cont.InsertNode(result.m_parent, result.m_isRight, extracted);
        }
    }

    void merge(RBTree&& other)
    {
        merge(other);
    }

protected:
    template <class ElemConstructTag>
    DECL_NODISCARD constexpr void CopyRBTree(const RBTree& other, ElemConstructTag tag) DECL_NOEXCEPT
    {
        m_ctrl.m_cont.SetRoot(CopyRBTreeNodes(other.m_ctrl.m_cont.GetHead()->Parent(), tag));
        m_ctrl.m_cont.SetSize(other.m_ctrl.m_cont.GetSize());
        if (m_ctrl.m_cont.Root()->IsNull())
        {
            m_ctrl.m_cont.SetMin(m_ctrl.m_cont.GetHead());
            m_ctrl.m_cont.SetMax(m_ctrl.m_cont.GetHead());
        }
        else
        {
            m_ctrl.m_cont.SetMin(m_ctrl.m_cont.Root()->Min());
            m_ctrl.m_cont.SetMax(m_ctrl.m_cont.Root()->Max());
        }
    }

    template <class ElemConstructTag>
    DECL_NODISCARD constexpr node_pointer CopyRBTreeNodes(node_pointer node, ElemConstructTag tag) DECL_NOEXCEPT
    {
        if (node->IsNull())
        {
            return m_ctrl.m_cont.GetHead();
        }

        node_pointer newNode = ConstructNode(node->Value(), tag);
        node->IsRed() ? newNode->SetRed() : newNode->SetBlack();
        newNode->Left()  = CopyRBTreeNodes(node->Left(), tag);
        newNode->Right() = CopyRBTreeNodes(node->Right(), tag);
        return newNode;
    }

    template <class ElemConstructTag>
    DECL_NODISCARD constexpr node_pointer ConstructNode(value_type& value, ElemConstructTag) DECL_NOEXCEPT
    {
        if constexpr (container::is_element_copy_v<ElemConstructTag>)
        {
            return m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), value);
        }
        else if constexpr (std::is_same_v<key_type, value_type>) // 仅有key的树
        {
            return m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::move(value));
        }
        else
        {
            return m_ctrl.m_cont.GenerateNode(get_node_allocator(), m_ctrl.m_cont.GetHead(), std::move(const_cast<key_type&>(value.first)), std::move(value.second));
        }
    }

protected:
    control_block_type m_ctrl;
};
}
} // namespace screw::data_structure