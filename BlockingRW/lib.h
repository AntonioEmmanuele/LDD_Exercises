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
#ifndef LIB_H
#define LIB_H
#include "linux/module.h"
#include "linux/init.h"
#include "linux/fs.h"
#include "linux/spinlock.h"
#include "asm/uaccess.h"
#include "linux/cdev.h"
#include "linux/wait.h"
#define BUFF_SIZE 10
typedef struct{
	/* Char device*/
	struct cdev cdev;
	/*device numb*/
	dev_t dev;
	/*Buffer*/
	int buff[BUFF_SIZE];
	/*number of elements in the buffer*/
	atomic_t num_elements;
	/*Spinlock to access the buffer, never sleep with a spinlock obtained*/
	struct spinlock lock;
	/* blocking io,writers queue */
	wait_queue_head_t myq_wr;
	/*readers*/
	wait_queue_head_t myq_rd;
	/*flg writers*/
	unsigned int flg_wr;
	/*flg readers*/
	unsigned int flg_rd;
}myfile;
int myfile_init(myfile*,unsigned int ,unsigned int ,char*,struct file_operations*);
void myfile_destroy(myfile*,unsigned int);
ssize_t myfile_read(struct file*,char*,size_t ,loff_t* );
ssize_t myfile_write(struct file*,const char*,size_t ,loff_t*);
int myfile_open(struct inode*,struct file*);
int myfile_release(struct inode*,struct file*);
#endif
