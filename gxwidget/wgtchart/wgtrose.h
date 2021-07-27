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

#ifndef WGTROSE_H
#define WGTROSE_H

#include <QWidget>
#include <QMap>
#include <QColor>
#include <QLabel>

class QPaintEvent;

//南丁格尔玫瑰图
class wgtRose : public QWidget
{
    Q_OBJECT
public:
    explicit wgtRose(QWidget* parent = nullptr);

    void setTitle(QMap<int, QString> titleMap, QMap<int, QColor> colorMap);
    void setData(QMap<int, int> valueMap, int alarmCount);
    void setCoreColor(QColor color);

protected:
    void paintEvent(QPaintEvent *event);

   // 画玫瑰饼图
    void drawRose(QPainter& painter);

private:
    int getIndexFromaValue(int value);

private:
    // 玫瑰图颜色
    QMap<int, QColor>  m_colorMap;
    // 玫瑰图各个数据的标题
    QMap<int, QString> m_titleMap;
    // 玫瑰图各个数据的值
    QMap<int, int>     m_valueMap;
    // 各个数据值从小到大排序
    QList<int>         m_sortValueList;
    // 视口宽度
    int     m_iSide;
    int     m_iWindowRadius;
    int     m_iRadius;
    int     m_alarmCount;
    QColor  m_coreColor;
};

#endif // WGTROSE_H
