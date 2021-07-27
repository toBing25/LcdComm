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

#ifndef ACTPARAWIDGET_H
#define ACTPARAWIDGET_H

//对应框的大小
#define  STYLE_MIN   0
#define  STYLE_MID   1
#define  STYLE_MAX   2

#include <QWidget>

namespace Ui {
class ActParaWidget;
}

class ActParaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActParaWidget(const QString &title, QWidget *parent = nullptr);
    ~ActParaWidget();

    void setTitle(QString title);
    void setStyle(int style);
    void setTitleWidgetVisible(bool l);
protected:
    void paintEvent(QPaintEvent *);

public:
    Ui::ActParaWidget *ui;

    QString   m_title;
};

#endif // ACTPARAWIDGET_H
