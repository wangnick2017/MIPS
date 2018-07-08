#include "Instruction.h"

Instruction::Instruction(Reader &reader, const string &name)
{
    string s1, s2, s3;
    reader.ReadString(s1);
    reader.ReadString(s2);
    bool haveS3 = reader.ReadString(s3);
    ULL u = IName[name];
    if ((u == MUL || u == MULU || u == DIV || u == DIVU) && !haveS3)
        u = IName[name + "sub"];
    switch (u)
    {
    case LI:
        tr.ull = (RName[s1] << 18) | ((ULL)stoi(s2) << 32);
        break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    case REM:
    case XOR:
    case SEQ:
    case SNE:
    case SLE:
    case SLT:
    case SGE:
    case SGT:
        tr.ull = s3[0] != '$' ? 1ull | (RName[s1] << 18) | (RName[s2] << 6) | (u << 24) | ((ULL)stoi(s3) << 32)
            : 2ull | (RName[s1] << 18) | (RName[s2] << 6) | (RName[s3] << 12) | (u << 24);
        break;
    case ADDU:
    case SUBU:
    case MULU:
    case DIVU:
    case REMU:
    case XORU:
        tr.ull = s3[0] != '$' ? 1ull | (RName[s1] << 18) | (RName[s2] << 6) | (u << 24) | (stoull(s3) << 32)
            : 2ull | (RName[s1] << 18) | (RName[s2] << 6) | (RName[s3] << 12) | (u << 24);
        break;
    case MULSUB:
    case DIVSUB:
        tr.ull = s2[0] != '$' ? 3ull | (RName[s1] << 6) | (u << 24) | ((ULL)stoi(s2) << 32)
            : 4ull | (RName[s1] << 6) | (RName[s2] << 12) | (u << 24);
        break;
    case MULUSUB:
    case DIVUSUB:
        tr.ull = s2[0] != '$' ? 3ull | (RName[s1] << 6) | (u << 24) | (stoull(s2) << 32)
            : 4ull | (RName[s1] << 6) | (RName[s2] << 12) | (u << 24);
        break;
    case NEG:
    case NEGU:
        tr.ull = 4ull | (RName[s1] << 6) | (RName[s2] << 12) | (u << 24);
        break;
    case B:
        tr.ull = 5ull | ((ULL)jumpers[s1] << 18);
        break;
    case BEQ:
    case BNE:
    case BLE:
    case BLT:
    case BGE:
    case BGT:
        tr.ull = s2[0] != '$' ? (u - 20) | (RName[s1] << 6) | ((ULL)jumpers[s3] << 18) | ((ULL)stoi(s2) << 32)
            : (u - 14) | (RName[s1] << 6) | (RName[s2] << 12) | ((ULL)jumpers[s3] << 18);
        break;
    case BEQZ:
    case BNEZ:
    case BLEZ:
    case BLTZ:
    case BGEZ:
    case BGTZ:
        tr.ull = (u - 14) | (RName[s1] << 6) | ((ULL)jumpers[s2] << 18);
        break;
    case JR:
        tr.ull = 24ull | (RName[s1] << 6);
        break;
    case JAL:
        tr.ull = 25ull | ((ULL)jumpers[s1] << 18);
        break;
    case JALR:
        tr.ull = 26ull | (RName[s1] << 6);
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
        tr.ull = 27ull | (RName[s1] << (u < SB ? 18 : 6)) | (u << 24) | (address << 32);
        break;
    }
    case MOVE:
        tr.ull = 28ull | (RName[s1] << 18) | (RName[s2] << 6);
        break;
    case MFHI:
        tr.ull = 29ull | (RName[s1] << 18);
        break;
    case MFLO:
        tr.ull = 30ull | (RName[s1] << 18);
        break;
    case NOP:
        tr.ull = 31;
        break;
    case SYSCALL:
        tr.ull = 32;
        break;
    }
}

Instruction::Instruction(char *memory, int ptr)
{
    for (int i = 0; i < 8; ++i)
        tr.c[i] = memory[ptr + i];
    ULL u = tr.ull & 0x3Full;
    rs = tr.ull & 0xFC0ull;
    rt = tr.ull & 0x3F000ull;
    imm = tr.ull & 0xFFFFFFFF00000000ull;
    switch (u)
    {
    case 0:
        name = 0;
        rd = tr.ull & 0xFC0000ull;
        break;
    }
}

void Instruction::WriteMemory(char *memory, int &ptr)
{
    for (int i = 0; i < 8; ++i)
        memory[ptr++] = tr.c[i];
}