#include "Cpu.hpp"

#include <iostream>

std::map<u8, Cpu::OperationCode> Cpu::OpCodes = ConstructOperations();

Cpu::Cpu(RAM* _ram) : ram(_ram)
{
}

u8 Cpu::ReadOneByte()
{
    u8 data = (*ram)[PC];

    PC++;

    cycles++;

    return data;
}

void Cpu::Write(u16 loc, u8 data)
{
    (*ram)[loc] = data;
}

u8& Cpu::ReadAt(u16 loc)
{
    return (*ram)[loc];
}

void Cpu::PushOnStack(u8 value)
{
    (*ram)[StackStartAddr + S] = value;
    S++;
}

u8 Cpu::PopStack()
{
    S--;
    return (*ram)[StackStartAddr + S];
}

u8 Cpu::GetProcStatus()
{
    u8 P = (((u8) C) << 6) & (((u8) Z) << 5) & (((u8) I) << 4) & (((u8) D) << 3) & (((u8) B) << 2) & (((u8) V) << 1) &
           ((u8) N);

    return P;
}

void Cpu::SetProcStatus(u8 /*status*/)
{
    ASSERT_NOT_REACHED();
}

void Cpu::Run()
{
    u8 opCode = ReadOneByte();

    while (opCode != 0)
    {
        (this->*OpCodes[opCode])();
        opCode = ReadOneByte();
    }
}

void Cpu::Dump()
{
    std::cout << "Registries :" << std::endl;
    std::cout << "Accumulator " << std::to_string(A) << std::endl;
    std::cout << "Index Register X " << std::to_string(X) << std::endl;
    std::cout << "Index Register Y " << std::to_string(Y) << std::endl;
    std::cout << "Stack pointer " << std::to_string(S) << std::endl;
    std::cout << "Program counter " << std::to_string(PC) << std::endl;

    std::cout << "\nFlags :" << std::endl;
    std::cout << "Carry Flag " << std::to_string(C) << std::endl;
    std::cout << "Zero Flag " << std::to_string(Z) << std::endl;
    std::cout << "Interrupt Disable " << std::to_string(I) << std::endl;
    std::cout << "Decimal Mode " << std::to_string(D) << std::endl;
    std::cout << "Overflow Flag " << std::to_string(V) << std::endl;
    std::cout << "Negative Flag " << std::to_string(N) << std::endl;

    std::cout << "Cycles " << cycles << std::endl;

    std::cout << "Memory : " << std::endl;
    ram->Dump();
}

