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

#ifndef WGTPARAITEM_SPDS_H
#define WGTPARAITEM_SPDS_H

#include <QWidget>
#include <QVector>
#include <QScrollBar>
#include "loginmanage.h"
#include "Global/SysCfgMng.h"
#include <QMetaEnum>
#include "wgtparaitem_ups.h"
#include "actparaitem.h"
#include "actparawidget.h"

#define  METER_PARA   0


#define SPDS_MAIN     1
#define SPDS_BRANCH   2

typedef  struct SPDS_PARA{
     int         para_type;   //参数类别
     QList<Menu> menuList;    //参数列表
     QString     title;
     ActParaWidget *wgt;
}SPDS_list;

typedef QList<SPDS_list> spds_List;


typedef  struct SPDS_ITEM{
     QVector<ActParaItem *>  itemList;
     ActParaWidget           *wgt;
}spds_item;

typedef QVector<spds_item> spds_itemList;

namespace Ui {
class wgtParaItem_spds;
}

//配电
class wgtDateTime;
class wgtParaItem_spds : public QWidget
{
    Q_OBJECT

public:
    enum SpdsMainEnum
    {
        eSpdsMainVolt_AB, //0.1V
        eSpdsMainVolt_BC, //0.1V
        eSpdsMainVolt_CA, //0.1V
        eSpdsMainVolt_LN, //0.1V
        eSpdsMainVolt_A, //0.1V
        eSpdsMainVolt_B, //0.1V
        eSpdsMainVolt_C, //0.1V
        eSpdsMainCurr_A, //0.1A
        eSpdsMainCurr_B, //0.1A
        eSpdsMainCurr_C, //0.1A
        eSpdsMainCurr_N, //0.1A
        eSpdsMainLoad_A, //0.1%
        eSpdsMainLoad_B, //0.1%
        eSpdsMainLoad_C, //0.1%
        eSpdsMainFreq_A, //0.1Hz
        eSpdsMainFreq_B, //0.1Hz
        eSpdsMainFreq_C, //0.1Hz
        eSpdsMainKWhHi_A, //0.1Kwh
        eSpdsMainKWhLo_A, //0.1Kwh
        eSpdsMainKWhHi_B, //0.1Kwh
        eSpdsMainKWhLo_B, //0.1Kwh
        eSpdsMainKWhHi_C, //0.1Kwh
        eSpdsMainKWhLo_C, //0.1Kwh
        eSpdsMainKva_A, //0.01kVA
        eSpdsMainKva_B, //0.01kVA
        eSpdsMainKva_C, //0.01kVA
        eSpdsMainKw_A, //0.01kW
        eSpdsMainKw_B, //0.01kW
        eSpdsMainKw_C, //0.01kW
        eSpdsMainKVar_A, //0.01kVar
        eSpdsMainKVar_B, //0.01kVar
        eSpdsMainKVar_C, //0.01kVar
        eSpdsMainPF_A, //0.01
        eSpdsMainPF_B, //0.01
        eSpdsMainPF_C, //0.01
        eSpdsMainTHDU_A, //0.1%
        eSpdsMainTHDU_B, //0.1%
        eSpdsMainTHDU_C, //0.1%
        eSpdsMainTHDI_A, //0.1%
        eSpdsMainTHDI_B, //0.1%
        eSpdsMainTHDI_C, //0.1%
        eSpdsMainKWhHi_all, //0.1Kwh
        eSpdsMainKWhLo_all, //0.1Kwh
        eSpdsMainRsvd44,
        eSpdsMainRsvd45,
        eSpdsMainRsvd46,
        eSpdsMainRsvd47,
        eSpdsMainRsvd48,
        eSpdsMainRsvd49,
        eSpdsMainRsvd50,

        eSpdsMainNum
    };
    Q_ENUM(SpdsMainEnum)

    enum SpdsBranchEnum
    {
        eSpdsBranchVolt, //0.1V
        eSpdsBranchCurr, //0.1A
        eSpdsBranchLoad, //0.1%
        eSpdsBranchKWhHi, //0.01Kwh
        eSpdsBranchKWhLo, //0.01Kwh
        eSpdsBranchKva, //0.01kVA
        eSpdsBranchKw, //0.01kW
        eSpdsBranchTHDI, //0.1%
        eSpdsBranchSwSts, //Switch status
        eSpdsBranchCurrOvUp, //Current over upper limit
        eSpdsBranchCurrUnderLo, //Current under lower limit
        eSpdsBranchRsvd12,
        eSpdsBranchRsvd13,
        eSpdsBranchRsvd14,
        eSpdsBranchRsvd15,

        eSpdsBranchNum
    };
    Q_ENUM(SpdsBranchEnum)

public:
    explicit wgtParaItem_spds(int id, QWidget *parent = nullptr);
    ~wgtParaItem_spds();

    void setCurrentId(int id);
protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

    //创建参数界面的所有控件
    void createParaWidget(void);
    void createInfo(void);
    void clearAllWideget(void);


private slots:
    void getSpdsInfo(QList<spds_Info> infoList);
    void onScrollBarChange(void);

    void onPageClicked(bool checked);
private:
    void setupUi(void);
    void setConnectState(int state);

    //使能指引按钮
    void setGuideBtntVisible();
    void setWgtVisible();

    void setCurrentPage(void);
    void setTabWidgetStyle(void);
private:
    Ui::wgtParaItem_spds *ui;
    //wgtDateTime          *m_lblDate;
    int                   m_curId;
    spds_List             m_mainList;
    spds_List             m_branchList;
    QList<spds_Info>      spds_list;

    spds_itemList         m_itemList;

    int                   m_mainCurPage;
    int                   m_mainTotalPage;

    int                   m_branchCurPage;
    int                   m_branchTotalPage;
};

#endif // WGTPARAITEM_SPDS_H
