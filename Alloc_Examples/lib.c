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
static myfile* cb_ptr;
DEFINE_PER_CPU(unsigned long,counter);
/*Callback function of the timer ,don't use static*/
void timer_callback(unsigned long arg){
	/* 
		Allocating memory with kmalloc in a kernel timer requires a non-sleeping mode of the kmalloc
		(meaning that GFP_KERNEL can't be used because the fun can make the isr sleep)
		GFP_ATOMIC must be used in every atomic context (such as spinlocks)
	*/
	cb_ptr->ptr=kmalloc(cb_ptr->to_alloc,GFP_ATOMIC);
	if(!cb_ptr->ptr)
		printk(KERN_ALERT "Allocation failed \n");
	else
		printk(KERN_ALERT "Allocated %lu \n",ksize(cb_ptr->ptr));
}
/*Inits a new_file data structure */
int myfile_init(myfile* newfile,struct file_operations*fops,unsigned int baseminor,unsigned int count,char*name,unsigned long to_wait){
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
	cb_ptr=newfile;
	newfile->to_alloc=0;
	newfile->to_wait=to_wait;
	timer_setup(&newfile->my_timer,(void*)timer_callback,0);
	/* When accessing a mypcpu variable we must disable preemption, use these fun*/
	get_cpu_var(counter)=0;
	put_cpu_var(counter);
	return err;
}
int myfile_open(struct inode*inodep,struct file*filep){
	myfile* pointer=container_of(inodep->i_cdev,myfile,chdev);
	printk(KERN_ALERT "Opening file..\n");	
	if(pointer==0){
		printk(KERN_ALERT "Problem\n");
		return -EFAULT;
	}
	else
		printk(KERN_ALERT "Good pointer, file opened \n");
	/* Private data is never touched from the kernel, so it must be always initialized in the open function */
	filep->private_data=(void*)pointer;
	return 0;
}
int myfile_release(struct inode* inode, struct file* filep){
	filep->private_data=0;
	printk(KERN_ALERT "Releasing file... \n");
	return 0;
}
void myfile_remove( myfile*f,unsigned int number){
	cdev_del(&f->chdev);
	unregister_chrdev_region(f->dev,number);
	del_timer(&f->my_timer);
}
long myfile_ioctl(struct file*filep,unsigned int cmd,unsigned long arg){
	myfile*ptr=(myfile*)filep->private_data;
	int err=0;
	if(_IOC_TYPE(cmd)!=MYFILE_IOC_TYPE || _IOC_NR(cmd)>MYFILE_IOC_MAXNR)
		return -ENOTTY;
	if(_IOC_DIR(cmd)&&(!access_ok(arg,sizeof(unsigned long))))
		return -EFAULT;
	switch(cmd){
		case MYFILE_IOC_KTIMALLOC:
			__get_user(ptr->to_alloc,(size_t __user *)arg);
			err=mod_timer(&ptr->my_timer,jiffies+msecs_to_jiffies(ptr->to_wait));
			if(err)
				printk(KERN_ALERT "Can't set the timer \n");
			else
				printk(KERN_ALERT "Timer set \n");
			break;
		case MYFILE_IOC_KMEMCHECK:
			if(ptr->to_alloc!=0 && ptr->ptr){
				printk(KERN_ALERT "Memory requested %lu memory allocated %lu \n",ptr->to_alloc,ksize(ptr->ptr));
				__put_user(ksize(ptr->ptr),(unsigned long __user *)arg);
			}
			else{
				printk(KERN_ALERT "Memory allocation not yet requested \n");	
				__put_user(err,(unsigned long __user *)arg);
			}
			break;
		case MYFILE_IOC_KMEMFREE:
			kfree(ptr->ptr);
			ptr->ptr=0;
			printk(KERN_ALERT  "Memory freed \n");
			break;
		case MYFILE_IOC_PERCPUINC:
			/* When accessing a mypcpu variable we must disable preemption, use these fun*/
			get_cpu_var(counter)++;
			put_cpu_var(counter);
			break;
		case MYFILE_IOC_PERCPUDEC:
			/* When accessing a mypcpu variable we must disable preemption, use these fun*/
			get_cpu_var(counter)--;
			put_cpu_var(counter);
			break;
		case MYFILE_IOC_PERCPUPRINT:
			/* When accessing a mypcpu variable we must disable preemption, use these fun*/
			printk(KERN_ALERT "The value of the variable is %lu \n",get_cpu_var(counter));
			__put_user(get_cpu_var(counter),(unsigned long __user*)arg);
			break;
		default:
			printk(KERN_ALERT "Invalid cmd \n");
			return -ENOTTY;
	}
	return 0;
}
