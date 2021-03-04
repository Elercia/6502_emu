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

    void LoadFromFile(const char* filename);

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
void Memory<SIZE>::LoadFromFile(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return;
    }

    file.read((char*) Mem, SIZE);
}

constexpr u16 RamSize = 64 * 1000;  // 64 KB
using RAM = Memory<RamSize>;

constexpr u16 StackStartAddr = 0x0100;
constexpr u16 StackEndAddr = 0x01FF;
