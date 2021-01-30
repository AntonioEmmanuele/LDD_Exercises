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
#include "lib.h"
/*Inits a new_file data structure */
int my_file_init(my_file* newfile,struct file_operations*fops,unsigned int baseminor,unsigned int count,char*name){
	int err=0;
	if(newfile!=0)
		strcpy(newfile->name,name);
	else 
		return -EFAULT;
	err=alloc_chrdev_region(&newfile->dev,baseminor,count,newfile->name);
	if(IS_ERR(&err))
		return err;
	cdev_init(&newfile->chdev,fops);
	newfile->chdev.owner=THIS_MODULE;
	err=cdev_add(&newfile->chdev,newfile->dev,count);
	if(IS_ERR(&err))
		return err;
	newfile->written=0;
	newfile->size=0;
	atomic_set(&newfile->count,0);
	atomic_set(&newfile->ioctl_tst,0);
	set_bit(0,(volatile unsigned long *)&newfile->first);
	printk(KERN_ALERT "Instance inizialized \n");
	init_rwsem(&newfile->sem);
	return err;
}
int my_file_open(struct inode*inodep,struct file*filep){
	my_file* pointer=container_of(inodep->i_cdev,my_file,chdev);
	printk(KERN_ALERT "Opening file..\n");	
	if(pointer==0){
		printk(KERN_ALERT "Problem\n");
		return -EFAULT;
	}
	else
		printk(KERN_ALERT "Good pointer \n");
	if(test_and_clear_bit(0,(volatile unsigned long*)&pointer->first))
		printk(KERN_ALERT "FIRST ");
	/* Private data is never touched from the kernel, so it must be always initialized in the open function */
	filep->private_data=(void*)pointer;
	atomic_inc(&pointer->count);
	printk(KERN_ALERT "NEW INSTANCE OPENED , COUNT %d ",atomic_read(&pointer->count));
	return 0;
}
int my_file_release(struct inode* inode, struct file* filep){
	my_file* pointer=container_of(inode->i_cdev,my_file,chdev);
	/* it is the last*/
	atomic_dec(&pointer->count);
	if(atomic_read(&pointer->count)==0){
	/* Remove what was previously allocated */
		if(pointer->written!=0)
			kfree(pointer->written);
		filep-> private_data=0;
		set_bit(0,(unsigned long*)&pointer->first);
	}
	return 0;
}
ssize_t my_file_read(struct file*filep,char*buff,size_t len,loff_t * fpos){
	my_file* pointer=(my_file*)filep->private_data;
	size_t to_ret=0;
	if(buff==0)
		return -EFAULT;
	if(len==0)
		return -EINVAL;
	if(pointer->written==0)
		return -EFAULT;
	/* For the device implemented we don't know how big is written*/
	down_read(&pointer->sem);
	copy_to_user((void*)buff,(const void *)pointer->written,strlen(pointer->written)+1); 
	to_ret=strlen(buff)+1;
	up_read(&pointer->sem);
	return to_ret;
}
ssize_t my_file_write(struct file*filep,const char*buff,size_t len,loff_t*pos){
	size_t to_ret=0;
	my_file* pointer=(my_file*)filep->private_data;
	if(buff==0)
		return -EFAULT;
	if(len==0)
		return -EINVAL;
	downgrade_write(&pointer->sem);
	if(pointer->written==0){
		pointer->written=(char*)kmalloc(len,GFP_KERNEL);
		printk(KERN_ALERT "Allocated %zu \n",ksize(pointer->written));
	}
	else if(pointer->written!=0 && ksize(pointer->written)<len){
		kfree(pointer->written);
		pointer->written=(char*)kmalloc(len,GFP_KERNEL);	
		printk(KERN_ALERT"Allocated %zu \n",ksize(pointer->written));
	}
	/*For single int and char better get_user and put_user*/
	copy_from_user((void*)pointer->written,(const void*)buff,len);
	to_ret=strlen(pointer->written)+1;
	printk(KERN_ALERT "Value written %s , dim %zu \n: ",pointer->written,to_ret);
	up_write(&pointer->sem);
	return to_ret;
}
void my_file_remove( my_file*f,unsigned int number){
	cdev_del(&f->chdev);
	unregister_chrdev_region(f->dev,number);
}
long my_file_ioctl(struct file*filep,unsigned int cmd, unsigned long arg){
	int err=0;
	int value=0;
	my_file *ptr=(my_file*)filep->private_data;
	if(_IOC_TYPE(cmd)!=MYFILE_IOC_TYPE||_IOC_NR(cmd)>MYFILE_IOC_MAXNR){
		printk(KERN_ALERT "Invalid argument type \n");
		return -ENOTTY;
	}
	/*cmd : type-cmd-direction-size, it is a bitmask*/
	if(_IOC_DIR(cmd)){
		printk(KERN_ALERT "Reading or writing command received \n");
		err=!access_ok((void* __user)arg,_IOC_SIZE(cmd));
	}
	/* access_ok returns 0 if the address is definitely invalid*/
	if(err){
		printk(KERN_ALERT "Invalid argument \n");
		return -EFAULT;	
	}
	switch(cmd){
		case MYFILE_IOC_RESET:
			printk(KERN_ALERT "[IOCTL] Performing reset \n");
			atomic_set(&ptr->ioctl_tst,0);	
			break;
		case MYFILE_IOC_INC:
			printk(KERN_ALERT "[IOCTL] Incrementing \n");
			atomic_inc(&ptr->ioctl_tst);
			break;
		case MYFILE_IOC_DEC:
			printk(KERN_ALERT "[IOCTL] Decrementing \n");
			atomic_dec(&ptr->ioctl_tst);
			break;
		case MYFILE_IOC_CPY:
			printk(KERN_ALERT "[IOCTL] Writing data \n");
			/*use of unsafe __get and put,checked the address prev*/
			value=0;
			__get_user(value,(int __user*)arg);
			atomic_set(&ptr->ioctl_tst,value);
			printk(KERN_ALERT "[IOCTL ] Data writing performed,value %d  \n",value);
			break;
		case MYFILE_IOC_RD:
			printk(KERN_ALERT "[IOCTL] Reading data \n");
			value=atomic_read(&ptr->ioctl_tst);
			__put_user(value,(int __user*)arg);
			break;
		default:
			return -ENOTTY;
	}
	return 0;
}
