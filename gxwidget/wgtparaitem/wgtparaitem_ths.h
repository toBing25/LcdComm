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

#ifndef WGTPARAITEM_THS_H
#define WGTPARAITEM_THS_H

#include <QWidget>
#include "loginmanage.h"
#include <QMetaEnum>
#include "Global/SysCfgMng.h"
#include "ThsData/ThsDataStruct.h"
#include "wgtparaitem_ups.h"

#define  THS_PARA     0



namespace Ui {
class wgtParaItem_ths;
}

//温湿度
class wgtDateTime;
class wgtParaItem_ths : public QWidget
{
    Q_OBJECT

public:
    enum ThsParaEnum
    {
        eThsParaTmp, //temperature
        eThsParaHum, //humidity

        eThsParaAlmSts, //Bit0: integrated warning; Bit1: temperature over; Bit2: temperature low
                        //Bit3: humidity over; Bit4: humidity low
        eThsParaOutIO,

        eThsParaTmpUpper,
        eThsParaTmpLower,
        eThsParaTmpAlmDiff,

        eThsParaHumUpper,
        eThsParaHumLower,
        eThsParaHumAlmDiff,

        eThsParaNum
    };
    Q_ENUM(ThsParaEnum)

public:
    explicit wgtParaItem_ths(int id, QWidget *parent = nullptr);
    ~wgtParaItem_ths();

    void setCurrentId(int id);
protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void getThsInfo(QList<ths_Info> infoList);
    void onScrollBarChange(void);

private:
    void setupUi(void);
    void setConnectState(int state);

    //创建参数界面的所有控件
    void createParaWidget(void);
    void clearAllWideget(void);

    void createSetParaWidget(void);
    void clearSetWideget(void);
    void createInfo(void);

    //权限更新
    void permissUpdate(void);
    //使能指引按钮
    void setGuideBtntVisible();
    void setTabWidgetStyle(void);
private:
    Ui::wgtParaItem_ths *ui;
    //wgtDateTime *m_lblDate;
    int          m_curId;
    UPS_List     m_thsList;
    QList<Menu>  setList;
    QList<ths_Info> ths_list;
};

#endif // WGTPARAITEM_THS_H
