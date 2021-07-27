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

#ifndef SYSCFGMNG_H
#define SYSCFGMNG_H
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
//#include <QSerialPort>
#include "ShareMemory.h"
#include "GlobalParameter.h"



//-----------------------------------------------------------------------------
// Adding Conditional Definitions Stuff

#ifdef SYSCFGMNG_C
	#define SYSCFGMNG_PUBLIC
	#define SYSCFGMNG_CONST
#else
	#define SYSCFGMNG_PUBLIC	extern
	#define SYSCFGMNG_CONST	const
#endif


//-----------------------------------------------------------------------------
// Adding Constants and Macro Definitions

#define UPS_MAX_MOD_NUM			10
#define	UPS_MODB_ADDR_RANGE  	254

#define UPS_MAX_NUM				30
#define AC_MAX_NUM				8
#define EXT_DEV_MAX_NUM			64	// 2 UART
#define MQTT_MAX_NUM			8


#define THS_MAX_NUM			62
#define PDU_MAX_NUM			8
#define LEAK_MAX_NUM		8
#define SMS_MAX_NUM			1
#define ITPDC_MAX_NUM		8
#define METER_MAX_NUM		16
#define DC_MAX_NUM          20	//接线盒最大个数
#define BMS_MAX_NUM         4
#define ATS_MAX_NUM         1
#define FAN_MAX_NUM         1
#define SHOW_MAX_NUM        1
#define FANCTL_MAX_NUM      10
#define CTRLBOX_MAX_NUM		1 //2021

#define SPDS_MAX_NUM		8
#define SPDS_MAIN_NUM		4
#define SPDS_BRANCH_NUM		120

//#define EXT_PORT_NUM		6
#define EXT_MODB_ADDR_RANGE 31

#define SYS_CAB_MAX_NUM		5
#define PDU_PLUG_MAX_NUM	24

#define SMS_SEND_DATA_MAX_LEN	60
#define SMS_SENDER_MAX_LEN		20
#define SMS_CSCA_MAX_LEN		20
#define SMS_SEND_BUF_MAX_NUM	20

#define MQTT_SERVER_NAME_LEN	10
#define MQTT_USER_NAME_LEN		10
#define MQTT_PWD_LEN			10

#define SN_LEN					10

//-----------------------------------------------------------------------------
//Adding Private Enumerated and Structure Definitions


//-----------------------------------------------------------------------------
//Adding Public Enumerated and Structure Definitions Stuff

//---------------------------- SysInfo Setting ----------------------------
struct SysInfoStruct
{
	uint16 srlNum[SN_LEN];	//use ASCII
	uint16 sysType; //0:dangui(ISmart); 1:danpai(IWit); 2:weimokuai(ITalent); 3:ILegend, wanghf20180305
	uint16 sysLocked;	// 0: unlock, 1: locked, wanghf20180702
};


//---------------------------- EMtrSys Setting ----------------------------
enum DataSaveLocationEnum
{
	eDataSaveMem = 0,
	eDataSaveSd = 1,

	eDataSaveLocNum
};

enum SysTypeEnum
{
	eSysTypeISmart = 0,
	eSysTypeIWit,
	eSysTypeITalent,
	eSysTypeILegend,

	eSysTypeNum
};

struct EMtrSysSetStruct
{
	//wanghf20180703, move to ".ini", because these should not recover by user
	uint16 rsvd0;	// sysLocked: 0: unlock, 1: locked
	uint16 rsvd1;	// noLogo: 0: display logo, 1: no logo
	uint16 doorOpenToutSec;	//logoDispTime: sec, wanghf20210218: rsvd2 used for DoorOpenToutSec(0~1800s), 
	uint16 dateSaveLoc;	//0: internal flash, 1: SD card
	uint16 rsvd; //sysType: 0:dangui(ISmart); 1:danpai(IWit); 2:weimokuai(ITalent); 3:ILegend
};

struct EMtrNetSetStruct
{
    uint16 dhcp;	// 0: manual, 1: dhcp, nonuse
	
    uint16 ipByte1;
    uint16 ipByte2;
    uint16 ipByte3;
    uint16 ipByte4;

    uint16 maskByte1;
    uint16 maskByte2;
    uint16 maskByte3;
    uint16 maskByte4;

    uint16 routeByte1;
    uint16 routeByte2;
    uint16 routeByte3;
    uint16 routeByte4;

    uint16 dns1Byte1;
    uint16 dns1Byte2;
    uint16 dns1Byte3;
    uint16 dns1Byte4;

    uint16 dns2Byte1;
    uint16 dns2Byte2;
    uint16 dns2Byte3;
    uint16 dns2Byte4;
};

//wanghf20180817
enum DryIpFncEnum
{
	eDryIpFncDisable = 0,
		
    eDryIpFncFrontDoorOp,
    eDryIpFncRearDoorOp,
	eDryIpFncSmokeAlarm,
	eDryIpFncIrAlarm,
	eDryIpFncIrTamper,
	eDryIpFncLeakAlarm,
	eDryIpFncDoorSensor,
	eDryIpFncSysMute,
	eDryIpFncUpsTxByp,
	eDryIpFncUpsFaultClr,
	eDryIpFncFireAlarm,
	eDryIpFncFireLinkage,
	eDryIpFncUtilityAbnormal,	//wanghf20190925
	eDryIpFncFilterBlock,
	
	eDryIpFncNum
};


enum DryOpFncEnum
{
	eDryOpFncDisable = 0,
	
