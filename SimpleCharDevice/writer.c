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
