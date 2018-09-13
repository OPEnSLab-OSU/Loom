#!/bin/bash

cd "$( find ~/Downloads -type d -name "Dependencies" )"
for z in *.zip
do 
	unzip $z -d  ~/Documents/Arduino/libraries/
done