#!/bin/bash

export LC_ALL=C
#export DISTCC_FALLBACK=0

cd ../
for example in $( ls . )
do
	if [ -d "${example}" ]; then
		cd $example
		echo -e "\nBUILDING " $example " \n"
		#make clean
		make
		ret=$?
		if [ $ret -ne 0 ]; 
		then
			echo -e "\n" $example " compile FAILED \n" 
		else
			echo -e "\n" $example "compile SUCCESS \n" 
		fi
		cd ../
	fi
done