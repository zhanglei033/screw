#include "Base64.h"
namespace screw {
namespace algo {
namespace Base64 {
#define BASE64_PAD                '='
#define BASE64DE_FIRST            '+'
#define BASE64DE_LAST             'z'
static constexpr char base64en[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
static constexpr char base64de[] = {62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

// encode 需要在调用前就分配好BASE64_ENCODE_OUT_SIZE(inputSize) + 1个字节的内存
static size_t Base64EncodeImpl(const uint8* inputData, size_t inputSize, char* encode) DECL_NOEXCEPT
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
// decode 需要在调用前就分配好BASE64_DECODE_OUT_SIZE(inputSize) + 1个字节的内存
static size_t Base64DecodeImpl(const char* inputData, size_t inputSize, uint8* decode) DECL_NOEXCEPT
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

}}} // namespace screw::algo::Base64