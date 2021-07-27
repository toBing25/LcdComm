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

#ifndef UPSDATASTRUCT
#define UPSDATASTRUCT

//#include "../DevInterface.h"
#include "Global/GlobalPublic.h"
#include <QString>
// Ctrl cmd
#define	CMD_FAULT_CLR  			0x00FF
#define	CMD_HISLOG_CLR			0x00FF
#define	CMD_MUTE				0x00FF
#define	CMD_ESC_MUTE   			0x0000
#define	CMD_TX_BYP 				0x00FF
#define	CMD_ESC_BYP				0x0000

#define	CMD_BATT_TEST 			0x000F
#define	CMD_BATT_MAINT			0x00F0
#define	CMD_BATT_BOOST			0x0F00
#define	CMD_BATT_FOLAT			0xF000
#define	CMD_BATT_STOP_TEST		0x0000

#define	CMD_REC_OFF				0x00
#define	CMD_REC_ON				0x01
#define	CMD_INV_OFF				0x02
#define	CMD_INV_ON				0x03


#define	ALL_MODULE 				0x7F

//
#define MAX_INF_LEN     20

enum UpsStsEnum
{
    eUpsStsGeneralAlm,	// 1: noraml; 2: alarm;
    eUpsStsLoadOnSrc,	// 0: none, 1: ups; 2 byp
    eUpsStsEpo,         // 0: normal; 1:alarm
    eUpsStsFanFail,     // 0: normal; 1:alarm
    eUpsStsTmpFail,     // 0: normal; 1:alarm

    eUpsStsRecFail,     // 0: normal; 1:alarm
    eUpsStsIpFail,      // 0: normal; 1:alarm
    eUpsStsBypFail,     // 0: normal; 1:alarm
    eUpsStsBypVoltFail, // 0: normal; 1:alarm
    eUpsStsBypUntrack,  // 0: normal; 1:alarm

    eUpsStsBypOvLd,		// 0: noraml, 1: OvLdTout; 2: OvLd
    eUpsStsBypSeqFail,  // 0: normal; 1:alarm
    eUpsStsMaintCb,     // 0: normal; 1:alarm
    eUpsStsInvFail,     // 0: normal; 1:alarm
    eUpsStsInvOvLd,		// 0: noraml, 1: OvLdTout; 2: OvLd

    eUpsStsOpShorted,   // 0: normal; 1:alarm
    eUpsStsBattSts,		// 0: disconnect, 1: not work; 2: float; 3: boost; 4: discharge; 5: not detect
    eUpsStsBattVoltLow, // 0: normal; 1:alarm
    eUpsStsEod,         // 0: normal; 1:alarm
    eUpsStsBattReverse, // 0: normal; 1:alarm

    eUpsStsBattTestResult,	// 0: not test, 1: OK; 2: fail; 3: testing, @todo..., delete ???
    eUpsStsBattMaintResult,	// 0: not test, 1: OK; 2: fail; 3: testing, @todo..., delete ???
    eUpsStsOnUpsInhibited,  //0:Invertor On Enable; 1:Invertor On Disable
    eUpsStsManualTxByp,     //0:No; 1:Yes
	//wanghf20170322, @todo...
	eUpsStsRecSts,		// 0: Rec off; 1: softstart, 2: normal work
	
    eUpsStsNum
};

enum UpsParaEnum
{
    eUpsParaBypVoltPhA,     //coef: 0.1, V
    eUpsParaBypVoltPhB,     //coef: 0.1, V
    eUpsParaBypVoltPhC,     //coef: 0.1, V
    eUpsParaBypCurrPhA,     //coef: 0.1, A
    eUpsParaBypCurrPhB,     //coef: 0.1, A
    eUpsParaBypCurrPhC,     //coef: 0.1, A
    eUpsParaBypFreqPhA,     //coef: 0.01, Hz
    eUpsParaBypFreqPhB,     //coef: 0.01, Hz
    eUpsParaBypFreqPhC,     //coef: 0.01, Hz
    eUpsParaBypPfPhA,       //coef: 0.01
    eUpsParaBypPfPhB,       //coef: 0.01
    eUpsParaBypPfPhC,       //coef: 0.01

