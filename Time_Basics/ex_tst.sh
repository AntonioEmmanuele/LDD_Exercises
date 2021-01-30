# !/bin/sh

echo "Press 1 to build the time test and 2 to clean the time test"
read INPUT
if [ $INPUT -eq 1 ]
then
	gcc -o time_test time_test.c
elif [ $INPUT -eq 2 ]
then
	rm time_test
else
	echo "Wrong command inserted"
fi