//
// void Cpu::BVC()
//{
//    if (!V)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::BVS()
//{
//    if (!V)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::BRK() { ASSERT_NOT_REACHED(); }
//
// void Cpu::BPL()
//{
//    if (!Z)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::BNE()
//{
//    if (!Z)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::BMI()
//{
//    if (N)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::BEQ()
//{
//    if (Z)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::BCS()
//{
//    if (C)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::BCC()
//{
//    if (!C)
//    {
//        u8 M = ReadOneByte();
//
//        PC += (u16) M;
//    }
//}
//
// void Cpu::ASL()
//{
//    C = (A & 0x08) != 0;
//    A = A << 1;
//    Z = A == 0;
//    N = (A & 0x08) != 0;
//}
//
// void Cpu::AND()
//{
//    u8 M = ReadOneByte();
//
//    A = A & M;
//
//    Z = A == 0;
//    N = (A & 0x08) != 0;
//}
//
// void Cpu::SBC()
//{
//    u8 M = ReadOneByte();
//
//    u16 value = A - M - (1 - (u16) C);
//
//    V = (A ^ (u8) value) & ~(A ^ M);
//    A = (u8) value;
//    Z = A == 0;
//    N = (value & 0x0008) != 0;
//    C = (value & 0xFF00) != 0;
//}
//
// void Cpu::ADC()
//{
//    u8 M = ReadOneByte();
//
//    u16 value = A + M + (u16) C;
//
//    V = (A ^ (u8) value) & ~(A ^ M);
//    A = (u8) value;
//    Z = A == 0;
//    N = (value & 0x0008) != 0;
//    C = (value & 0xFF00) != 0;
//}
//
// void Cpu::LDA()
//{
//    u8 M = ReadOneByte();
//
//    u16 value = M;
//
//    A = (u8) value;
//    Z = A == 0;
//    N = (value & 0x0008) != 0;
//    C = (value & 0xFF00) != 0;
//}
//
// void Cpu::LDX()
//{
//    u8 M = ReadOneByte();
//
//    X = M;
//    Z = A == 0;
//    N = (M & 0x08) != 0;
//}
//
// void Cpu::LDY()
//{
//    u8 M = ReadOneByte();
//
//    Y = (u8) M;
//    Z = A == 0;
//    N = (M & 0x08) != 0;
//}
//
// void Cpu::CMP()
//{
//    u8 M = ReadOneByte();
//
//    u8 value = A - M;
//
//    Z = A == M;
//    N = (value & 0x08) != 0;
//    C = A >= M;
//}
//
// void Cpu::CPX()
//{
//    u8 M = ReadOneByte();
//
//    u8 value = X - M;
//
//    Z = X == M;
//    N = (value & 0x08) != 0;
//    C = X >= M;
//}
// void Cpu::CPY()
//{
//    u8 M = ReadOneByte();
//
//    u8 value = Y - M;
//
//    Z = Y == M;
//    N = (value & 0x08) != 0;
//    C = Y >= M;
//}
//
// void Cpu::DEX()
//{
//    X -= 1;
//
//    Z = X == 0;
//    N = (X & 0x08) != 0;
//}
//
// void Cpu::DEY()
//{
//    Y -= 1;
//
//    Z = Y == 0;
//    N = (Y & 0x08) != 0;
//}
//
// void Cpu::EOR()
//{
//    u8 M = ReadOneByte();
//
//    A = A ^ M;
//
//    Z = A == 0;
//    N = (A & 0x08) != 0;
//}
//
//
// void Cpu::INX()
//{
//    X += 1;
//
//    Z = X == 0;
//    N = (X & 0x08) != 0;
//}
// void Cpu::INY()
//{
//    Y += 1;
//
//    Z = Y == 0;
//    N = (Y & 0x08) != 0;
//}
//
// void Cpu::JMP()
//{
//    u8 M = ReadOneByte();
//
//    PC = (PC & 0xFF00) & (u16) M;  // absolute
//}
//
// void Cpu::JSR()
//{
//    u8 MSB = ReadOneByte();
//    u8 LSB = ReadOneByte();
//
//    PushOnStack(PC);  // TODO Push u16 (minus one)
//
//    PC = ((u16) MSB << 8) & (u16) LSB;
//}
//
// void Cpu::ORA()
//{
//    u8 M = ReadOneByte();
//
//    A = A | M;
//    Z = A == 0;
//    N = (A & 0x08) != 0;
//}
//
// void Cpu::PHA() { PushOnStack(A); }
//
// void Cpu::PHP()
//{
//    u8 P = (((u8) C) << 6) & (((u8) Z) << 5) & (((u8) I) << 4) & (((u8) D) << 3) & (((u8) B) << 2) & (((u8) V) << 1) &
//           ((u8) N);
//
//    PushOnStack(P);
//}
//
// void Cpu::PLA()
//{
//    A = PopStack();
//    Z = A == 0;
//    N = (A & 0x08) != 0;
//}
//
// void Cpu::PLP()
//{
//    u8 P = PopStack();
//    Z = A == 0;
//    N = (A & 0x08) != 0;
//
//    C == (P & 0x70) != 0;
//    Z == (P & 0x70) != 0;
//    I == (P & 0x70) != 0;
//    D == (P & 0x70) != 0;
//    B == (P & 0x70) != 0;
//    V == (P & 0x70) != 0;  // TODO
//}
//
// void Cpu::ROL()
//{
//    C = (A & 0x80) != 0;  // TODO TMP C
//
//    A = A << 1;
//    A += C;
//
//    Z = Y == 0;
//    N = (Y & 0x08) != 0;  // TODO There is a pb with the N flag mask
//}
//
// void Cpu::ROR()
//{
//    C = (A & 0x01) != 0;  // TODO TMP C
//
//    A = A >> 1;
//    A |= (u8) C << 7;
//
//    Z = Y == 0;
//    N = (Y & 0x08) != 0;  // TODO There is a pb with the N flag mask
//}

