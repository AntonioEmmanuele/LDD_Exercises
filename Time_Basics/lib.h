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
#ifndef LIB_H
#define LIB_H
#include "linux/module.h"
#include "linux/init.h"
#include "linux/fs.h"
#include "asm/uaccess.h"
#include "linux/cdev.h"
#include "linux/string.h"
#include "commands.h"
#include "linux/time.h"
#include "linux/delay.h"
#include "linux/wait.h"
typedef struct {
	/*chdev*/
	struct cdev chdev;
	/*device*/
	dev_t dev;
	/*name*/
	char name[256];
	/*wait queue*/
	wait_queue_head_t my_queue;
	/*flag for waiting*/
	unsigned int flg;
}myfile;
/*Inits a my_file object*/
int myfile_init(myfile*,struct file_operations*,unsigned int,unsigned int,char*);
int myfile_open(struct inode*,struct file*);
int myfile_release(struct inode*,struct file*);
void myfile_remove( myfile*,unsigned int);
long myfile_ioctl(struct file*filep,unsigned int cmd, unsigned long arg);
MODULE_LICENSE("GPL");
#endif
