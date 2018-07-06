#ifndef INSTRUCTION
#define INSTRUCTION

#include "MipsHead.h"
#include "Reader.h"

enum InstructionType
{
    LI,
    ADD, SUB, MUL, DIV, XOR, REM, NEG,
    ADDU, SUBU, MULU, DIVU, XORU, REMU, NEGU, ADDIU,
    SEQ, SGE, SGT, SLE, SLT, SNE,
    B, BEQ, BNE, BGE, BLE, BGT, BLT, BEQZ, BNEZ, BLEZ, BGEZ, BGTZ, BLTZ,
    J, JR, JAL, JALR,
    LA, LB, LH, LW,
    SB, SH, SW,
    MOVE, MFHI, MFLO,
    NOP,
    SYSCALL
};

union Word
{
    int i;
    unsigned int ui;
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
    unsigned long long ull;
    char c[8];
};

class Instruction
{
private:
    InstructionType type;
    Transcode tr;

public:
    Instruction(Reader &reader, const string &name);
    //Instruction();
    void WriteMemory(char *memory, int &ptr);
};

#endif // !INSTRUCTION

