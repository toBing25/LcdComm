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

#ifndef BMSDATASTRUCT
#define BMSDATASTRUCT

//#include "../DevInterface.h"
#include "Global/GlobalPublic.h"
#include <QList>
//system config file: bms.cfg
//device protocol config file: 0_xxx.cfg, 1_xxx.cfg, .....
#define BMS_CFG_PATH   "/opt/eMTR/uartExt/bms/"
#define BMS_MAX_GROUPS  4
#define BMS_MAX_BATTS   360

enum BmsSysEnum
{
    eBmsSysAlm, //0: normal; !0: alarm
    eBmsSysBattGroups, //max:4

    eBmsSysNum
};

enum BmsGroupEnum
{
    eBmsGroupAlm, //0: normal; !0: alarm
    eBmsGroupVoltOvH, //0: normal; !0: alarm
    eBmsGroupVoltOvL, //0: normal; !0: alarm
    eBmsGroupChgCurrAlm, //0: normal; !0: alarm
    eBmsGroupDisChgCurrAlm, //0: normal; !0: alarm
    eBmsGroupBattSwOpenAlm, //0: normal; !0: alarm
    eBmsGroupCommFailAlm, //0: normal; !0: alarm
    eBmsGroupSocLowAlm, //0: normal; !0: alarm
    eBmsGroupBattSts, //0: disconnect; 1: float; 2:boost; 3:discharge; 4:not use
    eBmsGroupBattNum, //max:250
    eBmsGroupBattType, //0:2V; 1:6V; 2:12V
    eBmsGroupBattAH,
    eBmsGroupVolt, //coeff: 0.1; unit: V;
    eBmsGroupCurr, //coeff: 0.1; unit: A;
    eBmsGroupAvgCellVolt, //coeff: 0.001; unit: V;
    eBmsGroupMaxCellVolt, //coeff: 0.001; unit: V;
    eBmsGroupMaxCellVoltID,
    eBmsGroupMinCellVolt, //coeff: 0.001; unit: V;
    eBmsGroupMinCellVoltID,
    eBmsGroupAvgCellRes, //coeff: 0.001; unit: mΩ;
    eBmsGroupMaxCellRes, //coeff: 0.001; unit: mΩ;
    eBmsGroupMaxCellResID,
    eBmsGroupMinCellRes, //coeff: 0.001; unit: mΩ;
    eBmsGroupMinCellResID,
    eBmsGroupAvgCellTemp, //coeff: 0.1; unit: oC;
    eBmsGroupMaxCellTemp, //coeff: 0.1; unit: oC;
    eBmsGroupMaxCellTempID,
    eBmsGroupMinCellTemp, //coeff: 0.1; unit: oC;
    eBmsGroupMinCellTempID,
    eBmsGroupBattSOC, //coeff: 0.1; unit: %;
    eBmsGroupBattSOH, //coeff: 0.1; unit: %;
    eBmsGroupBalanceID1,
    eBmsGroupBalanceID2,


    eBmsGroupNum
};

enum BmsCellEnum
{
    eBmsCellVolt,
    eBmsCellTemp,
    eBmsCellRes,
    eBmsCellVoltOvHvAlm, //0: normal; !0: alarm
    eBmsCellVoltOvLAlm, //0: normal; !0: alarm
    eBmsCellResOvHAlm, //0: normal; !0: alarm
    eBmsCellTempOvHAlm, //0: normal; !0: alarm
    eBmsCellResDiffOvHAlm, //0: normal; !0: alarm
    eBmsCellVBUSCommFailAlm, //0: normal; !0: alarm
    eBmsCellVoltAnomalyAlm, //0: normal; !0: alarm
    eBmsCellTempAnomalyAlm, //0: normal; !0: alarm

    eBmsCellNum
};


/*--------------------------------------------Struct------------------------------------------------*/
struct BmsDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    uint16 sys[eBmsSysNum];
    uint16 group[BMS_MAX_GROUPS][eBmsGroupNum];
    uint16 cell[BMS_MAX_BATTS][eBmsCellNum];
};


/*--------------------------------------------Local use------------------------------------------------*/

//struct BmsLocalDateStruct
//{
//    uint32 disconnctTimer;
//    uint16 noRespTimes;
//    CntAffirmFlagUnion cntAffirmFlag;
//};

struct BmsRegSetStruct
{
    bool enable;
    uint8 offset;
    uint16 bufId;
    QString type;
    double coeff;
    uint8 startBit;
    uint8 endBit;
    QString map;
    uint16 *pData;
};

/*struct BmsProCfgStruct
{
    QList<ModbProto::ModbReadTaskStruct*> taskList;
    uint16 maxGroups;
    uint16 maxCells;
    BmsRegSetStruct* sys[eBmsSysNum];
    BmsRegSetStruct* group[eBmsGroupNum];
    BmsRegSetStruct* cell[eBmsCellNum];
}*/;

//struct BmsSysCfgStruct
//{
//    uint16 protocolNum;
//    QList<int> onlineProList;
//    QList<BmsProCfgStruct*> onlineProCfg;
//};

//bool InitBmsProto(void);
//void CreatBmsShareMemery(void);
//void InitBmsData(uint8 id);
//void AddBmsPollTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void CheckBmsActiveTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void BmsConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void BmsUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);


#endif // BMSDATASTRUCT

