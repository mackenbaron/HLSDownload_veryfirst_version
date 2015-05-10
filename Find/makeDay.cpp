// Find.cpp : Defines the entry point for the console application.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <Windows.h>
using namespace std;
int main(int argc, char* argv[])
{
    size_t begin, end;
    char path[_MAX_PATH];

    if(3 != argc)
    {
        cout << "parameter error\n";
        exit(-1);
    }

    begin = strtoul(argv[1], NULL, 10);
    end = strtoul(argv[2], NULL, 10);

    if(begin > end)
    {
        cout << "parameter time sequence error\n";
        exit(-1);
    }

    for(; begin <= end; ++begin)
    {
        sprintf(path, "Day%02u", begin);
        ::CreateDirectory(path, NULL);
    }
    return 0;
}



