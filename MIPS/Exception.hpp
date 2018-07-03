#ifndef EXCEPTION
#define EXCEPTION

#include "MipsHead.h"

class Exception
{
private:
    string content;

public:
    Exception(string s) : content(s)
    {
    }
};

#endif // !EXCEPTION

