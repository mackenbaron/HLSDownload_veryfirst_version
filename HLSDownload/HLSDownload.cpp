// HLSDownload.cpp : Defines the entry point for the console application.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <vector>
#include <cstdio>
#include <Windows.h>
#include <tchar.h>
#include "myfunctions.h"
#include "tsfile.h"
using namespace std;

int main(int argc, char* argv[])
{
    char* pathBuffer = new char[1024];
    char* parameterCat = new char[256];
    char* mkvmergecommandbuff = new char[1024 * 1024];
    unsigned char key[16];
    char nonesence[] = "\"-a\" \"1\" \"-d\" \"0\" \"-S\" \"-T\" ";
    char noglgbal[] = "\"--no-global-tags\" ";
    char nochapter[] = "\"--no-chapters\" ";
    char serverName[] = "videotts.it211.com.cn";
    char referer[] = "http://tts8.tmooc.cn/scripts/bofang/bofang/StrobeMediaPlayback.swf";
    LPCSTR acceptTypes[] = {"*/*", NULL};
    size_t fileSize;
    size_t totalNumber;
    size_t begin;
    size_t end;
    DWORD exitCode;
    SHELLEXECUTEINFOA* mkvinfo = new SHELLEXECUTEINFOA;
    bool encrypted;
    unsigned char* receiveBuffer;
    vector<tsFile*> receiveTs;
    vector<tsFile*>::iterator it;

    if(argc != 4)
    {
        cout << "parameter format: class monday monday\n";
        exit(-1);
    }

    begin = strtoul(argv[2], NULL, 10);
    end = strtoul(argv[3], NULL, 10);

    if(begin > end)
    {
        cout << "parameter time sequence error\n";
        exit(-1);
    }

    for(size_t j = begin; j <= end; ++j)
    {
#ifndef JAVA
        for(size_t i = 1; i <= 2; ++i)
        {
#endif //JAVA
            encrypted = false;
#ifndef JAVA
            if(1 == i)
            {
                sprintf(parameterCat, "%s%04uam", argv[1], j);
            }
            else
            {
                sprintf(parameterCat, "%s%04upm", argv[1], j);
            }
#endif //JAVA
#ifdef JAVA
            sprintf(parameterCat, "%s%04u", argv[1], j);
#endif //JAVA

            CreateDirectoryA(parameterCat, NULL);

            sprintf(pathBuffer, "/%s/%s.m3u8", parameterCat, parameterCat);

#ifdef _DEBUG
            printf("%s\n", pathBuffer);
#endif

            receiveBuffer = download(serverName, pathBuffer, referer, acceptTypes, &fileSize);
            if(NULL == receiveBuffer)
            {
                cout << __FILE__ << ' ' << __LINE__ << ':' << "download error" << endl;
                exit(-1);
            }

            sprintf(pathBuffer, "http://videotts.it211.com.cn/%s/%s-", parameterCat, parameterCat);
#ifdef _DEBUG
            printf("%s\n", pathBuffer);
#endif

            totalNumber = 0;
            getTotalNumber(receiveBuffer, fileSize, &totalNumber, (unsigned char*)pathBuffer, strlen(pathBuffer));
            encrypted = (NULL != myFind(receiveBuffer, fileSize, (unsigned char*)"AES", strlen("SEA")));

#ifdef _DEBUG
            sprintf(pathBuffer, "./%s/%s.m3u8", parameterCat, parameterCat);
            FILE* fp = fopen(pathBuffer, "wb");
            if(NULL == fp)
            {
                perror("fopen");
                exit(-1);
            }
            if(0 >= fwrite(receiveBuffer, fileSize, 1, fp))
            {
                perror("fwrite");
                exit(-1);
            }
            fclose(fp);
#endif //_DEBUG

            delete[] receiveBuffer;

#ifdef _DEBUG
            cout << "m3u8 size:" << totalNumber << endl;
            cout << "encrypted?:" << boolalpha << encrypted << endl;
#endif //_DEBUG

            if(encrypted)
            {
                sprintf(pathBuffer, "/%s/static.key", parameterCat);
                receiveBuffer = download(serverName, pathBuffer, referer, acceptTypes, &fileSize);
                if(NULL == receiveBuffer)
                {
                    cout << __FILE__ << ' ' << __LINE__ << ':' << "download error" << endl;
                    printf("%s\n", pathBuffer);
                    exit(-1);
                }
#ifdef _DEBUG
                cout << "key transfered size:" << fileSize << endl;
#endif
                memcpy(key, receiveBuffer, 16);
                delete[] receiveBuffer;
            }

            for(size_t i = 1; i <= totalNumber; ++i)
            {
                sprintf(pathBuffer, "/%s/%s-%u.ts", parameterCat, parameterCat, i);
#ifdef _DEBUG
                printf("%s\n", pathBuffer);
#endif
                receiveBuffer = download(serverName, pathBuffer, referer, acceptTypes, &fileSize);
                if(NULL == receiveBuffer)
                {
                    cout << __FILE__ << ' ' << __LINE__ << ':' << "download error" << endl;
                    printf("%s\n", pathBuffer);
                }
                sprintf(pathBuffer, "./%s/%s-%u.ts", parameterCat, parameterCat, i);
                receiveTs.push_back(new tsFile(pathBuffer, key, receiveBuffer, fileSize));
            }

            if(encrypted)
            {
                for(it = receiveTs.begin(); it != receiveTs.end(); ++it)
                {
                    (*it)->decrypt();
                }
            }

            for(it = receiveTs.begin(); it != receiveTs.end(); ++it)
            {
                (*it)->checkFileHead();
            }

            for(it = receiveTs.begin(); it != receiveTs.end(); ++it)
            {
                (*it)->writeFile();
            }

            for(it = receiveTs.begin(); it != receiveTs.end(); ++it)
            {
                delete (*it);
            }

            receiveTs.clear();

            sprintf(mkvmergecommandbuff, "-o \"./%s.mkv\" \"--forced-track\" \"0:no\" \"--forced-track\" \"1:no\" %s", parameterCat, nonesence);

            for(size_t i = 1; i <= totalNumber; i++)
            {
                strcat(mkvmergecommandbuff, noglgbal);
                strcat(mkvmergecommandbuff, nochapter);
                if(1 == i)
                {
                    strcat(mkvmergecommandbuff, "\"(\" ");
                }
                else
                {
                    strcat(mkvmergecommandbuff, "\"+\" \"(\" ");
                }
                sprintf(pathBuffer, "\"./%s/%s-%u.ts\" \")\" ", parameterCat, parameterCat, i);
                strcat(mkvmergecommandbuff, pathBuffer);
                if(totalNumber == i)
                {
                    continue;
                }
                else
                {
                    strcat(mkvmergecommandbuff, nonesence);
                }
            }

            strcat(mkvmergecommandbuff, "\"--track-order\" \"0:0,0:1\" \"--append-to\" \"");
            for(size_t i = 0; i < 2; i++)
            {
                for(size_t j = 1; j < totalNumber; j++)
                {
                    if(1 == i && (totalNumber - 1) == j)
                    {
                        sprintf(pathBuffer, "%u:%u:%u:%u", j, i, j - 1, i);
                    }
                    else
                    {
                        sprintf(pathBuffer, "%u:%u:%u:%u,", j, i, j - 1, i);
                    }
                    strcat(mkvmergecommandbuff, pathBuffer);
                }
            }
            strcat(mkvmergecommandbuff, "\"");

            memset(mkvinfo, 0, sizeof(SHELLEXECUTEINFO));
            mkvinfo->cbSize = sizeof(SHELLEXECUTEINFO);
            mkvinfo->fMask = SEE_MASK_NOCLOSEPROCESS;
            mkvinfo->hwnd = NULL;
            mkvinfo->lpVerb = "open";
            mkvinfo->lpFile = "mkvmerge.exe";
            mkvinfo->lpParameters = mkvmergecommandbuff;
            mkvinfo->lpDirectory = NULL;
            mkvinfo->nShow = SW_HIDE;
            mkvinfo->hInstApp = NULL;

            ShellExecuteExA(mkvinfo);
            WaitForSingleObject(mkvinfo->hProcess, INFINITE);
            GetExitCodeProcess(mkvinfo->hProcess, &exitCode);
            if(0 != exitCode)
            {
                printf("%s merge error\n", parameterCat);
            }
#ifndef _DEBUG
            Sleep(1000);
            DeleteDirectory(parameterCat);
#endif
#ifndef JAVA
        }
#endif //JAVA
    }
    delete mkvinfo;
    delete[] mkvmergecommandbuff;
    delete[] pathBuffer;
    delete[] parameterCat;

    system("pause");

    return 0;
}

