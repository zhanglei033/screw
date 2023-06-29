#pragma once
#ifndef _MD5_H_
#define _MD5_H_
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
namespace screw {
namespace algo {
namespace MD5 {
struct MD5State
{
    uint32 state[4]{0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    uint32 count[2]{0};
    uint8 buffer[64]{0};
    uint8 digest[16]{0};
};
static void Update(const uint8* inputData, uint32 inputSize, MD5State& md5State) DECL_NOEXCEPT;

static void Digest(MD5State& md5State) DECL_NOEXCEPT;

template <class OutputType,
          class = std::enable_if_t<std::is_array_v<OutputType> ||                              //
                                   std::is_pointer_v<OutputType> ||                            //
                                   std::is_constructible_v<OutputType, const char*, size_t> || //
                                   std::is_constructible_v<OutputType, const uint8*, size_t>   //
                                   >>
static void GetMD5Code(const uint8* inputData, size_t inputSize, OutputType& output) DECL_NOEXCEPT
{
    MD5State md5State;
    Update(inputData, inputSize, md5State); // 单次处理为2的32次方大小，如果超出再说
    Digest(md5State);

    if constexpr (std::is_array_v<OutputType> || std::is_pointer_v<OutputType>)
    {
        ::memcpy(output, md5State.digest, 16);
    }
    else if constexpr (std::is_constructible_v<OutputType, const char*, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, (const char*)md5State.digest, 16);
    }
    else
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, (const uint8*)md5State.digest, 16);
    }
}

template <class InputType,
          class OutputType,
          class = std::enable_if_t<std::is_same_v<InputType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<InputType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >,
          class = std::enable_if_t<std::is_array_v<OutputType> ||                              //
                                   std::is_pointer_v<OutputType> ||                            //
                                   std::is_constructible_v<OutputType, const char*, size_t> || //
                                   std::is_constructible_v<OutputType, const uint8*, size_t>   //
                                   >>
static void GetMD5Code(InputType&& input, OutputType& output) DECL_NOEXCEPT
{
    if constexpr (std::is_same_v<InputType, char*>)
    {
        GetMD5Code(input, std::char_traits<char>::length(input), output);
    }
    else
    {
        using pointer_type = decltype(input.data());
        using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        GetMD5Code(reinterpret_cast<const char*>(input.data()), input.size() * sizeof(value_type), output);
    }
}
}}}    // namespace screw::algo::MD5
#endif // !_MD5_H_