	eDryOpFncSysAlm,
	eDryOpFncUpsIpFail,
	eDryOpFncUpsBattLow,
	eDryOpFncUpsNormalMode,
	eDryOpFncUpsOnByp,
	eDryOpFncUpsBattMode,
	eDryOpFncOpenSkylight,
	eDryOpFncSysImportantAlm,	//wanghf20170119, only level 2 alm
	eDryOpFncFanCtrl,	//wanghf20180529, for Fan power and blast gate
	eDryOpFncProjectileDoorCtrl,	//wanghf20180827
	eDryOpFncAudibleVisualAlm,	//wanghf20181025
	eDryOpFncAtmosphereLampRed,	//wanghf20190123
    eDryOpFncAtmosphereLampGreen,
	eDryOpFncAtmosphereLampBlue,
	eDryOpFncDoorCtrl,	//wanghf20200820
	eDryOpFncDoorSensor,
	eDryOpFncSolenoidValve,	//wanghf20201019
	eDryOpFncSmokeAlarm,	//wanghf20201223

    eDryOpFncFrontCtrl,
    eDryOpFncRearCtrl,
    eDryOpFncFrontLight,
    eDryOpFncRearLight,
	eDryOpFncNum
};

//wanghf20200413
enum DryIpEnum
{
	eDryIp1 = 0,
	eDryIp2,
	eDryIp3,
	eDryIp4,
	eDryIp5,
	eDryIp6,
	eDryIp7,


	eDryIpNum
};

enum DryOpEnum
{
	eDryOp1 = 0,
	eDryOp2,
	eDryOp3,
	eDryOp4,
	eDryOp5,
	eDryOp6,
	eDryOp7,
	eDryOp8,
	eDryOp9,

	eDryOpNum
};

struct EMtrDrySetStruct
{
	uint16 dryFncIp[eDryIpNum];
	uint16 dryFncOp[eDryOpNum];
	int16 dryActLevelIp[eDryIpNum];
};

struct eMtrEmerFanSocketMngStruct
{
	uint16 frontFanPduAddr;	//wanghf20170301, starting at "0", use idx actually.
	uint16 backFanPduAddr;
	uint16 frontFanRlyId;
	uint16 backFanRlyId;
};

struct EMtrThsMngStruct
{
	//wanghf20160914, value from web setting
	quint16 coldThsAddrRangeMin;
	quint16 coldThsAddrRangeMax;
	quint16 hotThsAddrRangeMin;
	quint16 hotThsAddrRangeMax;
	quint16 ambThsAddrRangeMin;
	quint16 ambThsAddrRangeMax;
	quint16 projDoorCriticalTmp;	//wanghf20180827, for TJM projectile door Critical temperature
};

struct EMtrModbusSetStruct
{
    quint16 rtuBaudrate;// default: 9600;
    quint16 modbusAddress; //default: 1;
    quint16 tcpPort; //default: 502;
};

//---------------------------- UPS Setting ----------------------------

enum UpsTypeEnum
{
    eUpsTypeRMX_10_600 = 1,
    eUpsTypeRM_10_200,
    eUpsTypeHT33_40_200,
    eUpsTypeHTX33_10_40,
    eUpsTypeHT33_10_30,
    eUpsTypeHT31_10_20,
    eUpsTypeHT11_6_20,
    eUpsTypeHT11_1_3,
    eUpsTypeHTX11_6_20,
    eUpsTypeRM31_5_20,

    eUpsTypeEnd
};

enum ModbModeEnum
{
    eModbModASCII = 0,
    eModbModRTU = 1,
    eModbModTCP = 2,
    
    eModbModNum
};

enum UpsProtoEnum
{
    eUpsProtoInvt ,
    eUpsProtoSNT11_TCP ,
    eUpsProtoSNT33_TCP ,
    eUpsProtoShanTeC620,
    eUpsProtoShanTe33,
    eUpsProtoGalleon,
    eUpsProtoVERTI,

    eUpsProtoNum
};

struct UpsHeadStruct
{
    uint16 ipAddr1; //default:0
    uint16 ipAddr2; //default:0
    uint16 ipAddr3; //default:0
    uint16 ipAddr4; //default:0
    uint16 modbTcpPort; //default:502
};
//---------------------------- AC Setting ----------------------------

enum AcProtoEnum
{
    eAcInvt = 0,
    eAcInvtOs, //invt外购主控
    eAcProtoNum
};

struct AcHeadStruct
{
    //mei20/6/2:add modbusTCP
    uint16 rateAirVolume; //m3/h
};

//---------------------------- UartExt Setting ----------------------------
//RS485 Device
enum ThsProtoEnum
{
    eThsProtoInvt = 0,
    eThsProtoTejeem,
    eThsProXW210,

    eThsProtoNum
};

enum PduProtoEnum
{
    ePduProtoInvt = 0,

    ePduProtoNum
};

enum LeakProtoEnum
{
    eLeakProtoInvt = 0,

    eLeakProtoNum
};

enum SmsProtoEnum
{
    eSmsProtoInvt = 0,

    eSmsProtoNum
};

enum ItPdcProtoEnum
{
    eItPdcProtoInvt = 0,

    eItPdcProtoNum
};

