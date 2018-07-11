#include "Program.h"
#include "Reader.h"

map<string, ULL> RName;
map<string, int> IName;
map<string, int> jumpers;
map<string, int> pointers;

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
    RName["$zero"] = 35;
    RName["$0"] = 35;
    RName["$at"] = 1;
    RName["$v0"] = 2;
    RName["$v1"] = 3;
    RName["$a0"] = 4;
    RName["$a1"] = 5;
    RName["$a2"] = 6;
    RName["$a3"] = 7;
    RName["$t0"] = 8;
    RName["$t1"] = 9;
    RName["$t2"] = 10;
    RName["$t3"] = 11;
    RName["$t4"] = 12;
    RName["$t5"] = 13;
    RName["$t6"] = 14;
    RName["$t7"] = 15;
    RName["$s0"] = 16;
    RName["$s1"] = 17;
    RName["$s2"] = 18;
    RName["$s3"] = 19;
    RName["$s4"] = 20;
    RName["$s5"] = 21;
    RName["$s6"] = 22;
    RName["$s7"] = 23;
    RName["$t8"] = 24;
    RName["$t9"] = 25;
    RName["$k0"] = 26;
    RName["$k1"] = 27;
    RName["$gp"] = 28;
    RName["$sp"] = 29;
    RName["$fp"] = 30;
    RName["$ra"] = 31;
    RName["$hi"] = 32;
    RName["$lo"] = 33;
    RName["$pc"] = 34;
    for (int i = 1; i <= 34; ++i)
        RName["$" + to_string(i)] = i;
    IName["li"] = LI;
    IName["add"] = ADD;
    IName["sub"] = SUB;
    IName["mul"] = MUL;
    IName["div"] = DIV;
    IName["mulsub"] = MULSUB;
    IName["mulusub"] = MULUSUB;
    IName["divsub"] = DIVSUB;
    IName["divusub"] = DIVUSUB;
    IName["xor"] = XOR;
    IName["neg"] = NEG;
    IName["rem"] = REM;
    IName["addu"] = ADDU;
    IName["addiu"] = ADDU;
    IName["subu"] = SUBU;
    IName["mulu"] = MULU;
    IName["divu"] = DIVU;
    IName["xoru"] = XORU;
    IName["negu"] = NEGU;
    IName["remu"] = REMU;
    IName["seq"] = SEQ;
    IName["sge"] = SGE;
    IName["sgt"] = SGT;
    IName["sle"] = SLE;
    IName["slt"] = SLT;
    IName["sne"] = SNE;
    IName["b"] = B;
    IName["beq"] = BEQ;
    IName["bne"] = BNE;
    IName["bge"] = BGE;
    IName["ble"] = BLE;
    IName["bgt"] = BGT;
    IName["blt"] = BLT;
    IName["beqz"] = BEQZ;
    IName["bnez"] = BNEZ;
    IName["blez"] = BLEZ;
    IName["bgez"] = BGEZ;
    IName["bgtz"] = BGTZ;
    IName["bltz"] = BLTZ;
    IName["j"] = B;
    IName["jr"] = JR;
    IName["jal"] = JAL;
    IName["jalr"] = JALR;
    IName["la"] = LA;
    IName["lb"] = LB;
    IName["lh"] = LH;
    IName["lw"] = LW;
    IName["sb"] = SB;
    IName["sh"] = SH;
    IName["sw"] = SW;
    IName["move"] = MOVE;
    IName["mfhi"] = MFHI;
    IName["mflo"] = MFLO;
    IName["nop"] = NOP;
    IName["syscall"] = SYSCALL;
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
    vector<string> dynamics;
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
                jumpers.insert(pair<string, int>(names.top(), ptr / 8));
                names.pop();
            }
            ptr += 8;
            dynamics.push_back(code);
        }
    }
    for (int i = 0, j = statics.size(); i < j; ++i)
    {
        Reader reader(statics[i]);
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
                n = 1 << n;
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
    int pcPtr = 0;
    for (int i = 0, j = dynamics.size(); i < j; ++i)
    {
        Reader reader(dynamics[i]);
        if (!reader.ReadString(tmp))
            continue;
        Instruction ins(reader, tmp);
        ins.WriteMemory(memory, pcPtr);
    }
}

int Program::Run()
{
    Pipeline pipeline(reg, memory, ptr);
    reg[34] = jumpers["main"] * 8;
    bool running = true;
    int ret;
    int cnt = 0;
    while (running)
        pipeline.Pipe(running, ret), ++cnt;
    return ret;
}