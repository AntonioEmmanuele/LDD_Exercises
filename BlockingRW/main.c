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
