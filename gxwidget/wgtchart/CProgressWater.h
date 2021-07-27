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

#ifndef CPROGRESSWATER_H
#define CPROGRESSWATER_H

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QDebug>

//水波进度条
class CProgressWater : public QWidget
{
    Q_OBJECT

public:
    explicit CProgressWater(QWidget *parent = 0);
    ~CProgressWater();
    enum PercentStyle_Type{
        PercentStyle_Rect = 0,
        PercentStyle_Circle,
        PercentStyle_Ellipse,
    };

    void setValue(int value);
    int  getValue();

    void setMaxValue(int value);
    int  getMaxValue();

    void setMinValue(int value);
    int  getMinValue();

    void setPercentStyle(PercentStyle_Type type);
    PercentStyle_Type  getPercentStyle();

    void setWaterDensity(int value);
    int  getWaterDensity();

    void setColor(QColor color);
    QColor  getColor();

    void setWaterHeight(double value);
    double  getWaterHeight();

    void setBorderWidth(int value);
    int  getBorderWidth();

    void setTextColor(QColor color);
    QColor  getTextColor();

    void setBoderColor(QColor color);
    QColor  getBoderColor();

    void setBgColor(QColor color);
    QColor  getBgColor();

protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void drawValue(QPainter *painter);

private:
    PercentStyle_Type m_percentStyle;
    QColor      m_usedColor;
    QColor      m_textColor;
    QColor      m_boderColor;
    QColor      m_bgColor;
    QColor      m_outlineColor;

    QFont       m_font;
    int         m_value;
    int         m_minValue;
    int         m_maxValue;
    int         m_waterDensity;  // 水波的密度
    double      m_waterHeight;
    double      m_offset;
    int         m_borderWidth;

};

#endif // CPROGRESSWATER_H
