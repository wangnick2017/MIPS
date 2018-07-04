#ifndef ORDER
#define ORDER

#include "MipsHead.h"

class Program;

enum ArgType
{
    REG, IMM, STR, LABEL, ADDRESS
};

enum OrderType
{
    __ALIGN,
    __ASCII, __ASCIIZ,
    __BYTE, __HALF, __WORD, __SPACE,
    __DATA, __TEXT,

    ADD,
    SUB,
    MUL, MUL_SUB,
    DIV, DIV_SUB,
    XOR,
    NEG,
    REM,

    ADDU, ADDIU,
    SUBU,
    MULU, MULU_SUB,
    DIVU, DIVU_SUB,
    XORU,
    NEGU,
    REMU,

    LI,
    SEQ, SGE, SGT, SLE, SLT, SNE,
    B, BEQ, BNE, BGE, BLE, BGT, BLT, BEQZ, BNEZ, BLEZ, BGEZ, BGTZ, BLTZ,
    J, JR, JAL, JALR,
    LA, LB, LH, LW,
    SB, SH, SW,
    MOVE, MFHI, MFLO,
    NOP,
    SYSCALL,
    __LABEL
};


class Arg
{
    friend class Program;
private:
    ArgType type;
    int r;
    int ll;
    unsigned int ull;
    string str;
    static int GetReg(const string &s);

public:
    Arg(const string &s, OrderType orderType,  bool isStr = false);
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
    void ReadArgs(vector<Arg> &args, OrderType orderType);
};

class Order
{
    friend class Program;
private:
    OrderType type;
    vector<Arg> args;
    static OrderType GetType(const string &s, const string &all);

public:
    OrderType Type();
    Order(const string &s, bool isLabel = false);
};

#endif // !ORDER

