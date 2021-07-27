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

#ifndef EMTRDATASTRUCT_H
#define EMTRDATASTRUCT_H
/******************************************************************************
* Copyright (c) 2016 INVT Power System(ShenZhen) Co., Ltd, All Rights Reserved
*
* Original Author:	Hongfei Wang
* Last rev by:	lcl
* Last rev date:	2021.3.10
* Last change list: 
*
* Overview:         
*
*Description:      
*
*  
*
*NOTE: 
*                       
*                        
*******************************************************************************/
//-----------------------------------------------------------------------------
// Adding Include Stuff

#include "GlobalParameter.h"
#include "SysCfgMng.h"


//-----------------------------------------------------------------------------
// Adding Conditional Definitions Stuff

#ifdef EMTRMAIN_C
	#define EMTRDATA_PUBLIC
	#define EMTRDATA_CONST
#else
	#define EMTRDATA_PUBLIC	extern
	#define EMTRDATA_CONST	const
#endif


//-----------------------------------------------------------------------------
// Adding Constants and Macro Definitions

#define CURR_ALM_BUFF_NUM	50

#define DRY_IP_PORT_NUM		(eDryIpNum*DC_MAX_NUM)

//-----------------------------------------------------------------------------
//Adding Private Enumerated and Structure Definitions

enum eMtrAnalogEnum
{
	eMtrAnaPue,
	eMtrAnaColdTmpAvg,
	eMtrAnaColdTmpMin,
	eMtrAnaColdTmpMax,
	eMtrAnaHotTmpAvg,
	eMtrAnaHotTmpMin,
	eMtrAnaHotTmpMax,
	eMtrAnaAmbTmpAvg,
	eMtrAnaAmbTmpMin,
	eMtrAnaAmbTmpMax,
	eMtrAnaAcTmpAvg,
	eMtrAnaAcTmpMin,
	eMtrAnaAcTmpMax,
	eMtrAnaColdHumAvg,
	eMtrAnaHotHumAvg,
	eMtrAnaAmbHumAvg,

	eMtrAnaSysTmpAvg,	//wanghf20180727
	eMtrAnaSysHumAvg,
	eMtrAnaRsvd3,
	eMtrAnaRsvd4,
	eMtrAnaRsvd5,
	eMtrAnaRsvd6,
	eMtrAnaRsvd7,
	eMtrAnaRsvd8,
	eMtrAnaRsvd9,
	eMtrAnaRsvd10,
	eMtrAnaRsvd11,
	eMtrAnaRsvd12,
	eMtrAnaRsvd13,
	eMtrAnaRsvd14,

	eMtrAnaNum
};

enum eMtrStsEnum
{
	eMtrStsFrontDoor,		// 0: close; 1: open; 2: inactive, not detect
	eMtrStsBackDoor,		// 0: close; 1: open; 2: inactive, not detect
	eMtrStsFrontFan,		// 0: off; 1: on
	eMtrStsBackFan,			// 0: off; 1: on
	eMtrStsSysRunSts,		// 0: normal; 1: alarm
	eMtrStsFrontOvTmp,		// 0: normal; 1: alarm
	eMtrStsFrontLoTmp,		// 0: normal; 1: alarm
	eMtrStsFrontOvHumi,		// 0: normal; 1: alarm
	eMtrStsFrontLoHumi,		// 0: normal; 1: alarm
	eMtrStsRearOvTmp,		// 0: normal; 1: alarm
	eMtrStsRearLoTmp,		// 0: normal; 1: alarm
	eMtrStsRearOvHumi,		// 0: normal; 1: alarm
	eMtrStsRearLoHumi,		// 0: normal; 1: alarm
	eMtrStsSmokeAlarm,		// 0: normal; 1: alarm; 2: not detect, only eMtr drycard sts
	eMtrStsIrAlarm,			// 0: normal; 1: alarm; 2: not detect, only eMtr drycard sts
	eMtrStsIrTamper,		// 0: normal; 1: alarm; 2: not detect, only eMtr drycard sts
	eMtrStsLeakAlarm,		// 0: normal; 1: alarm; 2: not detect, only eMtr drycard sts
	eMtrStsSdCardExit,		// 0: normal; 1: alarm
	eMtrStsSdCapLess,		// 0: normal; 1: alarm
	eMtrStsMemCapLess,		// 0: normal; 1: alarm
	eMtrStsDataSaveFail,	// 0: normal; 1: alarm	
	eMtrStsFireAlm,			// 0: normal; 1: alarm; 2: not detect, only eMtr drycard sts
	eMtrStsProjDoor,		// 0: normal; 1: alarm	
	eMtrStsUtilityAbnormal,	// 0: normal; 1: alarm	
	eMtrStsFilterBlock,		// 0: normal; 1: alarm	
	eMtrStsDoorSensor,		// 0: normal; 1: alarm; 2: not detect, only eMtr drycard sts, wanghf20210218
	eMtrStsDoorOpenTout,	// 0: normal; 1: alarm
	eMtrStsRsvd9,
	eMtrStsRsvd10,

	eMtrStsNum
};

