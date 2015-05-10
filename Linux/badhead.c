#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
unsigned char* myfind(unsigned char* source,size_t sourcesize,unsigned char* substrtofind,size_t substrsize)
{
    if(NULL == source || NULL == substrtofind)
    {
        return NULL;
    }
    if(sourcesize <= substrsize)
    {
        return NULL;
    }
    size_t i;
    for(i = 0; i < sourcesize - substrsize; i++)
    {
        if(0 == memcmp(source + i,substrtofind,substrsize))
        {
            //printf("%hhx\n",*(source + i));
            return source + i ;
        }
    }
    return NULL;
}

int main(int argc,char* argv[])
{
    unsigned char realhead[] = {0x46,0x46,0x6D,0x70,0x65,0x67};
    unsigned char* InputData;
    unsigned char* correct;
    struct dirent* pdirent;
    char* pathbuffer=(char*)malloc(256);
    DIR* pdir;
    FILE* hInputTsFile;
    FILE* hOutputTsFile;
    size_t FileSize,i;

    if(argc!=2)
    {
        printf("%d ",__LINE__);
        fflush(stdout);
        printf("I need dirname to correct\n");
        exit(-1);
    }

    sprintf(pathbuffer,"./%s",argv[1]);
    pdir=opendir(pathbuffer);
    if(NULL==pdir)
    {
        printf("%d ",__LINE__);
        fflush(stdout);
        perror("opendir");
        exit(-1);
    }

    for(;;)
    {
        pdirent=readdir(pdir);
        if(NULL==pdirent)
        {
            break;
        }
        // printf("%s\n",pdirent->d_name);
        strcpy(pathbuffer,pdirent->d_name);
        for(i=0; i<strlen(pathbuffer); i++)
        {
            pathbuffer[i]=tolower(pathbuffer[i]);
        }

        if(!strstr(pathbuffer,".ts"))
        {
            continue;
        }
        printf("%s\n",pathbuffer);

        sprintf(pathbuffer,"./%s/%s",argv[1],pdirent->d_name);
        hInputTsFile = fopen(pathbuffer,"rb");
        if(NULL == hInputTsFile)
        {
            printf("%d ",__LINE__);
            fflush(stdout);
            perror("fopen");
            //break;
            exit(-1);
        }
        fseek(hInputTsFile,0,SEEK_END);//移动到文件结尾
        FileSize = ftell(hInputTsFile);//获取文件尺寸
        rewind(hInputTsFile);//

        InputData = (unsigned char*) malloc(FileSize);
        //unsigned char* OutputData = (unsigned char*) malloc(FileSize);

        //printf("%u\n",sizeof(realhead));

        if(0 >= fread(InputData,FileSize,1,hInputTsFile))
        {
            printf("%d ",__LINE__);
            fflush(stdout);
            perror("fread");
            exit(0);
        }
        fclose(hInputTsFile);

        //printf("%hhx\n",*(char*) InputData);
        correct = myfind(InputData,FileSize,realhead,sizeof(realhead));
        //printf("%p\n",correct);
        if(correct==InputData)
            continue;
        correct=correct-0x19
        /*for( i = 0; i < 0x25; i++)
        {
            printf("%c",*(correct+i));
        }
        printf("\n");*/
        i = (unsigned int) correct - (unsigned int) InputData;
        //printf("%u\n",i);
        //strcat(pathbuffer,".decrypted.ts");
        hOutputTsFile = fopen(pathbuffer,"wb");
        if(0 >= fwrite(correct,FileSize - i,1,hOutputTsFile))
        {
            printf("%d ",__LINE__);
            fflush(stdout);
            perror("fwrite");
            exit(0);
        }
        fclose(hOutputTsFile);
        free(InputData);
    }
    free(pathbuffer);
    return 0;
}
