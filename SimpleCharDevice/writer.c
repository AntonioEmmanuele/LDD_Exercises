#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "assert.h"
#include "string.h"
#include "unistd.h"
int main(void){

	int file_descr=open("/dev/My_file0", O_RDWR);
	if(file_descr<0){
		perror("Error opening file \n");
		exit(1);
	}	
	int flg=1;
	char msg[256];
	ssize_t written;
	char msg2[256];
	while(flg){
		printf("Insert a string \n");
		scanf("%s",msg);
		printf("You have inserted %s \n ",msg);
		written=write(file_descr,msg,strlen(msg)+1);
		if(written==strlen(msg)+1)
			printf("Written correctly \n");
		else
			printf("Problem writing %zd \n",written);
		written=read(file_descr,msg2,256);
		printf("You are reading %s ,with dimension of %zd \n ",msg2,written);
		if(strcmp(msg,"exit")==0)
			flg=0;
	}
	close(file_descr);
	return 0;
}
