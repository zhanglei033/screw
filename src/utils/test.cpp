#include "algorithms/SHA.h"
#include "base/Assert.h"
#include "base/IteratorBase.h"
#include "base/TypesDef.h"
#include "container/ConcurrentMap.h"
#include "data structure/RBTree.h"
#include "googletest/gtest/gtest.h"
#include "mimalloc-new-delete.h"
#include "mimalloc-override.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <vector>
#include <memory>
using namespace screw;
using namespace screw::algo;

int main()
{
std::shared_ptr<int>;

    testing::InitGoogleTest();
    try
    {
        RUN_ALL_TESTS();
    } catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}