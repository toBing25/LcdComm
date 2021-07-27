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

#ifndef CGAUGESEMICIRCLE_H
#define CGAUGESEMICIRCLE_H

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <QPen>
#include <QTimer>

//半圆仪表盘
class GaugeSemiCircle : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(double minValue READ getMinValue WRITE setMinValue)
    Q_PROPERTY(double maxValue READ getMaxValue WRITE setMaxValue)
    Q_PROPERTY(double m_value READ getValue WRITE setValue)
    Q_PROPERTY(int precision READ getPrecision WRITE setPrecision)
    Q_PROPERTY(int scaleMajor READ getScaleMajor WRITE setScaleMajor)
    Q_PROPERTY(int scaleMinor READ getScaleMinor WRITE setScaleMinor)
    Q_PROPERTY(int startAngle READ getStartAngle WRITE setStartAngle)
    Q_PROPERTY(int endAngle READ getEndAngle WRITE setEndAngle)
    Q_PROPERTY(QColor scaleColor READ getScaleColor WRITE setScaleColor)
    Q_PROPERTY(QColor pointerColor READ getPointerColor WRITE setPointerColor)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)

public:
    explicit GaugeSemiCircle(QWidget *parent = 0);
    ~GaugeSemiCircle();

    //设置范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    //设置最大最小值
    void setMinValue(double value);
    double getMinValue() const;
    void setMaxValue(double value);
    double getMaxValue() const;

    //设置目标值
    void setValue(double value);
    void setValue(int value);
    double getValue() const;

    //设置精确度
    void setPrecision(int precision);
    int getPrecision() const;

    //设置主刻度数量
    void setScaleMajor(int scaleMajor);
    int getScaleMajor() const;

    //设置小刻度数量
    void setScaleMinor(int scaleMinor);
    int getScaleMinor() const;

    //设置开始旋转角度
    void setStartAngle(int startAngle);
    int getStartAngle() const;

    //设置结束旋转角度
    void setEndAngle(int endAngle);
    int getEndAngle() const;

    //设置刻度尺颜色
    void setScaleColor(const QColor &scaleColor);
    QColor getScaleColor() const;

    //设置指针颜色
    void setPointerColor(const QColor &pointerColor);
    QColor getPointerColor() const;

    //设置文本颜色
    void setTextColor(const QColor &textColor);
    QColor getTextColor() const;

    QSize sizeHint()  const;
    QSize minimumSizeHint() const;

Q_SIGNALS:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent *);
    void drawOuterCircle(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawPointerTriangle(QPainter *painter);
    void drawValue(QPainter *painter);

private:
    double minValue;      //最小值
    double maxValue;      //最大值
    double m_value;       //目标值
    int    precision;     //精确度,小数点后几位
    int    scaleMajor;    //大刻度数量
    int    scaleMinor;    //小刻度数量
    int    startAngle;    //开始旋转角度
    int    endAngle;      //结束旋转角度
    QColor scaleColor;    //刻度尺颜色
    QColor pointerColor;  //指针颜色
    QColor textColor;     //文字颜色

};

#endif // CGAUGESEMICIRCLE_H
