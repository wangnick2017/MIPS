#include "Instruction.h"

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

Instruction::Instruction(Reader &reader, const string &name)
{
    string s1, s2, s3;
    reader.ReadString(s1);
    reader.ReadString(s2);
    bool haveS3 = reader.ReadString(s3);
    ULL u = IName[name];
    unsigned long long u1, u2, u3;
    switch (u)
    {
    case LI:
        tr.ull = (RName[s1] << 6) | ((ULL)stoi(s2) << 11);
        break;
    case ADD:
    case SUB:
    case XOR:
    case REM:
        tr.ull = s3[0] != '$' ? u | (RName[s1] << 6) | (RName[s2] << 11) | ((ULL)stoi(s3) << 16)
            : 25ull | (RName[s1] << 6) | (RName[s2] << 11) | (RName[s3] << 16) | (u << 21);
        break;
    case MUL:
    case DIV:
        if (haveS3)
            tr.ull = s3[0] != '$' ? u | (RName[s1] << 6) | (RName[s2] << 11) | ((ULL)stoi(s3) << 16)
                : 25ull | (RName[s1] << 6) | (RName[s2] << 11) | (RName[s3] << 16) | (u << 21);
        else
            tr.ull = s3[0] != '$' ? (u + 4) | (RName[s1] << 11) | ((ULL)stoi(s2) << 16)
                : 25ull | (RName[s1] << 11) | (RName[s2] << 16) | ((u + 4) << 21);
        break;
    case NEG:
        tr.ull = 9ull | (RName[s1] << 6) | (RName[s2] << 11);
        break;
    case ADDU:
    case SUBU:
    case XORU:
    case REMU:
        tr.ull = s3[0] != '$' ? (u + 2) | (RName[s1] << 6) | (RName[s2] << 11) | (stoull(s3) << 16)
            : 25ull | (RName[s1] << 6) | (RName[s2] << 11) | (RName[s3] << 16) | ((u + 2) << 21);
        break;
    case MULU:
    case DIVU:
        if (haveS3)
            tr.ull = s3[0] != '$' ? (u + 2) | (RName[s1] << 6) | (RName[s2] << 11) | (stoull(s3) << 16)
            : 25ull | (RName[s1] << 6) | (RName[s2] << 11) | (RName[s3] << 16) | ((u + 2) << 21);
        else
            tr.ull = s3[0] != '$' ? (u + 6) | (RName[s1] << 11) | (stoull(s2) << 16)
            : 25ull | (RName[s1] << 11) | (RName[s2] << 16) | ((u + 6) << 21);
        break;
    case NEGU:
        tr.ull = 18ull | (RName[s1] << 6) | (RName[s2] << 11);
        break;
    case ADDIU:
        tr.ull = 10ull | (RName[s1] << 6) | (RName[s2] << 11) | (stoull(s3) << 16);
        break;
    case SEQ:
    case SGE:
    case SGT:
    case SLE:
    case SLT:
    case SNE:
        tr.ull = s3[0] != '$' ? (u + 3) | (RName[s1] << 6) | (RName[s2] << 11) | ((ULL)stoi(s3) << 16)
            : 25ull | (RName[s1] << 6) | (RName[s2] << 11) | (RName[s3] << 16) | ((u + 3) << 21);
        break;
    case B:
    case J:
        tr.ull = 26ull | ((ULL)jumpers[s1] << 6);
        break;
    case BEQ:
    case BNE:
    case BGE:
    case BLE:
    case BGT:
    case BLT:
    {
        tr.ull = s2[0] != '$' ? (u + 4) | (RName[s1] << 6) | ((ULL)stoi(s2) << 11) | ((ULL)stoi(s3) << 30)
            : (u + 10) | (RName[s1] << 6) | (RName[s2] << 11) | ((ULL)stoi(s3) << 30);
        break;
    }
    case BEQZ:
    case BNEZ:
    case BLEZ:
    case BGEZ:
    case BGTZ:
    case BLTZ:
    {
        tr.ull = (u + 10) | (RName[s1] << 6) | ((ULL)stoi(s2) << 11);
        break;
    }
    case JR:
    {
        break;
    }
    case JAL:
    {
        break;
    }
    case JALR:
    {
        break;
    }
    case LA:
    {
        break;
    }
    case LB:
    {
        break;
    }
    case LH:
    {
        break;
    }
    case LW:
    {
        break;
    }
    case SB:
    {
        break;
    }
    case SH:
    {
        break;
    }
    case SW:
    {
        break;
    }
    case MOVE:
    {
        break;
    }
    case MFHI:
    {
        break;
    }
    case MFLO:
    {
        break;
    }
    case NOP:
    {
        break;
    }
    case SYSCALL:
    {
        break;
    }
    }
}

void Instruction::WriteMemory(char *memory, int &ptr)
{
    for (int i = 0; i < 6; ++i)
        memory[ptr++] = tr.c[i];
}