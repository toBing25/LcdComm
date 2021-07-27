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

#ifndef ALARMMNG_H
#define ALARMMNG_H
#include <QMap>

enum ExtDevAlmEnum
{
    eAlmDevCap,   //机柜
    eAlmDevUps, //把UPS和空调放在枚举最前面 注意对应关系
    eAlmDevAc,  //把UPS和空调放在枚举最前面 注意对应关系
    eAlmDevThs,
    eAlmDevPdu,
    eAlmDevLeak,
    eAlmDevSms,
    eAlmDevItPdc,
    eAlmDevMeter,
    eAlmDevSpds,
    eAlmDevDc, //dry contact
    eAlmDevBMS,
    eAlmDevAts,
    eAlmDevFan,
    eAlmDevShow,
    eAlmDevFanCtl,
    eAlmDevCtrlBox,//2021


    eAlmDevNum
};



struct AlmDevTypeStruct
{
    QMap<int,QString> AlmDevType;


};
struct ContentAndStatus
{
    QString content;
    bool    status;
};

struct AlmDevLogStruct
{

    QMap<int,ContentAndStatus> AlmDevLogID;
};

struct AlmlevelStruct{
    QMap<int,QString> AlmLevel;
};


extern AlmlevelStruct   almLevelString;
extern AlmDevTypeStruct almDevTypeString;
extern AlmDevLogStruct  almDevLogString[eAlmDevNum];

void AlarmMngInit();
QString AlarmLogIDtoString(qint16 logid,ExtDevAlmEnum devName);
QString AlarmDevTypetoString(qint16 devtype);
#endif // ALARMMNG_H
