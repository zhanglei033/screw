#include "base/STL.h"
#include "base/StaticString.h"
#include "base/StringView.h"
#include "base/Variant.h"
#include "googletest/gtest/gtest.h"
#include "reflection/static/TypeInfo.h"
#include <cassert>
#include <type_traits>

using namespace screw::reflection;
struct Vec
{
    int v1;
    int v2;
};

template <>
struct TypeInfo<Vec> : TypeInfoBase<Vec>
{
    DECL_STATIC_CONSTEXPR auto variables = MakeVariables(
        MakeMemberVariable(&Type::v1, STRT("v1"){}),
        MakeMemberVariable(&Type::v2, STRT("v2"){}));
};

#include <tuple>

int main()
{
    TypeInfo<Vec>::variables.m_size;
    TypeInfo<Vec>::variables.is_contains_v<STRT("v1")>;

    std::tuple_element_t<0, decltype(TypeInfo<Vec>::variables.m_elements)>::traits_type;

     TypeInfo<Vec>::variables.ForEach([](auto ins) {
        std::cout << ins << std::endl;
        //std::cout << ins.GetValue(); << std::endl;
        //Vec v;
        //std::invoke(ins.GetValue(), v) = 10;
    });

    

    std::get<0>(TypeInfo<Vec>::variables.m_elements);

    auto t1 = std::chrono::system_clock::now().time_since_epoch().count();
    //
    // TypeInfo<Vec>::variables.ForEach([](auto&& ins) {
    //     std::cout << ins.name << std::endl;
    // });
    //
    // const auto b1 = TypeInfo<Vec>::variables.Contains<STRT("v1")>();
    // const auto b2 = TypeInfo<Vec>::variables.Contains<STRT("v2")>();

    /*  testing::InitGoogleTest();
      try
      {
          RUN_ALL_TESTS();
      } catch (const std::exception& ex)
      {
          std::cout << ex.what() << std::endl;
      }*/
    return 0;
}