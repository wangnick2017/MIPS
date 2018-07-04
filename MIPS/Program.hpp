#ifndef PROGRAM
#define PROGRAM

#include "MipsHead.h"
#include "Order.h"

class Program
{
private:
    enum
    {
        MemorySize = 4194304
    };
    enum CodeMode
    {
        DATA, TEXT
    };

    vector<Order> orders;
    map<string, int> labels;
    int reg[35];
    char *memory;
    CodeMode mode;

public:
    Program();
    ~Program();
    void InputCodes(char *fileName);
    int Run();
};

#endif // !PROGRAM