enum MeterProtoEnum
{
    eMeterProtoDirisA20 = 0,
    eMeterProtoTejeem,
    eMeterProtoHDS3100,
    eMeterProtoP3M96CL,
    eMeterProtoWX961,
    eMeterProtoT1UC,
    eMeterProtoM989,
    eMeterProtoBNZW,
    eMeterProtoPZ125,
    eMeterProtoDM40,
    eMeterProtoINSMD10EU,
    eMeterProtoINSMD30ES,
    eMeterProtoINHDD1818,
    eMeterProtoINHDD3000,
    eMeterProtoINHDD30YF,
    eMeterProtoINDBL3,
    eMeterProtoSQP7,
    eMeterProtoANTIN180G,
    eMeterProtoANTIN180GCT,
    eMeterProtoYDET703,

    eMeterProtoNum
};

enum AtsProtoEnum
{
    eAtsProtoOMD800 = 0,
    eAtsProtoSchneider,

    eAtsProtoNum
};

enum FanProtoEnum
{
    eFanProtoDl10027 = 0,
    eFanProtoXD800,

    eFanProtoNum
};

enum CtrlBoxProtoEnum
{
    eCtrlBoxProtoTengSS = 0,
    eCtrlBoxProtoNum
};


enum FanCtlProtoEnum
{
    eFanCtlProtoTjm = 0,

    eFanCtlProtoNum
};

enum UartIdEnum
{
    eUart1 = 0,
    eUart2,
    eUart3,
    eUart4,//接线盒使用
    eUart5,//短信报警器使用

    eUartNum
};

enum ExtDevEnum
{
    eExtDevUps,
    eExtDevAc,
    eExtDevThs,
    eExtDevPdu,
    eExtDevLeak,
    eExtDevSms,
    eExtDevItPdc,
    eExtDevMeter,
    eExtDevSpds,
    eExtDevDc, //改为接线盒
    eExtDevBMS,
    eExtDevAts,
    eExtDevFan,
    eExtDevShow,
    eExtDevFanCtl,
    eExtDevCtrlBox,//2021

    eExtDevNum,

    eExtDevINVT //for auto check device
};

struct UartExtSetDataStruct
{
    uint16 baud; //QSerialPort::BaudRate
    uint16 stopBits; //QSerialPort::StopBits
    uint16 parity; //QSerialPort::Parity
};

struct ExtDeviceHeadStruct
{
	uint16 extProto;
	uint16 devType;		//ExtDevEnum, 0:eExtDevUps; 1:eExtDevAc; 2:eExtDevThs; 3:eExtDevPdu; 4:eExtDevLeak;
	uint16 devId;		//offset
	uint16 uartId;		// 0:eUart1; 1:eUart2; 2:eUart3; 3:eUart4; 4:eUart5
	uint16 modbAddr;	//1-31
	uint16 modbMod;		//0:ASCII; 1:RTU
};


//----------------------Mqtt Setting------------------------------------------------------
struct MqttServerIpStruct
{
    uint16 ipByte1;
    uint16 ipByte2;
    uint16 ipByte3;
    uint16 ipByte4;
};
struct MqttSetDataStruct
{
    MqttServerIpStruct serverIp; //default: invt server: 14.215.130.180
    uint16 serverName[MQTT_SERVER_NAME_LEN]; //ASCII, default: INVT MQTT Server
    uint16 serverPort; //default: 1883
    uint16 userName[MQTT_USER_NAME_LEN]; // ASCII, default: InvtPower, username[0](0x496E); username[1] (0x7674); username[2] (0x506F);  username[3] (0x7765);  username[4] (0x7200); 
    uint16 passwd[MQTT_PWD_LEN]; //ASCII, default: InvtPower666
};

//wanghf20180702
//---------------------------- CustomInfo Setting ----------------------------
struct EMtrSysModelStruct
{
	//eMTR-DCRT, use ASCII
	uint16 modelWord0;	// bit15~8: e(0x65), bit7~0: M(0x4D)
	uint16 modelWord1;	// bit15~8: T(0x54), bit7~0: R(0x52)
	uint16 modelWord2;	// bit15~8: -(0x2D), bit7~0: D(0x44)
	uint16 modelWord3;	// bit15~8: C(0x43), bit7~0: R(0x52)
	uint16 modelWord4;	// bit15~8: T(0x54), bit7~0: (0x00)
	uint16 modelWord5;
	uint16 modelWord6;
	uint16 modelWord7;
	uint16 modelWord8;
	uint16 modelWord9;
};

struct EMtrCompNameStruct
{
	//INVT, use ASCII
	uint16 compWord0;	// bit15~8: I(0x49), bit7~0: N(0x4E)
	uint16 compWord1;	// bit15~8: V(0x56), bit7~0: T(0x54)
	uint16 compWord2;
	uint16 compWord3;
	uint16 compWord4;
	uint16 compWord5;
	uint16 compWord6;
	uint16 compWord7;	
};

struct CustomInfoStruct
{
	uint16 noLogo;	// 0: display logo, 1: no logo
	uint16 logoDispTime;	//sec
	uint16 brandType; // 0: INVT; 1~10: rsvd; 11: TEJEEM; 12~65535: other nonstandard user

	EMtrSysModelStruct model;
	EMtrCompNameStruct compName;
};


//--------------------------------------------------------

struct SysEmerFanMngStruct
{
	uint16 sysCabNum;
	eMtrEmerFanSocketMngStruct fanSocketMng[SYS_CAB_MAX_NUM];
};

struct EMtrMainSetStruct
{
	EMtrSysSetStruct sysSet;
	EMtrNetSetStruct netSet;
	EMtrDrySetStruct drySet[DC_MAX_NUM];
	SysEmerFanMngStruct fanMng;
	EMtrThsMngStruct thsMng;
	EMtrModbusSetStruct modbSet;
};

