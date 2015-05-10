#ifndef TSFILE_H_INCLUDED
#define TSFILE_H_INCLUDED
#include <openssl\aes.h>
#pragma comment(lib,"libeay32.lib")
#include "myfunctions.h"
class tsFile
{
private:
    char m_filePath[512];
    size_t m_fileSize;
    unsigned char* m_fileBuffer;
    unsigned char* m_temp;
    unsigned char m_key[16];
    unsigned char m_iv[16];
    FILE* m_pFile;
    AES_KEY m_aesKey;
    //file head name
    unsigned char m_realHead[6];
public:
    tsFile::tsFile(char* path, unsigned char* key, unsigned char* buffer, size_t size);
    ~tsFile();
    void decrypt(void);
    void checkFileHead(void);
    void writeFile(void);
};
#endif // TSFILE_H_INCLUDED
