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
