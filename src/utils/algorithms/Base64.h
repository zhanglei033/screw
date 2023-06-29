#pragma once
#ifndef _BASE64_H_
#define _BASE64_H_
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
namespace screw {
namespace algo {
namespace Base64 {
#define BASE64_ENCODE_OUT_SIZE(s) (((s) + 2) / 3 * 4)
#define BASE64_DECODE_OUT_SIZE(s) (((s)) / 4 * 3)
// encode 需要在调用前就分配好BASE64_ENCODE_OUT_SIZE(inputSize) + 1个字节的内存
DECL_NODISCARD static size_t Base64EncodeImpl(const uint8* inputData, size_t inputSize, char* encode) DECL_NOEXCEPT;

// OutputType: 指针（指针会再内部进行内存分配）
// 或者支持OutputType(const char*, size_t)或OutputType(char*&&, size_t)的构造方法
// 或者支持OutputType(const uint8*, size_t)或OutputType(uint8*&&, size_t)的构造方法
template <class OutputType,
          class = std::enable_if_t<
              std::is_pointer_v<OutputType> ||                             //
              std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
              std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
              std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
              std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
              >>
DECL_NODISCARD static size_t Base64Encode(const uint8* inputData, size_t inputSize, OutputType& output) DECL_NOEXCEPT
{
    char* encode = new char[BASE64_ENCODE_OUT_SIZE(inputSize) + 1]{0};
    auto size    = Base64EncodeImpl(inputData, inputSize, encode);
    if constexpr (std::is_pointer_v<OutputType>)
    {
        output = encode;
        return size;
    }
    else if constexpr (std::is_constructible_v<OutputType, char*&&, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, std::move(encode), size);
    }
    else if constexpr (std::is_constructible_v<OutputType, uint8*&&, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, std::move(reinterpret_cast<uint8*>(encode)), size);
    }
    else if constexpr (std::is_constructible_v<OutputType, const char*, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, encode, size);
        delete encode;
    }
    else
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, reinterpret_cast<uint8*>(encode), size);
        delete encode;
    }
}

// InputType: 存在可访问的data()和size()成员函数，获取指针地址和大小
// OutputType: 数组或者指针（指针需要外部就申请好内存）,或者支持OutputType(const char*, size_t)的构造方法
template <class InputType,
          class OutputType,
          class = std::enable_if_t<std::is_pointer_v<decltype(std::declval<InputType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
DECL_NODISCARD static size_t Base64Encode(InputType&& input, OutputType& output) DECL_NOEXCEPT
{
    using pointer_type = decltype(input.data());
    using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
    return Base64Encode(reinterpret_cast<const uint8*>(input.data()), input.size() * sizeof(value_type), output);
}

template <class T,
          size_t N,
          class OutputType,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
DECL_NODISCARD static size_t Base64Encode(const T (&input)[N], OutputType& output) DECL_NOEXCEPT
{
    return Base64Encode(reinterpret_cast<const uint8*>(input), N * sizeof(T), output);
}

// decode 需要在调用前就分配好BASE64_DECODE_OUT_SIZE(inputSize) + 1个字节的内存
DECL_NODISCARD static size_t Base64DecodeImpl(const char* inputData, size_t inputSize, uint8* decode) DECL_NOEXCEPT;

// OutputType: 指针（指针会再内部进行内存分配）
// 或者支持OutputType(const char*, size_t)或OutputType(char*&&, size_t)的构造方法
// 或者支持OutputType(const uint8*, size_t)或OutputType(uint8*&&, size_t)的构造方法
template <class OutputType,
          class = std::enable_if_t<
              std::is_pointer_v<OutputType> ||                             //
              std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
              std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
              std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
              std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
              >>
DECL_NODISCARD static size_t Base64Decode(const char* inputData, size_t inputSize, OutputType& output) DECL_NOEXCEPT
{
    uint8* decode = new uint8[BASE64_ENCODE_OUT_SIZE(inputSize) + 1]{0};
    auto size     = Base64DecodeImpl(inputData, inputSize, decode);
    if constexpr (std::is_pointer_v<OutputType>)
    {
        output = decode;
        return size;
    }
    else if constexpr (std::is_constructible_v<OutputType, uint8*&&, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, std::move(decode), size);
        return size;
    }
    else if constexpr (std::is_constructible_v<OutputType, char*&&, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, std::move(reinterpret_cast<char*>(decode)), size);
        return size;
    }
    else if constexpr (std::is_constructible_v<OutputType, const uint8*, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, decode, size);
        delete decode;
        return size;
    }
    else
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, reinterpret_cast<char*>(decode), size);
        delete decode;
        return size;
    }
}

// InputType: 存在可访问的data()和size()成员函数，获取指针地址和大小
// OutputType: 数组或者指针（指针需要外部就申请好内存）,或者支持OutputType(const char*, size_t)的构造方法
template <class InputType,
          class OutputType,
          class = std::enable_if_t<std::is_pointer_v<decltype(std::declval<InputType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
DECL_NODISCARD static size_t Base64Decode(InputType&& input, OutputType& output) DECL_NOEXCEPT
{
    using pointer_type = decltype(input.data());
    using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
    return Base64Decode(reinterpret_cast<const uint8*>(input.data()), input.size() * sizeof(value_type), output);
}

template <class T,
          size_t N,
          class OutputType,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
DECL_NODISCARD static size_t Base64Decode(const T (&input)[N], OutputType& output) DECL_NOEXCEPT
{
    return Base64Decode(reinterpret_cast<const char*>(input), N * sizeof(T), output);
}
}}}    // namespace screw::algo::Base64
#endif // !_BASE64_H_
