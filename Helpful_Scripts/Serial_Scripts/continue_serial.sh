#!/bin/bash

for i in 1 2 3
do 
	for p in /dev/tty.usbmodem* 
	do 
		echo $p; echo
		echo 1 > $p
	done
done
echo Done