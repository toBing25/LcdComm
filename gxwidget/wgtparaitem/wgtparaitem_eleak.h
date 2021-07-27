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

#ifndef WGTPARAITEM_ELEAK_H
#define WGTPARAITEM_ELEAK_H

#include <QWidget>
#include <QScrollBar>
#include "loginmanage.h"
#include "Global/SysCfgMng.h"
#include "wgtparaitem_ups.h"

#define  LEAK_PARA   0

namespace Ui {
class wgtParaItem_eLeak;
}

//水浸
class wgtDateTime;
class wgtParaItem_eLeak : public QWidget
{
    Q_OBJECT

public:
    enum LeakParaEnum
    {
        eLeakParaLeakPos, //Leak position
        eLeakParaAlmSts,  //0: Normal; 1: Leak; 2: Cable break
                          //3: Cable  interference; 4: Cable too long
        eLeakParaCableLength,

        eLeakParaCableResist, //resistivity
        eLeakParaOutIO,
        eLeakParaAlmLevel, //0-4->level1-level5

        eLeakParaNum
    };
    Q_ENUM(LeakParaEnum)

public:
    explicit wgtParaItem_eLeak(int id, QWidget *parent = nullptr);
    ~wgtParaItem_eLeak();

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
    void getLeakInfo(QList<leak_Info> infoList);
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
    Ui::wgtParaItem_eLeak *ui;
    //wgtDateTime *m_lblDate;
    int          m_curId;
    UPS_List     m_leakList;
    QList<Menu>  statusList;
    QList<Menu>  setList;
    QList<leak_Info>  leak_list;
};

#endif // WGTPARAITEM_ELEAK_H
