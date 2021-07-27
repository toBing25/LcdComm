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

#ifndef ACTPARAITEM_BTN_H
#define ACTPARAITEM_BTN_H

#include <QWidget>
#include "Global/SysCfgMng.h"
#include "wgtparaitem_ups.h"
#include "actparaitem_define.h"

namespace Ui {
class ActParaItem_btn;
}

class ActParaItem_btn : public QWidget
{
    Q_OBJECT

public:
    explicit ActParaItem_btn(DevTypeEnum devType, int devId, struct Menu menu, QWidget *parent = nullptr);
    ~ActParaItem_btn();

    void setValue(QString val);
    void setUnit(QString unit);
    void setDevId(int id){m_devid = id;}
signals:
    void signal_ths(ths_set set);
    void signal_ups(ups_set set);
    void signal_ac(ac_set set);
    void signal_leak(leak_set set);

private slots:
    void onbtnclicked(bool checked);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::ActParaItem_btn *ui;
    Menu          m_menu;
    QString       m_name;
    DevTypeEnum   m_devtype;
    int           m_devid;
};

#endif // ACTPARAITEM_BTN_H
