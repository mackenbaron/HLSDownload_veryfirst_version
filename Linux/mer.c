#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
int main(int argc,char* argv[])
{
    if(argc!=2)
    {
        printf("%d ",__LINE__);
        fflush(stdout);
        printf("I need dirname to merge\n");
        exit(-1);
    }

    struct dirent* pdirent;
    char* pathbuffer=(char*)malloc(256);
    sprintf(pathbuffer,"./%s",argv[1]);
    DIR* pdir;
    pdir=opendir(pathbuffer);
    if(NULL==pdir)
    {
        printf("%d ",__LINE__);
        fflush(stdout);
        perror("opendir");
        exit(-1);
    }
    size_t total=0;
    size_t i,j;
    for(;;)
    {
        pdirent=readdir(pdir);
        if(NULL==pdirent)
        {
            break;
        }
        printf("%s\n",pdirent->d_name);
        strcpy(pathbuffer,pdirent->d_name);
        for(i=0; i<strlen(pathbuffer); i++)
        {
            pathbuffer[i]=tolower(pathbuffer[i]);
        }
        if(NULL==strstr(pathbuffer,".ts"))
        {
            continue;
        }
        total++;
    }

    char nonesence[]="\"-a\" \"1\" \"-d\" \"0\" \"-S\" \"-T\" ";
    char noglgbal[]="\"--no-global-tags\" ";
    char nochapter[]="\"--no-chapters\" ";
    char* mkvmergecommandbuff=(char*)malloc(1024*1024);
    sprintf(mkvmergecommandbuff,"\"mkvmerge\" -o \"./%s.mkv\" \"--forced-track\" \"0:no\" \"--forced-track\" \"1:no\" %s",argv[1],nonesence);

    for(i=1; i<=total; i++)
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
        sprintf(pathbuffer,"\"./%s/%s-%u.ts\" \")\" ",argv[1],argv[1],i);
        strcat(mkvmergecommandbuff,pathbuffer);
        if(total==i)
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
        for(j=1; j<total; j++)
        {
			if(1==i&&(total-1)==j)
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
    free(mkvmergecommandbuff);
    free(pathbuffer);
    return 0;
}
