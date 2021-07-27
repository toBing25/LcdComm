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

#ifndef DATAMANAGE_H
#define DATAMANAGE_H
#include <QObject>
#include <QVector>
#include "language/languagePublic.h"
//历史数据界面下拉列表与这里顺序一致
enum DevDataEnum
{
    UPS,
    AC,
    THS,
    METER,
    SYSTEM,
    PUE
};

enum UpsDataEnum{
    ups_devNo,
    ups_ipVoltA,
    ups_ipVoltB,
    ups_ipVoltC,
    ups_ipFreqA,
    ups_ipFreqB,
    ups_ipFreqC,
    ups_opVoltA,
    ups_opVoltB,
    ups_opVoltC,
    ups_opCurrA,
    ups_opCurrB,
    ups_opCurrC,
    ups_opFreqA,
    ups_opFreqB,
    ups_opFreqC,
    ups_bypVoltA,
    ups_bypVoltB,
    ups_bypVoltC,
    ups_bypCurrA,
    ups_bypCurrB,
    ups_bypCurrC,
    ups_bypFreqA,
    ups_bypFreqB,
    ups_bypFreqC,
    ups_kvaA,
    ups_kvaB,
    ups_kvaC,
    ups_kwA,
    ups_kwB,
    ups_kwC,
    ups_loadA,
    ups_loadB,
    ups_loadC,
    ups_batVoltP,
    ups_batVoltM,
    ups_batCap,
    ups_batRemTime,
    ups_datetime,

    upsDataNum

};


enum AcDataEnum{
    ac_devNo,
    ac_inletTmp,
    ac_outletTmp,
    ac_datetime,

    acDataNum
};


enum MeterDataEnum{
    meter_devNo,
    meter_kwh,
    meter_lvoltUab,
    meter_lvoltUbc,
    meter_lvoltUca,
    meter_currA,
    meter_currB,
    meter_currC,
    meter_pfA,
    meter_pfB,
    meter_pfC,
    meter_freq,
    meter_kwA,
    meter_kwB,
    meter_kwC,
    meter_kvaA,
    meter_kvaB,
    meter_kvaC,
    meter_lTHDvUab,
    meter_lTHDvUbc,
    meter_lTHDvUca,
    meter_phTHDiA,
    meter_phTHDiB,
    meter_phTHDiC,
    meter_datetime,

    meterDataNum

};

enum StatsDataEnum{
    stats_tmpAvg  ,
    stats_frontTmp,
    stats_frontHum,
    stats_backTmp ,
    stats_backHum ,
    stats_ambTmp  ,
    stats_ambHum  ,
    stats_datetime,
    statsDataNum
};
enum PueDataEnum{
    pue_pue,
    pue_datetime,
    pueDataNum
};

enum ThsDataEnum{
    ths_devNo,
    ths_tmp,
    ths_hum,
    ths_datetime,
    thsDataNum
};

//-----------------------------------------------
typedef  struct STATS_INFO{
    QString data[statsDataNum];
}stats_data;

typedef  struct PUE_INFO{
    QString data[pueDataNum];
}pue_data;

typedef  struct THSData_INFO{
    QString data[thsDataNum];
}ths_data;

typedef  struct UPSData_INFO{
    QString data[upsDataNum];
}ups_data;

typedef  struct ACData_INFO{
    QString data[acDataNum];
}ac_data;

typedef struct METERData_INFO{
    QString data[meterDataNum];
}meter_data;

//------------------------------------------------
//方便传参
struct DevDataStruct{

};

struct thsDataStruct:DevDataStruct{
    QVector<ths_data> dataList;
    int pageCount;
};

struct upsDataStruct:DevDataStruct{
    QVector<ups_data> dataList;
    int pageCount;
};

struct acDataStruct:DevDataStruct{
    QVector<ac_data> dataList;
    int pageCount;
};

struct meterDataStruct:DevDataStruct{
    QVector<meter_data> dataList;
    int pageCount;
};


struct statsDataStruct:DevDataStruct{
    QVector<stats_data> dataList;
    int pageCount;
};
struct pueDataStruct:DevDataStruct{
    QVector<pue_data> dataList;
    int pageCount;
};


extern QList<QPair<langEnum,QString>> uDataList;
void uDataList_Init();
extern QList<QPair<langEnum,QString>> acDataList;
void acDataList_Init();
extern QList<QPair<langEnum,QString>> meterDataList;
void meterDataList_Init();
extern QList<QPair<langEnum,QString>> thsDataList;
void thsDataList_Init();
extern QList<QPair<langEnum,QString>> statsDataList;
void statsDataList_Init();
extern QList<QPair<langEnum,QString>> pueDataList;
void pueDataList_Init();



#endif // DATAMANAGE_H
