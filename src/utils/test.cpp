#include "algorithms/SHA.h"
#include "base/Assert.h"
#include "base/IteratorBase.h"
#include "base/TypesDef.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
using namespace screw;
using namespace screw::algo;
template <class InputType>
inline void GetSHA1Code(InputType&& input)
{
}

struct MyString
{
    MyString() {}
    MyString(unsigned char*&& str, size_t len) {}
    MyString(const unsigned char* str, size_t len) {}
};

int main()
{
    uint8* out = new uint8[16];
    uint32 data[4] {0x12345678, 0x34567890, 0x56789012, 0x78901234};
    // 78 56 34 12 90 78 56 34 12 90 78 56 34 12 90 78
    memcpy(out, data, 16);
    // 78 56 34 12 90 78 56 34 12 90 78 56 34 12 90 78
    for (auto i = 0; i < 16; ++i)
    {
        out[i] = (uint8)(data[i >> 2] >> ((i & 3) << 3));
    }
    

    return 0;
}