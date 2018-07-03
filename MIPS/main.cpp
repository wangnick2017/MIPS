#include "MipsHead.h"
#include "Program.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        //return 0;
    }
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    Program program;
    program.InputCodes(argv[1]);
    program.Run();
    return 0;
}