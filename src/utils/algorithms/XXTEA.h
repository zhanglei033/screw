#pragma once
#ifndef _XXTEA_H_
#define _XXTEA_H_
#include "Base64.h"
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
#include "mimalloc-override.h"
namespace screw {
namespace algo {
namespace XXTEA {
#include <string.h>
#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned __int8 uint8_t;
typedef unsigned __int32 uint32_t;
#else
#if defined(__FreeBSD__) && __FreeBSD__ < 5
/* FreeBSD 4 doesn't have stdint.h file */
#include <inttypes.h>
#else
#include <stdint.h>
#endif
#endif

#include <sys/types.h> /* This will likely define BYTE_ORDER */

#ifndef BYTE_ORDER
#if (BSD >= 199103)
#include <machine/endian.h>
#else
#if defined(linux) || defined(__linux__)
#include <endian.h>
#else
#define LITTLE_ENDIAN 1234 /* least-significant byte first (vax, pc) */
#define BIG_ENDIAN    4321 /* most-significant byte first (IBM, net) */
#define PDP_ENDIAN    3412 /* LSB first in word, MSW first in long (pdp)*/

#if defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || defined(vax) || defined(ns32000) || defined(sun386) || defined(MIPSEL) || defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) || defined(__alpha__) || defined(__alpha)
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#if defined(sel) || defined(pyr) || defined(mc68000) || defined(sparc) || defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || defined(MIPSEB) || defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) || defined(apollo) || defined(__convex__) || defined(_CRAY) || defined(__hppa) || defined(__hp9000) || defined(__hp9000s300) || defined(__hp9000s700) || defined(BIT_ZERO_ON_LEFT) || defined(m68k) || defined(__sparc)
#define BYTE_ORDER BIG_ENDIAN
#endif
#endif /* linux */
#endif /* BSD */
#endif /* BYTE_ORDER */

#ifndef BYTE_ORDER
#ifdef __BYTE_ORDER
#if defined(__LITTLE_ENDIAN) && defined(__BIG_ENDIAN)
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif
#ifndef BIG_ENDIAN
#define BIG_ENDIAN __BIG_ENDIAN
#endif
#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define BYTE_ORDER LITTLE_ENDIAN
#else
#define BYTE_ORDER BIG_ENDIAN
#endif
#endif
#endif
#endif

#define MX    (((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z))
#define DELTA 0x9e3779b9

#define FIXED_KEY                                     \
    size_t i;                                         \
    uint8_t fixed_key[16];                            \
    memcpy(fixed_key, key, 16);                       \
    for (i = 0; (i < 16) && (fixed_key[i] != 0); ++i) \
        ;                                             \
    for (++i; i < 16; ++i) fixed_key[i] = 0;

static uint32_t* xxtea_to_uint_array(const uint8_t* data, size_t len, int inc_len, size_t* out_len)
{
    uint32_t* out;
#if !(defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN))
    size_t i;
#endif
    size_t n;

    n = (((len & 3) == 0) ? (len >> 2) : ((len >> 2) + 1));

    if (inc_len)
    {
        out = (uint32_t*)calloc(n + 1, sizeof(uint32_t));
        if (!out) return NULL;
        out[n]   = (uint32_t)len;
        *out_len = n + 1;
    }
    else
    {
        out = (uint32_t*)calloc(n, sizeof(uint32_t));
        if (!out) return NULL;
        *out_len = n;
    }
#if defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN)
    memcpy(out, data, len);
#else
    for (i = 0; i < len; ++i)
    {
        out[i >> 2] |= (uint32_t)data[i] << ((i & 3) << 3);
    }
#endif

    return out;
}

static uint8_t* xxtea_to_ubyte_array(const uint32_t* data, size_t len, int inc_len, size_t* out_len)
{
    uint8_t* out;
#if !(defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN))
    size_t i;
#endif
    size_t m, n;

    n = len << 2;

    if (inc_len)
    {
        m = data[len - 1];
        n -= 4;
        if ((m < n - 3) || (m > n)) return NULL;
        n = m;
    }

    out = (uint8_t*)malloc(n + 1);

#if defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN)
    memcpy(out, data, n);
#else
    for (i = 0; i < n; ++i)
    {
        out[i] = (uint8_t)(data[i >> 2] >> ((i & 3) << 3));
    }
#endif

    out[n]   = '\0';
    *out_len = n;

    return out;
}

static uint32_t* xxtea_uint_encrypt(uint32_t* data, size_t len, uint32_t* key)
{
    uint32_t n = (uint32_t)len - 1;
    uint32_t z = data[n], y, p, q = 6 + 52 / (n + 1), sum = 0, e;

    if (n < 1) return data;

    while (0 < q--)
    {
        sum += DELTA;
        e = sum >> 2 & 3;

        for (p = 0; p < n; p++)
        {
            y = data[p + 1];
            z = data[p] += MX;
        }

        y = data[0];
        z = data[n] += MX;
    }

    return data;
}

static uint32_t* xxtea_uint_decrypt(uint32_t* data, size_t len, uint32_t* key)
{
    uint32_t n = (uint32_t)len - 1;
    uint32_t z, y = data[0], p, q = 6 + 52 / (n + 1), sum = q * DELTA, e;

    if (n < 1) return data;

    while (sum != 0)
    {
        e = sum >> 2 & 3;

        for (p = n; p > 0; p--)
        {
            z = data[p - 1];
            y = data[p] -= MX;
        }

        z = data[n];
        y = data[0] -= MX;
        sum -= DELTA;
    }

    return data;
}

static uint8_t* xxtea_ubyte_encrypt(const uint8_t* data, size_t len, const uint8_t* key, size_t* out_len)
{
    uint8_t* out;
    uint32_t *data_array, *key_array;
    size_t data_len, key_len;

    if (!len) return NULL;

    data_array = xxtea_to_uint_array(data, len, 1, &data_len);
    if (!data_array) return NULL;

    key_array = xxtea_to_uint_array(key, 16, 0, &key_len);
    if (!key_array)
    {
        free(data_array);
        return NULL;
    }

    out = xxtea_to_ubyte_array(xxtea_uint_encrypt(data_array, data_len, key_array), data_len, 0, out_len);

    free(data_array);
    free(key_array);

    return out;
}

static uint8_t* xxtea_ubyte_decrypt(const uint8_t* data, size_t len, const uint8_t* key, size_t* out_len)
{
    uint8_t* out;
    uint32_t *data_array, *key_array;
    size_t data_len, key_len;

    if (!len) return NULL;

    data_array = xxtea_to_uint_array(data, len, 0, &data_len);
    if (!data_array) return NULL;

    key_array = xxtea_to_uint_array(key, 16, 0, &key_len);
    if (!key_array)
    {
        free(data_array);
        return NULL;
    }

    out = xxtea_to_ubyte_array(xxtea_uint_decrypt(data_array, data_len, key_array), data_len, 1, out_len);

    free(data_array);
    free(key_array);

    return out;
}

// public functions

static void* xxtea_encrypt(const void* data, size_t len, const void* key, size_t* out_len)
{
    FIXED_KEY
    return xxtea_ubyte_encrypt((const uint8_t*)data, len, fixed_key, out_len);
}

static void* xxtea_decrypt(const void* data, size_t len, const void* key, size_t* out_len)
{
    FIXED_KEY
    return xxtea_ubyte_decrypt((const uint8_t*)data, len, fixed_key, out_len);
}

template <class InputType,
          class KeyType,
          class OutputType,
          class = std::enable_if_t<std::is_same_v<InputType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<InputType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >,
          class = std::enable_if_t<std::is_same_v<KeyType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<KeyType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<KeyType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
static size_t Encrypt(InputType&& input, KeyType&& key, OutputType& ouput)
{
    uint8 fixedKey[16]{0};
    if constexpr (std::is_same_v<KeyType, const char*>)
    {
        auto keySize = std::char_traits<char>::length(key);
        ::memcpy(fixedKey, key, keySize > 16 ? 16 : keySize);
    }
    else
    {
        using pointer_type = decltype(key.data());
        using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        ::memcpy(fixedKey, key.data(), key.size() * sizeof(value_type) > 16 ? 16 : key.size() * sizeof(value_type));
    }
    if constexpr (std::is_same_v<InputType, const char*>)
    {
        size_t encrypt_data_len = 0;
        uint8* encrypt_data     = xxtea_encrypt(input, std::char_traits<char>::length(key), fixedKey, &encrypt_data_len);
        if (!encrypt_data) return 0;

        size_t base64_data_len = Base64::Base64Encode(encrypt_data, encrypt_data_len, ouput);
        free(encrypt_data);
        return base64_data_len;
    }
    else
    {
        using pointer_type      = decltype(input.data());
        using value_type        = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        size_t encrypt_data_len = 0;
        uint8* encrypt_data     = xxtea_encrypt(input.data(), input.size() * sizeof(value_type), fixedKey, &encrypt_data_len);
        if (!encrypt_data) return 0;
        size_t base64_data_len = Base64::Base64Encode(encrypt_data, encrypt_data_len, ouput);
        free(encrypt_data);
        return base64_data_len;
    }
}

template <class T,
          size_t N,
          class KeyType,
          class OutputType,
          class = std::enable_if_t<std::is_same_v<KeyType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<KeyType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<KeyType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
static size_t Encrypt(const T (&input)[N], KeyType&& key, OutputType& ouput)
{
    uint8 fixedKey[16]{0};
    if constexpr (std::is_same_v<KeyType, const char*>)
    {
        auto keySize = std::char_traits<char>::length(key);
        ::memcpy(fixedKey, key, keySize > 16 ? 16 : keySize);
    }
    else
    {
        using pointer_type = decltype(key.data());
        using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        ::memcpy(fixedKey, key.data(), key.size() * sizeof(value_type) > 16 ? 16 : key.size() * sizeof(value_type));
    }

    size_t encrypt_data_len = 0;
    uint8* encrypt_data     = xxtea_encrypt(input, N * sizeof(T), fixedKey, &encrypt_data_len);
    if (!encrypt_data) return 0;

    size_t base64_data_len = Base64::Base64Encode(encrypt_data, encrypt_data_len, ouput);
    free(encrypt_data);
    return base64_data_len;
}

template <class InputType,
          class T,
          size_t N,
          class OutputType,
          class = std::enable_if_t<std::is_same_v<InputType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<InputType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                   >,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
static size_t Encrypt(InputType&& input, const T (&key)[N], OutputType& ouput)
{
    uint8 fixedKey[16]{0};
    ::memcpy(fixedKey, key, N * sizeof(T) > 16 ? 16 : N * sizeof(T));

    if constexpr (std::is_same_v<InputType, const char*>)
    {
        size_t encrypt_data_len = 0;
        uint8* encrypt_data     = xxtea_encrypt(input, std::char_traits<char>::length(key), fixedKey, &encrypt_data_len);
        if (!encrypt_data) return 0;

        size_t base64_data_len = Base64::Base64Encode(encrypt_data, encrypt_data_len, ouput);
        free(encrypt_data);
        return base64_data_len;
    }
    else
    {
        using pointer_type      = decltype(input.data());
        using value_type        = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
        size_t encrypt_data_len = 0;
        uint8* encrypt_data     = xxtea_encrypt(input.data(), input.size() * sizeof(value_type), fixedKey, &encrypt_data_len);
        if (!encrypt_data) return 0;
        size_t base64_data_len = Base64::Base64Encode(encrypt_data, encrypt_data_len, ouput);
        free(encrypt_data);
        return base64_data_len;
    }
}

template <class T1,
          size_t N1,
          class T2,
          size_t N2,
          class OutputType,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // 将指针移动到容器内部，函数内申请的内存无需释放
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   std::is_constructible_v<OutputType, const char*, size_t>     // 重新拷贝一份数据到容器内部，函数内申请的内存需要释放
                                   >>
static size_t Encrypt(const T1 (&input)[N1], const T2 (&key)[N2], OutputType& ouput)
{
    uint8 fixedKey[16]{0};
    ::memcpy(fixedKey, key, N * sizeof(T) > 16 ? 16 : N * sizeof(T));

    size_t encrypt_data_len = 0;
    uint8* encrypt_data     = xxtea_encrypt(input, N * sizeof(T), fixedKey, &encrypt_data_len);
    if (!encrypt_data) return 0;

    size_t base64_data_len = Base64::Base64Encode(encrypt_data, encrypt_data_len, ouput);
    free(encrypt_data);
    return base64_data_len;
}

}}}    // namespace screw::algo::XXTEA
#endif // !_XXTEA_H_
