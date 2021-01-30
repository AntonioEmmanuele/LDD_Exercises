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
#define MINOR_NUM 0
#define DEV_NO 1
static myfile mf;
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.read=myfile_read,
	.write=myfile_write,
	.open=myfile_open,
	.release=myfile_release,
};
static int my_mod_init(void){
	int err=0;
	printk(KERN_ALERT "Here folks \n");
	err=myfile_init(&mf,MINOR_NUM,DEV_NO,"My_file",&my_fops);
	if(!IS_ERR(&err))
		printk(KERN_ALERT "MOD INITIALIZED \n");
	else 
		printk(KERN_ALERT "Problem inserting device \n");
	return 0;
}
static void my_mod_exit(void){
	myfile_destroy(&mf,DEV_NO);
	printk("Bye bye \n");
}
module_init(my_mod_init);
module_exit(my_mod_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Antonio Emmanuele");
