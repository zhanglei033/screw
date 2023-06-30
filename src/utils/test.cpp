#include "algorithms/SHA.h"
#include "base/Assert.h"
#include "base/IteratorBase.h"
#include "base/TypesDef.h"
#include "data structure/RBTree.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
using namespace screw;
using namespace screw::algo;

using namespace data_structure;
using Traits = RBTreeNodeWithValueTraits<int, int, std::less<int>, std::allocator<std::pair<const int, int>>>;

using key_type          = typename Traits::key_type;
using value_type        = typename Traits::value_type;
using allocator_type    = typename Traits::allocator_type;
using allocator_traits  = std::allocator_traits<allocator_type>;
using size_type         = typename allocator_traits::size_type;
using difference_type   = typename allocator_traits::difference_type;
using pointer           = typename allocator_traits::pointer;
using const_pointer     = typename allocator_traits::const_pointer;
using reference         = value_type&;
using const_reference   = const value_type&;
using comparator_type   = typename Traits::comparator_type;
using comparator_traits = ComparatorTraits<comparator_type>;

using node_type             = RBTreeNode<Traits>;
using node_traits           = typename node_type::node_traits;
using node_allocator_type   = typename node_type::node_allocator_type;
using node_allocator_traits = typename node_type::node_allocator_traits;
using node_pointer          = typename node_type::node_pointer;

using rbtree_types = TreeContainerTypes<key_type,
                                        value_type,
                                        size_type,
                                        difference_type,
                                        pointer,
                                        const_pointer,
                                        reference,
                                        const_reference,
                                        node_type,
                                        comparator_type>;
struct ControlBlock final : public node_allocator_type
{
    using container_type = RBTreeNodeContainer<rbtree_types>;
    container_type m_cont{};
};
using control_block_type = ControlBlock;
using container_type     = typename control_block_type::container_type;
using rbtree_type        = RBTree<Traits>;

control_block_type m_ctrl;
DECL_NODISCARD constexpr node_allocator_type& GetAllocator() DECL_NOEXCEPT
{
    return m_ctrl;
}

int main()
{
    rbtree_type rbtree;

    const node_pointer newNode   = node_allocator_type().allocate(1);
    newNode->m_parent            = nullptr;
    node_traits::Parent(newNode) = nullptr;
    std::map<int, int> map;
    for (int i = 0; i < 100; i++)
    {
        rbtree.emplace(i, i);
        map.emplace(i, i);
    }

    auto iter1 = map.upper_bound(100);
    auto iter2 = map.lower_bound(56);

    auto iter3 = rbtree.m_ctrl.m_cont.FindUpper(100);
    auto iter4 = rbtree.m_ctrl.m_cont.FindLower(56);


    for (auto iter = rbtree.begin(); iter != rbtree.end(); iter++)
    {
        std::cout << (*iter).first << ":" << (*iter).second << std::endl;
    }

    return 0;
}