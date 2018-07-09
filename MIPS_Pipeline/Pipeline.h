#ifndef PIPELINE
#define PIPELINE

#include "MipsHead.h"
#include "Instruction.h"

union BigNumber
{
    LL ll;
    ULL ull;
    int i[2];
    UI ui[2];
};

class Pipeline
{
private:
    int *reg;
    char *memory;
    int ptr;

public:
    Pipeline(int *r, char *m, int p);

    struct IF_ID
    {
        Instruction ir;
        int npc;
    };
    struct ID_EX
    {
        int a, b;
        Instruction ir;
        int npc;
    };
    struct EX_MEM
    {
        int a, b;
        Instruction ir;
        int aluOutput;
        bool cond;
    };
    struct MEM_WB
    {
        Instruction ir;
        int aluOutput, lmd;
    };

    IF_ID if_id;
    ID_EX id_ex;
    EX_MEM ex_mem;
    MEM_WB mem_wb;

    bool lock2, lock3, lock4, lock5;

    void IF();
    void ID();
    void EX();
    void MEM(bool &running, int &ret);
    void WB();

    void Pipe(bool &running, int &ret);
};

#endif // !PIPELINE

