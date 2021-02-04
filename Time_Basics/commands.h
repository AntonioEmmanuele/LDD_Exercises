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
