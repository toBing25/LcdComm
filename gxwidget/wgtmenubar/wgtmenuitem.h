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

#ifndef WGTMENUITEM_H
#define WGTMENUITEM_H

#include <QToolButton>

class WgtMenuItem : public QToolButton
{
    Q_OBJECT
public:
    explicit WgtMenuItem(QWidget *parent = 0);

    Q_PROPERTY(int index READ index WRITE setIndex)
    int index(void) { return currId; }
    void setIndex(int index) { currId = index; }
    void setSelect(bool set)
    {
        setChecked(set);
    }

signals:
    void itemClicked(int);

private slots:
    void onClicked(void);

private:
    int currId;
};

#endif // WGTMENUITEM_H
