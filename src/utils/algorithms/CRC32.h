#pragma once
#ifndef _CRC32_H_
#define _CRC32_H_
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
namespace screw {
namespace algo {
namespace CRC32 {
DECL_NODISCARD static uint32 GetCRC32Code(const uint8* inputData, size_t inputSize) DECL_NOEXCEPT;

template <class InputType,
          class = std::enable_if_t<std::is_pointer_v<decltype(std::declval<InputType>().data())> || // ����Ƿ��пɷ��ʵ�data()���ҷ���ֵ��ָ��
                                   std::is_integral_v<decltype(std::declval<InputType>().size())>   // ����Ƿ��пɷ��ʵ�size()���ҷ���ֵ������
                                   >>
DECL_NODISCARD static uint32 GetCRC32Code(InputType&& input) DECL_NOEXCEPT
{
    return GetCRC32Code(reinterpret_cast<uint8*>(input.data()), input.size());
}

template <class T, size_t N>
DECL_NODISCARD static uint32 GetCRC32Code(const T (&input)[N]) DECL_NOEXCEPT
{
    return GetCRC32Code(reinterpret_cast<uint8*>(input), N * sizeof(T));
}
}}}    // namespace screw::algo::CRC32
#endif // !_CRC32_H_
