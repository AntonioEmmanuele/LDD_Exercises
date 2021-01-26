#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "assert.h"
#include "string.h"
#include "unistd.h"
#include "pthread.h"
static int file_descr;
#define NUM_THD 1
static pthread_t thd[NUM_THD];
static void * thd_reader(void*c){
	unsigned int flg=1;
	char msg[256];
	ssize_t dim;
	while(flg){
		dim=read(file_descr,msg,256);
		printf("Reading %zd %s \n ",dim,msg);
		if(strcmp(msg,"exit")==0)
			flg=0;
		sleep(1);
	}
	pthread_exit(NULL);
}
int main(void){
	file_descr=open("/dev/My_file0",O_RDWR);
	if(file_descr<0){
		perror("Error opening \n");
		exit(1);
	}
	for(unsigned int i=0;i<NUM_THD;i++){
		pthread_create(&thd[i],NULL,thd_reader,NULL);
	}
	for(unsigned int i=0;i<NUM_THD;i++){
		pthread_join(thd[i],NULL);
	}
	pthread_exit(NULL);
}
