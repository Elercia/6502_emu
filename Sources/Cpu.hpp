#pragma once

#include "Common.hpp"
#include "Memory.hpp"

#include <map>
#include <string>

#define ACCUMULATOR(OP, CODE) OPERATION(OP, CODE, ACCUMULATOR)
#define IMMIDIATE(OP, CODE) OPERATION(OP, CODE, IMEDIATE)
#define ZERO_PAGE(OP, CODE) OPERATION(OP, CODE, ZERO_PAGE)
#define ZERO_PAGE_X(OP, CODE) OPERATION(OP, CODE, ZERO_PAGE_X)
#define ZERO_PAGE_Y(OP, CODE) OPERATION(OP, CODE, ZERO_PAGE_Y)
#define RELATIVE(OP, CODE) OPERATION(OP, CODE, RELATIVE)
#define ABSOLUTE(OP, CODE) OPERATION(OP, CODE, ABSOLUTE)
#define ABSOLUTE_X(OP, CODE) OPERATION(OP, CODE, ABSOLUTE_X)
#define ABSOLUTE_Y(OP, CODE) OPERATION(OP, CODE, ABSOLUTE_Y)
#define INDIRECT(OP, CODE) OPERATION(OP, CODE, INDIRECT)
#define INDIRECT_X(OP, CODE) OPERATION(OP, CODE, INDEXED_INDIRECT)
#define INDIRECT_Y(OP, CODE) OPERATION(OP, CODE, INDIRECT_INDEXED)
#define IMPLIED(OP, CODE) OPERATION(OP, CODE, IMPLIED)

