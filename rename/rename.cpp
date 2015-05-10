// rename.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    if(3 != argc)
    {
        cout << "parameter error\n";
        exit(-1);
    }

    size_t begin = stoul(argv[1]);
    size_t end = stoul(argv[2]);

    wchar_t buffer1[MAX_PATH];
    wchar_t buffer2[MAX_PATH];

    for(size_t i = begin; i < end; i++)
    {
        wsprintf(buffer1, L"./%u", i);
        wsprintf(buffer2, L"./Day%02u", i - begin + 1);
        ::MoveFile(buffer1,buffer2);
    }

    return 0;
}