struct SpdsSetStruct
{
    uint16 spdsPt;
    uint16 spdsCt;
    uint16 mainEn[SPDS_MAIN_NUM]; //0, disable; 1, enable;
    uint16 branchEn[SPDS_BRANCH_NUM]; //0, disable; 1, enable;
};

struct ThsSetStruct
{
	uint16 tmpUpper;
	uint16 tmpLower;
	uint16 tmpAlmDiff;
	uint16 humUpper;
	uint16 humLower;
	uint16 humAlmDiff;
};

//wanghf20190926, for TJM Fan controller
struct FanCtlSetStruct
{
	quint16 onOffSts;	// 0:off, 1:on 
	quint16 tmp2Pwm30;
	quint16 tmp2Pwm50;
	quint16 tmp2Pwm80;
};

//lcl20201110, meter set pt and ct
struct MeterSetStruct
{
    uint16 meterPt;
    uint16 meterCt;
};

struct UartExtSetStruct
{
	uint16 devSettedFlag;
	uint16 devNum[eExtDevNum];
	uint16 onlineDev[eExtDevNum][EXT_DEV_MAX_NUM];
	ExtDeviceHeadStruct extDevHead[eExtDevNum][EXT_DEV_MAX_NUM];
	UartExtSetDataStruct setData[eUartNum];
	SpdsSetStruct spdsSet[SPDS_MAX_NUM];
	ThsSetStruct thsSet[THS_MAX_NUM];	//wanghf20181031
	FanCtlSetStruct fanCtlSet[FANCTL_MAX_NUM];
	MeterSetStruct meterSet[METER_MAX_NUM];//lcl20201110
	UpsHeadStruct upsHead[UPS_MAX_NUM];
	AcHeadStruct acHead[AC_MAX_NUM];
	//uint16 cabBoxNum;//lcl20210311,接线盒个数（与机柜数区分开）
};

struct MqttSetStruct
{
    uint16 enable; //default 0:disable; 1:enable
    uint16 serverNum;	//default: 1, range(1~8)
    uint16 currServerIdx;	//default: 0(invt), range(0~7)
    MqttSetDataStruct setData[MQTT_MAX_NUM];
};


//wanghf20190108
enum CabAlmCfgEnum
{
    eCabAlmEmerFanRunSet = 0,
	eCabAlmFrontOvTmpSet,
	eCabAlmFrontLoTmpSet,
	eCabAlmFrontOvHumiSet,
	eCabAlmFrontLoHumiSet,
	eCabAlmRearOvTmpSet,
	eCabAlmRearLoTmpSet,
	eCabAlmRearOvHumiSet,
    eCabAlmRearLoHumiSet,
	eCabAlmSdCardExitSet,
	eCabAlmSdCapLessSet,
	eCabAlmMemCapLessSet,
	eCabAlmDataSaveFailSet,
	eCabAlmFireAlarmSet,
	eCabAlmProjDoorSet,
    eCabAlmUtilityAbnormalSet,	//wanghf20190925

	eCabAlmCfgNum
};

enum UpsAlmCfgEnum
{
	eUpsAlmComFailSet = 0,
	eUpsAlmMaintCbClosedSet,
	eUpsAlmBattDiscntedSet,
	eUpsAlmBattTestFailSet,
	eUpsAlmBattMaintFailSet,
	eUpsAlmEpoSet,
	eUpsAlmFanFailSet,
	eUpsAlmTmpFailSet,
	eUpsAlmRecFailSet,
	eUpsAlmIpFailSet,
	eUpsAlmBypFailSet,
	eUpsAlmBypVoltFailSet,
	eUpsAlmBypUntrackSet,
	eUpsAlmBypOvLdSet,
	eUpsAlmBypOvLdToutSet,
	eUpsAlmBypSeqFailSet,
	eUpsAlmInvFailSet,
	eUpsAlmInvOvLdSet,
	eUpsAlmInvOvLdToutSet,
	eUpsAlmOpShortedSet,
	eUpsAlmBattVoltLowSet,
	eUpsAlmEodSet,
	eUpsAlmBattReverseSet,
	eUpsAlmOnInhibitedSet,
	eUpsAlmManualTxBypSet,

	eUpsAlmCfgNum
};

