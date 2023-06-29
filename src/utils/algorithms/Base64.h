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
// encode ��Ҫ�ڵ���ǰ�ͷ����BASE64_ENCODE_OUT_SIZE(inputSize) + 1���ֽڵ��ڴ�
DECL_NODISCARD static size_t Base64EncodeImpl(const uint8* inputData, size_t inputSize, char* encode) DECL_NOEXCEPT;

// OutputType: ָ�루ָ������ڲ������ڴ���䣩
// ����֧��OutputType(const char*, size_t)��OutputType(char*&&, size_t)�Ĺ��췽��
// ����֧��OutputType(const uint8*, size_t)��OutputType(uint8*&&, size_t)�Ĺ��췽��
template <class OutputType,
          class = std::enable_if_t<
              std::is_pointer_v<OutputType> ||                             //
              std::is_constructible_v<OutputType, char*&&, size_t> ||      // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
              std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
              std::is_constructible_v<OutputType, const uint8*, size_t> || // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
              std::is_constructible_v<OutputType, const char*, size_t>     // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
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

// InputType: ���ڿɷ��ʵ�data()��size()��Ա��������ȡָ���ַ�ʹ�С
// OutputType: �������ָ�루ָ����Ҫ�ⲿ��������ڴ棩,����֧��OutputType(const char*, size_t)�Ĺ��췽��
template <class InputType,
          class OutputType,
          class = std::enable_if_t<std::is_pointer_v<decltype(std::declval<InputType>().data())> || // ����Ƿ��пɷ��ʵ�data()���ҷ���ֵ��ָ��
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // ����Ƿ��пɷ��ʵ�size()���ҷ���ֵ������
                                   >,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
                                   std::is_constructible_v<OutputType, const char*, size_t>     // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
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
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
                                   std::is_constructible_v<OutputType, const char*, size_t>     // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
                                   >>
DECL_NODISCARD static size_t Base64Encode(const T (&input)[N], OutputType& output) DECL_NOEXCEPT
{
    return Base64Encode(reinterpret_cast<const uint8*>(input), N * sizeof(T), output);
}

// decode ��Ҫ�ڵ���ǰ�ͷ����BASE64_DECODE_OUT_SIZE(inputSize) + 1���ֽڵ��ڴ�
DECL_NODISCARD static size_t Base64DecodeImpl(const char* inputData, size_t inputSize, uint8* decode) DECL_NOEXCEPT;

// OutputType: ָ�루ָ������ڲ������ڴ���䣩
// ����֧��OutputType(const char*, size_t)��OutputType(char*&&, size_t)�Ĺ��췽��
// ����֧��OutputType(const uint8*, size_t)��OutputType(uint8*&&, size_t)�Ĺ��췽��
template <class OutputType,
          class = std::enable_if_t<
              std::is_pointer_v<OutputType> ||                             //
              std::is_constructible_v<OutputType, char*&&, size_t> ||      // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
              std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
              std::is_constructible_v<OutputType, const uint8*, size_t> || // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
              std::is_constructible_v<OutputType, const char*, size_t>     // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
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

// InputType: ���ڿɷ��ʵ�data()��size()��Ա��������ȡָ���ַ�ʹ�С
// OutputType: �������ָ�루ָ����Ҫ�ⲿ��������ڴ棩,����֧��OutputType(const char*, size_t)�Ĺ��췽��
template <class InputType,
          class OutputType,
          class = std::enable_if_t<std::is_pointer_v<decltype(std::declval<InputType>().data())> || // ����Ƿ��пɷ��ʵ�data()���ҷ���ֵ��ָ��
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // ����Ƿ��пɷ��ʵ�size()���ҷ���ֵ������
                                   >,
          class = std::enable_if_t<std::is_pointer_v<OutputType> ||                             //
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
                                   std::is_constructible_v<OutputType, const char*, size_t>     // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
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
                                   std::is_constructible_v<OutputType, char*&&, size_t> ||      // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, uint8*&&, size_t> ||     // ��ָ���ƶ��������ڲ���������������ڴ������ͷ�
                                   std::is_constructible_v<OutputType, const uint8*, size_t> || // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
                                   std::is_constructible_v<OutputType, const char*, size_t>     // ���¿���һ�����ݵ������ڲ���������������ڴ���Ҫ�ͷ�
                                   >>
DECL_NODISCARD static size_t Base64Decode(const T (&input)[N], OutputType& output) DECL_NOEXCEPT
{
    return Base64Decode(reinterpret_cast<const char*>(input), N * sizeof(T), output);
}
}}}    // namespace screw::algo::Base64
#endif // !_BASE64_H_
