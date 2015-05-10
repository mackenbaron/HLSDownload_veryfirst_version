#!/bin/bash

for((I=$1;I<=$2;I++))
do
#	./dir csd14060"$I"am
#	./dir csd14060"$I"pm
	./mer csd14060"$I"am
	./mer csd14060"$I"pm
#	mkdir ./csd1410"$I"
#	mv ./csd1410"$I"am ./csd1410"$I"
#	mv ./csd1410"$I"pm ./csd1410"$I"
done
