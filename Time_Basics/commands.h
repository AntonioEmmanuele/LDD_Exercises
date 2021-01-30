#ifndef COMMANDS_H
#define COMMANDS_H
#include "linux/ioctl.h"
/* IOCTL defines */
/*An IOCTL command is composed by type-number-direction-size*/
/*8 bit type*/
#define MYFILE_IOC_TYPE 0x82
/*For selecting commands we should use macros defined in linux/ioctl.h*/

/*Prints using printk the current time in jiffies.*/
#define MYFILE_IOC_PRINTCURRTIME _IO(MYFILE_IOC_TYPE,0)
/* Test of  time_before and time_after*/
#define MYFILE_IOC_TIMETESTS _IO(MYFILE_IOC_TYPE,1)
/* Performs a busy waiting of the specified value.*/
#define MYFILE_IOC_BUSYWAITING _IOR(MYFILE_IOC_TYPE,2,int)
/*Waits the specified time yielding the processor*/
#define MYFILE_IOC_CPUYIELD _IOR(MYFILE_IOC_TYPE,3,int)
/*waits the specified time with a wait queue*/
#define MYFILE_IOC_WAITWITHWQ _IOR(MYFILE_IOC_TYPE,4,int)
#define MYFILE_IOC_MAXNR 4
#endif
