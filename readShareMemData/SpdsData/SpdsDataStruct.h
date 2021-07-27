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

#ifndef SPDSDATASTRUCT
#define SPDSDATASTRUCT

#include "Global/GlobalPublic.h"
#include "Global/SysCfgMng.h"
//#include "../DevInterface.h"


//system config file: spds.cfg
//device protocol config file: 0_xxx.cfg, 1_xxx.cfg, .....
#define SPDS_CFG_PATH   "/opt/eMTR/uartExt/spds/"

//system
enum SpdsSysEnum
{
    eSpdsSysCompAlm, //Comprehensive alarm
    eSpdsSysRsvd1,
    eSpdsSysRsvd2,
    eSpdsSysRsvd3,
    eSpdsSysRsvd4,
    eSpdsSysRsvd5,
    eSpdsSysRsvd6,
    eSpdsSysRsvd7,
    eSpdsSysRsvd8,
    eSpdsSysRsvd9,

    eSpdsSysNum
};

//main input
enum SpdsMainEnum
{
    eSpdsMainVolt_AB, //0.1V
    eSpdsMainVolt_BC, //0.1V
    eSpdsMainVolt_CA, //0.1V
    eSpdsMainVolt_LN, //0.1V
    eSpdsMainVolt_A, //0.1V
    eSpdsMainVolt_B, //0.1V
    eSpdsMainVolt_C, //0.1V
    eSpdsMainCurr_A, //0.1A
    eSpdsMainCurr_B, //0.1A
    eSpdsMainCurr_C, //0.1A
    eSpdsMainCurr_N, //0.1A
    eSpdsMainLoad_A, //0.1%
    eSpdsMainLoad_B, //0.1%
    eSpdsMainLoad_C, //0.1%
    eSpdsMainFreq_A, //0.1Hz
    eSpdsMainFreq_B, //0.1Hz
    eSpdsMainFreq_C, //0.1Hz
    eSpdsMainKWhHi_A, //0.1Kwh
    eSpdsMainKWhLo_A, //0.1Kwh
    eSpdsMainKWhHi_B, //0.1Kwh
    eSpdsMainKWhLo_B, //0.1Kwh
    eSpdsMainKWhHi_C, //0.1Kwh
    eSpdsMainKWhLo_C, //0.1Kwh
    eSpdsMainKva_A, //0.01kVA
    eSpdsMainKva_B, //0.01kVA
    eSpdsMainKva_C, //0.01kVA
    eSpdsMainKw_A, //0.01kW
    eSpdsMainKw_B, //0.01kW
    eSpdsMainKw_C, //0.01kW
    eSpdsMainKVar_A, //0.01kVar
    eSpdsMainKVar_B, //0.01kVar
    eSpdsMainKVar_C, //0.01kVar
    eSpdsMainPF_A, //0.01
    eSpdsMainPF_B, //0.01
    eSpdsMainPF_C, //0.01
    eSpdsMainTHDU_A, //0.1%
    eSpdsMainTHDU_B, //0.1%
    eSpdsMainTHDU_C, //0.1%
    eSpdsMainTHDI_A, //0.1%
    eSpdsMainTHDI_B, //0.1%
    eSpdsMainTHDI_C, //0.1%
    eSpdsMainKWhHi_all, //0.1Kwh
    eSpdsMainKWhLo_all, //0.1Kwh
    eSpdsMainRsvd44,
    eSpdsMainRsvd45,
    eSpdsMainRsvd46,
    eSpdsMainRsvd47,
    eSpdsMainRsvd48,
    eSpdsMainRsvd49,
    eSpdsMainRsvd50,

    eSpdsMainNum
};


//Branch output
enum SpdsBranchEnum
{
    eSpdsBranchVolt, //0.1V
    eSpdsBranchCurr, //0.1A
    eSpdsBranchLoad, //0.1%
    eSpdsBranchKWhHi, //0.01Kwh
    eSpdsBranchKWhLo, //0.01Kwh
    eSpdsBranchKva, //0.01kVA
    eSpdsBranchKw, //0.01kW
    eSpdsBranchTHDI, //0.1%
    eSpdsBranchSwSts, //Switch status
    eSpdsBranchCurrOvUp, //Current over upper limit
    eSpdsBranchCurrUnderLo, //Current under lower limit
    eSpdsBranchRsvd12,
    eSpdsBranchRsvd13,
    eSpdsBranchRsvd14,
    eSpdsBranchRsvd15,

    eSpdsBranchNum
};

/*--------------------------------------------Struct------------------------------------------------*/
struct SpdsDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    uint16 sys[eSpdsSysNum];
    uint16 main[SPDS_MAIN_NUM][eSpdsMainNum];
    uint16 branch[SPDS_BRANCH_NUM][eSpdsBranchNum];
};

typedef  struct SPDS_PROTO{
    int mainNum;
    int branchNum;
    bool MainCfg[eSpdsMainNum];
    bool BranchCfg[eSpdsBranchNum];
}spds_proto;

typedef  struct SPDS_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected
     int           almSts;
     int           mainNum;
     int           branchNum;
     int           mainEn[SPDS_MAIN_NUM]; //0, disable; 1, enable;
     int           branchEn[SPDS_BRANCH_NUM]; //0, disable; 1, enable;
     QString       main[SPDS_MAIN_NUM][eSpdsMainNum];
     QString       mainUnit[SPDS_MAIN_NUM][eSpdsMainNum];

     QString       branch[SPDS_BRANCH_NUM][eSpdsBranchNum];
     QString       branchUnit[SPDS_BRANCH_NUM][eSpdsBranchNum];
}spds_Info;

/*--------------------------------------------config Inf------------------------------------------------*/
struct SpdsProSetStruct
{
    uint16 mainNum;
    uint16 branchNum;
};

struct SpdsRegSetStruct
{
    bool enable;
    uint8 function;
    uint16 address;
    QString type;
    double coeff;
    uint8 startBit;
    uint8 endBit;
    QString map;
    uint16 *pData;
};

struct SpdsProCfgStruct
{
    SpdsProSetStruct proCfg;
    SpdsRegSetStruct sysCfg[eSpdsSysNum];
    SpdsRegSetStruct mainCfg[SPDS_MAIN_NUM][eSpdsMainNum];
    SpdsRegSetStruct branchCfg[SPDS_BRANCH_NUM][eSpdsBranchNum];
};

struct SpdsSysCfgStruct
{
    uint16 protocolNum;
    QList<int> onlineProList;
    QList<SpdsProCfgStruct> onlineProCfg;
};


/*--------------------------------------------Local Inf------------------------------------------------*/

struct SpdsLocalDateStruct
{
    uint32 disconnctTimer;
    uint16 noRespTimes;
};


//bool InitSpdsProto();
//void CreatSpdsShareMemery(void);
//void InitSpdsData(uint8 id);
//void AddSpdsPollTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void CheckSpdsActiveTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void SpdsConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void SpdsUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);


#endif // SPDSDATASTRUCT

