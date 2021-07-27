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

#ifndef WTRITESHAREMEM_H
#define WTRITESHAREMEM_H
#include "ThsData/ThsDataStruct.h"
#include "UPSData/UPSDataStruct.h"
#include "ACData/ACDataStruct.h"
#include "LeakData/LeakDataStruct.h"
#include "Global/eMtrDataStruct.h"
#include <QObject>

class WtriteShareMem : public QObject
{
    Q_OBJECT
public:
    explicit WtriteShareMem(QObject *parent = nullptr);
public slots:
    void writeDevData(int devType,int devNo,int door);
    void writeThsDevData(ths_set);
    void wirteUpsDevData(ups_set);
    void writeAcDevData(ac_set);
    void writeLeakDevData(leak_set);
    //告警消音
    //void writeeMtrCtrl(eMtrCtrlStruct);

signals:

};

#endif // WTRITESHAREMEM_H
