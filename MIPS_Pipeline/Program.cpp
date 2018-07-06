#include "Program.h"
#include "Reader.h"

Program::Program()
{
    memory = new char[MemorySize];
}

Program::~Program()
{
    delete[] memory;
}

void Program::InitNames()
{
    RegNames["$zero"] = 0;
    RegNames["$at"] = 1;
    RegNames["$v0"] = 2;
    RegNames["$v1"] = 3;
    RegNames["$a0"] = 4;
    RegNames["$a1"] = 5;
    RegNames["$a2"] = 6;
    RegNames["$a3"] = 7;
    RegNames["$t0"] = 8;
    RegNames["$t1"] = 9;
    RegNames["$t2"] = 10;
    RegNames["$t3"] = 11;
    RegNames["$t4"] = 12;
    RegNames["$t5"] = 13;
    RegNames["$t6"] = 14;
    RegNames["$t7"] = 15;
    RegNames["$s0"] = 16;
    RegNames["$s1"] = 17;
    RegNames["$s2"] = 18;
    RegNames["$s3"] = 19;
    RegNames["$s4"] = 20;
    RegNames["$s5"] = 21;
    RegNames["$s6"] = 22;
    RegNames["$s7"] = 23;
    RegNames["$t8"] = 24;
    RegNames["$t9"] = 25;
    RegNames["$k0"] = 26;
    RegNames["$k1"] = 27;
    RegNames["$gp"] = 28;
    RegNames["$sp"] = 29;
    RegNames["$fp"] = 30;
    RegNames["$ra"] = 31;
    RegNames["$hi"] = 32;
    RegNames["$lo"] = 33;
    RegNames["$pc"] = 34;
    for (int i = 0; i <= 34; ++i)
        RegNames["$" + to_string(i)] = i;
    InsNames["li"] = LI;
    InsNames["add"] = ADD;
    InsNames["sub"] = SUB;
    InsNames["mul"] = MUL;
    InsNames["div"] = DIV;
    InsNames["xor"] = XOR;
    InsNames["neg"] = NEG;
    InsNames["rem"] = REM;
    InsNames["addu"] = ADDU;
    InsNames["addiu"] = ADDIU;
    InsNames["subu"] = SUBU;
    InsNames["mulu"] = MULU;
    InsNames["divu"] = DIVU;
    InsNames["xoru"] = XORU;
    InsNames["negu"] = NEGU;
    InsNames["remu"] = REMU;
    InsNames["seq"] = SEQ;
    InsNames["sge"] = SGE;
    InsNames["sgt"] = SGT;
    InsNames["sle"] = SLE;
    InsNames["slt"] = SLT;
    InsNames["sne"] = SNE;
    InsNames["b"] = B;
    InsNames["beq"] = BEQ;
    InsNames["bne"] = BNE;
    InsNames["bge"] = BGE;
    InsNames["ble"] = BLE;
    InsNames["bgt"] = BGT;
    InsNames["blt"] = BLT;
    InsNames["beqz"] = BEQZ;
    InsNames["bnez"] = BNEZ;
    InsNames["blez"] = BLEZ;
    InsNames["bgez"] = BGEZ;
    InsNames["bgtz"] = BGTZ;
    InsNames["bltz"] = BLTZ;
    InsNames["j"] = J;
    InsNames["jr"] = JR;
    InsNames["jal"] = JAL;
    InsNames["jalr"] = JALR;
    InsNames["la"] = LA;
    InsNames["lb"] = LB;
    InsNames["lh"] = LH;
    InsNames["lw"] = LW;
    InsNames["sb"] = SB;
    InsNames["sh"] = SH;
    InsNames["sw"] = SW;
    InsNames["move"] = MOVE;
    InsNames["mfhi"] = MFHI;
    InsNames["mflo"] = MFLO;
    InsNames["nop"] = NOP;
    InsNames["syscall"] = SYSCALL;
}

