# !/bin/sh

echo "Press 1 to build the alloc test and 2 to clean the alloc test"
read INPUT
if [ $INPUT -eq 1 ]
then
	gcc -o alloc_test alloc_test.c
elif [ $INPUT -eq 2 ]
then
	rm alloc_test
else
	echo "Wrong command inserted"
fi
