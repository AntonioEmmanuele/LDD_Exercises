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
static size_t  mem =0;
static unsigned long counter=0;
int main(void){
	fd=open("/dev/My_file0",O_RDWR);
	if(fd<0){
		perror("Error opening the device \n");
		exit(1);
	}
	int err=0;
	while(ctl){
		printf(" \n \n \n Insert 1 for allocate mem using ktim \n Insert 2 to do a mem test  \n Insert 3  to free mem \n Insert 4 to incremenent per-cpu counter \n Insert 5 to decrement pcpu counter \n Insert 6 to print PCPU counter \n Insert 10 to exit \n \n \n \n " );
		scanf("%u",&cmd);
		switch(cmd){
			case 1:
				printf("Insert the number of bytes to allocate \n");
				scanf("%lu",&mem);
				printf("Inserted %lu \n",mem);
				err=ioctl(fd,MYFILE_IOC_KTIMALLOC,&mem);
				if(err<0)
					perror("Error in  ioctl \n");
				break;
			case 2:
				err=ioctl(fd,MYFILE_IOC_KMEMCHECK,&mem);
				if(err<0)
					printf("Error %d \n",err);
				else
					printf("Memory allocated %lu \n",mem);
				break;
			case 3:
				err=ioctl(fd,MYFILE_IOC_KMEMFREE,NULL);
				if(err<0)
					printf("Error %d \n",err);
				break;
			case 4:
				err=ioctl(fd,MYFILE_IOC_PERCPUINC,NULL);
				if(err<0)
					printf("Error %d \n",err);
				break;
			case 5:
				err=ioctl(fd,MYFILE_IOC_PERCPUDEC,NULL);
				if(err<0)
					printf("Error %d \n",err);
				break;
			case 6:
				err=ioctl(fd,MYFILE_IOC_PERCPUPRINT,&counter);
				if(err<0)
					printf("Error %d \n",err);
				else
					printf("The value of the counter is %lu \n",counter);
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
