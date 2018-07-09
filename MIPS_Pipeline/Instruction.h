#ifndef INSTRUCTION
#define INSTRUCTION

#include "MipsHead.h"
#include "Reader.h"

enum InstructionType
{
    LI,
    ADD, SUB, MUL, DIV, REM, XOR,
    SEQ, SNE, SLE, SLT, SGE, SGT,
    ADDU, SUBU, MULU, DIVU, REMU, XORU,
    MULSUB, DIVSUB, MULUSUB, DIVUSUB,
    NEG, NEGU,
    B, BEQ, BNE, BLE, BLT, BGE, BGT, BEQZ, BNEZ, BLEZ, BLTZ, BGEZ, BGTZ,
    JR, JAL, JALR,
    LA, LB, LH, LW,
    SB, SH, SW,
    MOVE, MFHI, MFLO,
    NOP,
    SYSCALL
};

union Word
{
    int i;
    UI ui;
    short s[2];
    char c[4];
};

union Half
{
    short s;
    char c[2];
};

union Transcode
{
    ULL ull;
    char c[8];
};

class Instruction
{
private:
    Transcode tr;

public:
    ULL rs, rt, rd, imm, name, u, address, jumpto;
    int offset;

    Instruction();
    Instruction(Reader &reader, const string &name);
    Instruction(char *memory, int ptr);
    void WriteMemory(char *memory, int &ptr);
};

#endif // !INSTRUCTION

