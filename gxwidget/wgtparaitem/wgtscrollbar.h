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

#ifndef WGTSCROLLBAR_H
#define WGTSCROLLBAR_H

#include <QScrollBar>
#include <QStyle>

class wgtScrollbar : public QScrollBar
{
    Q_OBJECT
public:
    wgtScrollbar(Qt::Orientation orientation, QWidget *parent = 0);
    void setSliderLength(int length);
protected:
    void paintEvent(QPaintEvent *);

private:
    QRect subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, /*const*/ QWidget *widget);
    QRect visualRect(Qt::LayoutDirection direction, const QRect &boundingRect, const QRect &logicalRect);
    int sliderPositionFromValue(int min, int max, int logicalValue, int span, bool upsideDown);
private:
    int _sliderLength;
};

#endif // WGTSCROLLBAR_H
