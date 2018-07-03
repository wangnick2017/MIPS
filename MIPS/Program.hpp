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
    int reg[32];
    char *memory;

public:
    Program()
    {
        memory = new char[MemorySize];
    }

    ~Program()
    {
        delete[] memory;
    }

    void InputCodes(char *fileName)
    {
        ifstream codes("56.s");
        string code;
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
                labels.insert(pair<string, int>(code, orders.size()));
            }
            else
                orders.push_back(Order(code));
        }
    }

    void Run()
    {
        memset(memory, 0, MemorySize);
        reg[29] = (int)(memory + MemorySize);
    }
};

#endif // !PROGRAM
