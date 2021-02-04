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
#include "linux/semaphore.h"
#include "asm/uaccess.h"
#include "linux/slab.h"
#include "linux/cdev.h"
#include "linux/string.h"
#include "commands.h"
typedef struct {
	/*Number of tasks in the resource*/
	atomic_t count;
	/* if it is the first*/
	unsigned long first;
	/*semaphore*/
	struct rw_semaphore sem;
	/*written string from the user */
	char* written;
	/*size of the last written string*/
	size_t size;
	/*chdev*/
	struct cdev chdev;
	/*device*/
	dev_t dev;
	/*name*/
	char name[256];
	/*Number used to test ioctl*/
	atomic_t ioctl_tst;
}my_file;
/*Inits a my_file object*/
int my_file_init(my_file*,struct file_operations*,unsigned int,unsigned int,char*);
int my_file_open(struct inode*,struct file*);
int my_file_release(struct inode*,struct file*);
ssize_t my_file_write(struct file*, const char*,size_t,loff_t *);
ssize_t my_file_read(struct file*,  char*,size_t,loff_t *);
void my_file_remove( my_file*,unsigned int);
long my_file_ioctl(struct file*filep,unsigned int cmd, unsigned long arg);
MODULE_LICENSE("GPL");
#endif
