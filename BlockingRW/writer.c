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
