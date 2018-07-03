#ifndef ORDER
#define ORDER

#include "MipsHead.h"

enum ArgType
{
    REG, IMM, STR
};

class Arg
{
private:
    ArgType type;
    int c;
    string str;

public:
    Arg(const string &s)
    {
        if (s[0] == '$')
        {
            type = REG;
            if (s[1] >= '0' && s[1] <= '9')
                c = stoi(s.substr(1, s.length() - 1));
            else if (s == "$zero")
                c = 0;
            else if (s == "$at")
                c = 1;
            else if (s == "$v0")
                c = 2;
            else if (s == "$v1")
                c = 3;
            else if (s == "$a0")
                c = 4;
            else if (s == "$a1")
                c = 5;
            else if (s == "$a2")
                c = 6;
            else if (s == "$a3")
                c = 7;
            else if (s == "$t0")
                c = 8;
            else if (s == "$t1")
                c = 9;
            else if (s == "$t2")
                c = 10;
            else if (s == "$t3")
                c = 11;
            else if (s == "$t4")
                c = 12;
            else if (s == "$t5")
                c = 13;
            else if (s == "$t6")
                c = 14;
            else if (s == "$t7")
                c = 15;
            else if (s == "$s0")
                c = 16;
            else if (s == "$s1")
                c = 17;
            else if (s == "$s2")
                c = 18;
            else if (s == "$s3")
                c = 19;
            else if (s == "$s4")
                c = 20;
            else if (s == "$s5")
                c = 21;
            else if (s == "$s6")
                c = 22;
            else if (s == "$s7")
                c = 23;
            else if (s == "$t8")
                c = 24;
            else if (s == "$t9")
                c = 25;
            else if (s == "$")
                c = 26;
            else if (s == "$")
                c = 27;
            else if (s == "$gp")
                c = 28;
            else if (s == "$sp")
                c = 29;
            else if (s == "$fp")
                c = 30;
            else if (s == "$ra")
                c = 31;
        }
        else
        {

        }
    }
};

class Reader
{
private:
    const string &input;
    int pos, len;
public:
    Reader(const string &s) : input(s), pos(0), len(s.length())
    {
    }

    const string &ReadString()
    {
        while (pos < len && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == ','))
            ++pos;
        int start = pos;
        while (pos < len && input[pos] != ' ' && input[pos] != '\t' && input[pos] != '\n' && input[pos] != ',')
            ++pos;
        return input.substr(start, pos - start);
    }

    void ReadArgs(vector<Arg> &args)
    {
        while (pos < len)
            args.push_back(Arg(ReadString()));
    }
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
    static OrderType GetType(const string &s, const string &all)
    {
        if (s == ".align")
            return __ALIGN;
        else if (s == ".ascii")
            return __ASCII;
        else if (s == ".asciiz")
            return __ASCIIZ;
        else if (s == ".byte")
            return __BYTE;
        else if (s == ".half")
            return __HALF;
        else if (s == ".word")
            return __WORD;
        else if (s == ".space")
            return __SPACE;
        else if (s == ".data")
            return __DATA;
        else if (s == ".text")
            return __TEXT;
        else if (s == "add")
            return ADD;
        else if (s == "addu")
            return ADDU;
        else if (s == "addiu")
            return ADDIU;
        else if (s == "sub")
            return SUB;
        else if (s == "subu")
            return SUBU;
        else if (s == "mul" && all.find(',') != all.rfind(','))
            return MUL;
        else if (s == "mulu" && all.find(',') != all.rfind(','))
            return MULU;
        else if (s == "mul")
            return MUL_SUB;
        else if (s == "mulu")
            return MULU_SUB;
        else if (s == "div" && all.find(',') != all.rfind(','))
            return DIV;
        else if (s == "divu" && all.find(',') != all.rfind(','))
            return DIVU;
        else if (s == "div")
            return DIV_SUB;
        else if (s == "divu")
            return DIVU_SUB;
        else if (s == "xor")
            return XOR;
        else if (s == "xoru")
            return XORU;
        else if (s == "neg")
            return NEG;
        else if (s == "negu")
            return NEGU;
        else if (s == "rem")
            return REM;
        else if (s == "remu")
            return REMU;
        else if (s == "li")
            return LI;
        else if (s == "seq")
            return SEQ;
        else if (s == "sge")
            return SGE;
        else if (s == "sgt")
            return SGT;
        else if (s == "sle")
            return SLE;
        else if (s == "slt")
            return SLT;
        else if (s == "sne")
            return SNE;
        else if (s == "b")
            return B;
        else if (s == "beq")
            return BEQ;
        else if (s == "bne")
            return BNE;
        else if (s == "bge")
            return BGE;
        else if (s == "ble")
            return BLE;
        else if (s == "bgt")
            return BGT;
        else if (s == "blt")
            return BLT;
        else if (s == "beqz")
            return BEQZ;
        else if (s == "bnez")
            return BNEZ;
        else if (s == "blez")
            return BLEZ;
        else if (s == "bgez")
            return BGEZ;
        else if (s == "bgtz")
            return BGTZ;
        else if (s == "bltz")
            return BLTZ;
        else if (s == "j")
            return J;
        else if (s == "jr")
            return JR;
        else if (s == "jal")
            return JAL;
        else if (s == "jalr")
            return JALR;
        else if (s == "la")
            return LA;
        else if (s == "lb")
            return LB;
        else if (s == "lh")
            return LH;
        else if (s == "lw")
            return LW;
        else if (s == "sb")
            return SB;
        else if (s == "sh")
            return SH;
        else if (s == "sw")
            return SW;
        else if (s == "move")
            return MOVE;
        else if (s == "mfhi")
            return MFHI;
        else if (s == "mflo")
            return MFLO;
        else if (s == "nop")
            return NOP;
        else if (s == "syscall")
            return SYSCALL;
    }

    Order(const string &s)
    {
        Reader reader(s);
        type = GetType(reader.ReadString(), s);
        reader.ReadArgs(args);
    }
};

#endif // !ORDER