    eUpsParaIpVoltPhA,      //coef: 0.1, V
    eUpsParaIpVoltPhB,      //coef: 0.1, V
    eUpsParaIpVoltPhC,      //coef: 0.1, V
    eUpsParaIpCurrPhA,      //coef: 0.1, A
    eUpsParaIpCurrPhB,      //coef: 0.1, A
    eUpsParaIpCurrPhC,      //coef: 0.1, A
    eUpsParaIpFreqPhA,      //coef: 0.01, Hz
    eUpsParaIpFreqPhB,      //coef: 0.01, Hz
    eUpsParaIpFreqPhC,      //coef: 0.01, Hz
    eUpsParaIpPfPhA,        //coef: 0.01
    eUpsParaIpPfPhB,        //coef: 0.01
    eUpsParaIpPfPhC,        //coef: 0.01

    eUpsParaOpVoltPhA,		//coef: 0.1, V
    eUpsParaOpVoltPhB,      //coef: 0.1, V
    eUpsParaOpVoltPhC,      //coef: 0.1, V
    eUpsParaOpCurrPhA,      //coef: 0.1, A
    eUpsParaOpCurrPhB,      //coef: 0.1, A
    eUpsParaOpCurrPhC,      //coef: 0.1, A
    eUpsParaOpFreqPhA,      //coef: 0.01, Hz
    eUpsParaOpFreqPhB,      //coef: 0.01, Hz
    eUpsParaOpFreqPhC,      //coef: 0.01, Hz
    eUpsParaOpPfPhA,        //coef: 0.01
    eUpsParaOpPfPhB,        //coef: 0.01
    eUpsParaOpPfPhC,        //coef: 0.01

    eUpsParaOpKvaPhA,		//coef: 0.1, KVA
    eUpsParaOpKvaPhB,       //coef: 0.1, KVA
    eUpsParaOpKvaPhC,       //coef: 0.1, KVA
    eUpsParaOpKwPhA,        //coef: 0.1, KW
    eUpsParaOpKwPhB,        //coef: 0.1, KW
    eUpsParaOpKwPhC,        //coef: 0.1, KW

    eUpsParaLdPerPhA,		//coef: 0.1, %
    eUpsParaLdPerPhB,       //coef: 0.1, %
    eUpsParaLdPerPhC,       //coef: 0.1, %

    eUpsParaEnvirTemp,      //coef: 0.1, oC

    eUpsParaBattVoltPos,    //coef: 0.1, V
    eUpsParaBattVoltNeg,    //coef: 0.1, V
    eUpsParaBattCurrPos,    //coef: 0.1, A
    eUpsParaBattCurrNeg,    //coef: 0.1, A
    eUpsParaBattTemp,       //coef: 0.1, oC
    eUpsParaBattRemdTime,   //coef: 0.1, min
    eUpsParaBattCapa,       //coef: 0.1, %

    eUpsParaBypFanRunTime,  //coef: 1, h

    eUpsParaNum
};

enum UpsCfgEnum
{
    eUpsCfgSysMode,     //1:single; 2:parallel; 4:single ECO; 6:parallel ECO; 8:LBS; 10:parallel LBS;
    eUpsCfgBattNum,     //coef: 1
    eUpsCfgBattAH,      //coef: 1, AH
    eUpsCfgCabPower,    //coef: 1, kVA
    eUpsCfgIpVolt,      //coef: 1, V
    eUpsCfgIpHz,        //coef: 1, Hz
    eUpsCfgOpVolt,      //coef: 1, V
    eUpsCfgOpHz,        //coef: 1, Hz
    eUpsCfgSysTimeMinSec, //Bit15-8:second; Bit7-0:minute; BCD code
    eUpsCfgSysTimeDayHour, //Bit15-8:hour; Bit7-0:day; BCD code
    eUpsCfgSysTimeYearMon, //Bit15-8:month; Bit7-0:year; BCD code

    eUpsCfgNum
};

enum UpsCtlEnum
{
    eUpsCtlFaultClear,  //0x00FF:clear
    eUpsCtlMute,        //0x00FF:mute; 0x0000:ESC mute
    eUpsCtlManTxByp,    //0x00FF:Tx Bypass; 0x0000:ESC manual bypass
    eUpsCtlBattCmd,    //0x000F:batt test; 0x00F0:batt maintenance; 0x0000:stop test;
    eUpsCtlTimedShut,   //5-32767s;
    eUpsCtlTimedON,     //0-65535min;

