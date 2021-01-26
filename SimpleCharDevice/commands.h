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
