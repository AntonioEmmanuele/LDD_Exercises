#include "lib.h"
/*Inits a new_file data structure */
int myfile_init(myfile* newfile,struct file_operations*fops,unsigned int baseminor,unsigned int count,char*name){
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
	newfile->flg=0;
	init_waitqueue_head(&newfile->my_queue);
	if(IS_ERR(&err))
		return err;
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
}
/*
	If we are waiting then arg is a value of seconds!!.
*/
long myfile_ioctl(struct file*filep,unsigned int cmd,unsigned long arg){
	int err=0;
	unsigned long time=0;
	unsigned long time2=0;
	unsigned long to_wait=0;
	unsigned long seconds=0;
	myfile *ptr=(myfile*)filep->private_data;
	if(_IOC_TYPE(cmd)!=MYFILE_IOC_TYPE ||_IOC_NR(cmd)>MYFILE_IOC_MAXNR){
		printk(KERN_ALERT "Invalid cmd \n");
		return -ENOTTY;
	}
	/* Testing if the cmd has the same number of my device and if it is a valid cmd(not greater than MYFILE_IOC_MAXNR*/
	if(_IOC_DIR(cmd))
		err=!access_ok((void* __user)arg,sizeof(unsigned long));
	if(err)
		return -EFAULT;
	switch(cmd){
		
		case MYFILE_IOC_PRINTCURRTIME:
			time=jiffies;
			printk(KERN_ALERT "The current time is %lu \n", time);
			break;
		case MYFILE_IOC_TIMETESTS:
			time=jiffies;
			printk(KERN_ALERT "The current time is %lu, going to wait 10 ms \n",time);
			/* could use sleepm to yield the processor, mdleay won't be precisely 10 ms*/
			mdelay(10);
			time2=jiffies;
			/* 
				time is rappresented in jiffies, the actual value of systick counter. It is incremented using a 			   	periodic interrupt, unless the system works in tickless mode.
			   	Hz is the triggering frequency of this cyclic isr. 
			   	So a times interval is given in seconds by : (Final Time- Start Time)/HZ 
			*/
			printk(KERN_ALERT "Time passed in jiffies %lu and in ms %lu \n",time2-time,(time2-time)*1000/HZ);
			break;
		/* 
			this performs a busy waiting using the argument
		   	Busy Waiting means that the process holds the cpu while waiting the curr time (unless it is not preempted).
		   	In systems with preemption it is not a very big deal but if the system has some hard-cpu tasks then busy 				waiting is actually a bomb, even with preemption.
		   	However if we are creating a real-time driver where determinism is very important then BW could be a 				valid option.
		   	time is the time to wait.
		 */
		case MYFILE_IOC_BUSYWAITING:
			time=jiffies;
			__get_user(seconds,(unsigned long __user*)arg);
			to_wait=time+(seconds)*HZ;
			printk(KERN_ALERT "Seconds to wait %lu and time in jiff to_wait %lu \n",seconds,to_wait);
			/*use time_before_eq64 for 64 bit values*/
			while(time_before_eq(jiffies,to_wait))
				/* basically do nothing*/
				cpu_relax();
			time2=jiffies;
			printk(KERN_ALERT "Waited %lu seconds \n",(time2-time)/HZ);
			break;
		/* Yields the proc until the time passed*/
		case MYFILE_IOC_CPUYIELD:
			time=jiffies;
			__get_user(seconds,(unsigned long __user*)arg);
			to_wait=time+(seconds)*HZ;
			printk(KERN_ALERT "Seconds to wait %lu and time in jiff to_wait %lu \n",seconds,to_wait);
			/*use time_before_eq64 for 64 bit values*/
			while(time_before_eq(jiffies,to_wait))
				/* basically do nothing*/
				schedule();
			time2=jiffies;
			printk(KERN_ALERT "Waited %lu seconds \n",(time2-time)/HZ);
			break;
		case MYFILE_IOC_WAITWITHWQ:
			time=jiffies;
			__get_user(seconds,(unsigned long __user*)arg);
			/* 
			this function puts the process to sleep in a wq, the proc could be awakened by flg becoming true(in this
			case never ) or when the timeout expires.
			If the timeout expires then the return value will be 0 , else the remaing time in jiffies.
			wait_exent_timeout is a no-interruptible form
			*/
			err=wait_event_interruptible_timeout(ptr->my_queue,ptr->flg,seconds*HZ);
			if(!err){
				time2=jiffies;
				printk(KERN_ALERT "Timeout expired \n");
				printk(KERN_ALERT "Seconds passed %lu \n",(time2-time)/HZ);
			}
			/*
				BEWARE THAT ERR IS INT (NOT U LONG)!
				else
					printk(KERN_ALERT "Remaining jiffies %lu, err);
			*/
			break;
		default:
			printk(KERN_ALERT "Invalid cmd \n");
			return -ENOTTY;
	}
	return 0;
}
