#include "MipsHead.h"
#include "Order.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return 0;
    }
    ifstream codes(string(argv[1]));
    string code;
    vector<Order> orders;
    while (!codes.eof())
    {
        getline(codes, code);
        orders.push_back(Order(code));
    }
    return 0;
}