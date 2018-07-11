#ifndef READER
#define READER

#include "MipsHead.h"

class Reader
{
    friend class Order;
private:
    const string &input;
    int pos, len;
public:
    Reader(const string &s);
    bool ReadInt(int &i);
    bool ReadString(string &s);
    bool ReadFormatString(string &s);
};

#endif // !READER
