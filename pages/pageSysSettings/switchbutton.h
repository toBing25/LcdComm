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

#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>

class SwitchButton : public QPushButton
{
    Q_OBJECT

public:
    SwitchButton(QWidget *parent);
    ~SwitchButton();
    void setSwitch(bool Switch);

private:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void drawBg(QPainter& painter);
    void drawSlidBlock(QPainter& painter);
    void drawText(QPainter& painter);

signals:
    void btnState(bool bswitch);

private:
    bool bSwitch;       //开关状态:开true，关false

    QColor bgColorOn;
    QColor bgColorOff;
    //QColor sliderColor;
    QString sliderColor;
    QColor sliderColorOn;
    QColor sliderColorOff;
    QColor textColorOn;
    QColor textColorOff;

    QString strText;
    QPoint startPoint;
    QPoint endPoint;
    QPoint centerPoint;

    int mouseX;
    bool bPress;

};

#endif // SWITCHBUTTON_H