//wanghf20190121, @todo...
enum AcAlmCfgEnum
{
    eAcAlmComFailSet = 0,
    eAcAlmIS_SYNT_ALM,
    eAcAlmIS_IT_OH_ALM,
    eAcAlmIS_IT_OL_ALM,
    eAcAlmIS_IH_OH_ALM,
    eAcAlmIS_IH_OL_ALM,
    eAcAlmIS_OT_OH_ALM,
    eAcAlmIS_OT_OL_ALM,
    eAcAlmIS_SMK_ALM,
    eAcAlmIS_FSD_ALM,
    eAcAlmIS_SYSDW_ALM,
    eAcAlmIS_IPW_A_VOH_ALM,
    eAcAlmIS_IPW_B_VOH_ALM,
    eAcAlmIS_IPW_C_VOH_ALM,
    eAcAlmIS_IPW_A_VOL_ALM,
    eAcAlmIS_IPW_B_VOL_ALM,
    eAcAlmIS_IPW_C_VOL_ALM,
    eAcAlmIS_IPW_PHUB_ALM,
    eAcAlmIS_IPW_FOL_ALM,
    eAcAlmIS_IPW_FOH_ALM,
    eAcAlmIS_IPW_A_PHL_ALM,
    eAcAlmIS_IPW_B_PHL_ALM,
    eAcAlmIS_IPW_C_PHL_ALM,
    eAcAlmIS_IPW_PHSE_ALM,
    eAcAlmIS_EFAN1_ALM,
    eAcAlmIS_EFAN2_ALM,
    eAcAlmIS_EFAN3_ALM,
    eAcAlmIS_AF_LOST_ALM,
    eAcAlmIS_EFAN_FLT_ALM,
    eAcAlmIS_CFAN_ALM,
    eAcAlmIS_CMPR_HP_ALM,
    eAcAlmIS_CMPR_HPLK_ALM,
    eAcAlmIS_CMPR_LP_ALM,
    eAcAlmIS_CMPR_LPLK_ALM,
    eAcAlmIS_CMPR_EXSU_HT_ALM,
    eAcAlmIS_CMPR_EXSU_HTLK_ALM,
    eAcAlmIS_CMPR_F_ABN,
    eAcAlmIS_CMPR_F_ABNLK,
    eAcAlmIS_CMPR_HP_LMTF,
    eAcAlmIS_VFD_COMM_FLT,
    eAcAlmIS_VFD_AVOI,
    eAcAlmIS_VFD_DVOI,
    eAcAlmIS_VFD_SVOI,
    eAcAlmIS_VFD_BAD_FLT,
    eAcAlmIS_VFD_AVOV,
    eAcAlmIS_VFD_DVOV,
    eAcAlmIS_VFD_SVOV,
    eAcAlmIS_VFD_BUS_LV,
    eAcAlmIS_VFD_MOL,
    eAcAlmIS_VFD_OL,
    eAcAlmIS_VFD_SPI,
    eAcAlmIS_VFD_SPO,
    eAcAlmIS_VFD_REV_OT,
    eAcAlmIS_VFD_INV_OT,
    eAcAlmIS_VFD_485_FLT,
    eAcAlmIS_VFD_IDTT_FLT,
    eAcAlmIS_VFD_EEPROM_FLT,
    eAcAlmIS_VFD_ETH1,
    eAcAlmIS_VFD_ETH2,
    eAcAlmIS_VFD_DEU,
    eAcAlmIS_VFD_STO,
    eAcAlmIS_VFD_LL,
    eAcAlmIS_HUMI_ALM,
    eAcAlmIS_HUMI_AHI_DTT_ALM,
    eAcAlmIS_HUMI_DWV_ALM,
    eAcAlmIS_HUMI_WFLWL_ALM,
    eAcAlmIS_HUMI_WLS_ALM,
    eAcAlmIS_HUMI_CWHWL_ALM,
    eAcAlmIS_HEAT_ALM,
    eAcAlmIS_HEAT_PTT_ALM,
    eAcAlmIS_HEAT1_ALM,
    eAcAlmIS_HEAT2_ALM,
    eAcAlmIS_ITSD1_FLT,
    eAcAlmIS_ITSD2_FLT,
    eAcAlmIS_IHSD1_FLT,
    eAcAlmIS_OTSD1_FLT,
    eAcAlmIS_OTSD2_FLT,
    eAcAlmIS_OTSD3_FLT,
    eAcAlmIS_OHSD1_FLT,
    eAcAlmIS_EXSU_TSD_FLT,
    eAcAlmIS_SUCT_TSD_FLT,
    eAcAlmIS_SUCT_PSD_FLT,
    eAcAlmIS_EXSU_PSD_FLT,
    eAcAlmIS_LEWYSD_FLT,
    eAcAlmIS_RTHSD0_FLT,
    eAcAlmIS_RTHSD1_FLT,
    eAcAlmIS_RTHSD2_FLT,
    eAcAlmIS_RTHSD3_FLT,
    eAcAlmIS_RTHSD4_FLT,
    eAcAlmIS_RTHSD5_FLT,
    eAcAlmIS_RTHSD6_FLT,
    eAcAlmIS_RTHSD7_FLT,
    eAcAlmIS_RTHSD8_FLT,
    eAcAlmIS_RTHSD9_FLT,
    eAcAlmIS_RTHSD10_FLT,
    eAcAlmIS_RTHSD11_FLT,
    eAcAlmIS_RTHSD12_FLT,
    eAcAlmIS_RTHSD13_FLT,
    eAcAlmIS_RTHSD14_FLT,
    eAcAlmIS_RTHSD15_FLT,
    eAcAlmIS_NET_COMM_FLT,
    eAcAlmIS_NET_ADDR_CNF,
    eAcAlmIS_NET_SFVER_ERR,
    eAcAlmIS_NET_S00_OFFLN,
    eAcAlmIS_NET_S01_OFFLN,
    eAcAlmIS_NET_S02_OFFLN,
    eAcAlmIS_NET_S03_OFFLN,
    eAcAlmIS_NET_S04_OFFLN,
    eAcAlmIS_NET_S05_OFFLN,
    eAcAlmIS_NET_S06_OFFLN,
    eAcAlmIS_NET_S07_OFFLN,
    eAcAlmIS_NET_S08_OFFLN,
    eAcAlmIS_NET_S09_OFFLN,
    eAcAlmIS_NET_S10_OFFLN,
    eAcAlmIS_NET_S11_OFFLN,
    eAcAlmIS_NET_S12_OFFLN,
    eAcAlmIS_NET_S13_OFFLN,
    eAcAlmIS_NET_S14_OFFLN,
    eAcAlmIS_NET_S15_OFFLN,
    eAcAlmIS_NET_S16_OFFLN,
    eAcAlmIS_NET_S17_OFFLN,
    eAcAlmIS_NET_S18_OFFLN,
    eAcAlmIS_NET_S19_OFFLN,
    eAcAlmIS_NET_S20_OFFLN,
    eAcAlmIS_NET_S21_OFFLN,
    eAcAlmIS_NET_S22_OFFLN,
    eAcAlmIS_NET_S23_OFFLN,
    eAcAlmIS_NET_S24_OFFLN,
    eAcAlmIS_NET_S25_OFFLN,
    eAcAlmIS_NET_S26_OFFLN,
    eAcAlmIS_NET_S27_OFFLN,
    eAcAlmIS_NET_S28_OFFLN,
    eAcAlmIS_NET_S29_OFFLN,
    eAcAlmIS_NET_S30_OFFLN,
    eAcAlmIS_NET_S31_OFFLN,
    eAcAlmIS_NET_S32_OFFLN,
    eAcAlmIS_NET_S33_OFFLN,
    eAcAlmIS_NET_S34_OFFLN,
    eAcAlmIS_NET_S35_OFFLN,
    eAcAlmIS_NET_S36_OFFLN,
    eAcAlmIS_NET_S37_OFFLN,
    eAcAlmIS_NET_S38_OFFLN,
    eAcAlmIS_NET_S39_OFFLN,
    eAcAlmIS_NET_S40_OFFLN,
    eAcAlmIS_NET_S41_OFFLN,
    eAcAlmIS_NET_S42_OFFLN,
    eAcAlmIS_NET_S43_OFFLN,
    eAcAlmIS_NET_S44_OFFLN,
    eAcAlmIS_NET_S45_OFFLN,
    eAcAlmIS_NET_S46_OFFLN,
    eAcAlmIS_NET_S47_OFFLN,
    eAcAlmIS_NET_S48_OFFLN,
    eAcAlmIS_NET_S49_OFFLN,
    eAcAlmIS_NET_S50_OFFLN,
    eAcAlmIS_NET_S51_OFFLN,
    eAcAlmIS_NET_S52_OFFLN,
    eAcAlmIS_NET_S53_OFFLN,
    eAcAlmIS_NET_S54_OFFLN,
    eAcAlmIS_NET_S55_OFFLN,
    eAcAlmIS_NET_S56_OFFLN,
    eAcAlmIS_NET_S57_OFFLN,
    eAcAlmIS_NET_S58_OFFLN,
    eAcAlmIS_NET_S59_OFFLN,
    eAcAlmIS_NET_S60_OFFLN,
    eAcAlmIS_NET_S61_OFFLN,
    eAcAlmIS_NET_S62_OFFLN,
    eAcAlmIS_NET_S63_OFFLN,
    eAcAlmIS_DI_EFAN_OL_1,
    eAcAlmIS_DI_EFAN_OL_2,
    eAcAlmIS_DI_EFAN_OL_3,
    eAcAlmIS_DI_WFH_LWL,
    eAcAlmIS_DI_CL_HWL,
    eAcAlmIS_DI_HEAT_OL,
    eAcAlmIS_DI_CFAN_ALM,
    eAcAlmIS_DI_AF_LS,
    eAcAlmIS_DI_FLT_BLK,
    eAcAlmIS_DI_SMK_ALM,

