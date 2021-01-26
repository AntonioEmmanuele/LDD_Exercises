# !/bin/sh

echo "Press 1 to build the reader/writer test and 2 to clean the r/w test"
echo "Press 3 to build the ioctl test and 4 to clean the ioctl test"
read INPUT
if [ $INPUT -eq 1 ]
then
	gcc -o writer writer.c
	gcc -o reader reader.c -lpthread
elif [ $INPUT -eq 2 ]
then
	rm writer
	rm reader
elif [ $INPUT -eq 3 ]
then
	gcc -o ioctl_tst ioctl_test.c
elif [ $INPUT -eq 4 ]
then
	rm ioctl_tst
else
	echo "Wrong command inserted"
fi
