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
/* IOCTL defines */
/*An IOCTL command is composed by type-number-direction-size*/
/*8 bit type*/
#define MYFILE_IOC_TYPE 0x82
/*For selecting commands we should use macros defined in linux/ioctl.h*/

/*Arms a Kernel timer that after one second will allocate memory.*/
#define MYFILE_IOC_KTIMALLOC _IOW(MYFILE_IOC_TYPE,0,int)
/*Free the memory previously allocated */
#define MYFILE_IOC_KMEMFREE   _IO(MYFILE_IOC_TYPE,1)
/*Checks if we have allocated memory*/
#define MYFILE_IOC_KMEMCHECK  _IOR(MYFILE_IOC_TYPE,2,int)
/*Allocate memory with kmem_cache_t*/
#define MYFILE_IOC_CACHEALLOC _IO(MYFILE_IOC_TYPE,3)
/*Free memory preeviously allocated with kmem_cache_t*/
#define MYFILE_IOC_CACHEFREE _IO(MYFILE_IOC_TYPE,4)

#define MYFILE_IOC_MAXNR 4
#endif
