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
* along with The LDD_Exercises Project. If not, see <https://www.gnu.org/licenses/>. *
* *
* In case of use of this project, I ask you to mention me, to whom it may concern. *
*****************************************************************************************/
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
	int msg;
	ssize_t dim;
	while(flg){
		dim=read(file_descr,(char*)&msg,sizeof(int));
		if(dim<0)
			perror("Error \n");
		else
			printf("Reading %d \n ",msg);
		if(msg==255)
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