void Program::InputCodes(char *fileName)
{
    memset(memory, 0, MemorySize);
    ptr = 0;
    memset(reg, 0, sizeof(reg));
    reg[29] = MemorySize;
    ifstream codes(fileName);
    string code, tmp;
    stack<string> names;
    bool isData = false;
    vector<string> statics;
    while (!codes.eof())
    {
        getline(codes, code);
        size_t comment = code.find('#');
        if (comment != string::npos)
            code = code.erase(comment);
        Reader reader(code);
        if (!reader.ReadString(tmp))
            continue;
        if (tmp == ".data")
            isData = true;
        else if (tmp == ".text")
            isData = false;
        else if (isData)
        {
            statics.push_back(code);
        }
        else if (tmp.find(':') != string::npos)
        {
            tmp.pop_back();
            names.push(tmp);
        }
        else
        {
            while (!names.empty())
            {
                jumpers.insert(pair<string, int>(names.top(), ptr));
                names.pop();
            }
            Instruction ins(reader, tmp);
            ins.WriteMemory(memory, ptr);
        }
    }
    for (int i = 0, j = statics.size(); i < j; ++i)
    {
        Reader reader(code);
        if (!reader.ReadString(tmp))
            continue;
        if (tmp[0] != '.')
        {
            tmp.pop_back();
            names.push(tmp);
        }
        else
        {
            while (!names.empty())
            {
                pointers.insert(pair<string, int>(names.top(), ptr));
                names.pop();
            }
            int n;
            if (tmp == ".align")
            {
                reader.ReadInt(n);
                if (ptr % n != 0)
                    ptr = n * (ptr / n + 1);
            }
            else if (tmp == ".ascii")
            {
                string s;
                reader.ReadFormatString(s);
                for (int k = 0, l = s.size(); k < l; ++k)
                    memory[ptr++] = s[k];
            }
            else if (tmp == ".asciiz")
            {
                string s;
                reader.ReadFormatString(s);
                for (int k = 0, l = s.size(); k < l; ++k)
                    memory[ptr++] = s[k];
                memory[ptr++] = 0;
            }
            else if (tmp == ".byte")
            {
                while (reader.ReadInt(n))
                    memory[ptr++] = n;
            }
            else if (tmp == ".half")
            {
                Half h;
                while (reader.ReadInt(n))
                {
                    h.s = n;
                    for (int l = 0; l < 2; ++l)
                        memory[ptr++] = h.c[l];
                }
            }
            else if (tmp == ".word")
            {
                Word w;
                while (reader.ReadInt(n))
                {
                    w.i = n;
                    for (int l = 0; l < 4; ++l)
                        memory[ptr++] = w.c[l];
                }
            }
            else if (tmp == ".space")
            {
                reader.ReadInt(n);
                ptr += n;
            }
        }
    }
}

