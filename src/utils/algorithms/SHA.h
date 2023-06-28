#pragma once
#ifndef _SHA_H_
#define _SHA_H_
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
#include <memory>
namespace screw {
namespace algo {
namespace SHA {
#define SHA1_ROTL(a, b)    (((a >> (32 - b)) & (0x7fffffff >> (31 - b))) | (a << b))
#define SHA1_F(B, C, D, t) ((t < 40) ? ((t < 20) ? ((B & C) | ((~B) & D)) : (B ^ C ^ D)) : ((t < 60) ? ((B & C) | (B & D) | (C & D)) : (B ^ C ^ D)))

static void GetSHA1CodeImpl(const char* inputData, size_t inputSize, uint32 codes[5]) DECL_NOEXCEPT
{
    size_t size = inputSize;
    size_t len  = size + ((size % 64 > 56) ? (128 - size % 64) : (64 - size % 64));
    char* pp    = new char[len];
    if (pp == nullptr) return;
    char* pps = pp;

    for (size_t i = 0; i < size; i++) pp[i + 3 - 2 * (i % 4)] = inputData[i];
    pp[size + 3 - 2 * (size % 4)] = 128;
    for (size_t i = size + 1; i < len; i++)
    {
        pp[i + 3 - 2 * (i % 4)] = 0;
    }

    *((uint32*)(pp + len - 4)) = size << 3;
    *((uint32*)(pp + len - 8)) = size >> 29;

    uint32 w[80]{0};
    uint32 h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
    uint32 k[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
    for (char* ppend = pp + len; pp < ppend; pp += 64)
    {
        for (size_t i = 0; i < 16; i++) w[i] = ((uint32*)pp)[i];

        for (size_t i = 16; i < 80; i++) w[i] = SHA1_ROTL((w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]), 1);

        uint32 a = h[0];
        uint32 b = h[1];
        uint32 c = h[2];
        uint32 d = h[3];
        uint32 e = h[4];
        for (size_t i = 0; i < 80; i++)
        {
            uint32 temp = SHA1_ROTL(a, 5) + SHA1_F(b, c, d, i) + e + w[i] + k[i / 20];
            e           = d;
            d           = c;
            c           = SHA1_ROTL(b, 30);
            b           = a;
            a           = temp;
        }
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
    }
    delete pps;
    codes[0] = h[0];
    codes[1] = h[1];
    codes[2] = h[2];
    codes[3] = h[3];
    codes[4] = h[4];
}

// OutputType: 数组或者指针（指针需要外部就申请好内存）、或者支持OutputType(const char*, size_t)的构造方法
template <class OutputType,
          class = std::enable_if_t<std::is_array_v<OutputType> ||                              //
                                   std::is_pointer_v<OutputType> ||                            //
                                   std::is_constructible_v<OutputType, const char*, size_t> || //
                                   std::is_constructible_v<OutputType, const uint8*, size_t>   //
                                   >>
static void GetSHA1Code(const char* inputData, size_t inputSize, OutputType& output) DECL_NOEXCEPT
{
    uint32 codes[5]{0};
    GetSHA1CodeImpl(inputData, inputSize, codes);

    if constexpr (std::is_array_v<OutputType> || std::is_pointer_v<OutputType>)
    {
        ::memcpy(output, codes, 20);
    }
    else if constexpr (std::is_constructible_v<OutputType, const char*, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, (const char*)codes, 20);
    }
    else
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, (const uint8*)codes, 20);
    }
}

// InputType: 数组或者char*(因为需要计算长度,所以最后一个字节需要是'\0'),或者含有data()和size()，或者存在可访问的data()和size()成员函数，获取指针地址和大小
// OutputType: 数组或者指针（指针需要外部就申请好内存）,或者支持OutputType(const char*, size_t)的构造方法
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
static void GetSHA1Code(InputType&& input, OutputType& output) DECL_NOEXCEPT
{
    if constexpr (std::is_same_v<InputType, char*>)
    {
        GetSHA1Code(input, std::char_traits<char>::length(input), output);
    }
    else
    {
        using pointer_type = decltype(input.data());
        using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        GetSHA1Code(reinterpret_cast<const char*>(input.data()), input.size() * sizeof(value_type), output);
    }
}
template <class T,
          size_t N,
          class OutputType,
          class = std::enable_if_t<std::is_array_v<OutputType> || std::is_pointer_v<OutputType> || std::is_constructible_v<OutputType, const char*, size_t>>>
static void GetSHA1Code(const T (&input)[N], OutputType& output) DECL_NOEXCEPT
{
    GetSHA1Code(reinterpret_cast<const char*>(input), sizeof(T) * N, output);
}

#define SHA256_ROTL(a, b)   (((a >> (32 - b)) & (0x7fffffff >> (31 - b))) | (a << b))
#define SHA256_SR(a, b)     ((a >> b) & (0x7fffffff >> (b - 1)))
#define SHA256_Ch(x, y, z)  ((x & y) ^ ((~x) & z))
#define SHA256_Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_E0(x)        (SHA256_ROTL(x, 30) ^ SHA256_ROTL(x, 19) ^ SHA256_ROTL(x, 10))
#define SHA256_E1(x)        (SHA256_ROTL(x, 26) ^ SHA256_ROTL(x, 21) ^ SHA256_ROTL(x, 7))
#define SHA256_O0(x)        (SHA256_ROTL(x, 25) ^ SHA256_ROTL(x, 14) ^ SHA256_SR(x, 3))
#define SHA256_O1(x)        (SHA256_ROTL(x, 15) ^ SHA256_ROTL(x, 13) ^ SHA256_SR(x, 10))

