#include "Order.h"

int Arg::GetReg(const string &s)
{
    if (s == "$zero")
        return 0;
    else if (s == "$at")
        return 1;
    else if (s == "$v0")
        return 2;
    else if (s == "$v1")
        return 3;
    else if (s == "$a0")
        return 4;
    else if (s == "$a1")
        return 5;
    else if (s == "$a2")
        return 6;
    else if (s == "$a3")
        return 7;
    else if (s == "$t0")
        return 8;
    else if (s == "$t1")
        return 9;
    else if (s == "$t2")
        return 10;
    else if (s == "$t3")
        return 11;
    else if (s == "$t4")
        return 12;
    else if (s == "$t5")
        return 13;
    else if (s == "$t6")
        return 14;
    else if (s == "$t7")
        return 15;
    else if (s == "$s0")
        return 16;
    else if (s == "$s1")
        return 17;
    else if (s == "$s2")
        return 18;
    else if (s == "$s3")
        return 19;
    else if (s == "$s4")
        return 20;
    else if (s == "$s5")
        return 21;
    else if (s == "$s6")
        return 22;
    else if (s == "$s7")
        return 23;
    else if (s == "$t8")
        return 24;
    else if (s == "$t9")
        return 25;
    else if (s == "$k0")
        return 26;
    else if (s == "$k1")
        return 27;
    else if (s == "$gp")
        return 28;
    else if (s == "$sp")
        return 29;
    else if (s == "$fp")
        return 30;
    else if (s == "$ra")
        return 31;
    else if (s == "$hi")
        return 32;
    else if (s == "$lo")
        return 33;
    else if (s == "$pc")
        return 34;
    else
        return stoi(s.substr(1));
}

Arg::Arg(const string &s, OrderType orderType, bool isStr)
{
    size_t leftBracket;
    if (isStr)
    {
        type = STR;
        str = s;
    }
    else if (s[0] == '$')
    {
        type = REG;
        if (s[1] >= '0' && s[1] <= '9')
            r = stoi(s.substr(1, s.length() - 1));
        else
            r = GetReg(s);
    }
    else if (s[0] == '_' || s[0] >= 'a' && s[0] <= 'z')
    {
        type = LABEL;
        str = s;
    }
    else if ((leftBracket = s.find('(')) != string::npos)
    {
        type = ADDRESS;
        v.i = leftBracket == 0 ? 0 : stoi(s.substr(0, leftBracket));
        r = GetReg(s.substr(leftBracket + 1, s.length() - leftBracket - 2));
    }
    else
    {
        type = IMM;
        if (orderType == __HALF)
            v.s[0] = stoi(s);
        else if (orderType >= ADDU && orderType <= REMU)
            v.ui = stoul(s);
        else
            v.i = stoi(s);
    }
}

Reader::Reader(const string &s) : input(s), pos(0), len(s.length())
{
}

string Reader::ReadString()
{
    while (pos < len && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == ','))
        ++pos;
    int start = pos;
    while (pos < len && input[pos] != ' ' && input[pos] != '\t' && input[pos] != '\n' && input[pos] != ',')
        ++pos;
    return input.substr(start, pos - start);
}

string Reader::ReadFormatString()
{
    while (pos < len && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == ','))
        ++pos;
    ++pos;
    string ret;
    while (input[pos] != '\"')
    {
        if (input[pos] != '\\')
            ret.push_back(input[pos]);
        else
            switch (input[++pos])
            {
            case 'n':
                ret.push_back('\n');
                break;
            case 'r':
                ret.push_back('\r');
                break;
            case 't':
                ret.push_back('\t');
                break;
            case '\\':
                ret.push_back('\\');
                break;
            case '\'':
                ret.push_back('\'');
                break;
            case '\"':
                ret.push_back('\"');
                break;
            case '\?':
                ret.push_back('\?');
                break;
            case '\0':
                ret.push_back('\0');
                break;
            }
        ++pos;
    }
    return ret;
}

void Reader::ReadArgs(vector<Arg> &args, OrderType orderType)
{
    while (pos < len)
        args.push_back(Arg(ReadString(), orderType));
}

OrderType Order::GetType(const string &s, const string &all)
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
    else //s == "syscall"
        return SYSCALL;
}

OrderType Order::Type()
{
    return type;
}

Order::Order(Reader &reader, const string &first, bool isLabel)
{
    if (isLabel)
    {
        type = __LABEL;
        args.push_back(Arg(first, type, false));
        return;
    }
    type = GetType(first, reader.input);
    if (type == __ASCII || type == __ASCIIZ)
        args.push_back(Arg(reader.ReadFormatString(), type, true));
    else
        reader.ReadArgs(args, type);
}