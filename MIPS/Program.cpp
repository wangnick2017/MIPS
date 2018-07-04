#include "Program.h"

Program::Program()
{
    memory = new char[MemorySize];
}

Program::~Program()
{
    delete[] memory;
}

void Program::InputCodes(char *fileName)
{
    ifstream codes("56.s");
    string code;
    stack<string> names;
    string tmp;
    while (!codes.eof())
    {
        getline(codes, code);
        size_t comment = code.find('#');
        if (comment != string::npos)
            code = code.erase(comment);
        bool nullCode = true;
        for (int i = 0, j = code.length(); i < j; ++i)
            if (code[i] >= 'a' && code[i] <= 'z' || code[i] == '_')
            {
                nullCode = false;
                break;
            }
        if (nullCode)
            continue;
        Reader reader(code);
        tmp = reader.ReadString();
        if (tmp.find(':') != string::npos)
        {
            tmp.pop_back();
            orders.push_back(Order(reader, tmp, true));
            names.push(tmp);
        }
        else
        {
            while (!names.empty())
            {
                labels.insert(pair<string, int>(names.top(), orders.size()));
                names.pop();
            }
            orders.push_back(Order(reader, tmp));
        }
    }
}

int Program::Run()
{
    memset(memory, 0, MemorySize);
    memset(reg, 0, sizeof(reg));
    reg[29] = (int)(memory + MemorySize);
    int pc, ptr;
    int orderSize = orders.size();
    ++orderSize;
    --orderSize;
    for (pc = 0, ptr = 0; pc < orderSize; ++pc)
    {
        Order &o = orders[pc];
        switch (o.Type())
        {
        case __ALIGN:
        {

            int n = (1 << o.args[0].v.i);
            if (ptr % n != 0)
                ptr = n * (ptr / n + 1);
            break;
        }
        case __ASCII:
        {
            pointers.insert(pair<int, char *>(pc, memory + ptr));
            for (int i = 0, j = o.args[0].str.length(); i < j; ++i)
            {
                memory[ptr++] = o.args[0].str[i];
            }
            break;
        }
        case __ASCIIZ:
        {
            pointers.insert(pair<int, char *>(pc, memory + ptr));
            for (int i = 0, j = o.args[0].str.length(); i < j; ++i)
            {
                memory[ptr++] = o.args[0].str[i];
            }
            memory[ptr++] = 0;
            break;
        }
        case __BYTE:
        {
            pointers.insert(pair<int, char *>(pc, memory + ptr));
            for (int i = 0, j = o.args.size(); i < j; ++i)
                memory[ptr++] = o.args[i].v.i;
            break;
        }
        case __HALF:
        {
            pointers.insert(pair<int, char *>(pc, memory + ptr));
            for (int i = 0, j = o.args.size(); i < j; ++i)
                for (int k = 0; k < 2; ++k)
                    memory[ptr++] = o.args[i].v.c[k];
            break;
        }
        case __WORD:
        {
            pointers.insert(pair<int, char *>(pc, memory + ptr));
            for (int i = 0, j = o.args.size(); i < j; ++i)
                for (int k = 0; k < 4; ++k)
                    memory[ptr++] = o.args[i].v.c[k];
            break;
        }
        case __SPACE:
        {
            pointers.insert(pair<int, char *>(pc, memory + ptr));
            ptr += o.args[0].v.i;
            break;
        }
        default:
            break;
        }
    }
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
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : ((char *)reg[o.args[1].r] + o.args[1].v.i);
            reg[o.args[0].r] = (int)p;
            break;
        }
        case LB:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : ((char *)reg[o.args[1].r] + o.args[1].v.i);
            reg[o.args[0].r] = *p;
            break;
        }
        case LH:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : ((char *)reg[o.args[1].r] + o.args[1].v.i);
            Value v;
            v.c[0] = p[0];
            v.c[1] = p[1];
            reg[o.args[0].r] = v.s[0];
            break;
        }
        case LW:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : ((char *)reg[o.args[1].r] + o.args[1].v.i);
            Value v;
            v.c[0] = p[0];
            v.c[1] = p[1];
            v.c[2] = p[2];
            v.c[3] = p[3];
            reg[o.args[0].r] = v.i;
            break;
        }
        case SB:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : ((char *)reg[o.args[1].r] + o.args[1].v.i);
            p[0] = reg[o.args[0].r];
            break;
        }
        case SH:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : ((char *)reg[o.args[1].r] + o.args[1].v.i);
            Value v;
            v.s[0] = reg[o.args[0].r];
            p[0] = v.c[0];
            p[1] = v.c[1];
            break;
        }
        case SW:
        {
            char *p = o.args[1].type == LABEL ? pointers[labels[o.args[1].str]] : ((char *)reg[o.args[1].r] + o.args[1].v.i);
            Value v;
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
                for (char *p = (char *)reg[4]; p && *p != 0; ++p)
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
                char *p = (char *)reg[4];
                int i = 0;
                for (int j = min(reg[5] - 1, (int)tmp.length()); i < j; ++i)
                    p[i] = tmp[i];
                p[i] = 0;
                break;
            }
            case 9:
                if (ptr % 4 != 0)
                    ptr = 4 * (ptr / 4 + 1);
                reg[2] = (int)(memory + ptr);
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