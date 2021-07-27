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

#ifndef PAGEALARM_H
#define PAGEALARM_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "alarmwidget.h"

class QStackedWidget;
class WgtMenuBar;
class WgtHistory;
class PageAlarm : public QWidget
{
    Q_OBJECT
public:
    explicit PageAlarm(QWidget *parent = nullptr);
    ~PageAlarm();

    void setTabWidgetIndex(int index);
    void setMenuBarIndex(int index);

public slots:
    void getAlarmList(QList<alarm_list>& list);
    void currentIndexChange(int index);

private:
    void setupUi(void);
    void windowInit(void);
    void pageInit(void);

    void setAllAlarm(QList<alarm_list>& list);
    void setGeneralAlarm(QList<alarm_list>& list);
    void setCriticalAlarm(QList<alarm_list>& list);
    void setNormalAlarm(QList<alarm_list>& list);
    void setHorizontalHeaderLabels(void);

    void retranslateUi(void);
    void setTabWidgetStyle(void);
protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent* event);

private:
    QStackedWidget  *m_stkWidget;
    WgtMenuBar      *m_menuBar;
    QWidget         *m_wgtMenu;
    QWidget         *m_wgtCurrentAlarm;
    WgtHistory      *m_wgtHistory;

    QTabWidget      *m_tabWidget;

    //所有告警
    QWidget         *m_wgtAllAlarm;
    QTableWidget    *m_tabAllAlarm;
    QPushButton     *m_AlarmPageUp;
    QPushButton     *m_AlarmPageDown;

    //一般告警
    QWidget         *m_wgtGeneralAlarm;
    QTableWidget    *m_tabGeneralAlarm;
    QPushButton     *m_GenAlarmPageUp;
    QPushButton     *m_GenAlarmPageDown;

    //严重告警
    QWidget         *m_wgtCriticalAlarm;
    QTableWidget    *m_tabCriticalAlarm;
    QPushButton     *m_CriAlarmPageUp;
    QPushButton     *m_CriAlarmPageDown;
};

#endif // PAGEALARM_H
