#!bin/sh

MODNAME=ex1.ko
major=0
mode=664
while :
do
	echo " Press:
		1 to build
		2 to insert mod
		3 to remove
		4 to clean
		5 exit
		6 to  create nodes
		7 to delete nodes
		"
	read INPUT
	if [ $INPUT -eq 1 ]
	then
		make
	elif [ $INPUT -eq 2 ]
	then
		insmod $MODNAME
	elif [ $INPUT -eq 3 ]
	then
		rmmod $MODNAME
	elif [ $INPUT -eq 4 ]
	then
			make clean
	elif [ $INPUT -eq 5 ]
	then
		break
	elif [ $INPUT -eq 6 ]
	then
		dev_name="My_file"
		#retriving major number
		major=$(awk '$2== "My_file" {print $1}' /proc/devices)
		echo $major
		#removing stale nodes
		rm -f /dev/${dev_name}0
		#creating nodes
		mknod /dev/${dev_name}0 c $major 0
		#mknod /dev/${dev_name}1 c $major 1
		#mknod /dev/${dev_name}2 c $major 2
		#changing permissions
		group="staff"
		grep -q '^staff:' /etc/group || group="wheel"
		chgrp $group /dev/${dev_name}0
		chmod $mode  /dev/${dev_name}0
	elif [ $INPUT -eq 7 ]
	then
		rm -f /dev/${dev_name}0
	else
		echo" Reinsert a number"
	fi	
done
echo "Bye\n"
echo $INPUT
