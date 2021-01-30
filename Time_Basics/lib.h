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
