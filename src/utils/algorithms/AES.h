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
        SHA::GetSHA256Code(inputKey, key); // ����ʹ����sha256�������inputKeyת��Ϊ32�ֽڵ�key
        KeyExpansion(key);
    }

    template <class BufType,
              class = std::enable_if_t<std::is_same_v<BufType, const char*> ||                        // char* ��Ҫ���Լ��㳤�ȣ���β��'\0'
                                       std::is_pointer_v<decltype(std::declval<BufType>().data())> || // ����Ƿ��пɷ��ʵ�data()���ҷ���ֵ��ָ��
                                       std::is_integral_v<decltype(std::declval<BufType>().size())>   // ����Ƿ��пɷ��ʵ�size()���ҷ���ֵ������
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
              class = std::enable_if_t<std::is_same_v<BufType, const char*> ||                        // char* ��Ҫ���Լ��㳤�ȣ���β��'\0'
                                       std::is_pointer_v<decltype(std::declval<BufType>().data())> || // ����Ƿ��пɷ��ʵ�data()���ҷ���ֵ��ָ��
                                       std::is_integral_v<decltype(std::declval<BufType>().size())>   // ����Ƿ��пɷ��ʵ�size()���ҷ���ֵ������
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
    // ��Կ��չ
    void KeyExpansion(uint8 key[32]) DECL_NOEXCEPT;
    // ����һ��block ����16
    void EncryptImpl(uint8* block) DECL_NOEXCEPT;
    // ����һ��block ����16
    void DecryptImpl(uint8* block) DECL_NOEXCEPT;
    // ��Կ�ӷ�
    // round: ��������
    void AddRoundKey(uint8 round, state_t* state) DECL_NOEXCEPT;
    // S���ֽڴ���
    void SubBytes(state_t* state) DECL_NOEXCEPT;
    // ��S���ֽڴ���
    void InvSubBytes(state_t* state) DECL_NOEXCEPT;
    // ��λ��
    void ShiftRows(state_t* state) DECL_NOEXCEPT;
    // ����λ��
    void InvShiftRows(state_t* state) DECL_NOEXCEPT;
    // �л���
    void MixColumns(state_t* state) DECL_NOEXCEPT;
    // ���л���
    void InvMixColumns(state_t* state) DECL_NOEXCEPT;

protected:
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_LEN      = 32;  // AES ��Կ����
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_EXP_SIZE = 240; // AES ��Կ��չ����
    DECL_STATIC_CONSTEXPR uint8 AES_BLOCK_SIZE   = 16;  // AES �鳤��
    DECL_STATIC_CONSTEXPR uint8 NR               = 14;  // ѭ������
    DECL_STATIC_CONSTEXPR uint8 NK               = 8;   // ��Կ����
    DECL_STATIC_CONSTEXPR uint8 NB               = 4;   // aes���һ��״̬������������aes�ǳ���
    struct AesContext
    {
        uint8 key[AES_KEY_LEN];          // ��Կ
        uint8 iv[AES_BLOCK_SIZE];        // ��ʼ������
        uint8 key_exp[AES_KEY_EXP_SIZE]; // ��Կ��չ
    };

    AesContext m_aesContext;
};

class AES128 : public AESCore
{
protected:
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_LEN      = 16;  // AES ��Կ����
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_EXP_SIZE = 176; // AES ��Կ��չ����
    DECL_STATIC_CONSTEXPR uint8 AES_BLOCK_SIZE   = 16;  // AES �鳤��
    DECL_STATIC_CONSTEXPR uint8 NR               = 10;  // ѭ������
    DECL_STATIC_CONSTEXPR uint8 NK               = 4;   // ��Կ����
    DECL_STATIC_CONSTEXPR uint8 NB               = 4;   // aes���һ��״̬������������aes�ǳ���
};

class AES256 : public AESCore
{
protected:
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_LEN      = 32;  // AES ��Կ����
    DECL_STATIC_CONSTEXPR uint8 AES_KEY_EXP_SIZE = 240; // AES ��Կ��չ����
    DECL_STATIC_CONSTEXPR uint8 AES_BLOCK_SIZE   = 16;  // AES �鳤��
    DECL_STATIC_CONSTEXPR uint8 NR               = 14;  // ѭ������
    DECL_STATIC_CONSTEXPR uint8 NK               = 8;   // ��Կ����
    DECL_STATIC_CONSTEXPR uint8 NB               = 4;   // aes���һ��״̬������������aes�ǳ���
};

}
}}     // namespace screw::algo::AES
#endif // !_AES_H_
