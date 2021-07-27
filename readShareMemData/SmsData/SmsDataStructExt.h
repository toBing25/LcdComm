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

#ifndef SMSDATASTRUCTEXT_H
#define SMSDATASTRUCTEXT_H
#include "SmsDataStruct.h"

typedef  struct SMS_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected
     int           almSts;  //0正常，其他不正常
     QString cscaNo;
     uint16     signalLV; //0-5
}sms_Info;

#endif // SMSDATASTRUCTEXT_H
