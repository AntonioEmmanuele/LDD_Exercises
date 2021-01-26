#include"lib.h"
#define MINOR_NUM 0
#define DEV_NO 1
static my_file mf;
static struct file_operations my_fops={
	.owner=THIS_MODULE,
	.read=my_file_read,
	.write=my_file_write,
	.open=my_file_open,
	.release=my_file_release,
};
static int my_mod_init(void){
	int err=0;
	printk(KERN_ALERT "Here folks \n");
	err=my_file_init(&mf,&my_fops,MINOR_NUM,DEV_NO,"My_file");
	if(!IS_ERR(&err))
		printk(KERN_ALERT "MOD INITIALIZED \n");
	else 
		printk(KERN_ALERT "Problem inserting device \n");
		
	return 0;
}
static void my_mod_exit(void){
	my_file_remove(&mf,COUNT);
	printk("Bye bye \n");
}
module_init(my_mod_init);
module_exit(my_mod_exit);
MODULE_LICENSE("GPL");
