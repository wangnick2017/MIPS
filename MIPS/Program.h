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

    vector<Order> orders;
    map<string, int> labels;
    map<int, char *> pointers;
    int reg[35];
    char *memory;

public:
    Program();
    ~Program();
    void InputCodes(char *fileName);
    int Run();
};

#endif // !PROGRAM
