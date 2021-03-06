#pragma once

#include "Common.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>

// Memory
template <u16 SIZE>
struct Memory
{
    Memory() { memset(Mem, 0, SIZE); }

    u8& operator[](int loc) { return Mem[loc]; }

    u8 operator[](int loc) const { return Mem[loc]; }

    void LoadFromFile(u16 startLocation, const char* filename);

    void Dump();

    u8 Mem[SIZE];
};

template <u16 SIZE>
void Memory<SIZE>::Dump()
{
    for (u16 i = 0; i < SIZE; i++)
    {
        printf("%x ", Mem[i]);

        if (i % 1000 == 0)
        {
            printf("\n");
        }
    }
}

template <u16 SIZE>
void Memory<SIZE>::LoadFromFile(u16 startLocation, const char* filename)
{
    FILE* file = fopen(filename, "rb");

    if (file == NULL)
        return;

    fseek(file, 0, SEEK_END);

    size_t fileSize = ftell(file);

    fseek(file, 0, SEEK_SET);

    size_t readSize = fread((Mem + startLocation), 1, fileSize, file);

    if (readSize != fileSize)
    {
        ASSERT_NOT_REACHED();
    }

    fclose(file);
}

constexpr u16 RamSize = 65526;  // 64 KB
using RAM = Memory<RamSize>;

constexpr u16 StackStartAddr = 0x0100;
constexpr u16 StackEndAddr = 0x01FF;
