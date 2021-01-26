#ifndef COMMANDS_H
#define COMMANDS_H
#include "linux/ioctl.h"
/* cmd=type-nr-dir-size*/
/* IOCTL defines */
/*An IOCTL command is composed by type-number-direction-size*/
/*8 bit type*/
#define MYFILE_IOC_TYPE 0x82
/*For selecting commands we should use macros defined in linux/ioctl.h*/

/* resets ioctl_tst to 0,_IO stands for an ioctl command that doesn't need read and write operations . First Arg: Type, Sec Arg: command num*/
#define MYFILE_IOC_RESET _IO(MYFILE_IOC_TYPE,0)
/* Increments the value to 1*/
#define MYFILE_IOC_INC _IO(MYFILE_IOC_TYPE,1)
/* decrements the value of 1*/
#define MYFILE_IOC_DEC _IO(MYFILE_IOC_TYPE,2)
/*IOW, io write to the device*/
#define MYFILE_IOC_CPY _IOW(MYFILE_IOC_TYPE,3,int)
/*IOR , ioctl read from the device */
#define MYFILE_IOC_RD _IOR(MYFILE_IOC_TYPE,4,int)
/* max number , used to test if we are receiving a valid command */
#define MYFILE_IOC_MAXNR 4
#endif
