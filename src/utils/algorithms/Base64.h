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
#define BASE64_PAD                '='
#define BASE64DE_FIRST            '+'
#define BASE64DE_LAST             'z'
static constexpr char base64en[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
static constexpr char base64de[] = {62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

// encode 需要在调用前就分配好BASE64_ENCODE_OUT_SIZE(inputSize) + 1个字节的内存
static size_t Base64EncodeImpl(const uint8* inputData, size_t inputSize, char* encode)
{
    size_t curIndex = 0;
    for (size_t i = 0; i < inputSize; i++)
    {
        uint8 s = i % 3;
        switch (s)
        {
        case 0: encode[curIndex++] = base64en[(inputData[i] >> 2) & 0x3F]; break;
        case 1: encode[curIndex++] = base64en[((inputData[i - 1] & 0x3) << 4) + ((inputData[i] >> 4) & 0xF)]; break;
        case 2:
            encode[curIndex++] = base64en[((inputData[i - 1] & 0xF) << 2) + ((inputData[i] >> 6) & 0x3)];
            encode[curIndex++] = base64en[inputData[i] & 0x3F];
            break;
        }
    }

    uint8 near = inputSize % 3;
    if (near == 1)
    {
        encode[curIndex++] = base64en[(inputData[inputSize - 1] & 0x3) << 4];
        encode[curIndex++] = BASE64_PAD;
        encode[curIndex++] = BASE64_PAD;
    }
    else if (near == 2)
    {
        encode[curIndex++] = base64en[(inputData[inputSize - 1] & 0xF) << 2];
        encode[curIndex++] = BASE64_PAD;
    }

    return curIndex;
}

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
static size_t Base64Encode(const uint8* inputData, size_t inputSize, OutputType& output)
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
static size_t Base64Encode(InputType&& input, OutputType& output)
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
static size_t Base64Encode(const T (&input)[N], OutputType& output)
{
    return Base64Encode(reinterpret_cast<const uint8*>(input), N * sizeof(T), output);
}

// decode 需要在调用前就分配好BASE64_DECODE_OUT_SIZE(inputSize) + 1个字节的内存
static size_t Base64DecodeImpl(const char* inputData, size_t inputSize, uint8* decode)
{
    size_t curIndex = 0;
    for (size_t i = 0; i < inputSize; i++)
    {
        uint8 s = i % 4;
        if (inputData[i] == '=')
        {
            ::memset(decode, 0, curIndex);
            return curIndex;
        }

        char c;
        if (inputData[i] < BASE64DE_FIRST || inputData[i] > BASE64DE_LAST || (c = base64de[inputData[i] - BASE64DE_FIRST]) == -1)
        {
            return 0;
        }

        switch (s)
        {
        case 0: decode[curIndex] = ((uint8)c << 2) & 0xFF; break;
        case 1:
            decode[curIndex++] += ((uint8)c >> 4) & 0x03;
            if (i < (inputSize - 3) || inputData[inputSize - 2] != '=')
            {
                decode[curIndex] = ((uint8)c & 0x0F) << 4;
            }
            break;
        case 2:
            decode[curIndex++] += ((uint8)c >> 2) & 0xF;
            if (i < (inputSize - 2) || inputData[inputSize - 1] != '=')
            {
                decode[curIndex] = ((uint8)c & 0x3) << 6;
            }
            break;
        case 3: decode[curIndex++] += c; break;
        }
    }

    return curIndex;
}

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
static size_t Base64Decode(const char* inputData, size_t inputSize, OutputType& output)
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
static size_t Base64Decode(InputType&& input, OutputType& output)
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
static size_t Base64Decode(const T (&input)[N], OutputType& output)
{
    return Base64Decode(reinterpret_cast<const char*>(input), N * sizeof(T), output);
}
}}}    // namespace screw::algo::Base64
#endif // !_BASE64_H_
