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

#ifndef WGTDASHBOARD_H
#define WGTDASHBOARD_H

#include <QWidget>

//仪表盘
class wgtDashboard : public QWidget
{
    Q_OBJECT

public:
    wgtDashboard(QWidget *parent = nullptr);
    ~wgtDashboard();

    void set_range(const double min, const double max);
    void set_min_range(const double min);
    void set_max_range(const double max);
    void set_unit(const QString& unit);
    void set_text(const QString& text);
public slots:
    void set_value(const double value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void draw_dash_board(QPainter& painter);
    void draw_hand(QPainter& painter);
    void draw_value(QPainter& painter);
    void draw_text(QPainter& painter);

private:
    double _value;
    double _min;
    double _max;
    QString _unit;
    QString _text;
};
#endif // WGTDASHBOARD_H
