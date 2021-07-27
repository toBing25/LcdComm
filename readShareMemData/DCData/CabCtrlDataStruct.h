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

#ifndef JUN_DATA_H_
#define JUN_DATA_H_

//#include "../DevInterface.h"
#include "Global/GlobalPublic.h"


#define ID_MAX_NUM 20 //20 cards number reserved
#define DOOR_NUM 2 //Total number of doors per cabinet

enum OutputCabCtrlEnum
{
    eCabCtrlOp1,
    eCabCtrlOp2,
    eCabCtrlOp3,
    eCabCtrlOp4,
    eCabCtrlOp5,
    eCabCtrlOp6,
    eCabCtrlOp7,
    eCabCtrlOp8,
    eCabCtrlOp9,

    eCabCtrlOpNum
};

enum InputCabCtrlEnum
{
    eCabCtrlIp1,
    eCabCtrlIp2,
    eCabCtrlIp3,
    eCabCtrlIp4,
    eCabCtrlIp5,
    eCabCtrlIp6,
    eCabCtrlIp7,

    eCabCtrlIpNum
};

/*--------------------------------------------Struct------------------------------------------------*/

struct CabCtrlRegInfBitStruct
{
    uint16 sts:1; //Bit0; Dry contact sts, 0: open; 1: close;
    uint16 changed:1; //Bit1; Sts if changed, 0: sts not change ; 1: sts changed;
    uint16 reserved:14; //Bit2-Bit5;reserved;
};

union CabCtrlRegInfUnion
{
    uint16 all;
    CabCtrlRegInfBitStruct bits;
};

struct CabCtrlChangeFlagBitStruct
{
    uint16 ipCabCtrlChanged:1; //Bit0; If any ip dry contact sts changed, this bit will be set to 1;
    uint16 opCabCtrlChanged:1; //Bit1; If any op dry contact sts changed, this bit will be set to 1;

    uint16 reserved:14; //Bit2-Bit15;reserved;
};

struct CabCtrlIdChangedStruct
{
    uint16 web:1; //Bit0; if id changed ,the bit is set 1;
    uint16 lcd:1; //Bit1; if id changed ,the bit is set 1;

    uint16 reserved:14; //Bit2-Bit15;reserved;
};

union CabCtrlChangeFlagUnion
{
    uint16 all;
    CabCtrlChangeFlagBitStruct bits;
};

union CabCtrlIdChangedUnion
{
    uint16 all;
    CabCtrlIdChangedStruct bits;
};

struct CabCtrlIdStruct
{
    uint16 id_h;
    uint16 id_l;
};

struct CabCtrlWriteIdStruct
{
    uint16 writeIdNum;//NO more than 20.Reset after being written,indication the number of writes.
    CabCtrlIdStruct writeIdtab[ID_MAX_NUM];//When the card written is negative,it means deletion.
};

struct CabCtrlDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    CabCtrlChangeFlagUnion flag;
//    uint16 IdWRFlag;//When this flag is set 0,all card numbers should be written in.
    uint16 ipCabCtrlNum;
    uint16 opCabCtrlNum;
    CabCtrlRegInfUnion ipCabCtrl[eCabCtrlIpNum];
    CabCtrlRegInfUnion opCabCtrl[eCabCtrlOpNum];
    CabCtrlRegInfUnion opSetCabCtrl[eCabCtrlOpNum];
    
    CabCtrlIdStruct readId;
    CabCtrlIdChangedUnion IdChangedFlag;
//    CabCtrlWriteIdStruct writeId;

};

/*--------------------------------------------Local Inf------------------------------------------------*/

struct CabCtrlLocalDataStruct
{
    uint32 disconnctTimer;
    uint16 noRespTimes;
};

//extern CabCtrlDataStruct *pCabCtrlData;
//extern CabCtrlLocalDataStruct *pCabCtrlLocalData;
//extern uint8 CabCtrlRegBuf[256]; //0-2s

//void CreatCabCtrlShareMemery(void);
//void InitCabCtrlData(uint8 id);
//void AddCabCtrlPollTask(SerialTaskSchedule *serial);
//void CheckCabCtrlActiveTask(SerialTaskSchedule *serial);
//void CabCtrlConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void CabCtrlUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);

#endif
