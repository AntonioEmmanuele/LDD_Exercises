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
