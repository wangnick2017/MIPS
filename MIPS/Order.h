#ifndef ORDER
#define ORDER

#include "MipsHead.h"

enum ArgType
{
    REG, IMM, STR, LABEL, ADDRESS
};

class Arg
{
private:
    ArgType type;
    int c, r;
    string str;

public:
    static int GetReg(const string &s);
    Arg(const string &s, bool isStr = false);
};

class Reader
{
private:
    const string &input;
    int pos, len;
public:
    Reader(const string &s);
    string ReadString();
    string ReadFormatString();
    void ReadArgs(vector<Arg> &args);
};

enum OrderType
{
    __ALIGN,
    __ASCII, __ASCIIZ,
    __BYTE, __HALF, __WORD, __SPACE,
    __DATA, __TEXT,
    ADD, ADDU, ADDIU,
    SUB, SUBU,
    MUL, MULU, MUL_SUB, MULU_SUB,
    DIV, DIVU, DIV_SUB, DIVU_SUB,
    XOR, XORU,
    NEG, NEGU,
    REM, REMU,
    LI,
    SEQ, SGE, SGT, SLE, SLT, SNE,
    B, BEQ, BNE, BGE, BLE, BGT, BLT, BEQZ, BNEZ, BLEZ, BGEZ, BGTZ, BLTZ,
    J, JR, JAL, JALR,
    LA, LB, LH, LW,
    SB, SH, SW,
    MOVE, MFHI, MFLO,
    NOP,
    SYSCALL
};

class Order
{
private:
    OrderType type;
    vector<Arg> args;

public:
    static OrderType GetType(const string &s, const string &all);
    Order(const string &s);
};

#endif // !ORDER