// **********************************************
// **********************************************
// Addressing mode
// **********************************************
// **********************************************

u8 Cpu::Immediate()
{
    return ReadOneByte();
}

u8& Cpu::ZeroPage()
{
    u8 offset = ReadOneByte();
    return ReadAt(0x0000 + (u16) offset);
}

void Cpu::ZeroPageWrite(u8 data)
{
    u8 offset = ReadOneByte();
    Write(0x0000 + (u16) offset, data);
}

u8& Cpu::ZeroPageX()
{
    u8 offset = ReadOneByte();
    return ReadAt(0x0000 + (u16) offset + (u16) X);
}

void Cpu::ZeroPageXWrite(u8 data)
{
    u8 offset = ReadOneByte();
    Write(0x0000 + (u16) offset + (u16) X, data);
}

u8 Cpu::ZeroPageY()
{
    u8 offset = ReadOneByte();
    return ReadAt(0x0000 + (u16) offset + (u16) Y);
}

void Cpu::ZeroPageYWrite(u8 data)
{
    u8 offset = ReadOneByte();
    Write(0x0000 + (u16) offset + (u16) Y, data);
}

u16 Cpu::Relative()
{
    u8 offset = ReadOneByte();
    return PC + (u16) offset;
}

u8& Cpu::Absolute()
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    return ReadAt((((u16) offset1) << 8) | (u16) offset2);
}

void Cpu::AbsoluteWrite(u8 data)
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    Write((((u16) offset1) << 8) | (u16) offset2, data);
}

u8& Cpu::AbsoluteX()
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    u16 fullOffset = (((u16) offset1) << 8) | (u16) offset2;

    return ReadAt(fullOffset + (u16) X);
}

void Cpu::AbsoluteXWrite(u8 data)
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    u16 fullOffset = (((u16) offset1) << 8) | (u16) offset2;

    Write(fullOffset + (u16) X, data);
}

u8 Cpu::AbsoluteY()
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    u16 fullOffset = (((u16) offset1) << 8) | (u16) offset2;

    return ReadAt(fullOffset + (u16) Y);
}

void Cpu::AbsoluteYWrite(u8 data)
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    u16 fullOffset = (((u16) offset1) << 8) | (u16) offset2;

    Write(fullOffset + (u16) Y, data);
}

u8 Cpu::Indirect()
{
    ASSERT_NOT_REACHED();

    return 0;
}

u8 Cpu::IndexedIndirect()
{
    u8 offset = ReadOneByte();
    offset += X;

    return offset;
}

void Cpu::IndexedIndirectWrite(u8 data)
{
    u8 offset = ReadOneByte();
    offset += X;

    Write(offset, data);
}

u8 Cpu::IndirectIndexed()
{
    u8 offset = ReadOneByte();
    offset += Y;

    return offset;
}

void Cpu::IndirectIndexedWrite(u8 data)
{
    u8 offset = ReadOneByte();
    offset += Y;

    Write(offset, data);
}

// **********************************************
// **********************************************
// Operations code
// **********************************************
// **********************************************

#define OPERATION(OP, CODE, MODE) void Cpu::OP##_##MODE()

IMMIDIATE(ADC, 0x69)
{
}

