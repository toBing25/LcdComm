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

#ifndef RATIOSTRUCT_H
#define RATIOSTRUCT_H
#include "SysCfgMng.h"
#define acStsbitMax     2
//#define acParabitMax    8
#define upsStsbitMax    6
#define upsCfgbitMax    11
#define leakParabitMax  5
#define thsParabitMax   6
#define spdsBranchbitMax 2
#define bmsGroupbitMax  5
#define bmsCellbitMax   2
//空调
#define acStsbitMax 2
#define acParabitMax 13



struct ExtDevRatioStruct{
    //PUE系数
    double PUERatio;

    double eUpsRatio[eUpsParaNum];    
    //double eAcRatio[eAcParaNum];
    double eMeterRatio[eMeterParaNum];

    double eAcStsRatio[eAcStsNum];
    double eAcParaRatio[eAcParaNum];

    //带负数的int
    bool    eMeterIsInt16[eMeterParaNum];
    double eSpdsMainRatio[eSpdsMainNum];
    double eSpdsBranchRatio[eSpdsBranchNum];

    double eThsRatio[eThsParaNum];
    double eLeakRatio[eLeakParaNum];

    //BMS系数和单位
    double eBMSGroupRatio[eBmsGroupNum];
    double eBMSCellRatio[eBmsCellNum];
    QString eBMSGroupUnit[eBmsGroupNum];
    QString eBMSCellUnit[eBmsCellNum];

};



//显示中文
struct ExtDevShowStruct{
    //空调
    //QString eAcStsShow[eAcStsNum][acStsbitMax];
    QString eAcStsShow[eAcStsNum][acStsbitMax];
    QString eAcParaShow[3][acParabitMax]; //参数中的状态量

    QString eAcParaUnit[eAcParaNum];




    //UPS
    QString eUpsStsShow[eUpsStsNum][upsStsbitMax];
    QString eUpsParaUnit[eUpsParaNum];
    QString eUpsCfgShow[eUpsCfgNum][upsCfgbitMax];
    QString eUpsCfgUnit[eUpsCfgNum];
    //电量仪
    QString eMeterParaUnit[eMeterParaNum];
    //温湿度
    QString eThsParaShow[eThsParaNum][thsParabitMax];
    QString eThsParaUnit[eThsParaNum];
    //水浸
    QString eLeakParaShow[eLeakParaNum][leakParabitMax];
    QString eLeakParaUnit[eLeakParaNum];
    //spds
    QString eSpdsMainUnit[eSpdsMainNum];
    QString eSpdsBranchUnit[eSpdsBranchNum];
    QString eSpdsBranchShow[eSpdsBranchNum][spdsBranchbitMax];

    //BMS

    QString eBMSGroupShow[eBmsGroupNum][bmsGroupbitMax];

    QString eBMSCellShow[eBmsCellNum][bmsCellbitMax];


};

extern ExtDevRatioStruct extDevRatio;
extern ExtDevShowStruct  extDevShow;
void ExtDevRatioInit();
void ExtDevShowInit();
#endif // RATIOSTRUCT_H