#define ALL_OPS            \
    IMMIDIATE(ADC, 0x69)   \
    ZERO_PAGE(ADC, 0x65)   \
    ZERO_PAGE_X(ADC, 0x75) \
    ABSOLUTE(ADC, 0x6D)    \
    ABSOLUTE_X(ADC, 0x7D)  \
    ABSOLUTE_Y(ADC, 0x79)  \
    INDIRECT_X(ADC, 0x61)  \
    INDIRECT_Y(ADC, 0x71)  \
    IMMIDIATE(AND, 0x29)   \
    ZERO_PAGE(AND, 0x25)   \
    ZERO_PAGE_X(AND, 0x35) \
    ABSOLUTE(AND, 0x2D)    \
    ABSOLUTE_X(AND, 0x3D)  \
    ABSOLUTE_Y(AND, 0x39)  \
    INDIRECT_X(AND, 0x21)  \
    INDIRECT_Y(AND, 0x31)  \
    ACCUMULATOR(ASL, 0x0A) \
    ZERO_PAGE(ASL, 0x06)   \
    ZERO_PAGE_X(ASL, 0x16) \
    ABSOLUTE(ASL, 0x0E)    \
    ABSOLUTE_X(ASL, 0x1E)  \
    RELATIVE(BCC, 0x90)    \
    RELATIVE(BCS, 0xB0)    \
    RELATIVE(BEQ, 0xF0)    \
    ZERO_PAGE(BIT, 0x24)   \
    ABSOLUTE(BIT, 0x2C)    \
    RELATIVE(BMI, 0x30)    \
    RELATIVE(BNE, 0xD0)    \
    RELATIVE(BPL, 0x10)    \
    IMPLIED(BRK, 0x00)     \
    RELATIVE(BVC, 0x50)    \
    RELATIVE(BVS, 0x70)    \
    IMPLIED(CLC, 0x18)     \
    IMPLIED(CLD, 0xD8)     \
    IMPLIED(CLI, 0x58)     \
    IMPLIED(CLV, 0xB8)     \
    IMMIDIATE(CMP, 0xC9)   \
    ZERO_PAGE(CMP, 0xC5)   \
    ZERO_PAGE_X(CMP, 0xD5) \
    ABSOLUTE(CMP, 0xCD)    \
    ABSOLUTE_X(CMP, 0xDD)  \
    ABSOLUTE_Y(CMP, 0xD9)  \
    INDIRECT_X(CMP, 0xC1)  \
    INDIRECT_Y(CMP, 0xD1)  \
    IMMIDIATE(CPX, 0xE0)   \
    ZERO_PAGE(CPX, 0xE4)   \
    ABSOLUTE(CPX, 0xEC)    \
    IMMIDIATE(CPY, 0xC0)   \
    ZERO_PAGE(CPY, 0xC4)   \
    ABSOLUTE(CPY, 0xCC)    \
    ZERO_PAGE(DEC, 0xC6)   \
    ZERO_PAGE_X(DEC, 0xD6) \
    ABSOLUTE(DEC, 0xCE)    \
    ABSOLUTE_X(DEC, 0xDE)  \
    IMPLIED(DEX, 0xCA)     \
    IMPLIED(DEY, 0x88)     \
    IMMIDIATE(EOR, 0x49)   \
    ZERO_PAGE(EOR, 0x45)   \
    ZERO_PAGE_X(EOR, 0x55) \
    ABSOLUTE(EOR, 0x4D)    \
    ABSOLUTE_X(EOR, 0x5D)  \
    ABSOLUTE_Y(EOR, 0x59)  \
    INDIRECT_X(EOR, 0x41)  \
    INDIRECT_Y(EOR, 0x51)  \
    ZERO_PAGE(INC, 0xE6)   \
    ZERO_PAGE_X(INC, 0xF6) \
    ABSOLUTE(INC, 0xEE)    \
    ABSOLUTE_X(INC, 0xFE)  \
    IMPLIED(INX, 0xE8)     \
    IMPLIED(INY, 0xC8)     \
    ABSOLUTE(JMP, 0x4C)    \
    INDIRECT(JMP, 0x6C)    \
    ABSOLUTE(JSR, 0x20)    \
    IMMIDIATE(LDA, 0xA9)   \
    ZERO_PAGE(LDA, 0xA5)   \
    ZERO_PAGE_X(LDA, 0xB5) \
    ABSOLUTE(LDA, 0xAD)    \
    ABSOLUTE_X(LDA, 0xBD)  \
    ABSOLUTE_Y(LDA, 0xB9)  \
    INDIRECT_X(LDA, 0xA1)  \
    INDIRECT_Y(LDA, 0xB1)  \
    IMMIDIATE(LDX, 0xA2)   \
    ZERO_PAGE(LDX, 0xA6)   \
    ZERO_PAGE_Y(LDX, 0xB6) \
    ABSOLUTE(LDX, 0xAE)    \
    ABSOLUTE_Y(LDX, 0xBE)  \
    IMMIDIATE(LDY, 0xA0)   \
    ZERO_PAGE(LDY, 0xA4)   \
    ZERO_PAGE_X(LDY, 0xB4) \
    ABSOLUTE(LDY, 0xAC)    \
    ABSOLUTE_X(LDY, 0xBC)  \
    ACCUMULATOR(LSR, 0x4A) \
    ZERO_PAGE(LSR, 0x46)   \
    ZERO_PAGE_X(LSR, 0x56) \
    ABSOLUTE(LSR, 0x4E)    \
    ABSOLUTE_X(LSR, 0x5E)  \
    IMPLIED(NOP, 0xEA)     \
    IMMIDIATE(ORA, 0x09)   \
    ZERO_PAGE(ORA, 0x05)   \
    ZERO_PAGE_X(ORA, 0x15) \
    ABSOLUTE(ORA, 0x0D)    \
    ABSOLUTE_X(ORA, 0x1D)  \
    ABSOLUTE_Y(ORA, 0x19)  \
    INDIRECT_X(ORA, 0x01)  \
    INDIRECT_Y(ORA, 0x11)  \
    IMPLIED(PHA, 0x48)     \
    IMPLIED(PHP, 0x08)     \
    IMPLIED(PLA, 0x68)     \
    IMPLIED(PLP, 0x28)     \
    ACCUMULATOR(ROL, 0x2A) \
    ZERO_PAGE(ROL, 0x26)   \
    ZERO_PAGE_X(ROL, 0x36) \
    ABSOLUTE(ROL, 0x2E)    \
    ABSOLUTE_X(ROL, 0x3E)  \
    ACCUMULATOR(ROR, 0x6A) \
    ZERO_PAGE(ROR, 0x66)   \
    ZERO_PAGE_X(ROR, 0x76) \
    ABSOLUTE(ROR, 0x6E)    \
    ABSOLUTE_X(ROR, 0x7E)  \
    IMPLIED(RTI, 0x40)     \
    IMPLIED(RTS, 0x60)     \
    IMMIDIATE(SBC, 0xE9)   \
    ZERO_PAGE(SBC, 0xE5)   \
    ZERO_PAGE_X(SBC, 0xF5) \
    ABSOLUTE(SBC, 0xED)    \
    ABSOLUTE_X(SBC, 0xFD)  \
    ABSOLUTE_Y(SBC, 0xF9)  \
    INDIRECT_X(SBC, 0xE1)  \
    INDIRECT_Y(SBC, 0xF1)  \
    IMPLIED(SEC, 0x38)     \
    IMPLIED(SED, 0xF8)     \
    IMPLIED(SEI, 0x78)     \
    ZERO_PAGE(STA, 0x85)   \
    ZERO_PAGE_X(STA, 0x95) \
    ABSOLUTE(STA, 0x8D)    \
    ABSOLUTE_X(STA, 0x9D)  \
    ABSOLUTE_Y(STA, 0x99)  \
    INDIRECT_X(STA, 0x81)  \
    INDIRECT_Y(STA, 0x91)  \
    ZERO_PAGE(STX, 0x86)   \
    ZERO_PAGE_Y(STX, 0x96) \
    ABSOLUTE(STX, 0x8E)    \
    ZERO_PAGE(STY, 0x84)   \
    ZERO_PAGE_X(STY, 0x94) \
    ABSOLUTE(STY, 0x8C)    \
    IMPLIED(TAX, 0xAA)     \
    IMPLIED(TAY, 0xA8)     \
    IMPLIED(TSX, 0xBA)     \
    IMPLIED(TXA, 0x8A)     \
    IMPLIED(TXS, 0x9A)     \
    IMPLIED(TYA, 0x98)

