#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <Windows.h>
#include <iostream>
#include <cstdio>
using namespace std;

int main(int argc, TCHAR* argv[])
{
    if(3 != argc)
    {
        cout << "parameter error\n";
        exit(0);
    }

    TCHAR* buffer = new TCHAR[2048];
    SHELLEXECUTEINFO* cmd;

    cmd = new SHELLEXECUTEINFO;
    memset(cmd, 0, sizeof(SHELLEXECUTEINFO));
    cmd->cbSize = sizeof(SHELLEXECUTEINFO);
    cmd->fMask = SEE_MASK_NOCLOSEPROCESS;
    cmd->hwnd = NULL;
    cmd->lpVerb = "open";
    cmd->lpFile = "powershell.exe";
    cmd->lpParameters = buffer;
    cmd->lpDirectory = NULL;
    cmd->nShow = SW_HIDE;
    cmd->hInstApp = NULL;

    size_t begin = strtoul(argv[1], NULL, 10);
    size_t end = strtoul(argv[2], NULL, 10);
    for(size_t i = begin; i <= end; i++)
    {
        //CASE
        sprintf(buffer, "rm ./DAY%02u/CASE/01/index.files/*.js\nrm ./DAY%02u/CASE/01/*.bak\nmv ./DAY%02u/CASE/01/index* ./DAY%02u/CASE\nrm ./DAY%02u/CASE/01", i, i, i, i, i);
        ::ShellExecuteEx(cmd);
        WaitForSingleObject(cmd->hProcess, INFINITE);
        //EXERCISE
        sprintf(buffer, "rm ./DAY%02u/EXERCISE/01/index_answer.files/*.js\nrm ./DAY%02u/EXERCISE/01/*.bak\nmv ./DAY%02u/EXERCISE/01/index* ./DAY%02u/EXERCISE\nrm ./DAY%02u/EXERCISE/01", i, i, i, i, i);
        ::ShellExecuteEx(cmd);
        WaitForSingleObject(cmd->hProcess, INFINITE);
    }
    delete[] buffer;
    return 0;
}

