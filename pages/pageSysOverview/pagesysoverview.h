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

#ifndef PAGESYSOVERVIEW_H
#define PAGESYSOVERVIEW_H

#include <QWidget>
#include <QMap>
#include "Global/eMtrDataStruct.h"
#include "Global/AlarmStruct.h"
namespace Ui {
class PageSysOverview;
}
//英智主页
class PageSysOverview : public QWidget
{
    Q_OBJECT

public:
    explicit PageSysOverview(QWidget *parent = nullptr);
    ~PageSysOverview();

private slots:
    void setColdOrHotHumAvg(bool isCold, double tmp, double hum);

protected:
    void showEvent(QShowEvent *event);
    void timerEvent(QTimerEvent *event);
    void changeEvent(QEvent* event);

private:
    std::vector<int> generateDiffNumber(int min, int max, int num);
    void retranslateUi(void);

private:
    Ui::PageSysOverview *ui;
    QList<alarm_list> m_list;
    int               m_timerId2000ms;
};

#endif // PAGESYSOVERVIEW_H
