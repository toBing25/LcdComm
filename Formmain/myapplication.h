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

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>
#include <QTimer>
#include "formmain.h"
#include "sysfunction.h"

class MyApplication : public QApplication
{
    enum STATUS{
        status_open,
        status_close
    };

public:
    MyApplication(int &argc, char **argv);
    bool notify(QObject *obj, QEvent *e);

private:
    void readConfig(QString &enable, QString &interval);

private:
   QTimer     *m_timer;
   STATUS      m_screenSaver_status; //屏保状态
   bool        m_screenSaver_enable; //屏保使能
};

#endif // MYAPPLICATION_H
