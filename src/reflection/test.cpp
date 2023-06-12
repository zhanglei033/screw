#include "base/STL.h"
#include "base/StringView.h"
#include "base/Variant.h"
#include "test/VariantTest.h"
#include <cassert>

#define ASSERT_NOT_NOEXCEPT(v)   static_assert(!noexcept(v), "must be not noexcept");
#define ASSERT_NOEXCEPT(v)       static_assert(noexcept(v), "must be noexcept");
#define ASSERT_SAME_TYPE(t1, t2) static_assert(std::is_same_v<t1, t2>, "must be same type");

int main()
{
    VariantTest::run_all_test();
    return 0;
}