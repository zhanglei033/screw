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

static void GetSHA1CodeImpl(const char* inputData, size_t inputSize, uint32 codes[5]) DECL_NOEXCEPT;

// OutputType: �������ָ�루ָ����Ҫ�ⲿ��������ڴ棩������֧��OutputType(const char*, size_t)�Ĺ��췽��
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

// InputType: �������char*(��Ϊ��Ҫ���㳤��,�������һ���ֽ���Ҫ��'\0'),���ߺ���data()��size()�����ߴ��ڿɷ��ʵ�data()��size()��Ա��������ȡָ���ַ�ʹ�С
// OutputType: �������ָ�루ָ����Ҫ�ⲿ��������ڴ棩,����֧��OutputType(const char*, size_t)�Ĺ��췽��
template <class InputType,
          class OutputType,
          class = std::enable_if_t<std::is_same_v<InputType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<InputType>().data())> || // ����Ƿ��пɷ��ʵ�data()���ҷ���ֵ��ָ��
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // ����Ƿ��пɷ��ʵ�size()���ҷ���ֵ������
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

static void GetSHA256CodeImpl(const char* inputData, size_t inputSize, uint32 codes[8]) DECL_NOEXCEPT;

// OutputType: �������ָ�루ָ����Ҫ�ⲿ��������ڴ棩������֧��OutputType(const char*, size_t)�Ĺ��췽��
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

// InputType: �������char*(��Ϊ��Ҫ���㳤��,�������һ���ֽ���Ҫ��'\0'),���ߺ���data()��size()�����ߴ��ڿɷ��ʵ�data()��size()��Ա��������ȡָ���ַ�ʹ�С
// OutputType: �������ָ�루ָ����Ҫ�ⲿ��������ڴ棩,����֧��OutputType(const char*, size_t)�Ĺ��췽��
template <class InputType,
          class OutputType,
          class = std::enable_if_t<std::is_same_v<InputType, const char*> ||                        //
                                   std::is_pointer_v<decltype(std::declval<InputType>().data())> || // ����Ƿ��пɷ��ʵ�data()���ҷ���ֵ��ָ��
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // ����Ƿ��пɷ��ʵ�size()���ҷ���ֵ������
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
