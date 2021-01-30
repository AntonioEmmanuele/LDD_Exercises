/****************************************************************************************
* This file is part of LDD_Exercises Project. *
* *
* Copyright  © 2021 By Antonio Emmanuele. All rights reserved. *
* @mail: antony.35.ae@gmail.com *
* *
* LDD_Exercises is free software: you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation, either version 3 of the License, or *
* (at your option) any later version. *
* *
* LDD_Exercises is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY; without even the implied warranty of *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
* GNU General Public License for more details. *
* *
* You should have received a copy of the GNU General Public License *
* along with The ProjectName Project. If not, see <https://www.gnu.org/licenses/>. *
* *
* In case of use of this project, I ask you to mention me, to whom it may concern. *
*****************************************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "commands.h"
#include "assert.h"
#include "sys/ioctl.h"
//#include "libexplain/ioctl.h"
static int fd;
static unsigned int ctl=1;
static unsigned int cmd=0;
static int read_val =0;
unsigned long seconds;
int main(void){
	fd=open("/dev/My_file0",O_RDWR);
	if(fd<0){
		perror("Error opening the device \n");
		exit(1);
	}
	int err=0;
	while(ctl){
		printf(" \n \n \n Insert 1 for print curr time \n Insert 2 to do a time test  \n Insert 3  to busy wait \n Insert 4 to wait yielding cpu \n Insert 5 to wait with wait_queues \n Insert 10 to exit \n \n \n \n " );
		scanf("%u",&cmd);
		switch(cmd){
			case 1:
				err=ioctl(fd,MYFILE_IOC_PRINTCURRTIME,&read_val);
				if(err<0)
					perror("Error in  ioctl \n");
				break;
			case 2:
				err=ioctl(fd,MYFILE_IOC_TIMETESTS,NULL);
				if(err<0)
					printf("Error %d \n",err);
				break;
			case 3:
				printf("Insert seconds to wait \n");
				scanf("%lu",&seconds);
				printf("Seconds inserted %lu \n",seconds);
				err=ioctl(fd,MYFILE_IOC_BUSYWAITING,&seconds);
				if(err<0)
					printf("Error %d \n",err);
				break;
			case 4:
				printf("Insert seconds to wait \n");
				scanf("%lu",&seconds);
				printf("Seconds inserted %lu \n",seconds);
				err=ioctl(fd,MYFILE_IOC_CPUYIELD,&seconds);
				if(err<0)
					printf("Error %d \n",err);
				break;
			
			case 5:
				printf("Insert seconds to wait \n");
				scanf("%lu",&seconds);
				printf("Seconds inserted %lu \n",seconds);
				err=ioctl(fd,MYFILE_IOC_WAITWITHWQ,&seconds);
				if(err<0)
					printf("Error %d \n",err);
				break;
				
			case 10:
				ctl=0;
				break;
			default:
				printf("Invalid cmd \n");
				break;
		}
	}
	return 0;
}
