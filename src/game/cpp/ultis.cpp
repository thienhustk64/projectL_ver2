#include "ultis.h"

using namespace std;

vector<string> splitString(string str, string delimeters)
{
    vector<string> result;
    char *i; 

    vector<char> buffer(str.begin(), str.end());
    buffer.push_back('\0');

    i = strtok(&buffer[0], delimeters.c_str());

    while(i != NULL)
    {
        result.push_back(i);
        i = strtok(NULL, delimeters.c_str());
    }

    return result;
}