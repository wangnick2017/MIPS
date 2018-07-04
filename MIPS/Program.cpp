#include "Program.hpp"

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
        if (code.find(':') != string::npos)
        {
            code.pop_back();
            orders.push_back(Order(code, true));
            names.push(code);
        }
        else
        {
            while (!names.empty())
            {
                tmp = names.top();
                labels.insert(pair<string, int>(tmp, orders.size()));
                names.pop();
            }
            orders.push_back(Order(code));
        }
    }
}

int Program::Run()
{
    memset(memory, 0, MemorySize);
    memset(reg, 0, sizeof(reg));
    reg[29] = (int)(memory + MemorySize);
    for (int pc = labels["main"], ptr = 0;;)
    {
        Order &o = orders[pc];
        switch (o.Type())
        {
        case __DATA:
        {
            mode = DATA;
            break;
        }
        case __TEXT:
        {
            mode = TEXT;
            break;
        }
        case __LABEL:
        {
            break;
        }
        case __ALIGN:
        {
            int n = (1 << o.args[0].ll);
            if (ptr % n != 0)
                ptr = n * (ptr / n + 1);
            break;
        }
        case __ASCII:
        {
            for (int i = 0, j = o.args[0].str.length(); i < j; ++i)
            {
                memory[ptr++] = o.args[0].str[i];
            }
            break;
        }
        case __ASCIIZ:
        {
            for (int i = 0, j = o.args[0].str.length(); i < j; ++i)
            {
                memory[ptr++] = o.args[0].str[i];
            }
            memory[ptr++] = 0;
            break;
        }
        case __BYTE:
        {
            for (int i = 0, j = o.args.size(); i < j; ++i)
            {
                memory[ptr++] = o.args[i].ll;
            }
            break;
        }
        case __HALF:
        {
            break;
        }
        case __WORD:
        {
            break;
        }
        case __SPACE:
        {
            ptr += o.args[0].ll;
            break;
        }
        case ADD:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = reg[o.args[1].r] + i;
            break;
        }
        case ADDU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ull;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] + i);
            break;
        }
        case ADDIU:
        {
            unsigned int i = o.args[2].ull;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] + i);
            break;
        }
        case SUB:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = reg[o.args[1].r] - i;
            break;
        }
        case SUBU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ull;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] - i);
            break;
        }
        case MUL:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = reg[o.args[1].r] * i;
            break;
        }
        case MULU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ull;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] * i);
            break;
        }
        case MUL_SUB:
        {
            break;
        }
        case MULU_SUB:
        {
            break;
        }
        case DIV:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = reg[o.args[1].r] / i;
            break;
        }
        case DIVU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ull;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] / i);
            break;
        }
        case DIV_SUB:
        {
            break;
        }
        case DIVU_SUB:
        {
            break;
        }
        case XOR:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = reg[o.args[1].r] ^ i;
            break;
        }
        case XORU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ull;
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
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = reg[o.args[1].r] % i;
            break;
        }
        case REMU:
        {
            unsigned int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ull;
            reg[o.args[0].r] = (unsigned int)((unsigned int)reg[o.args[1].r] % i);
            break;
        }
        case LI:
        {
            reg[o.args[0].r] = o.args[1].ll;
            break;
        }
        case SEQ:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = (reg[o.args[1].r] == i);
            break;
        }
        case SGE:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = (reg[o.args[1].r] >= i);
            break;
        }
        case SGT:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = (reg[o.args[1].r] > i);
            break;
        }
        case SLE:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = (reg[o.args[1].r] <= i);
            break;
        }
        case SLT:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = (reg[o.args[1].r] < i);
            break;
        }
        case SNE:
        {
            int i = o.args[2].type == REG ? reg[o.args[2].r] : o.args[2].ll;
            reg[o.args[0].r] = (reg[o.args[1].r] != i);
            break;
        }
        case B:
        case J:
        {
            break;
        }
        case BEQ:
        {
            break;
        }
        case BNE:
        {
            break;
        }
        case BGE:
        {
            break;
        }
        case BLE:
        {
            break;
        }
        case BGT:
        {
            break;
        }
        case BLT:
        {
            break;
        }
        case BEQZ:
        {
            break;
        }
        case BNEZ:
        {
            break;
        }
        case BLEZ:
        {
            break;
        }
        case BGEZ:
        {
            break;
        }
        case BGTZ:
        {
            break;
        }
        case BLTZ:
        {
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
            case 5:
                cin >> reg[2];
                break;
            case 8:
            case 9:
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