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

#ifndef LEAKDATASTRUCT
#define LEAKDATASTRUCT

//#include "../DevInterface.h"
#include "Global/GlobalPublic.h"
enum LeakParaEnum
{
    eLeakParaLeakPos, //Leak position
    eLeakParaAlmSts,  //0: Normal; 1: Leak; 2: Cable break
                      //3: Cable  interference; 4: Cable too long
    eLeakParaCableLength,

    eLeakParaCableResist, //resistivity
    eLeakParaOutIO,
    eLeakParaAlmLevel, //0-4->level1-level5

    eLeakParaNum
};

struct LeakParaBitStruct
{
    uint16 changed:1; 	//Bit0
    uint16 settable:1;	//Bit1

    uint16 rsvd:14;	//Bit2-Bit15
};

union LeakParaBitUnion
{
    uint16 all;
    LeakParaBitStruct bit;
};

struct LeakParaStruct
{
    int16 data[eLeakParaNum];
    int16 setValue[eLeakParaNum];
    LeakParaBitUnion flag[eLeakParaNum];
};

struct LeakParaInfStruct
{
    uint16 regAddr;
    int16 coeff;
    int16 lmtDn;
    int16 lmtUp;
    int16 defaultValue;
    bool settable;
    bool used;
};

struct LeakLocalDateStruct
{
    uint32 disconnctTimer;
    uint16 noRespTimes;
    bool stsFlashed;
};

struct LeakDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    LeakParaStruct para;
    uint16 hardwareVer;
    uint16 softwareVerH;
    uint16 softwareVerL;
};

typedef  struct LEAK_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected     
     int           almSts;
     QString       data[eLeakParaNum];
     QString       dataUnit[eLeakParaNum];
}leak_Info;

typedef  struct LEAK_SET{
    int             devNo;
    LeakParaEnum    para;
    uint16          val;
}leak_set;

typedef LeakParaInfStruct LeakParaInfArray[eLeakParaNum];

//extern LeakDataStruct *pLeakData;
//extern LeakLocalDateStruct *pLeakLocalData;

//void CreatLeakShareMemery(void);
//void InitLeakData(uint8 id);
//void AddLeakPollTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void CheckLeakActiveTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void LeakConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void LeakUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);

#endif // LEAKDATASTRUCT