enum eMtrVerEnum
{
	//wanghf20161229
	eMtrVerEMtrSerNo,
	eMtrVerEMtrHw,

	eMtrVerOsFw,	//ARM OS Version, 0~999, @ /sys/version.ini

	//proc version
	eMtrVerMainHigh,
	eMtrVerMainLow,

	eMtrVerLcdHigh,//qt°æ
	eMtrVerLcdLow,//qt°æ

	eMtrVerUartExtHigh,
	eMtrVerUartExtLow,

	eMtrVerWebHigh,
	eMtrVerWebLow,
	
	eMtrVerCabHigh,
	eMtrVerCabLow,
	
	eMtrVerSmsHigh,
	eMtrVerSmsLow,

	eMtrVerMqttHigh,
	eMtrVerMqttLow,

	eMtrVerEMtrSwHigh,	//wanghf20170206, eMTR software version
	eMtrVerEMtrSwLow,

	eMtrVerNum
};


struct eMtrThsSortStruct
{
	quint16 coldOnlineThsNum;
	quint16 coldThsIdxSet[THS_MAX_NUM];
	
	quint16 hotOnlineThsNum;
	quint16 hotThsIdxSet[THS_MAX_NUM];
	
	quint16 ambOnlineThsNum;
	quint16 ambThsIdxSet[THS_MAX_NUM];
};

struct eMtrCurrAlmIdStruct
{
	quint16 devType;
	quint16 devNo;	//wanghf20170228
	quint16 addr;
	quint16 logId;
	quint16 uartNo;
};

struct eMtrDataAndTimeStruct
{
	quint16 lsw;
	quint16 msw;
};

struct eMtrAlmFlagStruct
{
	quint16 newMail;// should clear to "0" when mail program judge end
	quint16 newSms;	// should clear to "0" when uart program judge end

	quint16 rsvd;
};

struct eMtrAlmItemStruct
{
	eMtrCurrAlmIdStruct almId;
	eMtrDataAndTimeStruct dAndT;
	eMtrAlmFlagStruct almFlag;
	quint16 level;
	quint16 rsvd1;		//@todo..., almCfmed
	quint16 rsvd2;		//@todo..., hislogId in tb_hislog
};

struct eMtrCurrAlmStruct
{
	quint16 num;		//total alm number
	eMtrAlmItemStruct item[CURR_ALM_BUFF_NUM];
};

struct eMtrCtrlStruct
{
	quint16 needRebootSys; // Notice: set "1" will reboot eMTR system
	quint16 muteOnOffSts;// 0: silence, 1: alarm
	quint16 recover;	// 1: drop database, delete ini, Notice: only INVT admin can operate
	quint16 hislogClr;	// 1: clear
	quint16 hisdataClr;	// 1: clear
	quint16 timeChged;	// 1: changed, only for DGUS LCD sync time
	quint16 fwUploadEnd;	// 1: firmware upload ok
	quint16 shutdownSys; // Notice: set "1" will shutdown eMTR system
	quint16 memFull;	//1: memory full, forbidden to write
	quint16 sdFull;	//1: SD card full, forbidden to write
	quint16 actDataSaveLoc;	//Database save location actually, 0: internal flash, 1: SD card
	quint16 dbTableRst;	// 1: delete table if it is fail, will reboot eMTR system(proc)
	quint16 uartExtRst;	// web set when device changed, uartProc clear
	quint16 cabRst;  // web set when device changed, uartProc clear
	quint16 smsRst;  // web set when device changed, uartProc clear
	quint16 mqttRst;
	quint16 sdFormatting;  // 0: No, 1: SD card is formatting
	quint16 customInfoUploadEnd;	// 1: customInfo upload ok

	quint16 rsvd[4];
};

//wanghf20200620
enum DryDevEnum
{
	eDryDevSmoke,
	eDryDevIr,
	eDryDevDoorSensor,

	eDryDevNum,
};

struct eMtrDryDevStruct
{
	uint16 devNum[eDryDevNum];
	uint16 devIdx[eDryDevNum][DRY_IP_PORT_NUM];
	uint16 alm[eDryDevNum][DRY_IP_PORT_NUM];	// 0: normal, 1: alarm
};


struct eMtrDataStruct
{
	qint16 ana[eMtrAnaNum];
	qint16 sts[eMtrStsNum];
	qint16 ver[eMtrVerNum];

	eMtrThsSortStruct thsSort;
	eMtrCurrAlmStruct currAlm;
	eMtrCtrlStruct ctrl; 
	eMtrDryDevStruct dryDev;	//wanghf20200620	
};

//-----------------------------------------------------------------------------
//Adding Public Enumerated and Structure Definitions Stuff


//-----------------------------------------------------------------------------
//Adding Private Function Prototypes (i.e. static)


//-----------------------------------------------------------------------------
//Adding Public Function Prototypes


//-----------------------------------------------------------------------------
//Adding Private Variables (i.e. static)



//-----------------------------------------------------------------------------
//Adding Public Variables Stuff


//-----------------------------------------------------------------------------
//Adding Private Function 


//-----------------------------------------------------------------------------
//Adding Public Function 




#endif // EMTRDATASTRUCT_H

