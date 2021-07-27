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

#ifndef PAGEEXTEND_H
#define PAGEEXTEND_H

#include <QWidget>
#include <QHBoxLayout>
#include "Global/SysCfgMng.h"

#define MAX_DEVICES           50 //该节点下有多少个子节点
#define SYSTEM_TYPE_DEVICE    0  //设备总览
#define SYSTEM_TYPE_POW       1  //配电系统
#define SYSTEM_TYPE_ENVI      2  //环境系统
#define SYSTEM_TYPE_SECURITY  3  //安防系统

#define  DEVTYPE_THS    1
#define  DEVTYPE_UPS    2
#define  DEVTYPE_AC     3
#define  DEVTYPE_METER  4
#define  DEVTYPE_LEAK   5
#define  DEVTYPE_SPDS   6
#define  DEVTYPE_SMS    7
#define  DEVTYPE_NUM    DEVTYPE_SMS


class QStackedWidget;
class WgtMenuBar;
class WgtMenuBar_Ext;
class PageExtend : public QWidget
{
    Q_OBJECT
public:
    explicit PageExtend(QWidget *parent = nullptr);
    ~PageExtend();

protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent* event);

private slots:
    void getThsInfo(QList<ths_Info> ths_list);
    void getAcInfo(QList<ac_Info>   ac_list);
    void getUpsInfo(QList<ups_Info> ups_list);
    void getMeterInfo(QList<meter_Info> meter_list);
    void getLeakInfo(QList<leak_Info> leak_list);
    void getSpdsInfo(QList<spds_Info> spds_list);
    void getSmsInfo(QList<sms_Info> sms_list);

public:
    static PageExtend *Instance();
private:
    static PageExtend *_instance;

private:
    void setupUi(void);
    //创建所有的设备
    void createDevice();
    //删除所有设备
    void clearDevice(void);

    //创建温湿度
    void createThsDev(int &treeNubmer);
    //创建空调设备
    void createAcDev(int &treeNubmer);
    //创建配电设备
    void createMeterDev(int &treeNubmer);
    //创建水浸
    void createLakDev(int &treeNubmer);
    //创建UPS设备
    void createUpsDev(int &treeNubmer);
    //创建智能配电柜
    void createSpdsDev(int &treeNubmer);
    //创建短信报警器
    void createSmsDev(int &treeNubmer);

    void retranslateUi(void);
private:
    QWidget         *m_wgtMenu;
    QStackedWidget  *m_stkWidget;
    QHBoxLayout     *mainLayout;

    //当设备第一次创建时，给它们分配编号
    int   m_powNubmer;
    int   m_enviNubmer;
    int   m_secuNubmer;
public:
    WgtMenuBar_Ext  *m_wgtLeftMenu;
    //每个设备在列表中的编号，默认初始化为-1
    int   m_thstree;
    int   m_actree;
    int   m_upstree;
    int   m_metertree;
    int   m_leaktree;
    int   m_spdstree;
    int   m_smsstree;

    QList<spds_Info>   m_spdsList;
    QList<ths_Info>    m_thsList;
    QList<ac_Info>     m_acList;
    QList<ups_Info>    m_upsList;
    QList<meter_Info>  m_meterList;
    QList<leak_Info>   m_leakList;
    QList<sms_Info>    m_smsList;
};

#endif // PAGEEXTEND_H
