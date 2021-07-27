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

#ifndef MDIAL_H
#define MDIAL_H

#include <QWidget>

class wgtDial : public QWidget
{
    Q_OBJECT
public:
    explicit wgtDial(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void valueChange(qreal val);

private:
    void drawBg(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawValue(QPainter *painter);

private:
    qreal m_value;
    int   m_maxVal;
    int   m_minVal;
    int   m_radius;
    int   m_startAngle;
    int   m_endAngle;
};

#endif // MDIAL_H