static void GetSHA256CodeImpl(const char* inputData, size_t inputSize, uint32 codes[8]) DECL_NOEXCEPT
{
    size_t size = inputSize;
    size_t len  = size + ((size % 64 > 56) ? (128 - size % 64) : (64 - size % 64));
    char* pp    = new char[len];
    if (!pp) return;
    char* pps = pp;

    for (size_t i = 0; i < size; i++) pp[i + 3 - 2 * (i % 4)] = inputData[i];
    size_t pos = size + 3 - 2 * (size % 4);
    if (pos >= len)
    {
        delete pps;
        return;
    }
    pp[pos] = 128;
    for (size_t i = size + 1; i < len; i++) pp[i + 3 - 2 * (i % 4)] = 0;
    *((uint32*)(pp + len - 4)) = size << 3;
    *((uint32*)(pp + len - 8)) = size >> 29;

    uint32 h[8]{0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
    uint32 k[64]{0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
    uint32 w[64]{0};

    for (char* ppend = pp + len; pp < ppend; pp += 64)
    {
        for (size_t i = 0; i < 16; i++) w[i] = ((uint32*)pp)[i];

        for (size_t i = 16; i < 64; i++) w[i] = (SHA256_O1(w[i - 2]) + w[i - 7] + SHA256_O0(w[i - 15]) + w[i - 16]);

        uint32 h0 = h[0];
        uint32 h1 = h[1];
        uint32 h2 = h[2];
        uint32 h3 = h[3];
        uint32 h4 = h[4];
        uint32 h5 = h[5];
        uint32 h6 = h[6];
        uint32 h7 = h[7];

        for (size_t i = 0; i < 64; i++)
        {
            uint32 temp1 = h7 + SHA256_E1(h4) + SHA256_Ch(h4, h5, h6) + k[i] + w[i];
            uint32 temp2 = SHA256_E0(h0) + SHA256_Maj(h0, h1, h2);
            h7 = h6, h6 = h5, h5 = h4, h4 = h3 + temp1;
            h3 = h2, h2 = h1, h1 = h0, h0 = temp1 + temp2;
        }
        h[0] += h0, h[1] += h1, h[2] += h2, h[3] += h3;
        h[4] += h4, h[5] += h5, h[6] += h6, h[7] += h7;
    }
    delete[] pps;
    codes[0] = h[0];
    codes[1] = h[1];
    codes[2] = h[2];
    codes[3] = h[3];
    codes[4] = h[4];
    codes[5] = h[5];
    codes[6] = h[6];
    codes[7] = h[7];
}

// OutputType: 数组或者指针（指针需要外部就申请好内存）、或者支持OutputType(const char*, size_t)的构造方法
template <class OutputType,
          class = std::enable_if_t<std::is_array_v<OutputType> ||                              //
                                   std::is_pointer_v<OutputType> ||                            //
                                   std::is_constructible_v<OutputType, const char*, size_t> || //
                                   std::is_constructible_v<OutputType, const uint8*, size_t>   //
                                   >>
static void GetSHA256Code(const char* inputData, size_t inputSize, OutputType& output) DECL_NOEXCEPT
{
    uint32 codes[8]{0};
    GetSHA256CodeImpl(inputData, inputSize, codes);
    if constexpr (std::is_array_v<OutputType> || std::is_pointer_v<OutputType>)
    {
        ::memcpy(output, codes, 32);
    }
    else if constexpr (std::is_constructible_v<OutputType, const char*, size_t>)
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, (const char*)codes, 32);
    }
    else
    {
        std::destroy_in_place(output);
        std::construct_in_place(output, (const uint8*)codes, 32);
    }
}

// InputType: 数组或者char*(因为需要计算长度,所以最后一个字节需要是'\0'),或者含有data()和size()，或者存在可访问的data()和size()成员函数，获取指针地址和大小
// OutputType: 数组或者指针（指针需要外部就申请好内存）,或者支持OutputType(const char*, size_t)的构造方法
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
static void GetSHA256Code(InputType&& input, OutputType& output) DECL_NOEXCEPT
{
    if constexpr (std::is_same_v<InputType, char*>)
    {
        GetSHA256Code(input, std::char_traits<char>::length(input), output);
    }
    else
    {
        using pointer_type = decltype(input.data());
        using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        return GetSHA256Code(reinterpret_cast<const char*>(input.data()), input.size() * sizeof(value_type), output);
    }
}
template <class T,
          size_t N,
          class OutputType,
          class = std::enable_if_t<std::is_array_v<OutputType> ||                              //
                                   std::is_pointer_v<OutputType> ||                            //
                                   std::is_constructible_v<OutputType, const char*, size_t> || //
                                   std::is_constructible_v<OutputType, const uint8*, size_t>   //
                                   >>
static void GetSHA256Code(const T (&input)[N], OutputType& output) DECL_NOEXCEPT
{
    GetSHA256Code(reinterpret_cast<const char*>(input), sizeof(T) * N, output);
}
}}}    // namespace screw::algo::SHA
#endif // !_SHA_H_
