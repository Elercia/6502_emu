#include "Cpu.hpp"

#include <iostream>

Cpu::OperationMap Cpu::OpCodes = ConstructOperations();

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
    u16 stackPtr = StackStartAddr | (u16) S;
    (*ram)[stackPtr] = value;
    S--;
}

u8 Cpu::PopStack()
{
    S++;
    u16 stackPtr = StackStartAddr | (u16) S;
    return (*ram)[stackPtr];
}

u8 Cpu::GetProcStatus(bool forceBreak /*= false*/)
{
    u8 P = (((u8) C) << 0) | (((u8) Z) << 1) | (((u8) I) << 2) | (((u8) D) << 3) | (((u8) 1) << 5) |
           (((u8) V) << 6) | ((u8) N << 7);

    if (forceBreak)
    {
        P |= (((u8) 1) << 4);
    }

    return P;
}

void Cpu::SetProcStatus(u8 status)
{
    C = (status & 0X01) != 0;
    Z = (status & 0X02) != 0;
    I = (status & 0X04) != 0;
    D = (status & 0X08) != 0;
    V = (status & 0X40) != 0;
    N = (status & 0X80) != 0;
}

void Cpu::Run()
{
    int nbOperationDone = 0;
    u8 opCode = 0;
    while (true)
    {
        u16 opPC = PC;
        opCode = ReadOneByte();

        auto opIt = OpCodes.find(opCode);
        if (opIt == OpCodes.end())
        {
            ASSERT_NOT_REACHED();
            break;
        }

        printf("Doing $%04x %s\n", opPC, opIt->second.first);

        (this->*(opIt->second.second))();

        if (PC == opPC)  // We are in a trap
        {
            ASSERT_NOT_REACHED();
        }

        nbOperationDone++;
    }
}

void Cpu::Dump()
{
    printf("\n\n===========\n");
    printf("Registries :\n");
    printf("\tAccumulator : 0x%02x\n", A);
    printf("\tIndex Register : 0x%02x\n", X);
    printf("\tIndex Register Y : 0x%02x\n", Y);
    printf("\tStack pointer : 0x%02x\n", S);
    printf("\tProgram counter : 0x%04x\n", PC);

    printf("Flags :\n");
    printf("\tCarry Flag : %d\n", C);
    printf("\tZero Flag : %d\n", Z);
    printf("\tInterrupt Disable : %d\n", I);
    printf("\tDecimal Mode : %d\n", D);
    printf("\tOverflow Flag : %d\n", V);
    printf("\tNegative Flag : %d\n", N);

    printf("===========\n\n");

    // std::cout << "Cycles " << cycles << std::endl;

    // std::cout << "Memory : " << std::endl;
    // ram->Dump();
}

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
    return ReadAt((0x0000 | (u16) offset) + (u16) X);
}

void Cpu::ZeroPageXWrite(u8 data)
{
    u8 offset = ReadOneByte();
    Write((0x0000 | (u16) offset) + (u16) X, data);
}

u8 Cpu::ZeroPageY()
{
    u8 offset = ReadOneByte();
    return ReadAt((0x0000 | (u16) offset) + (u16) Y);
}

void Cpu::ZeroPageYWrite(u8 data)
{
    u8 offset = ReadOneByte();
    Write((0x0000 | (u16) offset) + (u16) Y, data);
}

u16 Cpu::Relative()
{
    u8 offset = ReadOneByte();
    i8 signedOffset = (i8) offset;
    u16 newValue = (u16)((i16) PC + (i16) signedOffset);

    return newValue;
}

u8& Cpu::Absolute()
{
    return ReadAt(Absolute16());
}

u16 Cpu::Absolute16()
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    return ((u16) offset1) | ((u16) offset2 << 8);
}

void Cpu::AbsoluteWrite(u8 data)
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    Write(((u16) offset1) | ((u16) offset2 << 8), data);
}

u8& Cpu::AbsoluteX()
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    u16 fullOffset = ((u16) offset1) | ((u16) offset2 << 8);

    return ReadAt(fullOffset + (u16) X);
}

void Cpu::AbsoluteXWrite(u8 data)
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    u16 fullOffset = ((u16) offset1) | ((u16) offset2 << 8);

    Write(fullOffset + (u16) X, data);
}