    eAcAlmCfgNum
};

enum ThsAlmCfgEnum
{
	eThsAlmComFailSet = 0,
	eThsAlmOvTmpSet,
	eThsAlmLoTmpSet,
	eThsAlmOvHumiSet,
	eThsAlmLoHumiSet,

	eThsAlmCfgNum
};

enum PduAlmIdxEnum
{
	ePduAlmComFailSet = 0,
	ePduAlmTotalCurrOvLmtSet,
	ePduAlmTotalCurrUnderLmtSet,
	ePduAlmVoltOvLmtB1Set,
	ePduAlmVoltUnderLmtB1Set,
	ePduAlmCurrOvLmtB1Set,
	ePduAlmCurrUnderLmtB1Set,
	ePduAlmVoltOvLmtB2Set,
	ePduAlmVoltUnderLmtB2Set,
	ePduAlmCurrOvLmtB2Set,
	ePduAlmCurrUnderLmtB2Set,
	ePduAlmVoltOvLmtB3Set,
	ePduAlmVoltUnderLmtB3Set,
	ePduAlmCurrOvLmtB3Set,
	ePduAlmCurrUnderLmtB3Set,

	ePduAlmCfgNum
};

enum WlsAlmCfgEnum
{
	eWlsAlmComFailSet = 0,
	eWlsAlmAlmSet,
	eWlsAlmCableFailSet,

	eWlsAlmCfgNum
};

enum SmsAlmCfgEnum
{
	eSmsAlmComFailSet = 0,

	eSmsAlmCfgNum
};

