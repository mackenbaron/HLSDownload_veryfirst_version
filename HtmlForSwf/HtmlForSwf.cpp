#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
    if(4 != argc)
    {
        cout << "parameter error: filename day day\n";
        exit(-1);
    }
    size_t begin;
    size_t end;
    begin = strtoul(argv[2], NULL, 10);
    end = strtoul(argv[3], NULL, 10);
    if(begin > end)
    {
        cout << "parameter time sequence error\n";
        exit(-1);
    }
    char* htmlBuffer = new char[1024 * 1024];
    char* filename = new char[1024];

    FILE* fp;

    for(size_t i = begin;i <= end;++i)
    {
        sprintf(htmlBuffer, "<object data=\"../FlexPaperViewer.swf\" type=\"application/x-shockwave-flash\" width=\"100%%\" height=\"100%%\"><param name=\"allowfullscreen\" value=\"true\"><param name=\"allowscriptaccess\" value=\"always\"><param name=\"quality\" value=\"high\"><param name=\"wmode\" value=\"transparent\"><param name=\"flashvars\" value=\"SwfFile=./%s%02u.swf&amp;Scale=0.6&amp;ZoomTransition=easeOut&amp;ZoomTime=0.5&amp;ZoomInterval=0.2&amp;FitWidthOnLoad=true&amp;MinZoomSize=0.2&amp;MaxZoomSize=5&amp;InitViewMode=Portrait&amp;ViewModeToolsVisible=true&amp;ZoomToolsVisible=true&amp;NavToolsVisible=true&amp;CursorToolsVisible=true&amp;SearchToolsVisible=true&amp;localeChain=en_US\"></object>\n", argv[1], i);
        sprintf(filename, "./%s%02u.html", argv[1], i);
        fp = fopen(filename, "wb");
        if(NULL == fp)
        {
            perror("fopen");
        }
        if(0 >= fwrite(htmlBuffer, strlen(htmlBuffer), 1, fp))
        {
            perror("fwrite");
        }
        fclose(fp);
    }

    delete[] htmlBuffer;
    delete[] filename;
    return 0;
}

