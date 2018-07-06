#ifndef ORDER
#define ORDER

#include "MipsHead.h"
#include "Reader.h"

class Program;
class Order;

enum ArgType
{
    REG, IMM, STR, LABEL, ADDRESS
};

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

union ULLl
{
    long long ll;
    int i[2];
    unsigned long long ull;
    unsigned int ui[2];
};

class Arg
{
    friend class Program;
private:
    ArgType type;
    int r;
    Word v;
    string str;
    static int GetReg(const string &s);

public:
    Arg(const string &s, OrderType orderType,  bool isStr = false);
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
    Order(Reader &reader, const string &first, bool isLabel = false);
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
    Instruction();
    void WriteMemory(char *memory, int &ptr);
};

#endif // !ORDER

