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
// public functions
static void* xxtea_encrypt(const void* data, size_t len, const void* key, size_t* out_len) DECL_NOEXCEPT;

static void* xxtea_decrypt(const void* data, size_t len, const void* key, size_t* out_len) DECL_NOEXCEPT;

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
DECL_NODISCARD static size_t Encrypt(InputType&& input, KeyType&& key, OutputType& ouput) DECL_NOEXCEPT
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
DECL_NODISCARD static size_t Encrypt(const T (&input)[N], KeyType&& key, OutputType& ouput)
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
DECL_NODISCARD static size_t Encrypt(InputType&& input, const T (&key)[N], OutputType& ouput)
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
DECL_NODISCARD static size_t Encrypt(const T1 (&input)[N1], const T2 (&key)[N2], OutputType& ouput)
{
    uint8 fixedKey[16]{0};
    ::memcpy(fixedKey, key, N2 * sizeof(T2) > 16 ? 16 : N2 * sizeof(T2));

    size_t encrypt_data_len = 0;
    uint8* encrypt_data     = xxtea_encrypt(input, N1 * sizeof(T1), fixedKey, &encrypt_data_len);
    if (!encrypt_data) return 0;

    size_t base64_data_len = Base64::Base64Encode(encrypt_data, encrypt_data_len, ouput);
    free(encrypt_data);
    return base64_data_len;
}

}}}    // namespace screw::algo::XXTEA
#endif // !_XXTEA_H_
