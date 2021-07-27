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

#ifndef METERDATASTRUCT
#define METERDATASTRUCT

//#include "../DevInterface.h"
#include "Global/SysCfgMng.h"
//system config file: meter.cfg
//device protocol config file: 0_xxx.cfg, 1_xxx.cfg, .....
#define METER_CFG_PATH   "/opt/eMTR/uartExt/meter/"

enum MeterParaEnum
{
    eMeterParaCurrA, //coeff: 0.1; unit: A; uint16
    eMeterParaCurrB, //coeff: 0.1; unit: A; uint16
    eMeterParaCurrC, //coeff: 0.1; unit: A; uint16
    eMeterParaCurrN, //coeff: 0.1; unit: A; uint16
    eMeterParaMaxCurrA, //coeff: 0.1; unit: A; uint16
    eMeterParaMaxCurrB, //coeff: 0.1; unit: A; uint16
    eMeterParaMaxCurrC, //coeff: 0.1; unit: A; uint16
    eMeterParaMaxCurrN, //coeff: 0.1; unit: A; uint16
    eMeterParaVoltAB, //coeff: 0.1; unit: V; uint16
    eMeterParaVoltBC, //coeff: 0.1; unit: V; uint16
    eMeterParaVoltCA, //coeff: 0.1; unit: V; uint16
    eMeterParaVoltA, //coeff: 0.1; unit: V; uint16
    eMeterParaVoltB, //coeff: 0.1; unit: V; uint16
    eMeterParaVoltC, //coeff: 0.1; unit: V; uint16
    eMeterParaFreq, //coeff: 0.01; unit: Hz; uint16
    eMeterParaKwAll, //coeff: 0.1; unit: Kw; int16
    eMeterParaMaxKwAll, //coeff: 0.1; unit: Kw; int16
    eMeterParaKwA, //coeff: 0.1; unit: Kw; int16
    eMeterParaKwB, //coeff: 0.1; unit: Kw; int16
    eMeterParaKwC, //coeff: 0.1; unit: Kw; int16
    eMeterParaKVarAll, //coeff: 0.1; unit: KVar; int16
    eMeterParaMaxKVarAll, //coeff: 0.1; unit: KVar; int16
    eMeterParaKVarA, //coeff: 0.1; unit: KVar; int16
    eMeterParaKVarB, //coeff: 0.1; unit: KVar; int16
    eMeterParaKVarC, //coeff: 0.1; unit: KVar; int16
    eMeterParaKVAAll, //coeff: 0.1; unit: KVA; uint16
    eMeterParaMaxKVAAll, //coeff: 0.1; unit: KVA; uint16
    eMeterParaKvaA, //coeff: 0.1; unit: KVA; uint16
    eMeterParaKvaB, //coeff: 0.1; unit: KVA; uint16
    eMeterParaKvaC, //coeff: 0.1; unit: KVA; uint16
    eMeterParaPfAll, //coeff: 0.01; unit: ; int16
    eMeterParaPfA, //coeff: 0.01; unit: ; int16
    eMeterParaPfB, //coeff: 0.01; unit: ; int16
    eMeterParaPfC, //coeff: 0.01; unit: ; int16
    eMeterParaKWH_High, //coeff: 1; unit: KWH; uint16
    eMeterParaKWH_Low, //coeff: 1; unit: KWH; uint16
    eMeterParaKVarH_High, //coeff: 1; unit: KvarH; uint16
    eMeterParaKVarH_Low, //coeff: 1; unit: KvarH; uint16
    eMeterParaVoltABThd, //coeff: 0.1; unit: %; uint16
    eMeterParaVoltBCThd, //coeff: 0.1; unit: %; uint16
    eMeterParaVoltCAThd, //coeff: 0.1; unit: %; uint16
    eMeterParaVoltAThd, //coeff: 0.1; unit: %; uint16
    eMeterParaVoltBThd, //coeff: 0.1; unit: %; uint16
    eMeterParaVoltCThd, //coeff: 0.1; unit: %; uint16
    eMeterParaCurrAThd, //coeff: 0.1; unit: %; uint16
    eMeterParaCurrBThd, //coeff: 0.1; unit: %; uint16
    eMeterParaCurrCThd, //coeff: 0.1; unit: %; uint16
    eMeterParaCurrNThd, //coeff: 0.1; unit: %; uint16

    eMeterParaNum
};


/*--------------------------------------------Struct------------------------------------------------*/
struct MeterParaBitStruct
{
    uint16 changed:1; 	//Bit0
    uint16 settable:1;	//Bit1

    uint16 rsvd:14;	//Bit2-Bit15
};

union MeterParaBitUnion
{
    uint16 all;
    MeterParaBitStruct bit;
};

struct MeterParaStruct
{
    int16 data[eMeterParaNum];
    int16 setValue[eMeterParaNum];
    MeterParaBitUnion flag[eMeterParaNum];
};

struct MeterLocalDateStruct
{
    uint32 disconnctTimer;
    uint16 noRespTimes;
    //CntAffirmFlagUnion cntAffirmFlag;
};

struct MeterDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    MeterParaStruct para;
    uint16 hardwareVer;
    uint16 softwareVerH;
    uint16 softwareVerL;
};

struct MeterRegSetStruct
{
    bool enable;
    uint16 bufId;
    QString type;
    double coeff;
    uint8 startBit;
    uint8 endBit;
    QString map;
    uint16 *pData;
};

struct MeterProCfgStruct
{
    //QList<ModbProto::ModbReadTaskStruct*> taskList;
    MeterRegSetStruct* para[eMeterParaNum];
};

struct MeterSysCfgStruct
{
    uint16 protocolNum;
    QList<int> onlineProList;
    QList<MeterProCfgStruct*> onlineProCfg;
};

typedef  struct METER_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected
     int           almSts;  //0正常，其他不正常
     QString data[eMeterParaNum];
     QString eMeterParaKWH;
     QString eMeterParaKVarH;

     QString dataUnit[eMeterParaNum];
}meter_Info;

//bool InitMeterProto(void);
//void CreatMeterShareMemery(void);
//void InitMeterData(uint8 id);
//void AddMeterPollTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void CheckMeterActiveTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void MeterConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void MeterUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);


#endif // METERDATASTRUCT

