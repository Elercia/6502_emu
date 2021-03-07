#pragma once

#include <cstdint>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;

#define ASSERT_NOT_REACHED() __debugbreak()
#define ASSERT(X)           \
    do                      \
    {                       \
        if (!X)             \
        {                   \
            __debugbreak(); \
        }                   \
    } while (0)