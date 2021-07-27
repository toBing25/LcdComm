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

#ifndef WGTMENUITEM__EXT_H
#define WGTMENUITEM__EXT_H

#include <QToolButton>
#include <QLabel>

class WgtMenuItem_Ext : public QToolButton
{
    Q_OBJECT
public:
    explicit WgtMenuItem_Ext(QWidget *parent = 0);

    void setSelect(bool set)
    {
        setChecked(set);
    }

    void setText(const QString text);
    void setIcon(const QString icon);
    void setColor(QString color);
signals:
    void itemClicked(int);

private slots:
    void onClicked(bool checked);

private:
    QLabel    *m_text;
    QLabel    *m_icon;
};

#endif // WGTMENUITEM__EXT_H
