#include "Cpu.hpp"

int main(int argc, const char* argv[])
{
    // $0000-$00FF Zero Page
    // $0100-$01FF System reserved
    // $FFFA-$FFFF Interrupt location
    RAM ram;

    if (argc > 1)
    {
        ram.LoadFromFile(0x000A, argv [1]); 
    }

    Cpu cpu(&ram);
    cpu.PC = 0x400;
    cpu.Run();

    cpu.Dump();

    return 0;
}