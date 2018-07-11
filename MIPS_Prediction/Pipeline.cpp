#include "Pipeline.h"

void Pipeline::IF_ID::Clear()
{
    ir.name = NULLName;
    ir.rs = ir.rt = ir.rd = 0;
    ir.address = 0;
}

void Pipeline::ID_EX::Clear()
{
    ir.name = NULLName;
    ir.rs = ir.rt = ir.rd = 0;
    ir.address = 0;
}

void Pipeline::EX_MEM::Clear()
{
    ir.name = NULLName;
    ir.rs = ir.rt = ir.rd = 0;
    ir.address = 0;
}

void Pipeline::MEM_WB::Clear()
{
    ir.name = NULLName;
    ir.rs = ir.rt = ir.rd = 0;
    ir.address = 0;
    lmd = 0;
}

Pipeline::Pipeline(int *r, char *m, int p)
    : reg(r), memory(m), ptr(p)
{
    lock1 = lock2 = lock3 = lock4 = lock5 = false;
    free1 = free2 = free3 = free4 = free5 = 0;
}

void Pipeline::IF()
{
    if (ex_mem.ir.name >= 7 && ex_mem.ir.name <= 24 && ex_mem.cond)
        reg[34] = ex_mem.aluOutput;
    if (lock3 || lock2)
        return;
    if (free1 > 0)
    {
        --free1;
        ++free2;
        return;
    }
    if (id_ex.ir.name >= 7 && id_ex.ir.name <= 24) //·ÖÖ§Ô¤²â
    {
        free2 += 1;
        return;
    }
    /*if (if_id.ir.name >= 6 && if_id.ir.name <= 27)
    {
        free1 += 2;
        return;
    }*/
    if (mem_wb.ir.name == 28) //structure
    {
        lock1 = true;
        ++free2;
        return;
    }
    //cout << reg[34] / 8 << endl;
    if_id.ir = Instruction(memory, reg[34]);
    if (if_id.ir.name == 33)
    {
        if_id.ir.rs = 2;
        if_id.ir.rt = 4;
    }
}

void Pipeline::ID()
{
    if (lock3)
        return;
    if (free2 > 0)
    {
        --free2;
        ++free3;
        return;
    }
    if (if_id.ir.rs != 0 && (if_id.ir.rs == id_ex.ir.rd || if_id.ir.rs == ex_mem.ir.rd || if_id.ir.rs == mem_wb.ir.rd) ||
        if_id.ir.rt != 0 && (if_id.ir.rt == id_ex.ir.rd || if_id.ir.rt == ex_mem.ir.rd || if_id.ir.rt == mem_wb.ir.rd))
        //(if_id.ir.rs == 32 || if_id.ir.rt == 33) && (if_id.ir.name == 3 || if_id.ir.name == 4 || ex_mem.ir.name == 3 || ex_mem.ir.name == 4 || mem_wb.ir.name == 3 || mem_wb.ir.name == 4))
    {
        lock2 = true;
        ++free3;
        return;
    }
    if (if_id.ir.name == NULLName)
        return;
    id_ex.ir = if_id.ir;
    id_ex.a = reg[if_id.ir.rs];
    id_ex.b = reg[if_id.ir.rt];
    if (if_id.ir.name == 33)
    {
        if (id_ex.a == 5 || id_ex.a == 9)
            id_ex.ir.rd = 2;
    }
    id_ex.npc = reg[34] + 8;
    if (if_id.ir.name == 6 || if_id.ir.name == 26)
    {
        reg[34] = if_id.ir.jumpto;
    }
    else if (if_id.ir.name == 25 || if_id.ir.name == 27)
    {
        reg[34] = id_ex.a;
    }
    else
        reg[34] = reg[34] + 8;
    if (if_id.ir.name >= 6 && if_id.ir.name <= 25)
    {
        id_ex.ir.rd = 0;
    }
    if (if_id.ir.name == 26 || if_id.ir.name == 27)
    {
        id_ex.ir.rd = 31;
    }
    if_id.Clear();
}

