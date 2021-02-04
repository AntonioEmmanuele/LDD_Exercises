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
#include "lib.h"
/*Inits a new_file data structure */
int myfile_init(myfile* newfile,unsigned int baseminor,unsigned int count,char*name,struct file_operations*fops){
	int err=0;
	unsigned int i=0;
	if(name==0||count==0)
		return -EINVAL;
	err=alloc_chrdev_region(&newfile->dev,baseminor,count,name);
	if(err<0)
		return err;
	cdev_init(&newfile->cdev,fops);
	newfile->cdev.owner=THIS_MODULE;
	err=cdev_add(&newfile->cdev,newfile->dev,count);
	if(err<0)
		return err;
	atomic_set(&newfile->num_elements,0);
	for(i=0;i<BUFF_SIZE;i++)
		newfile->buff[i]=0;
	spin_lock_init(&newfile->lock);
	init_waitqueue_head(&newfile->myq_wr);
	init_waitqueue_head(&newfile->myq_rd);
	newfile->flg_wr=0;
	set_bit(0,(void*)&newfile->flg_rd);
	return err;
}
void myfile_destroy(myfile*f,unsigned int number){
	cdev_del(&f->cdev);
	unregister_chrdev_region(f->dev,number);
}

int myfile_open(struct inode*inodep,struct file*filep){
	myfile* pointer=container_of(inodep->i_cdev,myfile,cdev);
	printk(KERN_ALERT "Opening file..\n");	
	if(pointer==0){
		printk(KERN_ALERT "Problem\n");
		return -EFAULT;
	}
	else
		printk(KERN_ALERT "Good pointer \n");
	/* Private data is never touched from the kernel, so it must be always initialized in the open function */
	filep->private_data=(void*)pointer;
	printk(KERN_ALERT "NEW INSTANCE OPENED ");
	return 0;
}
int myfile_release(struct inode* inode, struct file* filep){
	filep->private_data=0;
	return 0;
}
ssize_t myfile_read(struct file*filep,char*buff,size_t len,loff_t * fpos){
	unsigned long flg_irq=0;
	int value=0;
	myfile* pointer=(myfile*)filep->private_data;
	if(!access_ok(buff,sizeof(int)))
		return -EFAULT;
	wait_event_interruptible(pointer->myq_rd,!test_bit(0,(void*)&pointer->flg_rd));
	spin_lock_irqsave(&pointer->lock,flg_irq);
	*buff=pointer->buff[atomic_read(&pointer->num_elements)-1];
	spin_unlock_irqrestore(&pointer->lock,flg_irq);
	printk(KERN_ALERT "Reading %d \n",(int)*buff);
	value=atomic_dec_return(&pointer->num_elements);
	if(value==(BUFF_SIZE-1)){
		clear_bit(0,(void*)&pointer->flg_wr);
		/*wakes up the readers*/
		wake_up_interruptible(&pointer->myq_wr);	
	}
	else if(value==0)
		set_bit(0,(void*)&pointer->flg_rd);
	printk(KERN_ALERT "exiting reading \n");
	return len;
}
ssize_t myfile_write(struct file*filep,const char*buff,size_t len,loff_t*f_pos){
	unsigned long flg_irq=0;
	int value=0;
	myfile* pointer=(myfile*)filep->private_data;
	if(!access_ok(buff,sizeof(int)))
		return -EINVAL;
	/* The process can be interrupted using signals*/
	wait_event_interruptible(pointer->myq_wr,!test_bit(0,(void*)&pointer->flg_wr));
	/* out of spinlock, it can sleep*/
	__get_user(value,buff);
	spin_lock_irqsave(&pointer->lock,flg_irq);
	pointer->buff[atomic_read(&pointer->num_elements)]=value;
	spin_unlock_irqrestore(&pointer->lock,flg_irq);
	printk(KERN_ALERT "Written %d \n",pointer->buff[atomic_read(&pointer->num_elements)]);
	value=atomic_inc_return(&pointer->num_elements);
	if(value==1){
		clear_bit(0,(void*)&pointer->flg_rd);
		/*wakes up the readers*/
		wake_up_interruptible(&pointer->myq_rd);	
	}
	else if(value==BUFF_SIZE)
		set_bit(0,(void*)&pointer->flg_wr);
	printk(KERN_ALERT "Exiting writing... \n");
	return sizeof(int);
}
