#ifndef MYFUNCTIONS_H_INCLUDED
#define MYFUNCTIONS_H_INCLUDED
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstring>
#include <Windows.h>
#include <iostream>
#include <WinInet.h>

unsigned char* myFind(unsigned char* source, size_t sourceSize, unsigned char* subBintoFind, size_t subBinSize);
void myCopy(unsigned char* dest, unsigned char* source, size_t size);
void getRidOf(char* p, size_t size);
char* memcat(char* dst, const size_t dstsize, const char* src, const size_t srcsize);
unsigned char* download(const char* serverName, char* strObject, char* referer, LPCSTR* acceptTypes, size_t* size);
void getTotalNumber(unsigned char* m3u8Buffer, size_t m3u8size, size_t* totalNumber, unsigned char* strtofind, size_t sizeoftofind);
BOOL IsDirectory(const char *pDir);
BOOL DeleteDirectory(const char * DirName);

#endif // MYFUNCTIONS_H_INCLUDED