void Pipeline::EX()
{
    if (free3 > 0)
    {
        --free3;
        ++free4;
        return;
    }
    if (false) //data
    {
        lock3 = true;
        ++free4;
        return;
    }
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
            o = id_ex.a + (ir.name == 1 ? ir.offset : id_ex.b);
            break;
        case SUB:
            o = id_ex.a - (ir.name == 1 ? ir.offset : id_ex.b);
            break;
        case MUL:
            o = id_ex.a * (ir.name == 1 ? ir.offset : id_ex.b);
            break;
        case DIV:
            o = id_ex.a / (ir.name == 1 ? ir.offset : id_ex.b);
            break;
        case REM:
            o = id_ex.a % (ir.name == 1 ? ir.offset : id_ex.b);
            break;
        case XOR:
            o = id_ex.a ^ (ir.name == 1 ? ir.offset : id_ex.b);
            break;
        case SEQ:
            o = (id_ex.a == (ir.name == 1 ? ir.offset : id_ex.b));
            break;
        case SNE:
            o = (id_ex.a != (ir.name == 1 ? ir.offset : id_ex.b));
            break;
        case SLE:
            o = (id_ex.a <= (ir.name == 1 ? ir.offset : id_ex.b));
            break;
        case SLT:
            o = (id_ex.a < (ir.name == 1 ? ir.offset : id_ex.b));
            break;
        case SGE:
            o = (id_ex.a >= (ir.name == 1 ? ir.offset : id_ex.b));
            break;
        case SGT:
            o = (id_ex.a > (ir.name == 1 ? ir.offset : id_ex.b));
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
            reg[32] = a.ll % b.ll;
            reg[33] = a.ll / b.ll;
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
            reg[32] = a.ull % b.ull;
            reg[33] = a.ull / b.ull;
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
        ex_mem.cond = id_ex.a == ir.offset;
        break;
    case 8:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a != ir.offset;
        break;
    case 9:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a <= ir.offset;
        break;
    case 10:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a < ir.offset;
        break;
    case 11:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a >= ir.offset;
        break;
    case 12:
        o = ir.jumpto;
        ex_mem.cond = id_ex.a > ir.offset;
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
        o = id_ex.npc;
        ir.rd = 31;
        break;
    case 27:
        o = id_ex.npc;
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
    id_ex.Clear();
}

void Pipeline::MEM(bool &running, int &ret)
{
    if (free4 > 0)
    {
        --free4;
        ++free5;
        return;
    }
    mem_wb.ir = ex_mem.ir;
    mem_wb.aluOutput = ex_mem.aluOutput;
    if (mem_wb.ir.name == 28)
    {
        char *p = memory + ex_mem.aluOutput;
        switch (mem_wb.ir.u)
        {
        case LA:
            //mem_wb.lmd = ex_mem.aluOutput;
            break;
        case LB:
            mem_wb.aluOutput = *p;
            break;
        case LH:
        {
            Half h;
            for (int i = 0; i < 2; ++i)
                h.c[i] = p[i];
            mem_wb.aluOutput = h.s;
            break;
        }
        case LW:
        {
            Word w;
            for (int i = 0; i < 4; ++i)
                w.c[i] = p[i];
            mem_wb.aluOutput = w.i;
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
            for (char *p = memory + ex_mem.b; ; ++p)
            {
                if (*p == 0)
                    break;
                cout << p[0];
            }
            break;
        case 5:
            cin >> mem_wb.aluOutput;
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
            mem_wb.aluOutput = ptr;
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
    ex_mem.Clear();
}

void Pipeline::WB()
{
    if (free5 > 0)
    {
        --free5;
        return;
    }
    if (mem_wb.ir.rd > 0)
        reg[mem_wb.ir.rd] = mem_wb.aluOutput;
    mem_wb.Clear();
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
    lock1 = lock2 = lock3 = lock4 = lock5 = false;
}