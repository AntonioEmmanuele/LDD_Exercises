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
int main(void){
	fd=open("/dev/My_file0",O_RDWR);
	if(fd<0){
		perror("Error opening the device \n");
		exit(1);
	}
	int err=0;
	while(ctl){
		printf(" \n \n \n Insert 1 for resetting counter \n Insert 2 to increment \n Insert 3  to decrement \n Insert 4 to read \n Insert 5 to copy a value\n Insert 10 to exit \n \n \n \n " );
		scanf("%u",&cmd);
		switch(cmd){
			case 1:
				err=ioctl(fd,MYFILE_IOC_RESET,&read_val);
				if(err<0)
					perror("Error in  ioctl \n");
				break;
			case 2:
				err=ioctl(fd,MYFILE_IOC_INC,NULL);
				if(err<0)
					printf("Error %d \n",err);
				break;
			case 3:
				err=ioctl(fd,MYFILE_IOC_DEC,NULL);
				if(err<0)
					printf("Error %d \n",err);
				break;
			case 4:
				err=ioctl(fd,MYFILE_IOC_RD,&read_val);
				if(err<0){
					printf("Error %d \n",err);				
				}	
				printf("The value is %d \n",read_val);
				break;
			case 5:
				printf("Insert the number \n");
				scanf("%d",&read_val);
				printf ("Number inserted %d \n",read_val);
				err=ioctl(fd,MYFILE_IOC_CPY,&read_val);
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