int Program::Run()
{
    int pc, ptr;
    for (pc = labels["main"]; ; ++pc)
    {
        Order &o = orders[pc];
        switch (o.Type())
        {
        case ADD:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = reg[o.args[1].r] + i;
            break;
        }
        case ADDU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.ui;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] + i);
            break;
        }
        case ADDIU:
        {
            unsigned int i = o.args[2].v.ui;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] + i);
            break;
        }
        case SUB:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = reg[o.args[1].r] - i;
            break;
        }
        case SUBU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.ui;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] - i);
            break;
        }
        case MUL:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = reg[o.args[1].r] * i;
            break;
        }
        case MULU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.ui;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] * i);
            break;
        }
        case MUL_SUB:
        {
            BigNumber a, b;
            a.ll = reg[o.args[0].r];
            b.ll = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            a.ll *= b.ll;
            reg[32] = a.i[0];
            reg[33] = a.i[1];
            break;
        }
        case MULU_SUB:
        {
            BigNumber a, b;
            a.ull = reg[o.args[0].r];
            b.ull = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.ui;
            a.ull *= b.ull;
            reg[32] = a.ui[0];
            reg[33] = a.ui[1];
            break;
        }
        case DIV:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = reg[o.args[1].r] / i;
            break;
        }
        case DIVU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.ui;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] / i);
            break;
        }
        case DIV_SUB:
        {
            int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            reg[32] = reg[o.args[0].r] % i;
            reg[33] = reg[o.args[0].r] / i;
            break;
        }
        case DIVU_SUB:
        {
            unsigned int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.ui;
            reg[32] = reg[o.args[0].r] % i;
            reg[33] = reg[o.args[0].r] / i;
            break;
        }
        case XOR:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = reg[o.args[1].r] ^ i;
            break;
        }
        case XORU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.ui;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] ^ i);
            break;
        }
        case NEG:
        {
            reg[o.args[0].r] = -reg[o.args[1].r];
            break;
        }
        case NEGU:
        {
            reg[o.args[0].r] = (unsigned int)(~(unsigned int)reg[o.args[1].r]);
            break;
        }
        case REM:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = reg[o.args[1].r] % i;
            break;
        }
        case REMU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.ui;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] % i);
            break;
        }
        case LI:
        {
            reg[o.args[0].r] = o.args[1].v.i;
            break;
        }
        case SEQ:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = (reg[o.args[1].r] == i);
            break;
        }
        case SGE:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = (reg[o.args[1].r] >= i);
            break;
        }
        case SGT:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = (reg[o.args[1].r] > i);
            break;
        }
        case SLE:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = (reg[o.args[1].r] <= i);
            break;
        }
        case SLT:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = (reg[o.args[1].r] < i);
            break;
        }
        case SNE:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].v.i;
            reg[o.args[0].r] = (reg[o.args[1].r] != i);
            break;
        }
        case B:
        case J:
        {
            pc = labels[o.args[0].str] - 1;
            break;
        }
        case BEQ:
        {
            int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            if (reg[o.args[0].r] == i)
                pc = labels[o.args[2].str] - 1;
            break;
        }
        case BNE:
        {
            int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            if (reg[o.args[0].r] != i)
                pc = labels[o.args[2].str] - 1;
            break;
        }
        case BGE:
        {
            int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            if (reg[o.args[0].r] >= i)
                pc = labels[o.args[2].str] - 1;
            break;
        }
        case BLE:
        {
            int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            if (reg[o.args[0].r] <= i)
                pc = labels[o.args[2].str] - 1;
            break;
        }
        case BGT:
        {
            int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            if (reg[o.args[0].r] > i)
                pc = labels[o.args[2].str] - 1;
            break;
        }
        case BLT:
        {
            int i = o.args[1].type == REG ? reg[o.args[1].r] : o.args[1].v.i;
            if (reg[o.args[0].r] < i)
                pc = labels[o.args[2].str] - 1;
            break;
        }
        case BEQZ:
        {
            if (reg[o.args[0].r] == 0)
                pc = labels[o.args[1].str] - 1;
            break;
        }
        case BNEZ:
        {
            if (reg[o.args[0].r] != 0)
                pc = labels[o.args[1].str] - 1;
            break;
        }
        case BLEZ:
        {
            if (reg[o.args[0].r] <= 0)
                pc = labels[o.args[1].str] - 1;
            break;
        }
        case BGEZ:
        {
            if (reg[o.args[0].r] >= 0)
                pc = labels[o.args[1].str] - 1;
            break;
        }
        case BGTZ:
        {
            if (reg[o.args[0].r] > 0)
                pc = labels[o.args[1].str] - 1;
            break;
        }
        case BLTZ:
        {
            if (reg[o.args[0].r] < 0)
                pc = labels[o.args[1].str] - 1;
            break;
        }
        case JR:
        {
            pc = reg[o.args[0].r] - 1;
            break;
        }
        case JAL:
        {
            reg[31] = pc + 1;
            pc = labels[o.args[0].str] - 1;
            break;
        }
        case JALR:
        {
            reg[31] = pc + 1;
            pc = reg[o.args[0].r] - 1;
            break;
        }
        case LA:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : (memory + reg[o.args[1].r] + o.args[1].v.i);
            reg[o.args[0].r] = p - memory;
            break;
        }
        case LB:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : (memory + reg[o.args[1].r] + o.args[1].v.i);
            reg[o.args[0].r] = *p;
            break;
        }
        case LH:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : (memory + reg[o.args[1].r] + o.args[1].v.i);
            Word v;
            v.c[0] = p[0];
            v.c[1] = p[1];
            reg[o.args[0].r] = v.s[0];
            break;
        }
        case LW:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : (memory + reg[o.args[1].r] + o.args[1].v.i);
            Word v;
            v.c[0] = p[0];
            v.c[1] = p[1];
            v.c[2] = p[2];
            v.c[3] = p[3];
            reg[o.args[0].r] = v.i;
            break;
        }
        case SB:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : (memory + reg[o.args[1].r] + o.args[1].v.i);
            p[0] = reg[o.args[0].r];
            break;
        }
        case SH:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : (memory + reg[o.args[1].r] + o.args[1].v.i);
            Word v;
            v.s[0] = reg[o.args[0].r];
            p[0] = v.c[0];
            p[1] = v.c[1];
            break;
        }
        case SW:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : (memory + reg[o.args[1].r] + o.args[1].v.i);
            Word v;
            v.i = reg[o.args[0].r];
            p[0] = v.c[0];
            p[1] = v.c[1];
            p[2] = v.c[2];
            p[3] = v.c[3];
            break;
        }
        case MOVE:
        {
            reg[o.args[0].r] = reg[o.args[1].r];
            break;
        }
        case MFHI:
        {
            reg[o.args[0].r] = reg[32];
            break;
        }
        case MFLO:
        {
            reg[o.args[0].r] = reg[33];
            break;
        }
        case SYSCALL:
        {
            switch (reg[2])
            {
            case 1:
                cout << reg[4];
                break;
            case 4:
            {
                for (char *p = memory + reg[4]; *p != 0; ++p)
                    cout << p[0];
                break;
            }
            case 5:
                cin >> reg[2];
                break;
            case 8:
            {
                string tmp;
                getline(cin, tmp);
                if (tmp.length() == 0)
                {
                    getline(cin, tmp);
                }
                char *p = memory + reg[4];
                int i = 0;
                for (int j = min(reg[5] - 1, (int)tmp.length()); i < j; ++i)
                    p[i] = tmp[i];
                p[i] = 0;
                break;
            }
            case 9:
                if (ptr % 4 != 0)
                    ptr = 4 * (ptr / 4 + 1);
                reg[2] = ptr;
                ptr += reg[4];
                break;
            case 10:
                return 0;
            case 17:
                return reg[4];
            }
            break;
        }
        default:
            break;
        }
    }
    return 0;
}