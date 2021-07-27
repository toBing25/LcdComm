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

#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QWidget>
#include <QDebug>
#include <QToolButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "pagesysoverview.h"
#include "pagesysoverview_.h"
#include "wgtbgscreen.h"
#include "wgttitlebar.h"
#include "wgtmenubar.h"
#include "wgtdoorbox.h"
#include "pagesystemsettings.h"
#include "pagedatamanage.h"
#include "pagealarm.h"
#include "pageextend.h"
#include "loginmanage.h"
#include "readsharemem.h"
#include "alarmborder.h"
#include "DCData/CabCtrlDataStructExt.h"
#define LOGIN_NOT_LOGGED      -1
#define LOGIN_LOGGED           1

#define TYPE_STATUS            0
#define TYPE_SETTING           1
#define TYPE_PARA_SETTING      2
#define TYPE_STAT_SETTING      3

#define SHOWTYPE_LINEEDIT   0  //文本框
#define SHOWTYPE_BTN        1  //按钮
#define SHOWTYPE_COMBOBOX   2  //下拉框
#define SHOWTYPE_BIT        3  //位设置

#define HIDEITEMVALUE          -32765   //界面收到此值隐藏标签

class LoginManage;
class QPropertyAnimation;
class FormMain : public QWidget
{
    Q_OBJECT

    enum MUTER_STATUS{
       status_mute,  //静音
       status_noisy  //吵闹
   };



public:
    FormMain(QWidget *parent = nullptr);
    ~FormMain();

public:
    static FormMain *Instance();
private:
    static FormMain *_instance;

public slots:
     void getMenuList(bool flag);
     ReadShareMem* getShareMemManage(){return m_readShareMem;}
     void popupAlmListWidget();
     void setAlmIsShow(bool flag){ almIsHide = flag;}
     bool getAlmIsShow(){return almIsHide;}
     void opendoorWidget(door_Info);     

     void setMenuIndex(int index){m_stkWidget->setCurrentIndex(index);}

protected:
    bool event(QEvent *ev);
    void changeEvent(QEvent* event);

public:
    static void setLoginFunc(void);
    int  getLoginStatus(void){return  m_login;}

    SysTypeEnum getSysType(void) {return m_sysType;}
private:
    void setupUi(void);
    void login_init(void);
    void shareMem_init(void);
    void almPopupList_init(void);
    void language_init(void);

    //设置告警声音
    static void setMetu(void);

    //注册自定义事件用于更新权限显示
    void registerEvent(void);
    //发送自定义的事件
    void postCustomEvents(void);
    //语言翻译
    void retranslateUi(void);
    //根据告警等级获取相应颜色
    QString getAlarmColor(int grade);
public:
    LoginManage         *m_loginManage;
    ReadShareMem        *m_readShareMem;
    Login                m_loginInfo;
    MenuAndClassList     m_upsList;     //ups
    MenuAndClassList     m_meterList;   //配电
    MenuAndClassList     m_acList;      //空调
    MenuAndClassList     m_eleakList;   //水浸
    MenuAndClassList     m_thsList;     //温湿度
    MenuAndClassList     m_spdsList;    //配电柜


    static AcDevVector m_acDev;         //新空调数据存储
    static DcCardDevVector m_dcDev;     //控制模块列表

	
private:
    WgtBgScreen         *m_wgtMain;
    wgttitlebar         *m_titleBar;
    QStackedWidget      *m_stkWidget;
//#if iSMART_APP
//    PageSysOverview     *m_pageSysOverview;
//#else
//    PageSysOverview_    *m_pageSysOverview;
//#endif
    PageSystemSettings  *m_pageSysSettings;
    PageDataManage      *m_pageDataManage;
    PageAlarm           *m_pageAlarm;
    PageExtend          *m_pageExtend;
    static  int          m_login;
    QToolButton         *m_almListBtn;
    AlarmBorder         *m_almWidget;
    QPoint               m_almWidgetPos;
    QPropertyAnimation  *animation;
    QPropertyAnimation  *animation2;
    bool                 almIsHide;
    //door
    WgtDoorBox          *pDoor;
    bool                 m_doorWgtIsOpen;

    MUTER_STATUS         m_muter;
    QList<alarm_list>    m_list;     //告警列表
    int                  m_alrmNum;  //当前显示第几条告警
    SysTypeEnum          m_sysType;
};
#endif // FORMMAIN_H
