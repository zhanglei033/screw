#include "base/STL.h"
#include "base/StaticString.h"
#include "base/StringView.h"
#include "base/Variant.h"
#include "googletest/gtest/gtest.h"
#include "reflection/Element.h"
#include "reflection/TypeInfo.h"
#include <cassert>
#include <type_traits>

using namespace screw::reflection;
using namespace screw;
struct Vec
{
    int v1;
    int v2;
    static int v3;
    static const int v4;
    static constexpr int v5 = 8;

    Vec() = default;
    Vec(int a, int b)
        : v1(a), v2(b) {}

    void TestFunc1(int a1, int a2)
    {
    }
};
int Vec::v3       = 6;
const int Vec::v4 = 7;

template <>
struct Type<STRT("Vec")> : Type<Vec>
{
};
template <>
struct TypeInfo<Vec> : TypeInfoBase<Vec>
{
    DECL_STATIC_CONSTEXPR auto propertys = MakePropertys(
        MakeProperty<STRT("Serizable"), bool, true>());
    DECL_STATIC_CONSTEXPR auto members = MakeMembers(
        MakeMember<STRT("v1"), decltype(&Vec::v1), &Vec::v1>(), // v1
        MakeMember<STRT("v2"), decltype(&Vec::v2), &Vec::v2>(), // v2
        MakeMember<STRT("v3"), decltype(&Vec::v3), &Vec::v3>(), // v3
        MakeMember<STRT("v4"), decltype(&Vec::v4), &Vec::v4>(
            MakeProperty<STRT("Serizable"), bool, false>()), // v4
        MakeMember<STRT("v5"), decltype(&Vec::v5), &Vec::v5>(
            MakeProperty<STRT("Serizable"), bool, false>()),                        // v5
        MakeMember<STRT("TestFunc1"), decltype(&Vec::TestFunc1), &Vec::TestFunc1>() // TestFunc1
    );
};

struct ClassA
{
    int a = 1;
};
struct ClassB : ClassA
{
    int b = 2;
};
struct ClassC : ClassA
{
    int c = 3;
};
struct ClassD : ClassB, ClassC
{
    int d = 4;
};

template <>
struct TypeInfo<ClassA> : TypeInfoBase<ClassA>
{
    DECL_STATIC_CONSTEXPR auto propertys = MakePropertys();
    DECL_STATIC_CONSTEXPR auto members   = MakeMembers(MakeMember<STRT("a"), decltype(&ClassA::a), &ClassA::a>());
};

template <>
struct TypeInfo<ClassB> : TypeInfoBase<ClassB, ClassA>
{
    DECL_STATIC_CONSTEXPR auto propertys = MakePropertys();
    DECL_STATIC_CONSTEXPR auto members   = MakeMembers(MakeMember<STRT("b"), decltype(&ClassB::b), &ClassB::b>());
};

template <>
struct TypeInfo<ClassC> : TypeInfoBase<ClassC, ClassA>
{
    DECL_STATIC_CONSTEXPR auto propertys = MakePropertys();
    DECL_STATIC_CONSTEXPR auto members   = MakeMembers(MakeMember<STRT("c"), decltype(&ClassC::c), &ClassC::c>());
};

template <>
struct TypeInfo<ClassD> : TypeInfoBase<ClassD, ClassB, ClassC>
{
    DECL_STATIC_CONSTEXPR auto propertys = MakePropertys();
    DECL_STATIC_CONSTEXPR auto members   = MakeMembers(MakeMember<STRT("d"), decltype(&ClassD::d), &ClassD::d>());
};

struct Func
{
    template <class Element>
    constexpr void operator()(Element&& elem) const
    {
        using element_info = ElementInfo<Element>;
        using type_info    = typename element_info::type_info;

        std::cout << "type:" << type_info::type_name << ","
                  << "type_id:" << type_info::type_id << ","
                  << "name:" << elem.Name() << std::endl;
    }

    template <class Element, class ValueType>
    constexpr void operator()(Element&& elem, ValueType&& value) const
    {
        using element_info = ElementInfo<Element>;
        using type_info    = typename element_info::type_info;

        if constexpr (!element_info::is_member_const_v)
        {
            value += 1;
        }

        std::cout << "type:" << type_info::type_name << ","
                  << "type_id:" << type_info::type_id << ","
                  << "name:" << elem.Name() << ","
                  << "value:" << value << std::endl;
    }
};

int main()
{
    auto vv = GetByName<STRT("Vec")>(10, 20);

    Vec v;
    auto func = &Vec::TestFunc1;
    v.v1      = 10;
    v.v2      = 20;
    TypeInfo<Vec>::ForEachElement(Func{});
    TypeInfo<Vec>::ForEachMemberVar(v, Func{});

    constexpr auto elem_v1     = TypeInfo<Vec>::GetMember(STRT("v1"){});
    auto v1                    = elem_v1.Invoke(v);
    constexpr auto elem_v1_pro = elem_v1.GetProperty(STRT("Serizable"){}).Invoke();

    auto vv1           = Get<STRT("v1")>(v);
    constexpr auto vv5 = Get<STRT("v5")>(v);

    elem_v1.Invoke(v)          = 10;
    constexpr auto elem_v5     = TypeInfo<Vec>::GetMember(STRT("v5"){});
    constexpr auto v5          = elem_v5.Invoke();
    constexpr auto elem_v5_pro = elem_v5.GetProperty(STRT("Serizable"){}).Invoke();

    constexpr auto elem_TestFunc1 = TypeInfo<Vec>::GetMember(STRT("TestFunc1"){});
    elem_TestFunc1.Invoke(v, 10, 20);
    ClassD d;
    TypeInfo<ClassD>::ForEachElement(Func{});
    TypeInfo<ClassD>::ForEachMemberVar(d, Func{});

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