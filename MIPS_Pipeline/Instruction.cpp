#include "Instruction.h"

Instruction::Instruction(Reader &reader, const string &name)
{
    string s1, s2, s3;
    reader.ReadString(s1);
    reader.ReadString(s2);
    bool haveS3 = reader.ReadString(s3);
    ULL u = IName[name];
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
        tr.ull = s2[0] != '$' ? (u + 4) | (RName[s1] << 6) | ((ULL)stoi(s2) << 11) | ((ULL)jumpers[s3] << 30)
            : (u + 10) | (RName[s1] << 6) | (RName[s2] << 11) | ((ULL)jumpers[s3] << 30);
        break;
    case BEQZ:
    case BNEZ:
    case BLEZ:
    case BGEZ:
    case BGTZ:
    case BLTZ:
        tr.ull = (u + 10) | (RName[s1] << 6) | ((ULL)jumpers[s2] << 11);
        break;
    case JR:
        tr.ull = 45ull | (RName[s1] << 6);
        break;
    case JAL:
        tr.ull = 46ull | ((ULL)jumpers[s1] << 6);
        break;
    case JALR:
        tr.ull = 47ull | (RName[s1] << 6);
        break;
    case LA:
    case LB:
    case LH:
    case LW:
    case SB:
    case SH:
    case SW:
    {
        ULL address, leftBracket;
        if ((leftBracket = s2.find('(')) != string::npos)
        {
            int offset = leftBracket == 0 ? 0 : stoi(s2.substr(0, leftBracket));
            address = RName[s2.substr(leftBracket + 1, s2.length() - leftBracket - 2)] + offset;
        }
        else
            address = pointers[s2];
        tr.ull = (u + 9) | (RName[s1] << 6) | (address << 11);
        break;
    }
    case MOVE:
        tr.ull = 55ull | (RName[s1] << 6) | (RName[s2] << 11);
        break;
    case MFHI:
        tr.ull = 56ull | (RName[s1] << 6);
        break;
    case MFLO:
        tr.ull = 57ull | (RName[s1] << 6);
        break;
    case NOP:
        tr.ull = 58;
        break;
    case SYSCALL:
        tr.ull = 59;
        break;
    }
}

void Instruction::WriteMemory(char *memory, int &ptr)
{
    for (int i = 0; i < 6; ++i)
        memory[ptr++] = tr.c[i];
}