ZERO_PAGE(ADC, 0x65)
{
}
ZERO_PAGE_X(ADC, 0x75)
{
}
ABSOLUTE(ADC, 0x6D)
{
}
ABSOLUTE_X(ADC, 0x7D)
{
}
ABSOLUTE_Y(ADC, 0x79)
{
}
INDIRECT_X(ADC, 0x61)
{
}
INDIRECT_Y(ADC, 0x71)
{
}
IMMIDIATE(AND, 0x29)
{
    u8 m = Immediate();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(AND, 0x25)
{
    u8 m = ZeroPage();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE_X(AND, 0x35)
{
    u8 m = ZeroPageX();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE(AND, 0x2D)
{
    u8 m = Absolute();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_X(AND, 0x3D)
{
    u8 m = AbsoluteX();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_Y(AND, 0x39)
{
    u8 m = AbsoluteY();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_X(AND, 0x21)
{
    u8 m = IndexedIndirect();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_Y(AND, 0x31)
{
    u8 m = IndirectIndexed();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ACCUMULATOR(ASL, 0x0A)
{
}
ZERO_PAGE(ASL, 0x06)
{
}
ZERO_PAGE_X(ASL, 0x16)
{
}
ABSOLUTE(ASL, 0x0E)
{
}
ABSOLUTE_X(ASL, 0x1E)
{
}
RELATIVE(BCC, 0x90)
{
    u16 jump = Relative();

    if (!C)
    {
        PC = jump;
    }
}
RELATIVE(BCS, 0xB0)
{
    u16 jump = Relative();

    if (C)
    {
        PC = jump;
    }
}
RELATIVE(BEQ, 0xF0)
{
    u16 jump = Relative();

    if (Z)
    {
        PC = jump;
    }
}
ZERO_PAGE(BIT, 0x24)
{
}
ABSOLUTE(BIT, 0x2C)
{
}
RELATIVE(BMI, 0x30)
{
    u16 jump = Relative();

    if (N)
    {
        PC = jump;
    }
}
RELATIVE(BNE, 0xD0)
{
    u16 jump = Relative();

    if (!Z)
    {
        PC = jump;
    }
}
RELATIVE(BPL, 0x10)
{
    u16 jump = Relative();

    if (!N)
    {
        PC = jump;
    }
}
IMPLIED(BRK, 0x01)
{
    u16 loc = (ReadAt(0xFFFE) << 8) | ReadAt(0xFFFF);

    PushOnStack((u8)(PC >> 8));
    PushOnStack((u8) PC);
    PushOnStack(GetProcStatus());

    B = 1;
    PC = loc;
}
RELATIVE(BVC, 0x50)
{
    u16 jump = Relative();

    if (!V)
    {
        PC = jump;
    }
}
RELATIVE(BVS, 0x70)
{
    u16 jump = Relative();

    if (V)
    {
        PC = jump;
    }
}
IMPLIED(CLC, 0x18)
{
    C = 0;
}
IMPLIED(CLD, 0xD8)
{
    D = 0;
}
IMPLIED(CLI, 0x58)
{
    I = 0;
}
IMPLIED(CLV, 0xB8)
{
    V = 0;
}
IMMIDIATE(CMP, 0xC9)
{
}
ZERO_PAGE(CMP, 0xC5)
{
}
ZERO_PAGE_X(CMP, 0xD5)
{
}
ABSOLUTE(CMP, 0xCD)
{
}
ABSOLUTE_X(CMP, 0xDD)
{
}
ABSOLUTE_Y(CMP, 0xD9)
{
}
INDIRECT_X(CMP, 0xC1)
{
}
INDIRECT_Y(CMP, 0xD1)
{
}
IMMIDIATE(CPX, 0xE0)
{
}
ZERO_PAGE(CPX, 0xE4)
{
}
ABSOLUTE(CPX, 0xEC)
{
}
IMMIDIATE(CPY, 0xC0)
{
}
ZERO_PAGE(CPY, 0xC4)
{
}
ABSOLUTE(CPY, 0xCC)
{
}
ZERO_PAGE(DEC, 0xC6)
{
    u8& m = ZeroPage();

    m = m - 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ZERO_PAGE_X(DEC, 0xD6)
{
    u8& m = ZeroPageX();

    m = m - 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE(DEC, 0xCE)
{
    u8& m = Absolute();

    m = m - 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE_X(DEC, 0xDE)
{
    u8& m = AbsoluteX();

    m = m - 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
IMPLIED(DEX, 0xCA)
{
    X = X - 1;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
IMPLIED(DEY, 0x88)
{
    Y = Y - 1;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
IMMIDIATE(EOR, 0x49)
{
}
ZERO_PAGE(EOR, 0x45)
{
}
ZERO_PAGE_X(EOR, 0x55)
{
}
ABSOLUTE(EOR, 0x4D)
{
}
ABSOLUTE_X(EOR, 0x5D)
{
}
ABSOLUTE_Y(EOR, 0x59)
{
}
INDIRECT_X(EOR, 0x41)
{
}
INDIRECT_Y(EOR, 0x51)
{
}
ZERO_PAGE(INC, 0xE6)
{
}
ZERO_PAGE_X(INC, 0xF6)
{
}
ABSOLUTE(INC, 0xEE)
{
}
ABSOLUTE_X(INC, 0xFE)
{
}
IMPLIED(INX, 0xE8)
{
    X = X + 1;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
IMPLIED(INY, 0xC8)
{
    Y = Y + 1;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
ABSOLUTE(JMP, 0x4C)
{
    u16 m = Absolute();
}
INDIRECT(JMP, 0x6C)
{
}
ABSOLUTE(JSR, 0x20)
{
}
IMMIDIATE(LDA, 0xA9)
{
    u8 m = Immediate();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(LDA, 0xA5)
{
    u8 m = ZeroPage();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE_X(LDA, 0xB5)
{
    u8 m = ZeroPageX();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE(LDA, 0xAD)
{
    u8 m = Absolute();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_X(LDA, 0xBD)
{
    u8 m = AbsoluteX();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_Y(LDA, 0xB9)
{
    u8 m = AbsoluteY();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_X(LDA, 0xA1)
{
    u8 m = IndexedIndirect();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_Y(LDA, 0xB1)
{
    u8 m = IndirectIndexed();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
IMMIDIATE(LDX, 0xA2)
{
    u8 m = Immediate();

    X = m;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
ZERO_PAGE(LDX, 0xA6)
{
    u8 m = ZeroPage();

    X = m;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
ZERO_PAGE_Y(LDX, 0xB6)
{
    u8 m = ZeroPageX();

    X = m;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
ABSOLUTE(LDX, 0xAE)
{
    u8 m = ZeroPageY();

    X = m;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
ABSOLUTE_Y(LDX, 0xBE)
{
    u8 m = AbsoluteY();

    Y = m;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
IMMIDIATE(LDY, 0xA0)
{
    u8 m = Immediate();

    Y = m;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
ZERO_PAGE(LDY, 0xA4)
{
    u8 m = ZeroPage();

    Y = m;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
ZERO_PAGE_X(LDY, 0xB4)
{
    u8 m = ZeroPageX();

    Y = m;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
ABSOLUTE(LDY, 0xAC)
{
    u8 m = Absolute();

    Y = m;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
ABSOLUTE_X(LDY, 0xBC)
{
    u8 m = AbsoluteX();

    Y = m;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
ACCUMULATOR(LSR, 0x4A)
{
}
ZERO_PAGE(LSR, 0x46)
{
}
ZERO_PAGE_X(LSR, 0x56)
{
}
ABSOLUTE(LSR, 0x4E)
{
}
ABSOLUTE_X(LSR, 0x5E)
{
}
IMPLIED(NOP, 0xEA)
{
    // noop noop
}
IMMIDIATE(ORA, 0x09)
{
}
ZERO_PAGE(ORA, 0x05)
{
}
ZERO_PAGE_X(ORA, 0x15)
{
}
ABSOLUTE(ORA, 0x0D)
{
}
ABSOLUTE_X(ORA, 0x1D)
{
}
ABSOLUTE_Y(ORA, 0x19)
{
}
INDIRECT_X(ORA, 0x01)
{
}
INDIRECT_Y(ORA, 0x11)
{
}
IMPLIED(PHA, 0x48)
{
}
IMPLIED(PHP, 0x08)
{
}
IMPLIED(PLA, 0x68)
{
}
IMPLIED(PLP, 0x28)
{
}
ACCUMULATOR(ROL, 0x2A)
{
}
ZERO_PAGE(ROL, 0x26)
{
}
ZERO_PAGE_X(ROL, 0x36)
{
}
ABSOLUTE(ROL, 0x2E)
{
}
ABSOLUTE_X(ROL, 0x3E)
{
}
ACCUMULATOR(ROR, 0x6A)
{
}
ZERO_PAGE(ROR, 0x66)
{
}
ZERO_PAGE_X(ROR, 0x76)
{
}
ABSOLUTE(ROR, 0x6E)
{
}
ABSOLUTE_X(ROR, 0x7E)
{
}
IMPLIED(RTI, 0x40)
{
}
IMPLIED(RTS, 0x60)
{
}
IMMIDIATE(SBC, 0xE9)
{
}
ZERO_PAGE(SBC, 0xE5)
{
}
ZERO_PAGE_X(SBC, 0xF5)
{
}
ABSOLUTE(SBC, 0xED)
{
}
ABSOLUTE_X(SBC, 0xFD)
{
}
ABSOLUTE_Y(SBC, 0xF9)
{
}
INDIRECT_X(SBC, 0xE1)
{
}
INDIRECT_Y(SBC, 0xF1)
{
}
IMPLIED(SEC, 0x38)
{
    C = 1;
}
IMPLIED(SED, 0xF8)
{
    D = 1;
}
IMPLIED(SEI, 0x78)
{
    I = 1;
}
ZERO_PAGE(STA, 0x85)
{
    ZeroPageWrite(A);
}
ZERO_PAGE_X(STA, 0x95)
{
    ZeroPageXWrite(A);
}
ABSOLUTE(STA, 0x8D)
{
    AbsoluteWrite(A);
}
ABSOLUTE_X(STA, 0x9D)
{
    AbsoluteXWrite(A);
}
ABSOLUTE_Y(STA, 0x99)
{
    AbsoluteYWrite(A);
}
INDIRECT_X(STA, 0x81)
{
    IndexedIndirectWrite(A);
}
INDIRECT_Y(STA, 0x91)
{
    IndirectIndexedWrite(A);
}
ZERO_PAGE(STX, 0x86)
{
    ZeroPageWrite(X);
}
ZERO_PAGE_Y(STX, 0x96)
{
    ZeroPageYWrite(X);
}
ABSOLUTE(STX, 0x8E)
{
    AbsoluteWrite(X);
}
ZERO_PAGE(STY, 0x84)
{
    ZeroPageWrite(Y);
}
ZERO_PAGE_X(STY, 0x94)
{
    ZeroPageXWrite(Y);
}
ABSOLUTE(STY, 0x8C)
{
    AbsoluteWrite(Y);
}
IMPLIED(TAX, 0xAA)
{
    X = A;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
IMPLIED(TAY, 0xA8)
{
    Y = A;

    Z = Y == 0;
    N = (Y & 0x80) != 0;
}
IMPLIED(TSX, 0xBA)
{
    X = S;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
IMPLIED(TXA, 0x8A)
{
    A = X;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
IMPLIED(TXS, 0x9A)
{
    S = X;
}
IMPLIED(TYA, 0x98)
{
    A = Y;

    Z = A == 0;
    N = (A & 0x80) != 0;
}

#undef OPERATION

// **********************************************
// **********************************************
// Operation map
// **********************************************
// **********************************************

std::map<u8, Cpu::OperationCode> Cpu::ConstructOperations()
{
#define OPERATION(OP, CODE, MODE) {(u8) CODE, &Cpu::OP##_##MODE},

    return std::map<u8, Cpu::OperationCode>({ALL_OPS{(u8) 0xEA, &Cpu::NOP_IMPLIED}});

#undef OPERATION
}
