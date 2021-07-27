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

#ifndef ACTPARAITEM_H
#define ACTPARAITEM_H

#include <QWidget>
#include "actparaitem_define.h"

namespace Ui {
class ActParaItem;
}
//普通参数显示
class ActParaItem : public QWidget
{
    Q_OBJECT

public:
    explicit ActParaItem(QString name, QString val, QWidget *parent = nullptr);
    ~ActParaItem();

    void setVal(QString val);
    void setUnit(QString unit);
    void setWidgetStyle(int style);

protected:
    void paintEvent(QPaintEvent *);

private:
    Ui::ActParaItem *ui;
    QString   m_name;
    QString   m_value;

};

#endif // ACTPARAITEM_H
