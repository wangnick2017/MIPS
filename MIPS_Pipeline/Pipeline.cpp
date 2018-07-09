#include "Pipeline.h"

Pipeline::Pipeline(int *r, char *m, int p)
    : reg(r), memory(m), ptr(p), lock2(false), lock3(false), lock4(false)
{
}

void Pipeline::IF()
{
    if_id.ir = Instruction(memory, reg[34]);
    if_id.npc = reg[34] = (ex_mem.ir.name >= 5 && ex_mem.ir.name <= 26 && ex_mem.cond) ? ex_mem.aluOutput : reg[34] + 8;
}

void Pipeline::ID()
{
    id_ex.a = reg[if_id.ir.rs];
    id_ex.b = reg[if_id.ir.rt];
    if (if_id.ir.name == 33)
    {
        id_ex.a = reg[2];
        id_ex.b = reg[4];
    }
    id_ex.npc = if_id.npc;
    id_ex.ir = if_id.ir;
}

void Pipeline::EX()
{
    ex_mem.ir = id_ex.ir;
    int &o = ex_mem.aluOutput;
    Instruction &ir = ex_mem.ir;
    switch (ir.name)
    {
    case 0:
        o = ir.imm;
        break;
    case 1:
    case 2:
        switch (ir.u)
        {
        case ADD:
            o = id_ex.a + (ir.name == 1 ? ir.imm : id_ex.b);
            break;
        case SUB:
            o = id_ex.a - (ir.name == 1 ? ir.imm : id_ex.b);
            break;
        case MUL:
            o = id_ex.a * (ir.name == 1 ? ir.imm : id_ex.b);
            break;
        case DIV:
            o = id_ex.a / (ir.name == 1 ? ir.imm : id_ex.b);
            break;
        case REM:
            o = id_ex.a % (ir.name == 1 ? ir.imm : id_ex.b);
            break;
        case XOR:
            o = id_ex.a ^ (ir.name == 1 ? ir.imm : id_ex.b);
            break;
        case SEQ:
            o = (id_ex.a == (ir.name == 1 ? ir.imm : id_ex.b));
            break;
        case SNE:
            o = (id_ex.a != (ir.name == 1 ? ir.imm : id_ex.b));
            break;
        case SLE:
            o = (id_ex.a <= (ir.name == 1 ? ir.imm : id_ex.b));
            break;
        case SLT:
            o = (id_ex.a < (ir.name == 1 ? ir.imm : id_ex.b));
            break;
        case SGE:
            o = (id_ex.a >= (ir.name == 1 ? ir.imm : id_ex.b));
            break;
        case SGT:
            o = (id_ex.a > (ir.name == 1 ? ir.imm : id_ex.b));
            break;
        case ADDU:
            o = (UI)id_ex.a + (ir.name == 1 ? ir.imm : (UI)id_ex.b);
            break;
        case SUBU:
            o = (UI)id_ex.a - (ir.name == 1 ? ir.imm : (UI)id_ex.b);
            break;
        case MULU:
            o = (UI)id_ex.a * (ir.name == 1 ? ir.imm : (UI)id_ex.b);
            break;
        case DIVU:
            o = (UI)id_ex.a / (ir.name == 1 ? ir.imm : (UI)id_ex.b);
            break;
        case REMU:
            o = (UI)id_ex.a % (ir.name == 1 ? ir.imm : (UI)id_ex.b);
            break;
        case XORU:
            o = (UI)id_ex.a ^ (ir.name == 1 ? ir.imm : (UI)id_ex.b);
            break;
        }
        break;
    case 3:
    case 4:
    {
        BigNumber a, b;
        switch (ir.u)
        {
        case MULSUB:
            a.ll = id_ex.a;
            b.ll = ir.name == 3 ? (LL)ir.imm : id_ex.b;
            a.ll *= b.ll;
            reg[32] = a.i[0];
            reg[33] = a.i[1];
            break;
        case DIVSUB:
            a.ll = id_ex.a;
            b.ll = ir.name == 3 ? (LL)ir.imm : id_ex.b;
            a.ll /= b.ll;
            reg[32] = a.i[0];
            reg[33] = a.i[1];
            break;
        case MULUSUB:
            a.ull = id_ex.a;
            b.ull = ir.name == 3 ? ir.imm : (ULL)id_ex.b;
            a.ull *= b.ull;
            reg[32] = a.ui[0];
            reg[33] = b.ui[0];
            break;
        case DIVUSUB:
            a.ull = id_ex.a;
            b.ull = ir.name == 3 ? ir.imm : (ULL)id_ex.b;
            a.ull /= b.ull;
            reg[32] = a.ui[0];
            reg[33] = b.ui[0];
            break;
        }
        break;
    }
    case 5:
        o = (ir.u == NEG) ? -id_ex.a : (~(UI)id_ex.a);
        break;
    case 6:
        o = ir.jumpto;
        break;
    case 7:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a == ir.imm;
        break;
    case 8:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a != ir.imm;
        break;
    case 9:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a <= ir.imm;
        break;
    case 10:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a < ir.imm;
        break;
    case 11:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a >= ir.imm;
        break;
    case 12:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a > ir.imm;
        break;
    case 13:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a == id_ex.b;
        break;
    case 14:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a != id_ex.b;
        break;
    case 15:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a <= id_ex.b;
        break;
    case 16:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a < id_ex.b;
        break;
    case 17:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a >= id_ex.b;
        break;
    case 18:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a > id_ex.b;
        break;
    case 19:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a == 0;
        break;
    case 20:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a != 0;
        break;
    case 21:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a <= 0;
        break;
    case 22:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a < 0;
        break;
    case 23:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a >= 0;
        break;
    case 24:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a > 0;
        break;
    case 25:
        o = id_ex.a;
        break;
    case 26:
        o = ir.jumpto;
        ir.rd = 31;
        break;
    case 27:
        o = id_ex.a;
        ir.rd = 31;
        break;
    case 28:
        o = id_ex.ir.address == 0 ? id_ex.ir.offset : id_ex.b + id_ex.ir.offset;
        break;
    case 29:
        o = id_ex.a;
        break;
    case 30:
        o = reg[32];
        break;
    case 31:
        o = reg[33];
        break;
    case 33:
        //
        break;
    }
    ex_mem.a = id_ex.a;
    ex_mem.b = id_ex.b;
}

