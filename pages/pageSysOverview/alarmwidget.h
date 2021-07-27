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

#ifndef ALARMWIDGET_H
#define ALARMWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include  "Global/AlarmStruct.h"

#define   NormalAlarm     0    //正常告警
#define   GeneralAlarm    1    //一般告警
#define   CriticalAlarm   2    //严重告警


typedef QList<alarm_list> Alarm_List;

class AlarmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = nullptr);
    ~AlarmWidget();

protected:
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent* event);

public slots:
    void getAlarmList(QList<alarm_list> &list);

private:
    void setupUi(void);
    void setHorizontalHeaderLabels(void);
    void alarmListUpdate(QList<alarm_list> &list);
    QString getAlarmColor(int grade);

    void retranslateUi(void);
private:
    QTableWidget   *m_tableWidget;
    QList<alarm_list> m_list;
};

#endif // ALARMWIDGET_H
