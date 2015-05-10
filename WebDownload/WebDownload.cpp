#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    if(5 != argc)
    {
        cout << "parameter error: url day beginnumber endnumber\n";
        exit(-1);
    }
    size_t begin;
    size_t end;
    size_t day;
    day = strtoul(argv[2], NULL, 10);
    begin = strtoul(argv[3], NULL, 10);
    end = strtoul(argv[4], NULL, 10);
    if(begin > end)
    {
        cout << "parameter time sequence error\n";
        exit(-1);
    }

    char* URLBuffer = new char[1024];

    for(size_t i = begin;i <= end;++i)
    {
        /*
        http://pdf5.tarena.com.cn/tts6materials/TTS6/EMBED/ESD_V01/ARM/DAY05/COURSE/EMBEDESD_V01ARMDAY05_018.png
        URL: http://pdf5.tarena.com.cn/tts6materials/TTS6/EMBED/ESD_V01/ARM


        http://pdf5.tarena.com.cn/tts6materials/TTS6/EMBED/ESD_V01/PORTING/DAY01/COURSE/EMBEDESD_V01PORTINGDAY01_039.png


        http://pdf5.tarena.com.cn/tts6materials/TTS6/UID/USD_V01/GUI-PREPARATORY/DAY01/COURSE/UIDUSD_V01GUI-PREPARATORYDAY01_047.png

        http://pdf5.tarena.com.cn/tts6materials/TTS6/UID/USD_V01/GUI-PHOTOSHOP/DAY01/COURSE/UIDUSD_V01GUI-PHOTOSHOPDAY01_021.png

        */

        //--head \"User-Agent:	Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:34.0) Gecko/20100101 Firefox/34.0\" --head \"Referer:	http://tts6.tarena.com.cn/user/tts6\"--head \"Host:	pdf5.tarena.com.cn\" --head \"Connection:	keep-alive\" --head \"Accept-Encoding:	gzip, deflate\" --head \"Accept:	image/png,image/*;q=0.8,*/*;q = 0.5\"  


        sprintf(URLBuffer, "wget -P ./DAY%02u/COURSE http://pdf5.tarena.com.cn/tts6materials/TTS6/UID/USD_V01/%s/DAY%02u/COURSE/UIDUSD_V01%sDAY%02u_%03u.png ", day, argv[1], day, argv[1], day, i);
        //printf("%s\n", URLBuffer);

        system(URLBuffer);
    }

    delete[] URLBuffer;

    return 0;
}

