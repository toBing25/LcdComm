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

#ifndef FLICKERITEM_H
#define FLICKERITEM_H

#include <QWidget>
#include <QToolButton>

#define ALARM_STATE_NOCON  0 //未连接
#define ALARM_STATE_NORNAL 1 //正常
#define ALARM_STATE_ALARM  2 //告警

class flickerItem : public QToolButton
{
    Q_OBJECT

public:
    explicit flickerItem(QString name, int state, QWidget *parent = nullptr);
    ~flickerItem();

    void setState(int state);
private:
    void setIcons(int state);
    void setAlarmColor(QString color);

private:
   QStringList  m_list;  //0未连接/1正常/2告警
   int          m_state;
   QString      m_name;
   QTimer      *m_timer;
   int          m_alarmState;

};

#endif // FLICKERITEM_H
