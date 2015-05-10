#!/bin/bash
wget --head "User-Agent:	Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:34.0) Gecko/20100101 Firefox/34.0" --head "Referer:	http://tts5.tarena.com.cn/scripts/bofang/StrobeMediaPlayback.swf"--head "Host:	videotts.it211.com.cn" --head "DNT:	1" --head "Connection:	keep-alive" --head "Accept-Language:	zh-cn,zh;q=0.5" --head "Accept-Encoding:	gzip, deflate" --head "Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" -P ./csd14101030am http://videotts.it211.com.cn/csd14101030am/csd14101030am.m3u8
wget --head "User-Agent:	Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:34.0) Gecko/20100101 Firefox/34.0" --head "Referer:	http://tts5.tarena.com.cn/scripts/bofang/StrobeMediaPlayback.swf"--head "Host:	videotts.it211.com.cn" --head "DNT:	1" --head "Connection:	keep-alive" --head "Accept-Language:	zh-cn,zh;q=0.5" --head "Accept-Encoding:	gzip, deflate" --head "Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" -P ./csd14101030am http://videotts.it211.com.cn/csd14101030pm/static.key
for((i=1;i<=$1;i++))
do
	wget --head "User-Agent:	Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:34.0) Gecko/20100101 Firefox/34.0" --head "Referer:	http://tts5.tarena.com.cn/scripts/bofang/StrobeMediaPlayback.swf"--head "Host:	videotts.it211.com.cn" --head "DNT:	1" --head "Connection:	keep-alive" --head "Accept-Language:	zh-cn,zh;q=0.5" --head "Accept-Encoding:	gzip, deflate" --head "Accept:	text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8" -P ./csd14101030am http://videotts.it211.com.cn/csd14101030pm/csd14101030pm-$i.ts
done

