
#include "Assert.h"
#include "IteratorBase.h"
//#include "USRefl_99.h"
//#include "reflection/StaticReflection.h"
//#include "reflection/StaticString.h"
#include "rttr/registration.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

// using namespace screw::srefl;
using namespace rttr;
struct MyStruct
{
    MyStruct(){};
    void func(double){};
    int data;
};

static void rttr_auto_register_reflection_function_();
namespace {
struct rttr__auto__register__
{
    rttr__auto__register__()
    {
        rttr_auto_register_reflection_function_();
    }
};
} // namespace
static const rttr__auto__register__ auto_register__33;
static void rttr_auto_register_reflection_function_()
{
    registration::class_<MyStruct>("MyStruct")
        .constructor<>()
        .property("data", &MyStruct::data)
        .method("func", &MyStruct::func);
}

int main()
{
    type t = type::get<MyStruct>();
    return 0;
}