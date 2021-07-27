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

#ifndef CABCTRLDATASTRUCTEXT_H
#define CABCTRLDATASTRUCTEXT_H
#include "CabCtrlDataStruct.h"
#include <QVector>
typedef  struct DOORLISTSTRUCT{
     QVector<int>   door;
     int            cardID;
}door_Info;

struct DcCardDevVector{
    QVector<int> dcList;
    int          dcLen;
};

struct CardDb{
    QString cardNo;
    QString cardName;
    QStringList dcList;
};




#endif // CABCTRLDATASTRUCTEXT_H
