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

#ifndef WGTPARAITEM_METER_H
#define WGTPARAITEM_METER_H

#include <QWidget>
#include <QScrollBar>
#include "loginmanage.h"
#include "Global/SysCfgMng.h"
#include <QMetaEnum>
#include "wgtparaitem_ups.h"

#define  METER_PARA   0

namespace Ui {
class wgtParaItem_meter;
}

//配电
class wgtDateTime;
class ActParaItem;
class wgtParaItem_meter : public QWidget
{
    Q_OBJECT

public:
    enum MeterParaEnum
    {
        eMeterParaCurrA, //coeff: 0.1; unit: A; uint16
        eMeterParaCurrB, //coeff: 0.1; unit: A; uint16
        eMeterParaCurrC, //coeff: 0.1; unit: A; uint16
        eMeterParaCurrN, //coeff: 0.1; unit: A; uint16
        eMeterParaMaxCurrA, //coeff: 0.1; unit: A; uint16
        eMeterParaMaxCurrB, //coeff: 0.1; unit: A; uint16
        eMeterParaMaxCurrC, //coeff: 0.1; unit: A; uint16
        eMeterParaMaxCurrN, //coeff: 0.1; unit: A; uint16
        eMeterParaVoltAB, //coeff: 0.1; unit: V; uint16
        eMeterParaVoltBC, //coeff: 0.1; unit: V; uint16
        eMeterParaVoltCA, //coeff: 0.1; unit: V; uint16
        eMeterParaVoltA, //coeff: 0.1; unit: V; uint16
        eMeterParaVoltB, //coeff: 0.1; unit: V; uint16
        eMeterParaVoltC, //coeff: 0.1; unit: V; uint16
        eMeterParaFreq, //coeff: 0.01; unit: Hz; uint16
        eMeterParaKwAll, //coeff: 0.1; unit: Kw; int16
        eMeterParaMaxKwAll, //coeff: 0.1; unit: Kw; int16
        eMeterParaKwA, //coeff: 0.1; unit: Kw; int16
        eMeterParaKwB, //coeff: 0.1; unit: Kw; int16
        eMeterParaKwC, //coeff: 0.1; unit: Kw; int16
        eMeterParaKVarAll, //coeff: 0.1; unit: KVar; int16
        eMeterParaMaxKVarAll, //coeff: 0.1; unit: KVar; int16
        eMeterParaKVarA, //coeff: 0.1; unit: KVar; int16
        eMeterParaKVarB, //coeff: 0.1; unit: KVar; int16
        eMeterParaKVarC, //coeff: 0.1; unit: KVar; int16
        eMeterParaKVAAll, //coeff: 0.1; unit: KVA; uint16
        eMeterParaMaxKVAAll, //coeff: 0.1; unit: KVA; uint16
        eMeterParaKvaA, //coeff: 0.1; unit: KVA; uint16
        eMeterParaKvaB, //coeff: 0.1; unit: KVA; uint16
        eMeterParaKvaC, //coeff: 0.1; unit: KVA; uint16
        eMeterParaPfAll, //coeff: 0.01; unit: ; int16
        eMeterParaPfA, //coeff: 0.01; unit: ; int16
        eMeterParaPfB, //coeff: 0.01; unit: ; int16
        eMeterParaPfC, //coeff: 0.01; unit: ; int16
        eMeterParaKWH_High, //coeff: 1; unit: KWH; uint16
        eMeterParaKWH_Low, //coeff: 1; unit: KWH; uint16
        eMeterParaKVarH_High, //coeff: 1; unit: KvarH; uint16
        eMeterParaKVarH_Low, //coeff: 1; unit: KvarH; uint16
        eMeterParaVoltABThd, //coeff: 0.1; unit: %; uint16
        eMeterParaVoltBCThd, //coeff: 0.1; unit: %; uint16
        eMeterParaVoltCAThd, //coeff: 0.1; unit: %; uint16
        eMeterParaVoltAThd, //coeff: 0.1; unit: %; uint16
        eMeterParaVoltBThd, //coeff: 0.1; unit: %; uint16
        eMeterParaVoltCThd, //coeff: 0.1; unit: %; uint16
        eMeterParaCurrAThd, //coeff: 0.1; unit: %; uint16
        eMeterParaCurrBThd, //coeff: 0.1; unit: %; uint16
        eMeterParaCurrCThd, //coeff: 0.1; unit: %; uint16
        eMeterParaCurrNThd, //coeff: 0.1; unit: %; uint16

        eMeterParaNum
    };
    Q_ENUM(MeterParaEnum)

public:
    explicit wgtParaItem_meter(int id, QWidget *parent = nullptr);
    ~wgtParaItem_meter();

    void setCurrentId(int id);
protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

    //创建参数界面的所有控件
    void createParaWidget(void);
    void clearAllWideget(void);

    void createSetParaWidget(void);
    void clearSetWideget(void);
    void createInfo(void);

private slots:
    void getMeterInfo(QList<meter_Info> infoList);
    void onScrollBarChange(void);
private:
    void setupUi(void);
    void setConnectState(int state);

    //权限更新
    void permissUpdate(void);
    //使能指引按钮
    void setGuideBtntVisible();
    void setTabWidgetStyle(void);
private:
    Ui::wgtParaItem_meter *ui;
    //wgtDateTime *m_lblDate;
    int          m_curId;
    UPS_List     m_meterList;
    QList<meter_Info> meter_list;
    QList<ActParaItem *>itemList;
};

#endif // WGTPARAITEM_METER_H
