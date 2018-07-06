#ifndef PROGRAM
#define PROGRAM

#include "MipsHead.h"
#include "Instruction.h"

class Program
{
private:
    int reg[35];
    char *memory;
    int ptr;

public:
    Program();
    ~Program();
    void InitNames();
    void InputCodes(char *fileName);
    int Run();
};

#endif // !PROGRAM
