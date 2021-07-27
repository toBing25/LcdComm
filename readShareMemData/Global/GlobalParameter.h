/****************************************************************************
**
** Copyright (C) 2021 The INVT Company . 
**
** This file is part of LcdComm.
** LcdComm is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** LcdComm is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with LcdComm.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef GLOBALPARAMETER_H
#define GLOBALPARAMETER_H

#include <stdio.h>

//-----------------------------share memery key-------------------------------------------------------
#define UPS_SHARE		0x00001201

#define AC_SHARE		0x00001301

#define THS_SHARE		0x00001401
#define PDU_SHARE		0x00001402
#define LEAK_SHARE		0x00001403
#define SMS_SHARE		0x00001404
#define ITPDC_SHARE		0x00001405
#define METER_SHARE		0x00001406
#define SPDS_SHARE		0x00001407
#define DC_SHARE		0x00001408
#define BMS_SHARE		0x00001409
#define ATS_SHARE		0x0000140A
#define FAN_SHARE		0x0000140B
#define FANCTL_SHARE    0x0000140C
#define CTRLBOX_SHARE   0x0000140D //2021/1/13

#define EMTR_SHARE		0x00001701
#define SYS_CFG_SHARE	0x00001702

#define WEB_CFG_SHARE	0x00000101
#define SEND_SHM_KEY	0x00000102
#define SMSBUFF_SHM_KEY	0x00000103
#define LCD_CFG_SHARE   0x00000104

#define PRINT_INFO		1
#define PRINT_DATA		1
#define PRINT_ALM		1

#if (PRINT_INFO == 1)
#define PrintInfo(args...) printf("Uart log: " args)
#else
#define PrintInfo(args...)
#endif

#if (PRINT_DATA == 1)
#define PrintData(args...) printf("Uart data: " args)
#else
#define PrintData(args...)
#endif

#if (PRINT_ALM == 1)
#define PrintAlm(args...) printf("Uart err: " args)
#else
#define PrintAlm(args...)
#endif

#define DISCONNECT_NSPD_TIMES   5
#define DISCONNECT_MIN_TIME     5 //s
//#define PC_DEBUG 1
//#define DEBUG 1

#define MIN(A,B) (((A) < (B)) ? (A):(B))

//------------------------------data type define----------------------------------------------------------------------
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef  short int int16;
typedef  long int int32;


#endif // GLOBALPARAMETER_H