void Pipeline::MEM(bool &running, int &ret)
{
    mem_wb.ir = ex_mem.ir;
    mem_wb.aluOutput = ex_mem.aluOutput;
    if (mem_wb.ir.name == 28)
    {
        char *p = memory + ex_mem.aluOutput;
        switch (mem_wb.ir.u)
        {
        case LA:
            mem_wb.lmd = ex_mem.aluOutput;
            break;
        case LB:
            mem_wb.lmd = *p;
            break;
        case LH:
        {
            Half h;
            for (int i = 0; i < 2; ++i)
                h.c[i] = p[i];
            mem_wb.lmd = h.s;
            break;
        }
        case LW:
        {
            Word w;
            for (int i = 0; i < 4; ++i)
                w.c[i] = p[i];
            mem_wb.lmd = w.i;
            break;
        }
        case SB:
            p[0] = ex_mem.a;
            break;
        case SH:
        {
            Half h;
            h.s = ex_mem.a;
            for (int i = 0; i < 2; ++i)
                p[i] = h.c[i];
            break;
        }
        case SW:
        {
            Word w;
            w.i = ex_mem.a;
            for (int i = 0; i < 4; ++i)
                p[i] = w.c[i];
            break;
        }
        }
    }
    else if (mem_wb.ir.name == 33) //SYSCALL
    {
        switch (ex_mem.a)
        {
        case 1:
            cout << ex_mem.b;
            break;
        case 4:
            for (char *p = memory + ex_mem.b; *p != 0; ++p)
                cout << p[0];
            break;
        case 5:
            mem_wb.ir.rd = 2;
            cin >> mem_wb.lmd;
            break;
        case 8:
        {
            string tmp;
            getline(cin, tmp);
            if (tmp.length() == 0)
            {
                getline(cin, tmp);
            }
            char *p = memory + ex_mem.b;
            int i = 0;
            for (int j = min(reg[5] - 1, (int)tmp.length()); i < j; ++i)
                p[i] = tmp[i];
            p[i] = 0;
            break;
        }
        case 9:
            if (ptr % 4 != 0)
                ptr = 4 * (ptr / 4 + 1);
            mem_wb.ir.rd = 2;
            mem_wb.lmd = ptr;
            ptr += ex_mem.b;
            break;
        case 10:
            running = false;
            ret = 0;
            break;
        case 17:
            running = false;
            ret = ex_mem.b;
            break;
        }
    }
}

void Pipeline::WB()
{
    if (mem_wb.ir.rd > 0)
        reg[mem_wb.ir.rd] = mem_wb.lmd == 0 ? mem_wb.aluOutput : mem_wb.lmd;
}

void Pipeline::Pipe(bool &running, int &ret)
{
    WB();
    MEM(running, ret);
    if (!running)
        return;
    EX();
    ID();
    IF(); 
}