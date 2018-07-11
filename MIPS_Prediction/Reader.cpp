#include "Reader.h"

Reader::Reader(const string &s) : input(s), pos(0), len(s.length())
{
}

bool Reader::ReadInt(int &i)
{
    while (pos < len && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == ','))
        ++pos;
    if (pos >= len)
        return false;
    int sign = (input[pos] == '-') ? -1 : 1;
    if (sign < 0)
        ++pos;
    i = input[pos++] - '0';
    while (pos < len && input[pos] >= '0' && input[pos] <= '9')
        i = i * 10 + (int)(input[pos++] - '0');
    i *= sign;
    return true;
}

bool Reader::ReadString(string &s)
{
    while (pos < len && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == ','))
        ++pos;
    if (pos >= len)
        return false;
    int start = pos;
    while (pos < len && input[pos] != ' ' && input[pos] != '\t' && input[pos] != '\n' && input[pos] != ',')
        ++pos;
    s = input.substr(start, pos - start);
    return true;
}

bool Reader::ReadFormatString(string &s)
{
    while (pos < len && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == ','))
        ++pos;
    ++pos;
    if (pos >= len)
        return false;
    s = "";
    while (input[pos] != '\"')
    {
        if (input[pos] != '\\')
            s.push_back(input[pos]);
        else
            switch (input[++pos])
            {
            case 'n':
                s.push_back('\n');
                break;
            case 'r':
                s.push_back('\r');
                break;
            case 't':
                s.push_back('\t');
                break;
            case '\\':
                s.push_back('\\');
                break;
            case '\'':
                s.push_back('\'');
                break;
            case '\"':
                s.push_back('\"');
                break;
            case '\?':
                s.push_back('\?');
                break;
            case '\0':
                s.push_back('\0');
                break;
            }
        ++pos;
    }
    return true;
}
