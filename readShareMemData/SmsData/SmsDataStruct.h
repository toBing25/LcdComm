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

#ifndef SMSDATASTRUCT
#define SMSDATASTRUCT

//#include "../DevInterface.h"
#include "Global/GlobalPublic.h"
#include "Global/SysCfgMng.h"
enum SmsSendStsEnum
{
    eSmsSendStsIdle = 0,
    eSmsSendStsNeedSend,
    eSmsSendStsWaitRespond,
    eSmsSendStsWaitSend,
    eSmsSendStsNeedCall,
    eSmsSendStsWaitCall,
};

enum SmsSendErrEnum
{
    eSmsSendErrNone = 0,
    eSmsSendErrSendFail,
    eSmsSendErrNorespond
};

enum SmsTaskEnum
{
    eSmsTaskIdel,
    eSmsTaskSendAT,
    eSmsTaskWaitAT,
    eSmsTaskSendCSQ,
    eSmsTaskWaitCSQ,
    eSmsTaskSendGetCsca,
    eSmsTaskWaitGetCsca,
    eSmsTaskSendSetCsca,
    eSmsTaskWaitSetCsca,
    eSmsTaskSendCMGF,
    eSmsTaskWaitCMGF,
    eSmsTaskSendCMGS,
    eSmsTaskWaitCMGS,
    eSmsTaskSendData,
    eSmsTaskWaitData,
    eSmsTaskWaitSendEnd,
    eSmsTaskSendATD,
    eSmsTaskWaitATD,
    eSmsTaskWaitATDEnd,
};

struct SmsSendStruct
{
    uint16 sendSts; //0:idle; 1: need send; 2: wait respond; 3: wait send; 4: sending
    uint16 call; //0: disable; 1:enable;

    //Only use low 8-bit, the value is ascii of phone number.
    //e.g.: 8613712345678
    //send[0] -----------------------------------------------------> send[12]
    //0x38 0x36 0x31 0x33 0x37 0x31 0x32 0x32 0x34 0x35 0x36 0x37 0x38
    uint16 senderLen;
    uint16 sender[SMS_SENDER_MAX_LEN];

    uint16 sendDataBytes;//max:SMS_SEND_DATA_MAX_LEN*2
    uint16 sendBuffer[SMS_SEND_DATA_MAX_LEN];
    uint16 sendErr;
};

struct SmsCscaStruct
{
    uint16 valLen;
    uint16 value[SMS_CSCA_MAX_LEN]; //8613800755500
    uint16 setLen;
    uint16 set[SMS_CSCA_MAX_LEN]; //8613800755500
    uint16 changed; //1:changed
};

struct SmsDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    uint16 signalValue; //0-99

    SmsCscaStruct csca;

    SmsSendStruct send[SMS_SEND_BUF_MAX_NUM];
};

struct SmsLocalFlagBitStruct
{
    uint16 atFalshed:1; 	//Bit0
    uint16 csqFalshed:1; 	//Bit1
    uint16 getCscaFlashed:1; //Bit2
    uint16 cmgfSeted:1; //Bit3

    uint16 rsvd:12;	//Bit3-Bit15
};

union SmsLocalFlagBitUnion
{
    uint16 all;
    SmsLocalFlagBitStruct bit;
};

struct SmsLocalDataStruct
{
    uint32 disconnctTimer;
    uint16 noRespTimes;
    uint16 buffPointer;
    uint16 sendWaitTimer[SMS_SEND_BUF_MAX_NUM];
    SmsLocalFlagBitUnion flag;
    uint16 flashTime;
    uint16 waitTime;
    SmsTaskEnum currTask;
};

//extern SmsDataStruct *pSmsData;
//extern SmsLocalDataStruct *pSmsLocalData;

//void CreatSmsShareMemery(void);
//void AddSmsPollTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void CheckSmsActiveTask(UartIdEnum uart, SerialTaskSchedule *serial);
//void SmsConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void SmsUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);
//void SmsInitData(uint8 smsId);

#endif // SMSDATASTRUCT

