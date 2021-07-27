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

#ifndef SIGNALGRAPH_H
#define SIGNALGRAPH_H

#include <QObject>
#include <QWidget>
class QLabel;
class SignalGraph : public QWidget
{
    Q_OBJECT
public:
    explicit SignalGraph(QWidget *parent = nullptr);
    void setSigVal(int val);
protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_sig;
    QLabel *lab;

};

#endif // SIGNALGRAPH_H
