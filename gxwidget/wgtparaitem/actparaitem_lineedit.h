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

#ifndef ACTPARAITEM_LINEEDIT_H
#define ACTPARAITEM_LINEEDIT_H

#include <QWidget>
#include "wgtkeypad.h"
#include "actparaitem_define.h"
#include "Global/SysCfgMng.h"
#include "wgtparaitem_ups.h"

namespace Ui {
class ActParaItem_lineEdit;
}

//使用lineEdit设置参数
class ActParaItem_lineEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ActParaItem_lineEdit(DevTypeEnum devType, int devId, struct Menu menu, QWidget *parent = nullptr);
    ~ActParaItem_lineEdit();

    void setValue(QString val);
    void setUnit(QString unit);
    void setDevId(int id){m_devid = id;}
signals:
    void signal_ths(ths_set set);
    void signal_ups(ups_set set);
    void signal_ac(ac_set set);
    void signal_leak(leak_set set);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

protected:
    void paintEvent(QPaintEvent *);

    void writeData(void);
private:
    Ui::ActParaItem_lineEdit *ui;
    QVector<QLineEdit *> lieVector;
    Menu         m_menu;

    QString      m_name;
    QString      m_value;
    DevTypeEnum  m_devtype;
    int          m_devid;
};

#endif // ACTPARAITEM_LINEEDIT_H