u8 Cpu::AbsoluteY()
{
    u8 offset1 = ReadOneByte();
    u8 offset2 = ReadOneByte();
    u16 fullOffset = ((u16) offset1) | ((u16) offset2 << 8);

    return ReadAt(fullOffset + (u16) Y);
}

void Cpu::AbsoluteYWrite(u8 data)
{
    u8 low = ReadOneByte();
    u8 high = ReadOneByte();
    u16 fullOffset = ((u16) low) | ((u16) high << 8);

    Write(fullOffset + (u16) Y, data);
}

u16 Cpu::Indirect()
{
    u8 m1 = ReadOneByte();
    u8 m2 = ReadOneByte();

    u16 indirectLoc = ((u16) m1) | ((u16) m2 << 8);

    u8 v1 = ReadAt(indirectLoc);
    u8 v2 = ReadAt(indirectLoc + 1);

    return (u16) v1 | (((u16) v2) << 8);
}

u8 Cpu::IndirectX()
{
    u8 offset = ReadOneByte();
    offset += X;

    return offset;
}

void Cpu::IndirectXWrite(u8 data)
{
    u8 offset = ReadOneByte();
    offset += X;

    Write(offset, data);
}

u8 Cpu::IndirectY()
{
    u8 offset = ReadOneByte();
    offset += Y;

    return offset;
}

void Cpu::IndirectYWrite(u8 data)
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
    u8 m = Immediate();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}

