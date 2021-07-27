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

#ifndef WGTPAREITEM_UPS_H
#define WGTPAREITEM_UPS_H

#include <QWidget>
#include <QMetaEnum>
#include "wgtkeypad.h"
#include "loginmanage.h"
#include "Global/SysCfgMng.h"

typedef  struct UPS_PARA{
     int         para_type;   //参数类别
     QList<Menu> menuList;    //参数列表
}ups_list;

typedef QList<ups_list> UPS_List;

namespace Ui {
class wgtParaItem_ups;
}

//UPS
class wgtDateTime;
class wgtParaItem_ups : public QWidget
{
    Q_OBJECT

public:
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

//    enum UpsInfString
//    {
//        manufacturer, //20 unicode, end with 0x0000;
//        MtrVer,  //20 unicode, end with 0x0000;
//        series,  //20 unicode, end with 0x0000;
//        modelName, //20 unicode, end with 0x0000;
//    };

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

    Q_ENUM(UpsStsEnum)
    Q_ENUM(UpsParaEnum)
    Q_ENUM(UpsCfgEnum)
    //Q_ENUM(UpsInfString)
    Q_ENUM(UpsCtlEnum)

public:
    explicit wgtParaItem_ups(int id, QWidget *parent = nullptr);
    ~wgtParaItem_ups();

    void setCurrentId(int id);
protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void getUpsInfo(QList<ups_Info> infoList);
    void onScrollBarChange(void);

private:
    void setupUi(void);
    void setConnectState(int state);

    void clearAllWideget(void);
    //创建参数界面的所有控件
    void createParaWidget(void);

    void createSetParaWidget(void);
    void clearSetWideget(void);

    void createInfo(void);
    //匹配参数获取参数值
    int findParaValue(QString para,int &Type/*out*/);

    //权限更新
    void  permissUpdate(void);
    //使能指引按钮
    void  setGuideBtntVisible();

    void setTabWidgetStyle(void);
private:
    Ui::wgtParaItem_ups *ui;
    //wgtDateTime *m_lblDate;
    int          m_curId;
    UPS_List     m_upsList;
    QList<Menu>  setList;
    QList<ups_Info> Ups_list;
};

#endif // WGTPAREITEM_UPS_H
