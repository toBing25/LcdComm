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

#ifndef HOSTPAGE_H
#define HOSTPAGE_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class HostPage;
}

class HostPage : public QWidget
{
    Q_OBJECT

public:
    explicit HostPage(QWidget *parent = nullptr);
    ~HostPage();

public slots:
    void doorStsChange(QList<int>,int almNum);
    void switchImg();

private:
    Ui::HostPage *ui;
    QImage *img;
    QTimer *timer;
    int   switchTime;
    bool  prevIsNormalImg;
};

#endif // HOSTPAGE_H
