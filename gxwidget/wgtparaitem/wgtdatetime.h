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

#ifndef WGTDATETIME_H
#define WGTDATETIME_H

#include <QWidget>
#include <QLabel>

class wgtDateTime : public QLabel
{
    Q_OBJECT
public:
    explicit wgtDateTime(QWidget *parent = nullptr);

private slots:
    void updateTime(void);

};

#endif // WGTDATETIME_H
