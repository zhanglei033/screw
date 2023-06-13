#include "base/STL.h"
#include "base/StringView.h"
#include "base/Variant.h"
#include "googletest/gtest/gtest.h"
#include <cassert>
int main()
{
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