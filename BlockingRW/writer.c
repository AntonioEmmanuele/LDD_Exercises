#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "assert.h"

#include "unistd.h"
int main(void){

	int file_descr=open("/dev/My_file0", O_RDWR);
	if(file_descr<0){
		perror("Error opening file \n");
		exit(1);
	}	
	int flg=1;
	int msg;
	ssize_t written;
	char msg2[256];
	while(flg){
		printf("Insert a number from 0 to 254 , insert 255 to exit \n");
		scanf("%d",&msg);
		printf("You have inserted %d \n ",msg);
		written=write(file_descr,&msg,sizeof(int));
		if(written<0)
			perror("Problem writing \n");
		else
			printf(" Data written correctly  \n");
		if(msg==255)
			flg=0;
	}
	close(file_descr);
	return 0;
}
