#!/bin/bash

for((I=$2;I<=$3;I++))
do
	DIR=`printf "%s%04d" $1 $I`
	./superdownload "$DIR"am
	./superdownload "$DIR"pm
	#if [ -e ./"$DIR"pm.mkv ]
	#then
	#	mkdir ./"$DIR"
	#	mv ./"$DIR"am.mkv ./"$DIR"
	#	mv ./"$DIR"pm.mkv ./"$DIR"	
	#fi
	#rm -rf ./"$DIR"am
	#rm -rf ./"$DIR"pm
done

