#ifndef PIPELINE
#define PIPELINE

#include "MipsHead.h"

class Pipeline
{
private:

public:
    struct IF_ID
    {

    };
    struct ID_EX
    {
    };
    struct EX_MEM
    {
    };
    struct MEM_WB
    {
    };

    IF_ID if_id;
    ID_EX id_ex;
    EX_MEM ex_mem;
    MEM_WB mem_wb;

    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();
};

#endif // !PIPELINE