// Cpu
struct Cpu
{
    typedef void (Cpu::*OperationCode)();

    Cpu(RAM* _ram);
    ~Cpu() = default;

    u8 ReadOneByte();
    void Write(u16 loc, u8 data);
    u8& ReadAt(u16 loc);

    void PushOnStack(u8 value);
    void PushOnStack(u16 value);
    u8 PopStack();
    u16 PopStack16();

    u8 GetProcStatus();
    void SetProcStatus(u8 status);

    void Run();
    void Dump();

    // All the addressing mode (read / write)
    u8 Immediate();
    u8& ZeroPage();
    void ZeroPageWrite(u8 data);
    u8& ZeroPageX();
    void ZeroPageXWrite(u8 data);
    u8 ZeroPageY();
    void ZeroPageYWrite(u8 data);
    u16 Relative();
    u8& Absolute();    // Read the byte at absolute location
    u16 Absolute16();  // Give the absolute location
    void AbsoluteWrite(u8 data);
    u8& AbsoluteX();
    void AbsoluteXWrite(u8 data);
    u8 AbsoluteY();
    void AbsoluteYWrite(u8 data);
    u16 Indirect();
    u8 IndirectX();
    void IndirectXWrite(u8 data);
    u8 IndirectY();
    void IndirectYWrite(u8 data);

    // All the operations
#define OPERATION(OP, CODE, MODE) void OP##_##MODE();

    ALL_OPS

#undef OPERATION

    // Cpu Related registers
    u8 A{0};        // Accumulator
    u8 X{0}, Y{0};  // Index Registers
    u8 S{0xFF};     // Stack pointer
    u16 PC{0};      // Program counter

    bool C{0};  // Carry Flag
    bool Z{0};  // Zero Flag
    bool I{0};  // Interrupt Disable
    bool D{0};  // Decimal Mode
    bool B{0};  // Break Command
    bool V{0};  // Overflow Flag
    bool N{0};  // Negative Flag

    // Number of cycles made from the start
    int cycles{0};

    RAM* ram;

    using OperationMap = std::map<u8, std::pair<const char*, OperationCode>>;

    static OperationMap ConstructOperations();
    static OperationMap OpCodes;
};
