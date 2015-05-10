#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/aes.h>
#define DEGUBME
#undef DEGUBME
#define SLEEPTIME 0
unsigned char* myfind(unsigned char* source,size_t sourcesize,unsigned char* substrtofind,size_t substrsize)
{
    if(NULL == source || NULL == substrtofind)
    {
        return NULL;
    }
    if(sourcesize < substrsize)
    {
        return NULL;
    }
    size_t i;
    for(i = 0; i <= sourcesize - substrsize; i++)
    {
        if(0 == memcmp(source + i,substrtofind,substrsize))
        {
#ifdef DEGUBME
            printf("%u\n", i);
#endif // DEGUBME
            return source + i ;
        }
    }
    return NULL;
}
void getridof(char* p,size_t size)
{
    size_t i;
    for(i=0; i<size; i++)
    {
        if(*(p+i)=='\n')
        {
            *(p+i)=='\0';
        }
    }
}
int main(int Argc, char* Argv[])
{
    const char wgethead[]="wget --head \"User-Agent:	Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:34.0) Gecko/20100101 Firefox/34.0\" --head \"Referer:	http://tts5.tarena.com.cn/scripts/bofang/StrobeMediaPlayback.swf\"--head \"Host:	videotts.it211.com.cn\" --head \"DNT:	1\" --head \"Connection:	keep-alive\" --head \"Accept-Language:	zh-cn,zh;q=0.5\" --head \"Accept-Encoding:	gzip, deflate\" --head \"Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\"";
    const char wgeturl[]="http://videotts.it211.com.cn";
    unsigned char realhead[]= {0x46,0x46,0x6d,0x70,0x65,0x67};
    char nonesence[]="\"-a\" \"1\" \"-d\" \"0\" \"-S\" \"-T\" ";
    char noglgbal[]="\"--no-global-tags\" ";
    char nochapter[]="\"--no-chapters\" ";

    char* pathbuffer=(char*)malloc(1024);
    char* wgetbuffer=(char*)malloc(2048);
    unsigned char* Key=(unsigned char*)malloc(16);
    unsigned char* iv=(unsigned char*)malloc(16);
    char* mkvmergecommandbuff=(char*)malloc(1024*1024);


    FILE* ptofilestru;

    unsigned char* InputData;
    unsigned char* OutputData;

    DIR* pdir;
    struct dirent* pdirent;

    size_t i,j,FileSize;

    bool encrypted=false;

    if(Argc!=2)
    {
        printf("%d ",__LINE__);
        fflush(stdout);
        printf("I need dir (web) name ^_^\n");
        exit(-1);
    }

    mkdir(Argv[1],0777);

    //get .m3u8
    sprintf(wgetbuffer,"%s -P ./%s %s/%s/%s.m3u8",wgethead,Argv[1],wgeturl,Argv[1],Argv[1]);
#ifdef DEGUBME
    printf("%s\n\n",wgetbuffer);
#endif // DEBUG
    system(wgetbuffer);

    //open m3u8
    sprintf(pathbuffer,"./%s/%s.m3u8",Argv[1],Argv[1]);
#ifdef DEGUBME
    printf("%s\n\n",pathbuffer);
#endif // DEBUG

    ptofilestru=fopen(pathbuffer,"rb");
    if(NULL==ptofilestru)
    {
        printf("%d ",__LINE__);
        fflush(stdout);
        perror("fopen");
        exit(-1);
    }
    while(NULL!=fgets(pathbuffer,1024,ptofilestru))
    {
        if(0==strncmp(pathbuffer,"http:",strlen("http:")))
        {
            //get rid of \n
            getridof(pathbuffer,strlen(pathbuffer));
            //get ts
            sprintf(wgetbuffer,"%s -P ./%s %s",wgethead,Argv[1],pathbuffer);
#ifdef DEGUBME
            printf("%s\n\n",wgetbuffer);
#endif // DEGUBME
            system(wgetbuffer);
            usleep(SLEEPTIME);
        }
        if(0==strncmp(pathbuffer,"#EXT-X-KEY",strlen("#EXT-X-KEY")))
        {
            if(NULL!=strstr(pathbuffer,"AES"))
            {
                encrypted=true;
            }
        }
    }
    fclose(ptofilestru);

    sprintf(pathbuffer,"./%s",Argv[1]);
    pdir=opendir(pathbuffer);
    if(NULL==pdir)
    {
        printf("%d ",__LINE__);
        fflush(stdout);
        perror("opendir");
        exit(-1);
    }

    if(encrypted)
    {
        //get static.key
        sprintf(wgetbuffer,"%s -P ./%s %s/%s/static.key",wgethead,Argv[1],wgeturl,Argv[1]);
#ifdef DEGUBME
        printf("%s\n\n",wgetbuffer);
#endif // DEGUBME
        system(wgetbuffer);
        //get 128bit key
        sprintf(pathbuffer,"./%s/static.key",Argv[1]);
        ptofilestru=fopen(pathbuffer,"rb");
        if(NULL==ptofilestru)
        {
            printf("%d ",__LINE__);
            fflush(stdout);
            perror("fopen");
            exit(-1);
        }
        if(0>=fread(Key,16,1,ptofilestru))
        {
            printf("%d ",__LINE__);
            fflush(stdout);
            perror("fread");
            exit(-1);
        }
        fclose(ptofilestru);
#ifdef DEGUBME
        for(i=0; i<16; i++)
        {
            printf("%02hhX ",Key[i]);
        }
        printf("\n");
#endif // DEGUBME

        //decrypt ts
        for(;;)
        {
            pdirent=readdir(pdir);
            if(NULL==pdirent)
            {
                break;
            }
#ifdef DEGUBME
            printf("%s\n",pdirent->d_name);
#endif // DEGUBME
            strcpy(pathbuffer,pdirent->d_name);
            for(i=0; i<strlen(pathbuffer); i++)
            {
                pathbuffer[i]=tolower(pathbuffer[i]);
            }

            if(!strstr(pathbuffer,".ts"))
            {
                continue;
            }
#ifdef DEGUBME
            printf("%s\n",pathbuffer);
#endif // DEGUBME

            sprintf(pathbuffer,"./%s/%s",Argv[1],pdirent->d_name);
            ptofilestru = fopen(pathbuffer, "rb");
            if(NULL==ptofilestru)
            {
                printf("%d ",__LINE__);
                fflush(stdout);
                perror("fopen");
                break;
            }
            fseek(ptofilestru,0,SEEK_END);//移动到文件结尾
            FileSize=ftell(ptofilestru);//获取文件尺寸
            rewind(ptofilestru);//

            unsigned char* InputData = malloc(FileSize);
            unsigned char* OutputData = malloc(FileSize);

            if(0>=fread(InputData, FileSize, 1, ptofilestru))
            {
                printf("%d ",__LINE__);
                fflush(stdout);
                perror("fread");
                exit(0);
            }
            fclose(ptofilestru);

            memset(iv, 0, 16);
            AES_KEY AesKey;
            AES_set_decrypt_key(Key, 128, &AesKey);
            AES_cbc_encrypt(InputData, OutputData, FileSize, &AesKey, iv, AES_DECRYPT);

            //strcat(pathbuffer,".decrypted.ts");
            ptofilestru = fopen(pathbuffer, "wb");
            if(0>=fwrite(OutputData, FileSize, 1, ptofilestru))
            {
                printf("%d ",__LINE__);
                fflush(stdout);
                perror("fwrite");
                exit(0);
            }
            fclose(ptofilestru);
            usleep(SLEEPTIME);

            free(InputData);
            free(OutputData);
        }

    }

//check file head
    rewinddir(pdir);

    for(;;)
    {
        pdirent=readdir(pdir);
        if(NULL==pdirent)
        {
            break;
        }
        strcpy(pathbuffer,pdirent->d_name);
        for(i=0; i<strlen(pathbuffer); i++)
        {
            pathbuffer[i]=tolower(pathbuffer[i]);
        }
        if(!strstr(pathbuffer,".ts"))
        {
            continue;
        }
        sprintf(pathbuffer,"./%s/%s",Argv[1],pdirent->d_name);
        ptofilestru = fopen(pathbuffer, "rb");
        if(NULL==ptofilestru)
        {
            printf("%d ",__LINE__);
            fflush(stdout);
            perror("fopen");
            break;
        }

        if(0>=fread(pathbuffer,0x20,1, ptofilestru))
        {
            printf("%d ",__LINE__);
            fflush(stdout);
            perror("fread");
            exit(0);
        }
        OutputData=myfind(pathbuffer,0x20,realhead,sizeof(realhead));
        if(OutputData==(unsigned char*)pathbuffer+25)
        {
            continue;
        }
        else
        {
            rewind(ptofilestru);
            fseek(ptofilestru,0,SEEK_END);//移动到文件结尾
            FileSize=ftell(ptofilestru);//获取文件尺寸

            rewind(ptofilestru);//
            InputData = (unsigned char*) malloc(FileSize);
            if(0 >= fread(InputData,FileSize,1,ptofilestru))
            {
                printf("%d ",__LINE__);
                fflush(stdout);
                perror("fread");
                exit(0);
            }
            fclose(ptofilestru);

            OutputData=myfind(InputData,FileSize,realhead,sizeof(realhead));
            if(NULL==OutputData)
            {
                printf("can not find FFmpeg in %s\n",pdirent->d_name);
                continue;
            }
            OutputData-=0x19;

            i = (unsigned int) OutputData - (unsigned int) InputData;

            sprintf(pathbuffer,"./%s/%s",Argv[1],pdirent->d_name);
            ptofilestru = fopen(pathbuffer,"wb");
            if(0 >= fwrite(OutputData,FileSize - i,1,ptofilestru))
            {
                printf("%d ",__LINE__);
                fflush(stdout);
                perror("fwrite");
                exit(0);
            }
            fclose(ptofilestru);
            usleep(SLEEPTIME);
            free(InputData);
        }
    }


    //merge
    rewinddir(pdir);
    FileSize=0;
    for(;;)
    {
        pdirent=readdir(pdir);
        if(NULL==pdirent)
        {
            break;
        }
#ifdef DEGUBME
        printf("%s\n",pdirent->d_name);
#endif // DEGUBME
        strcpy(pathbuffer,pdirent->d_name);
        for(i=0; i<strlen(pathbuffer); i++)
        {
            pathbuffer[i]=tolower(pathbuffer[i]);
        }
        if(NULL==strstr(pathbuffer,".ts"))
        {
            continue;
        }
        FileSize++;
    }
    closedir(pdir);

    sprintf(mkvmergecommandbuff,"\"mkvmerge\" -o \"./%s.mkv\" \"--forced-track\" \"0:no\" \"--forced-track\" \"1:no\" %s",Argv[1],nonesence);

    for(i=1; i<=FileSize; i++)
    {
        strcat(mkvmergecommandbuff,noglgbal);
        strcat(mkvmergecommandbuff,nochapter);
        if(1==i)
        {
            strcat(mkvmergecommandbuff,"\"(\" ");
        }
        else
        {
            strcat(mkvmergecommandbuff,"\"+\" \"(\" ");
        }
        sprintf(pathbuffer,"\"./%s/%s-%u.ts\" \")\" ",Argv[1],Argv[1],i);
        strcat(mkvmergecommandbuff,pathbuffer);
        if(FileSize==i)
        {
            continue;
        }
        else
        {
            strcat(mkvmergecommandbuff,nonesence);
        }
    }

    strcat(mkvmergecommandbuff,"\"--track-order\" \"0:0,0:1\" \"--append-to\" \"");
    for(i=0; i<2; i++)
    {
        for(j=1; j<FileSize; j++)
        {
            if(1==i&&(FileSize-1)==j)
            {
                sprintf(pathbuffer,"%u:%u:%u:%u",j,i,j-1,i);
            }
            else
            {
                sprintf(pathbuffer,"%u:%u:%u:%u,",j,i,j-1,i);
            }
            strcat(mkvmergecommandbuff,pathbuffer);
        }
    }
    strcat(mkvmergecommandbuff,"\"");
    system(mkvmergecommandbuff);

    free(pathbuffer);
    free(wgetbuffer);
    free(Key);
    free(iv);
    free(mkvmergecommandbuff);
    return 0;
}
