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

#ifndef BMSDATASTRUCTEXT_H
#define BMSDATASTRUCTEXT_H
#include "BMSDataStruct.h"
enum BMSTypeEnum{
    BMSSts = 0,
    BMSPara,
    BMSTypeNum
};

struct ExtValueTypeStruct{
    uint BMSGroupType[eBmsGroupNum];
    uint BMSCellType[eBmsCellNum];
};

typedef  struct BMS_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected
     int           almSts;

}bms_Info;

extern ExtValueTypeStruct extBMSType;

void BMSTypeInit();

#endif // BMSDATASTRUCTEXT_H
