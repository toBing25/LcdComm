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

#ifndef WGTCHECKBOX_H
#define WGTCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
#include <QMouseEvent>

class wgtCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit wgtCheckBox(QWidget *parent = nullptr) : QCheckBox(parent){ };
    ~wgtCheckBox(){};

protected:
    void mousePressEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);

        Qt::CheckState state = this->checkState();
        switch (state) {
        case Qt::Unchecked:
            this->setCheckState(Qt::Checked);
            break;
        case Qt::Checked:
            this->setCheckState(Qt::Unchecked);
            break;
        default:
            break;
        }
    }
};

#endif // WGTCHECKBOX_H