enum ItPdcAlmCfgEnum
{
	eItPdcAlmComFailSet = 0,
	eItPdcAlmCurrOvLmtB1Set,
	eItPdcAlmCurrUnderLmtB1Set,
	eItPdcAlmCurrOvLmtB2Set,
	eItPdcAlmCurrUnderLmtB2Set,
	eItPdcAlmCurrOvLmtB3Set,
	eItPdcAlmCurrUnderLmtB3Set,
	eItPdcAlmCurrOvLmtB4Set,
	eItPdcAlmCurrUnderLmtB4Set,
	eItPdcAlmCurrOvLmtB5Set,
	eItPdcAlmCurrUnderLmtB5Set,
	eItPdcAlmCurrOvLmtB6Set,
	eItPdcAlmCurrUnderLmtB6Set,
	eItPdcAlmCurrOvLmtB7Set,
	eItPdcAlmCurrUnderLmtB7Set,
	eItPdcAlmCurrOvLmtB8Set,
	eItPdcAlmCurrUnderLmtB8Set,
	eItPdcAlmCurrOvLmtB9Set,
	eItPdcAlmCurrUnderLmtB9Set,
	eItPdcAlmCurrOvLmtB10Set,
	eItPdcAlmCurrUnderLmtB10Set,
	eItPdcAlmCurrOvLmtB11Set,
	eItPdcAlmCurrUnderLmtB11Set,
	eItPdcAlmCurrOvLmtB12Set,
	eItPdcAlmCurrUnderLmtB12Set,
	eItPdcAlmCurrOvLmtB13Set,
	eItPdcAlmCurrUnderLmtB13Set,
	eItPdcAlmCurrOvLmtB14Set,
	eItPdcAlmCurrUnderLmtB14Set,
	eItPdcAlmCurrOvLmtB15Set,
	eItPdcAlmCurrUnderLmtB15Set,
	eItPdcAlmCurrOvLmtB16Set,
	eItPdcAlmCurrUnderLmtB16Set,
	eItPdcAlmCurrOvLmtB17Set,
	eItPdcAlmCurrUnderLmtB17Set,
	eItPdcAlmCurrOvLmtB18Set,
	eItPdcAlmCurrUnderLmtB18Set,
	eItPdcAlmCurrOvLmtB19Set,
	eItPdcAlmCurrUnderLmtB19Set,
	eItPdcAlmCurrOvLmtB20Set,
	eItPdcAlmCurrUnderLmtB20Set,
	eItPdcAlmCurrOvLmtB21Set,
	eItPdcAlmCurrUnderLmtB21Set,
	eItPdcAlmCurrOvLmtB22Set,
	eItPdcAlmCurrUnderLmtB22Set,
	eItPdcAlmCurrOvLmtB23Set,
	eItPdcAlmCurrUnderLmtB23Set,
	eItPdcAlmCurrOvLmtB24Set,
	eItPdcAlmCurrUnderLmtB24Set,
	eItPdcAlmCurrOvLmtB25Set,
	eItPdcAlmCurrUnderLmtB25Set,
	eItPdcAlmCurrOvLmtB26Set,
	eItPdcAlmCurrUnderLmtB26Set,
	eItPdcAlmCurrOvLmtB27Set,
	eItPdcAlmCurrUnderLmtB27Set,
	eItPdcAlmCurrOvLmtB28Set,
	eItPdcAlmCurrUnderLmtB28Set,
	eItPdcAlmCurrOvLmtB29Set,
	eItPdcAlmCurrUnderLmtB29Set,
	eItPdcAlmCurrOvLmtB30Set,
	eItPdcAlmCurrUnderLmtB30Set,

	eItPdcAlmCfgNum
};

enum MeterAlmCfgEnum
{
	eMeterAlmComFailSet = 0,

	eMeterAlmCfgNum
};

enum SpdsAlmCfgEnum
{
	eSpdsAlmComFailSet = 0,
	eSpdsAlmCompAlmSet,

	eSpdsAlmCfgNum
};

enum DcAlmCfgEnum
{
	eDcAlmComFailSet = 0,
    eDcAlmFrontDoorOpenSet,
    eDcAlmBackDoorOpenSet,
    eDcAlmSmokeAlarmSet,
    eDcAlmIrAlarmSet,
    eDcAlmIrTamperSet,
    eDcAlmLeakAlarmSet,
    eDcAlmFilterBlockSet,
    eDcAlmDoorSensorSet,
    eDcAlmDoorOpenToutSet,
    eDcAlmIllegalOpen,

	eDcAlmCfgNum
};

enum BmsAlmCfgEnum
{
	eBmsAlmComFailSet = 0,
	eBmsAlmBattGroup1AlmSet,
	eBmsAlmBattGroup2AlmSet,
	eBmsAlmBattGroup3AlmSet,
	eBmsAlmBattGroup4AlmSet,

	eBmsAlmCfgNum
};

enum AtsAlmCfgEnum
{
	eAtsAlmComFailSet = 0,
	eAtsAlmCompAlmSet,

	eAtsAlmCfgNum
};

//wanghf20190507
enum FanAlmCfgEnum
{
	eFanAlmComFailSet = 0,
	eFanAlmOvTmpSet,
	eFanAlmOvHumiSet,
	eFanAlmFilterAlmSet,

	eFanAlmCfgNum
};

//wanghf20190925
enum FanCtlAlmCfgEnum
{
	eFanCtlAlmComFailSet = 0,
	eFanCtlAlmFanCtlFailSet,

	eFanCtlAlmCfgNum
};

//wanghf20210218
enum CtrlBoxAlmCfgEnum
{
	eCtrlBoxAlmComFailSet = 0,
	eCtrlBoxAlmEmergBtnAlmSet,
	eCtrlBoxAlmSkylightOpenSet,
	eCtrlBoxAlmIrAlmSet,

	eCtrlBoxAlmCfgNum
};

struct AlmParaStruct
{
	uint16 almId;
	uint16 level;		//0: disable, 1: general alm, 2: fault
	uint16 audVisEn;	//0: Disable; 1:Enable
};

