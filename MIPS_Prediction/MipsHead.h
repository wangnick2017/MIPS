#ifndef MIPSHEAD
#define MIPSHEAD

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <stack>
#include <algorithm>

using namespace std;

typedef long long LL;
typedef unsigned int UI;
typedef unsigned long long ULL;

const int MemorySize = 4194304;
const ULL NULLName = 0xFFFFFFull;

extern map<string, ULL> RName;
extern map<string, int> IName;
extern map<string, int> jumpers;
extern map<string, int> pointers;

#endif // !MIPSHEAD
