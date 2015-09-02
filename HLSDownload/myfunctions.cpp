#include"myfunctions.h"
using namespace std;
#pragma comment(lib,"wininet.lib")
unsigned char* myFind(unsigned char* source, size_t sourceSize, unsigned char* subBintoFind, size_t subBinSize)
{
    if(NULL == source || NULL == subBintoFind)
    {
        return NULL;
    }
    if(sourceSize < subBinSize)
    {
        return NULL;
    }
    size_t i;
    for(i = 0; i <= sourceSize - subBinSize; i++)
    {
        if(0 == memcmp(source + i, subBintoFind, subBinSize))
        {
            return source + i;
        }
    }
    return NULL;
}

void getRidOf(char* p, size_t size)
{
    if(NULL == p)
    {
        return;
    }
    size_t i;
    for(i = 0; i < size; i++)
    {
        if(*(p + i) == '\n')
        {
            *(p + i) = '\0';
        }
    }
}

void myCopy(unsigned char* dest, unsigned char* source, size_t size)
{
    if(NULL == dest || NULL == source)
    {
        return;
    }
    for(size_t i = 0; i < size; i++)
    {
        *(dest + i) = *(source + i);
    }
}

char* memcat(char* dst, const size_t dstsize, const char* src, const size_t srcsize)
{
    if(0 == dstsize || 0 == srcsize)
    {
        return NULL;
    }
    if(NULL == dst || NULL == src)
    {
        return NULL;
    }
    for(size_t i = 0;i < srcsize;i++)
    {
        *(dst + dstsize + i) = *(src + i);
    }
    return dst;
}

unsigned char* download(const char* serverName, char* strObject, char* referer, LPCSTR* acceptTypes, size_t* size)
{
    HINTERNET hSession = InternetOpenA("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:15.0) Gecko/20100101 Firefox/15.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_DONT_CACHE);
    if(NULL == hSession)
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << GetLastError() << endl;
        return NULL;
    }

    // Connect to www.******.com. 
    HINTERNET hConnect = InternetConnectA(hSession, serverName, INTERNET_DEFAULT_HTTP_PORT, "", "", INTERNET_SERVICE_HTTP, 0, 0);
    if(NULL == hConnect)
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << GetLastError() << endl;
        return NULL;
    }

    // Request the file /MSDN/MSDNINFO/ from the server. 
    HINTERNET hHttpFile = HttpOpenRequestA(hConnect, "GET", strObject, NULL, referer, acceptTypes, INTERNET_FLAG_RELOAD, 0);
    if(NULL == hHttpFile)
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << GetLastError() << endl;
        return NULL;
    }

    // Send the request. 
    BOOL bSendRequest = ::HttpSendRequest(hHttpFile, NULL, 0, NULL, 0);
    if(!bSendRequest)
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << GetLastError() << endl;
        return NULL;
    }

    // Get the length of the file. 
    char bufQuery[32];
    DWORD dwLengthBufQuery = sizeof(bufQuery);
    BOOL bQuery = ::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, bufQuery, &dwLengthBufQuery, NULL);
    if(!bQuery)
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << GetLastError() << endl;
        return NULL;
    }

    // Convert length from ASCII string to a DWORD. 
    DWORD dwFileSize = (DWORD)atol(bufQuery);
    *size = dwFileSize;
    if(dwFileSize <= 2)
    {
        return NULL;
    }
    // Allocate a buffer for the file. 
    unsigned char* buffer = new unsigned char[dwFileSize];

    //ZeroMemory(buffer, 0, sizeof(buffer));

    // Read the file into the buffer. 
    DWORD dwBytesRead;
    BOOL bRead = ::InternetReadFile(hHttpFile, buffer, dwFileSize, &dwBytesRead);
    if(!bRead || dwBytesRead != dwFileSize)
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << GetLastError() << endl;
        delete[] buffer;
        buffer = NULL;
    }
    ::InternetCloseHandle(hSession);
    ::InternetCloseHandle(hConnect);
    ::InternetCloseHandle(hHttpFile);
    return buffer;
}

void getTotalNumber(unsigned char* m3u8Buffer, size_t m3u8size, size_t* totalNumber, unsigned char* strtofind, size_t sizeoftofind)
{
    if(NULL == m3u8Buffer || NULL == totalNumber || 0 == m3u8size)
    {
        return;
    }
    unsigned char* tempBuffer = m3u8Buffer;
    size_t tempSize = m3u8size;
    size_t offSet = 0;
    while(true)
    {
        tempBuffer = myFind(tempBuffer, tempSize, strtofind, sizeoftofind);
        if(NULL != tempBuffer)
        {
            ++*totalNumber;
            tempBuffer += sizeoftofind;
            offSet = tempBuffer - m3u8Buffer;
            if(m3u8size > offSet)
            {
                tempSize = m3u8size - offSet;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

BOOL IsDirectory(const char *pDir)
{
    char szCurPath[500];
    ZeroMemory(szCurPath, 500);
    sprintf(szCurPath, "%s//*", pDir);
    WIN32_FIND_DATAA FindFileData;
    ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));

    HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData); /**< find first file by given path. */

    if(hFile == INVALID_HANDLE_VALUE)
    {
        FindClose(hFile);
        return FALSE; /** 如果不能找到第一个文件，那么没有目录 */
    }
    else
    {
        FindClose(hFile);
        return TRUE;
    }
}

BOOL DeleteDirectory(const char * DirName)
{
    //	CFileFind tempFind;		//声明一个CFileFind类变量，以用来搜索
    char szCurPath[MAX_PATH];		//用于定义搜索格式
    _snprintf(szCurPath, MAX_PATH, "%s//*.*", DirName);	//匹配格式为*.*,即该目录下的所有文件
    WIN32_FIND_DATAA FindFileData;
    ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
    HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);
    BOOL IsFinded = TRUE;
    while(IsFinded)
    {
        IsFinded = FindNextFileA(hFile, &FindFileData);	//递归搜索其他的文件
        if(strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) //如果不是"." ".."目录
        {
            string strFileName = "";
            strFileName = strFileName + DirName + "//" + FindFileData.cFileName;
            string strTemp;
            strTemp = strFileName;
            if(IsDirectory(strFileName.c_str())) //如果是目录，则递归地调用
            {
                printf("dir: %s\n", strFileName.c_str());
                DeleteDirectory(strTemp.c_str());
            }
            else
            {
                DeleteFileA(strTemp.c_str());
            }
        }
    }
    FindClose(hFile);

    BOOL bRet = RemoveDirectoryA(DirName);
    if(bRet == 0) //删除目录
    {
        printf("dir %s deletion failure\n", DirName);
        return FALSE;
    }
    return TRUE;
}