#include "Cpu.hpp"

int main(int argc, const char* argv[])
{
    // $0000-$00FF Zero Page
    // $0100-$01FF System reserved
    // $FFFA-$FFFF Interrupt location
    RAM ram;

    if (argc > 1)
    {
        ram.LoadFromFile(argv[1]);
    }

    ram[0] = 0xA9;
    ram[1] = 0x01;

    Cpu cpu(&ram);

    cpu.Run();

    cpu.Dump();

    return 0;
}