ZERO_PAGE(ADC, 0x65)
{
    u8 m = ZeroPage();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ZERO_PAGE_X(ADC, 0x75)
{
    u8 m = ZeroPageX();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ABSOLUTE(ADC, 0x6D)
{
    u8 m = Absolute();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ABSOLUTE_X(ADC, 0x7D)
{
    u8 m = AbsoluteX();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ABSOLUTE_Y(ADC, 0x79)
{
    u8 m = AbsoluteY();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
INDIRECT_X(ADC, 0x61)
{
    u8 m = IndirectX();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
INDIRECT_Y(ADC, 0x71)
{
    u8 m = IndirectY();

    u16 res = (u16) A + (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
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
    u8 m = IndirectX();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_Y(AND, 0x31)
{
    u8 m = IndirectY();
    A = A & m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ACCUMULATOR(ASL, 0x0A)
{
    C = (A & 0x80) != 0;

    A = A << 1;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(ASL, 0x06)
{
    u8& m = ZeroPage();
    C = (m & 0x80) != 0;

    m = m << 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ZERO_PAGE_X(ASL, 0x16)
{
    u8& m = ZeroPageX();
    C = (m & 0x80) != 0;

    m = m << 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE(ASL, 0x0E)
{
    u8& m = Absolute();
    C = (m & 0x80) != 0;

    m = m << 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE_X(ASL, 0x1E)
{
    u8& m = AbsoluteX();
    C = (m & 0x80) != 0;

    m = m << 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
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
    u8 m = ZeroPage();

    u8 AandM = A & m;

    Z = AandM == 0;
    N = 0x80 & m;
    V = 0x40 & m;
}
ABSOLUTE(BIT, 0x2C)
{
    u8 m = Absolute();

    u8 AandM = A & m;

    Z = AandM == 0;
    N = 0x80 & m;
    V = 0x40 & m;
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
IMPLIED(BRK, 0x00)
{
    u16 loc = (ReadAt(0xFFFE)) | (ReadAt(0xFFFF) << 8);

    u16 data = PC + 1;
    PushOnStack((u8)(data >> 8));
    PushOnStack((u8)(data));

    PushOnStack(GetProcStatus(true));

    I = 1;

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
    u8 m = Immediate();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ZERO_PAGE(CMP, 0xC5)
{
    u8 m = ZeroPage();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ZERO_PAGE_X(CMP, 0xD5)
{
    u8 m = ZeroPageX();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ABSOLUTE(CMP, 0xCD)
{
    u8 m = Absolute();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ABSOLUTE_X(CMP, 0xDD)
{
    u8 m = AbsoluteX();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ABSOLUTE_Y(CMP, 0xD9)
{
    u8 m = AbsoluteY();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
INDIRECT_X(CMP, 0xC1)
{
    u8 m = IndirectX();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
INDIRECT_Y(CMP, 0xD1)
{
    u8 m = IndirectY();

    u8 res = A - m;

    C = A >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
IMMIDIATE(CPX, 0xE0)
{
    u8 m = Immediate();

    u8 res = X - m;

    C = X >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ZERO_PAGE(CPX, 0xE4)
{
    u8 m = ZeroPage();

    u8 res = X - m;

    C = X >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ABSOLUTE(CPX, 0xEC)
{
    u8 m = Absolute();

    u8 res = X - m;

    C = X >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
IMMIDIATE(CPY, 0xC0)
{
    u8 m = Immediate();

    u8 res = Y - m;

    C = Y >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ZERO_PAGE(CPY, 0xC4)
{
    u8 m = ZeroPage();

    u8 res = Y - m;

    C = Y >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
}
ABSOLUTE(CPY, 0xCC)
{
    u8 m = Absolute();

    u8 res = Y - m;

    C = Y >= m;
    Z = res == 0;
    N = (0x80 & res) != 0;
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
    u8 m = Immediate();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(EOR, 0x45)
{
    u8 m = ZeroPage();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE_X(EOR, 0x55)
{
    u8 m = ZeroPageX();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE(EOR, 0x4D)
{
    u8 m = Absolute();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_X(EOR, 0x5D)
{
    u8 m = AbsoluteX();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_Y(EOR, 0x59)
{
    u8 m = AbsoluteY();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_X(EOR, 0x41)
{
    u8 m = IndirectX();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_Y(EOR, 0x51)
{
    u8 m = IndirectY();

    A = A ^ m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(INC, 0xE6)
{
    u8& m = ZeroPage();

    m += 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ZERO_PAGE_X(INC, 0xF6)
{
    u8& m = ZeroPageX();

    m += 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE(INC, 0xEE)
{
    u8& m = Absolute();

    m += 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE_X(INC, 0xFE)
{
    u8& m = AbsoluteX();

    m += 1;

    Z = m == 0;
    N = (m & 0x80) != 0;
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
    u16 m = Absolute16();
    PC = m;
}
INDIRECT(JMP, 0x6C)
{
    u16 m = Indirect();
    PC = m;
}
ABSOLUTE(JSR, 0x20)
{
    u16 data = PC + 1;
    PushOnStack((u8)(data >> 8));
    PushOnStack((u8)(data));

    PC = Absolute16();
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
    u8 m = IndirectX();

    A = m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_Y(LDA, 0xB1)
{
    u8 m = IndirectY();

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
    u8 m = ZeroPageY();

    X = m;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
ABSOLUTE(LDX, 0xAE)
{
    u8 m = Absolute();

    X = m;

    Z = X == 0;
    N = (X & 0x80) != 0;
}
ABSOLUTE_Y(LDX, 0xBE)
{
    u8 m = AbsoluteY();

    X = m;

    Z = X == 0;
    N = (X & 0x80) != 0;
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
    u8 m = A;
    C = (m & 0x01) != 0;

    m = m >> 1;

    Z = m == 0;
    N = 0;
}
ZERO_PAGE(LSR, 0x46)
{
    u8& m = ZeroPage();
    C = (m & 0x01) != 0;

    m = m >> 1;

    Z = m == 0;
    N = 0;
}
ZERO_PAGE_X(LSR, 0x56)
{
    u8& m = ZeroPageX();
    C = (m & 0x01) != 0;

    m = m >> 1;

    Z = m == 0;
    N = 0;
}
ABSOLUTE(LSR, 0x4E)
{
    u8& m = Absolute();
    C = (m & 0x01) != 0;

    m = m >> 1;

    Z = m == 0;
    N = 0;
}
ABSOLUTE_X(LSR, 0x5E)
{
    u8& m = AbsoluteX();
    C = (m & 0x01) != 0;

    m = m >> 1;

    Z = m == 0;
    N = 0;
}
IMPLIED(NOP, 0xEA)
{
    // noop noop
}
IMMIDIATE(ORA, 0x09)
{
    u8 m = Immediate();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(ORA, 0x05)
{
    u8 m = ZeroPage();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE_X(ORA, 0x15)
{
    u8 m = ZeroPageX();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE(ORA, 0x0D)
{
    u8 m = Absolute();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_X(ORA, 0x1D)
{
    u8 m = AbsoluteX();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ABSOLUTE_Y(ORA, 0x19)
{
    u8 m = AbsoluteY();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_X(ORA, 0x01)
{
    u8 m = IndirectX();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
INDIRECT_Y(ORA, 0x11)
{
    u8 m = IndirectY();
    A = A | m;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
IMPLIED(PHA, 0x48)
{
    PushOnStack(A);
}
IMPLIED(PHP, 0x08)
{
    u8 procStatus = GetProcStatus(true);
    PushOnStack(procStatus);
}
IMPLIED(PLA, 0x68)
{
    A = PopStack();

    Z = A == 0;
    N = (A & 0x80) != 0;
}
IMPLIED(PLP, 0x28)
{
    u8 data = PopStack();
    SetProcStatus(data);
}
ACCUMULATOR(ROL, 0x2A)
{
    C = (A & 0x80) != 0;

    A = A << 1 | (u8) C;

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(ROL, 0x26)
{
    u8& m = ZeroPage();

    C = (m & 0x80) != 0;

    m = m << 1 | (u8) C;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ZERO_PAGE_X(ROL, 0x36)
{
    u8& m = ZeroPageX();

    C = (m & 0x80) != 0;

    m = m << 1 | (u8) C;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE(ROL, 0x2E)
{
    u8& m = Absolute();

    C = (m & 0x80) != 0;

    m = m << 1 | (u8) C;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE_X(ROL, 0x3E)
{
    u8& m = AbsoluteX();

    C = (m & 0x80) != 0;

    m = m << 1 | (u8) C;

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ACCUMULATOR(ROR, 0x6A)
{
    C = (A & 0x80) != 0;

    A = A >> 1 | ((u8) C << 7);

    Z = A == 0;
    N = (A & 0x80) != 0;
}
ZERO_PAGE(ROR, 0x66)
{
    u8& m = ZeroPage();

    C = (m & 0x80) != 0;

    m = m << 1 | ((u8) C << 7);

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ZERO_PAGE_X(ROR, 0x76)
{
    u8& m = ZeroPageX();

    C = (m & 0x80) != 0;

    m = m << 1 | ((u8) C << 7);

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE(ROR, 0x6E)
{
    u8& m = Absolute();

    C = (m & 0x80) != 0;

    m = m << 1 | ((u8) C << 7);

    Z = m == 0;
    N = (m & 0x80) != 0;
}
ABSOLUTE_X(ROR, 0x7E)
{
    u8& m = AbsoluteX();

    C = (m & 0x80) != 0;

    m = m << 1 | ((u8) C << 7);

    Z = m == 0;
    N = (m & 0x80) != 0;
}
IMPLIED(RTI, 0x40)
{
    SetProcStatus(PopStack());

    u8 m1 = PopStack();
    u8 m2 = PopStack();

    PC = ((u16) m1) | ((u16) m2 << 8);
}
IMPLIED(RTS, 0x60)
{
    u8 m1 = PopStack();
    u8 m2 = PopStack();

    PC = ((u16) m1) | ((u16) m2 << 8);

    PC++;
}
IMMIDIATE(SBC, 0xE9)
{
    u8 m = Immediate();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ZERO_PAGE(SBC, 0xE5)
{
    u8 m = ZeroPage();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ZERO_PAGE_X(SBC, 0xF5)
{
    u8 m = ZeroPageX();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ABSOLUTE(SBC, 0xED)
{
    u8 m = Absolute();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ABSOLUTE_X(SBC, 0xFD)
{
    u8 m = AbsoluteX();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
ABSOLUTE_Y(SBC, 0xF9)
{
    u8 m = AbsoluteY();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
INDIRECT_X(SBC, 0xE1)
{
    u8 m = IndirectX();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
}
INDIRECT_Y(SBC, 0xF1)
{
    u8 m = IndirectY();

    u16 res = (u16) A - (u16) m;
    u8 newA = (u8)(res & 0x00FF);

    V = (~(A ^ m)) & (A ^ newA) & 0x80;

    A = newA;

    C = (res & 0xFF00) != 0;
    Z = A == 0;
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
    IndirectXWrite(A);
}
INDIRECT_Y(STA, 0x91)
{
    IndirectYWrite(A);
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

Cpu::OperationMap Cpu::ConstructOperations()
{
#define OPERATION(OP, CODE, MODE) {(u8) CODE, {#OP " " #MODE, &Cpu::OP##_##MODE}},

    return Cpu::OperationMap({ALL_OPS{(u8) 0xEA, {"", &Cpu::NOP_IMPLIED}}});

#undef OPERATION
}
