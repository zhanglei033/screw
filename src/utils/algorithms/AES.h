#pragma once
#ifndef _AES_H_
#define _AES_H_
#include "SHA.h"
#include "base/Platform.h"
#include "base/STL.h"
#include "base/TypesDef.h"
#include <memory>
namespace screw {
namespace algo {
namespace AES {

typedef uint8 state_t[4][4];
class AESCore
{
public:
    template <class InputKeyType>
    void Init(InputKeyType&& inputKey) DECL_NOEXCEPT
    {
        uint8 key[32]{0};
        SHA::GetSHA256Code(inputKey, key); // 这里使用了sha256将输入的inputKey转化为32字节的key
        KeyExpansion(key);
    }

    template <class BufType,
              class = std::enable_if_t<std::is_same_v<BufType, const char*> ||                        // char* 需要可以计算长度，结尾是'\0'
                                       std::is_pointer_v<decltype(std::declval<BufType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                       std::is_integral_v<decltype(std::declval<BufType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                       >>
    void Encrypt(BufType& buf) DECL_NOEXCEPT
    {
        uint8* data = nullptr;
        size_t size = 0;
        if constexpr (std::is_same_v<BufType, const char*>)
        {
            Encrypt(reinterpret_cast<uint8*>(buf), std::char_traits<char>::length(buf));
        }
        else
        {
            using pointer_type = decltype(buf.data());
            using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
            Encrypt(reinterpret_cast<uint8*>(buf.data()), buf.size() * sizeof(value_type));
        }
    }

    template <class T, size_t N>
    void Encrypt(T (&buf)[N]) DECL_NOEXCEPT
    {
        Encrypt(reinterpret_cast<uint8*>(buf), N * sizeof(T));
    }

    void Encrypt(uint8* data, size_t size) DECL_NOEXCEPT;

    template <class BufType,
              class = std::enable_if_t<std::is_same_v<BufType, const char*> ||                        // char* 需要可以计算长度，结尾是'\0'
                                       std::is_pointer_v<decltype(std::declval<BufType>().data())> || // 检查是否有可访问的data()，且返回值是指针
                                       std::is_integral_v<decltype(std::declval<BufType>().size())>   // 检查是否有可访问的size()，且返回值是整型
                                       >>
    void Decrypt(BufType& buf) DECL_NOEXCEPT
    {
        uint8* data = nullptr;
        size_t size = 0;
        if constexpr (std::is_same_v<BufType, const char*>)
        {
            Decrypt(reinterpret_cast<uint8*>(buf), std::char_traits<char>::length(buf));
        }
        else
        {
            using pointer_type = decltype(buf.data());
            using value_type   = typename std::pointer_traits<std::remove_cvref_t<pointer_type>>::element_type;
            Decrypt(reinterpret_cast<uint8*>(buf.data()), buf.size() * sizeof(value_type));
        }
    }

    template <class T, size_t N>
    void Decrypt(T (&buf)[N]) DECL_NOEXCEPT
    {
        Decrypt(reinterpret_cast<uint8*>(buf), N * sizeof(T));
    }

    void Decrypt(uint8* data, size_t size) DECL_NOEXCEPT;

protected:
    // 秘钥扩展
    void KeyExpansion(uint8 key[32]) DECL_NOEXCEPT;
    // 加密一段block 长度16
    void EncryptImpl(uint8* block) DECL_NOEXCEPT;
    // 解密一段block 长度16
    void DecryptImpl(uint8* block) DECL_NOEXCEPT;
    // 秘钥加法
    // round: 加密轮数
    void AddRoundKey(uint8 round, state_t* state) DECL_NOEXCEPT;
    // S盒字节代换
    void SubBytes(state_t* state) DECL_NOEXCEPT;
    // 逆S盒字节代换
    void InvSubBytes(state_t* state) DECL_NOEXCEPT;
    // 行位移
    void ShiftRows(state_t* state) DECL_NOEXCEPT;
    // 逆行位移
    void InvShiftRows(state_t* state) DECL_NOEXCEPT;
    // 列混淆
    void MixColumns(state_t* state) DECL_NOEXCEPT;
    // 逆列混淆
    void InvMixColumns(state_t* state) DECL_NOEXCEPT;

protected:
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_LEN      = 32;  // AES 秘钥长度
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_EXP_SIZE = 240; // AES 秘钥扩展长度
    DECL_STATIC_CONSTEXPR uint8 AES_BLOCK_SIZE   = 16;  // AES 块长度
    DECL_STATIC_CONSTEXPR uint8 NR               = 14;  // 循环轮数
    DECL_STATIC_CONSTEXPR uint8 NK               = 8;   // 秘钥列数
    DECL_STATIC_CONSTEXPR uint8 NB               = 4;   // aes组成一个状态的列数，对于aes是常量
    struct AesContext
    {
        uint8 key[AES_KEY_LEN];          // 秘钥
        uint8 iv[AES_BLOCK_SIZE];        // 初始化向量
        uint8 key_exp[AES_KEY_EXP_SIZE]; // 秘钥扩展
    };

    AesContext m_aesContext;
};

class AES128 : public AESCore
{
protected:
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_LEN      = 16;  // AES 秘钥长度
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_EXP_SIZE = 176; // AES 秘钥扩展长度
    DECL_STATIC_CONSTEXPR uint8 AES_BLOCK_SIZE   = 16;  // AES 块长度
    DECL_STATIC_CONSTEXPR uint8 NR               = 10;  // 循环轮数
    DECL_STATIC_CONSTEXPR uint8 NK               = 4;   // 秘钥列数
    DECL_STATIC_CONSTEXPR uint8 NB               = 4;   // aes组成一个状态的列数，对于aes是常量
};

class AES256 : public AESCore
{
protected:
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_LEN      = 32;  // AES 秘钥长度
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_EXP_SIZE = 240; // AES 秘钥扩展长度
    DECL_STATIC_CONSTEXPR uint8 AES_BLOCK_SIZE   = 16;  // AES 块长度
    DECL_STATIC_CONSTEXPR uint8 NR               = 14;  // 循环轮数
    DECL_STATIC_CONSTEXPR uint8 NK               = 8;   // 秘钥列数
    DECL_STATIC_CONSTEXPR uint8 NB               = 4;   // aes组成一个状态的列数，对于aes是常量
};

}
}}     // namespace screw::algo::AES
#endif // !_AES_H_
