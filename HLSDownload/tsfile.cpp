#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "tsfile.h"
#include <iostream>
using namespace std;
tsFile::tsFile(char* path, unsigned char* key, unsigned char* buffer, size_t size)
{
    strcpy(m_filePath, path);
    memcpy(m_key, key, sizeof(m_key));
    m_fileBuffer = buffer;
    m_fileSize = size;
    m_realHead[0] = 0x46;
    m_realHead[1] = 0x46;
    m_realHead[2] = 0x6d;
    m_realHead[3] = 0x70;
    m_realHead[4] = 0x65;
    m_realHead[5] = 0x67;
}

tsFile::~tsFile()
{
    delete[] m_fileBuffer;
}

void tsFile::decrypt(void)
{
    memset(m_iv, 0, 16);
    AES_set_decrypt_key(m_key, 128, &m_aesKey);
    m_temp = new unsigned char[m_fileSize];
    AES_cbc_encrypt(m_fileBuffer, m_temp, m_fileSize, &m_aesKey, m_iv, AES_DECRYPT);
    memcpy(m_fileBuffer, m_temp, m_fileSize);
    delete[] m_temp;
}

void tsFile::checkFileHead(void)
{
    m_temp = myFind(m_fileBuffer, 0x20, (unsigned char*)m_realHead, sizeof(m_realHead));
    if(m_temp == m_fileBuffer + 25)
    {
        return;
    }
    m_temp = myFind(m_fileBuffer, m_fileSize, (unsigned char*)m_realHead, sizeof(m_realHead));
    if(NULL == m_temp)
    {
        cout << "can not find FFmpeg in " << m_filePath << endl;
        return;
    }
    m_temp -= 0x19;
    m_fileSize -= (*(unsigned int*)&m_temp - *(unsigned int*)&m_fileBuffer);
    myCopy(m_fileBuffer, m_temp, m_fileSize);
}

void tsFile::writeFile()
{
    m_pFile = fopen(m_filePath, "wb");
    if(NULL == m_pFile)
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << "fopen error" << endl;
        printf("%s\n", m_filePath);
    }
    if(0 >= fwrite(m_fileBuffer, m_fileSize, 1, m_pFile))
    {
        cout << __FILE__ << ' ' << __LINE__ << ':' << "fwrite error" << endl;
        printf("%s\n", m_filePath);
        exit(0);
    }
    fclose(m_pFile);
}