    eUpsCtlNum
};


/*--------------------------------------------Struct------------------------------------------------*/
struct UpsSetBitStruct
{
    uint16 changed:1; 	//Bit0

    uint16 rsvd:15;	//Bit2-Bit15
};

struct UpsCfgStruct
{
    int16 data[eUpsCfgNum];
    int16 setValue[eUpsCfgNum];
    UpsSetBitStruct flag[eUpsCfgNum];
};

struct UpsCtlStruct
{
    int16 data[eUpsCtlNum];
    int16 setValue[eUpsCtlNum];
    UpsSetBitStruct flag[eUpsCtlNum];
};

struct UpsInfStruct
{
    uint16 manufacturer[MAX_INF_LEN]; //20 unicode, end with 0x0000;
    uint16 MtrVer[MAX_INF_LEN];  //20 unicode, end with 0x0000;
    uint16 series[MAX_INF_LEN];  //20 unicode, end with 0x0000;
    uint16 modelName[MAX_INF_LEN]; //20 unicode, end with 0x0000;
    uint16 ioType; //1: 3in3out; 2: 3in1out; 3: 1in1out; 4: 1in3out; 5: 2in2out
};

struct UpsInfString
{
    QString manufacturer; //20 unicode, end with 0x0000;
    QString MtrVer;  //20 unicode, end with 0x0000;
    QString series;  //20 unicode, end with 0x0000;
    QString modelName; //20 unicode, end with 0x0000;
    uint16 ioType; //1: 3in3out; 2: 3in1out; 3: 1in1out; 4: 1in3out; 5: 2in2out
};

struct UpsDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    uint16 sts[eUpsStsNum];
    uint16 para[eUpsParaNum];
    UpsCfgStruct cfg;
    UpsCtlStruct ctl;
    UpsInfStruct inf;
};


/************************************local used*************************************************/
struct UpsRegStruct
{
    uint16 funcCode;
    uint16 regAddr;
    int16 coeff;
    uint16 lmtDn;
    uint16 lmtUp;
    uint16 defaultValue;
    bool settable;
    bool used;
};

struct UpsLocalDateStruct
{
    uint32 disconnctTimer;
    uint16 noRespTimes;
    uint16 CfgFlsTimes;
    //CntAffirmFlagUnion cntAffirmFlag;
    uint16 UpsType;
    bool AddedCompAlm;
};

struct UpsMeterDataStruct{
   QString eUpsParaBypVoltPh;
   QString eUpsParaBypCurrPh;
   QString eUpsParaBypFreqPh;
   QString eUpsParaBypPfPh  ;
   QString eUpsParaIpVoltPh ;
   QString eUpsParaIpCurrPh ;
   QString eUpsParaIpFreqPh ;
   QString eUpsParaIpPfPh   ;
   QString eUpsParaOpVoltPh ;
   QString eUpsParaOpCurrPh ;
   QString eUpsParaOpFreqPh ;
   QString eUpsParaOpPfPh   ;
   QString eUpsParaOpKvaPh  ;
   QString eUpsParaOpKwPh   ;
   QString eUpsParaLdPerPh  ;

};

typedef  struct UPS_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected
     int           almSts;
     QString sts[eUpsStsNum];
     QString para[eUpsParaNum];
     QString cfgData[eUpsCfgNum];
     //QString ctlData[eUpsCtlNum];
     UpsInfString inf;
     UpsMeterDataStruct meterD;

     QString paraUnit[eUpsParaNum];
     QString cfgUnit[eUpsCfgNum];


}ups_Info;

typedef  struct UPS_SET{
    int         devNo;
    UpsCtlEnum  para;
    uint16      val;
}ups_set;


typedef UpsRegStruct UpsCfgRegArray[eUpsCfgNum];
typedef UpsRegStruct UpsCtlRegArray[eUpsCtlNum];

//extern UpsDataStruct *pUpsData;
//extern UpsLocalDateStruct *pUpsLocalData;

//void CreatUpsShareMemery(void);
//void InitUpsData(uint8 id);
//void AddUpsPollTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void CheckUpsActiveTask(UartIdEnum uart,SerialTaskSchedule *serial);
//void UpsConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void UpsUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);


#endif // UPSDATASTRUCT

