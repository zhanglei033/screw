#include "MD5.h"
namespace screw {
namespace algo {
namespace MD5 {
#define S11               7
#define S12               12
#define S13               17
#define S14               22
#define S21               5
#define S22               9
#define S23               14
#define S24               20
#define S31               4
#define S32               11
#define S33               16
#define S34               23
#define S41               6
#define S42               10
#define S43               15
#define S44               21

#define F(x, y, z)        (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z)        (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z)        ((x) ^ (y) ^ (z))
#define I(x, y, z)        ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define FF(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += F((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }
#define GG(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += G((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }
#define HH(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += H((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }
#define II(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += I((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s));        \
        (a) += (b);                         \
    }

DECL_INLINE_VAR DECL_CONSTEXPR11 uint8 PADDING[64]{0x80};

static void Transform(const uint8 block[64], MD5State& md5State) DECL_NOEXCEPT
{
    uint32 a = md5State.state[0];
    uint32 b = md5State.state[1];
    uint32 c = md5State.state[2];
    uint32 d = md5State.state[3];
    uint32 x[16]{0};

    for (size_t i = 0, j = 0; j < 64; ++i, j += 4)
        x[i] = ((uint32)block[j]) | (((uint32)block[j + 1]) << 8) | (((uint32)block[j + 2]) << 16) | (((uint32)block[j + 3]) << 24);

    FF(a, b, c, d, x[0], S11, 0xd76aa478);
    FF(d, a, b, c, x[1], S12, 0xe8c7b756);
    FF(c, d, a, b, x[2], S13, 0x242070db);
    FF(b, c, d, a, x[3], S14, 0xc1bdceee);
    FF(a, b, c, d, x[4], S11, 0xf57c0faf);
    FF(d, a, b, c, x[5], S12, 0x4787c62a);
    FF(c, d, a, b, x[6], S13, 0xa8304613);
    FF(b, c, d, a, x[7], S14, 0xfd469501);
    FF(a, b, c, d, x[8], S11, 0x698098d8);
    FF(d, a, b, c, x[9], S12, 0x8b44f7af);
    FF(c, d, a, b, x[10], S13, 0xffff5bb1);
    FF(b, c, d, a, x[11], S14, 0x895cd7be);
    FF(a, b, c, d, x[12], S11, 0x6b901122);
    FF(d, a, b, c, x[13], S12, 0xfd987193);
    FF(c, d, a, b, x[14], S13, 0xa679438e);
    FF(b, c, d, a, x[15], S14, 0x49b40821);

    GG(a, b, c, d, x[1], S21, 0xf61e2562);
    GG(d, a, b, c, x[6], S22, 0xc040b340);
    GG(c, d, a, b, x[11], S23, 0x265e5a51);
    GG(b, c, d, a, x[0], S24, 0xe9b6c7aa);
    GG(a, b, c, d, x[5], S21, 0xd62f105d);
    GG(d, a, b, c, x[10], S22, 0x2441453);
    GG(c, d, a, b, x[15], S23, 0xd8a1e681);
    GG(b, c, d, a, x[4], S24, 0xe7d3fbc8);
    GG(a, b, c, d, x[9], S21, 0x21e1cde6);
    GG(d, a, b, c, x[14], S22, 0xc33707d6);
    GG(c, d, a, b, x[3], S23, 0xf4d50d87);
    GG(b, c, d, a, x[8], S24, 0x455a14ed);
    GG(a, b, c, d, x[13], S21, 0xa9e3e905);
    GG(d, a, b, c, x[2], S22, 0xfcefa3f8);
    GG(c, d, a, b, x[7], S23, 0x676f02d9);
    GG(b, c, d, a, x[12], S24, 0x8d2a4c8a);

    HH(a, b, c, d, x[5], S31, 0xfffa3942);
    HH(d, a, b, c, x[8], S32, 0x8771f681);
    HH(c, d, a, b, x[11], S33, 0x6d9d6122);
    HH(b, c, d, a, x[14], S34, 0xfde5380c);
    HH(a, b, c, d, x[1], S31, 0xa4beea44);
    HH(d, a, b, c, x[4], S32, 0x4bdecfa9);
    HH(c, d, a, b, x[7], S33, 0xf6bb4b60);
    HH(b, c, d, a, x[10], S34, 0xbebfbc70);
    HH(a, b, c, d, x[13], S31, 0x289b7ec6);
    HH(d, a, b, c, x[0], S32, 0xeaa127fa);
    HH(c, d, a, b, x[3], S33, 0xd4ef3085);
    HH(b, c, d, a, x[6], S34, 0x4881d05);
    HH(a, b, c, d, x[9], S31, 0xd9d4d039);
    HH(d, a, b, c, x[12], S32, 0xe6db99e5);
    HH(c, d, a, b, x[15], S33, 0x1fa27cf8);
    HH(b, c, d, a, x[2], S34, 0xc4ac5665);

    II(a, b, c, d, x[0], S41, 0xf4292244);
    II(d, a, b, c, x[7], S42, 0x432aff97);
    II(c, d, a, b, x[14], S43, 0xab9423a7);
    II(b, c, d, a, x[5], S44, 0xfc93a039);
    II(a, b, c, d, x[12], S41, 0x655b59c3);
    II(d, a, b, c, x[3], S42, 0x8f0ccc92);
    II(c, d, a, b, x[10], S43, 0xffeff47d);
    II(b, c, d, a, x[1], S44, 0x85845dd1);
    II(a, b, c, d, x[8], S41, 0x6fa87e4f);
    II(d, a, b, c, x[15], S42, 0xfe2ce6e0);
    II(c, d, a, b, x[6], S43, 0xa3014314);
    II(b, c, d, a, x[13], S44, 0x4e0811a1);
    II(a, b, c, d, x[4], S41, 0xf7537e82);
    II(d, a, b, c, x[11], S42, 0xbd3af235);
    II(c, d, a, b, x[2], S43, 0x2ad7d2bb);
    II(b, c, d, a, x[9], S44, 0xeb86d391);

    md5State.state[0] += a;
    md5State.state[1] += b;
    md5State.state[2] += c;
    md5State.state[3] += d;
}

static void Update(const uint8* inputData, uint32 inputSize, MD5State& md5State) DECL_NOEXCEPT
{
    uint32 i       = 0;
    uint32 index   = (uint32)((md5State.count[0] >> 3) & 0x3f);
    uint32 partLen = 64 - index;

    if ((md5State.count[0] += (inputSize << 3)) < (inputSize << 3)) ++md5State.count[1];

    md5State.count[1] += (inputSize >> 29);

    if (inputSize >= partLen)
    {
        ::memcpy(&md5State.buffer[index], inputData, partLen);
        Transform(md5State.buffer, md5State);
        for (i = partLen; i + 63 < inputSize; i += 64)
            Transform(&inputData[i], md5State);
        index = 0;
    }
    else
    {
        i = 0;
    }

    ::memcpy(&md5State.buffer[index], &inputData[i], static_cast<uint64>(inputSize - i));
}

static void Digest(MD5State& md5State) DECL_NOEXCEPT
{
    uint8 bits[8]{0};
    uint32 oldState[4]{0};
    uint32 oldCount[2]{0};
    uint32 index, padLen;

    ::memcpy(oldState, md5State.state, 16);
    ::memcpy(oldCount, md5State.count, 8);

    for (size_t i = 0, j = 0; j < 8; ++i, j += 4)
    {
        bits[j]     = (uint8)(md5State.count[i] & 0xff);
        bits[j + 1] = (uint8)((md5State.count[i] >> 8) & 0xff);
        bits[j + 2] = (uint8)((md5State.count[i] >> 16) & 0xff);
        bits[j + 3] = (uint8)((md5State.count[i] >> 24) & 0xff);
    }

    index  = (uint32)((md5State.count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);

    Update(PADDING, padLen, md5State);
    Update(bits, 8, md5State);

    for (size_t i = 0, j = 0; j < 16; ++i, j += 4)
    {
        md5State.digest[j]     = (uint8)(md5State.state[i] & 0xff);
        md5State.digest[j + 1] = (uint8)((md5State.state[i] >> 8) & 0xff);
        md5State.digest[j + 2] = (uint8)((md5State.state[i] >> 16) & 0xff);
        md5State.digest[j + 3] = (uint8)((md5State.state[i] >> 24) & 0xff);
    }

    memcpy(md5State.state, oldState, 16);
    memcpy(md5State.count, oldCount, 8);
}
}}} // namespace screw::algo::MD5