struct AlmMngStruct
{
	AlmParaStruct cabAlm[eCabAlmCfgNum];
	AlmParaStruct upsAlm[eUpsAlmCfgNum];
	AlmParaStruct acAlm[eAcAlmCfgNum];
	AlmParaStruct thsAlm[eThsAlmCfgNum];
	AlmParaStruct pduAlm[ePduAlmCfgNum];
	AlmParaStruct wlsAlm[eWlsAlmCfgNum];
	AlmParaStruct smsAlm[eSmsAlmCfgNum];
	AlmParaStruct itPdcAlm[eItPdcAlmCfgNum];
	AlmParaStruct meterAlm[eMeterAlmCfgNum];
	AlmParaStruct spdsAlm[eSpdsAlmCfgNum];
	AlmParaStruct dcAlm[eDcAlmCfgNum];
	AlmParaStruct bmsAlm[eBmsAlmCfgNum];
	AlmParaStruct atsAlm[eAtsAlmCfgNum];
	AlmParaStruct fanAlm[eFanAlmCfgNum];	//wanghf20190507
	AlmParaStruct fanCtlAlm[eFanCtlAlmCfgNum];	//wanghf20190925
	AlmParaStruct ctrlBoxAlm[eCtrlBoxAlmCfgNum];	//wanghf20210218
};

struct CfgChgedFlagStruct
{
	// 1: changed
	//----- for mainProc -----
	uint16 eMtrMain;
	uint16 uartExt;
	uint16 mqtt;
	uint16 sysInfo;
	uint16 customInfo;	//wanghf20180702
	uint16 almMng;		//wanghf20190108

	uint16 rsvd[6];
};


struct SysCfgMngStruct
{
	EMtrMainSetStruct eMtrMain;
	UartExtSetStruct uartExt;
	MqttSetStruct mqtt;
	SysInfoStruct sysInfo;
	CustomInfoStruct customInfo;	//wanghf20180702
	AlmMngStruct almMng;	//wanghf20190108

	CfgChgedFlagStruct cfgChgedFlag;
};

//wanghf20170206, only used for mainProc
struct VerStruct
{
	uint16 hwVer;
	uint16 osVer;
	uint16 eMtrSwVerHi;
	uint16 eMtrSwVerLo;
};

//-----------------------------------------------------------------------------
//Adding Private Function Prototypes (i.e. static)


//-----------------------------------------------------------------------------
//Adding Public Function Prototypes

void CFG_Init(void);
void CFG_Exit(void);


//-----------------------------------------------------------------------------
//Adding Private Variables (i.e. static)



//-----------------------------------------------------------------------------
//Adding Public Variables Stuff

SYSCFGMNG_PUBLIC ShareMemory sysCfgShare;
SYSCFGMNG_PUBLIC SysCfgMngStruct *sysCfg;
SYSCFGMNG_PUBLIC int16 SysCfg_needSyncFanAddr;
SYSCFGMNG_PUBLIC VerStruct SysCfg_verStru;
SYSCFGMNG_PUBLIC uint16 DevMaxNum[eExtDevNum];

#include "eMtrDataStruct.h"

SYSCFGMNG_PUBLIC ShareMemory eMtrDataShare;
SYSCFGMNG_PUBLIC struct eMtrDataStruct *eMtrData;

#include "ThsData/ThsDataStruct.h"

SYSCFGMNG_PUBLIC ShareMemory thsDataShare;
SYSCFGMNG_PUBLIC ThsDataStruct* pThsData;

#include "ACData/ACDataStruct.h"

SYSCFGMNG_PUBLIC ShareMemory acDataShare;
SYSCFGMNG_PUBLIC AcDataStruct* pAcData;

#include "UPSData/UpsDataStruct.h"
SYSCFGMNG_PUBLIC ShareMemory upsDataShare;
SYSCFGMNG_PUBLIC UpsDataStruct *pUpsData;

#include "MeterData/MeterDataStruct.h"
SYSCFGMNG_PUBLIC ShareMemory meterDataShare;
SYSCFGMNG_PUBLIC MeterDataStruct *pMeterData;

#include "LeakData/LeakDataStruct.h"
SYSCFGMNG_PUBLIC ShareMemory leakDataShare;
SYSCFGMNG_PUBLIC LeakDataStruct *pLeakData;

#include "DCData/CabCtrlDataStructExt.h"
SYSCFGMNG_PUBLIC ShareMemory cabCtrDataShare;
SYSCFGMNG_PUBLIC CabCtrlDataStruct *pCabCtrlData;

#include "LcdData/LcdDataStruct.h"
SYSCFGMNG_PUBLIC ShareMemory lcdDataShare;
SYSCFGMNG_PUBLIC LcdDataStruct *pLcdData;

#include "SpdsData/SpdsDataStruct.h"
SYSCFGMNG_PUBLIC ShareMemory spdsDataShare;
SYSCFGMNG_PUBLIC SpdsDataStruct *pSpdsData;

#include "SmsData/SmsDataStructExt.h"
SYSCFGMNG_PUBLIC ShareMemory smsDataShare;
SYSCFGMNG_PUBLIC SmsDataStruct *pSmsData;

#include "BMSData/BMSDataStructExt.h"
SYSCFGMNG_PUBLIC ShareMemory bmsDataShare;
SYSCFGMNG_PUBLIC BmsDataStruct *pBmsData;

//-----------------------------------------------------------------------------
//Adding Private Function 


//-----------------------------------------------------------------------------
//Adding Public Function 

/******************************************************************************
Function Name: LcdTaskMng
--------------------
Function Descriptions:

Parameter Name list:

ReturnType: 

Refer documents:
******************************************************************************/




#endif // SYSCFGMNG_H

