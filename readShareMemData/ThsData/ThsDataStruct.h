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

#ifndef THSDATASTRUCT_H
#define THSDATASTRUCT_H
#include <QObject>
//#include "../DevInterface.h"
//#include "Global/SysCfgMng.h"

//------------------------------data type define----------------------------------------------------------------------
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef  short int int16;
typedef  long int int32;

enum ThsParaEnum
{
    eThsParaTmp, //temperature
    eThsParaHum, //humidity

    eThsParaAlmSts, //Bit0: integrated warning; Bit1: temperature over; Bit2: temperature low
                    //Bit3: humidity over; Bit4: humidity low
    eThsParaOutIO,

    eThsParaTmpUpper,
    eThsParaTmpLower,
    eThsParaTmpAlmDiff,

    eThsParaHumUpper,
    eThsParaHumLower,
    eThsParaHumAlmDiff,

    eThsParaNum
};

struct ThsParaBitStruct
{
    uint16 changed:1; 	//Bit0
    uint16 settable:1;	//Bit1

    uint16 rsvd:14;	//Bit2-Bit15
};

union ThsParaBitUnion
{
    uint16 all;
    ThsParaBitStruct bit;
};

struct ThsParaStruct
{
    int16 data[eThsParaNum];
    int16 setValue[eThsParaNum];
    ThsParaBitUnion flag[eThsParaNum];
};

struct ThsParaInfStruct
{
    uint16 regAddr;
    int16 coeff;
    int16 lmtDn;
    int16 lmtUp;
    int16 defaultValue;
    bool settable;
    bool used;
};



struct ThsDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    ThsParaStruct para;
    uint16 hardwareVer;
    uint16 softwareVerH;
    uint16 softwareVerL;
};

typedef ThsParaInfStruct ThsParaInfArray[eThsParaNum];

struct ThsUIDataStruct{
    uint16 devId;		//offset
    uint16 uartId;		// 0:eUart1; 1:eUart2; 2:eUart3; 3:eUart4; 4:eUart5
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    int16 data[eThsParaNum];

};

typedef  struct THS_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected
     int           almSts;
     double data[eThsParaNum];
     QString dataUnit[eThsParaNum];
     QString   eThsParaAlmSts;

}ths_Info;

typedef  struct THS_SET{
    int         devNo;
    ThsParaEnum para;
    double      val;
}ths_set;





#endif // THSDATASTRUCT_H

