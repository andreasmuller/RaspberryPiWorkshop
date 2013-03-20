#!/bin/bash

export LC_ALL=C
#export DISTCC_FALLBACK=0

cd ../
for example in $( ls . )
do
	if [ -d "${example}" ]; then
		cd $example
		echo -e "\nCLEANING " $example " \n"
		make clean
		ret=$?
		if [ $ret -ne 0 ]; 
		then
			echo -e "\n" $example " clean FAILED \n" 
		else
			echo -e "\n" $example "clean SUCCESS \n" 
		fi
		cd ../
	fi
done