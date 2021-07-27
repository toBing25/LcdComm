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

#ifndef PAGEDISTRIBUTION_H
#define PAGEDISTRIBUTION_H

#include <QWidget>

class QStackedWidget;
class WgtMenuBar;
class PageUpsData;
class PageDistribution;
class WgtHistoryData;
class PageDataManage : public QWidget
{
    Q_OBJECT
public:
    explicit PageDataManage(QWidget *parent = nullptr);
    ~PageDataManage();

protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);

private:
    void setupUi(void);

private:
    QStackedWidget   *m_stkWidget;
    QWidget          *m_wgtMenu;
    WgtMenuBar       *m_menuBar;
    WgtHistoryData   *m_wgtHistData;
};

#endif // PAGEDISTRIBUTION_H
