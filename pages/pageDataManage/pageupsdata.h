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

#ifndef PAGEUPSDATA_H
#define PAGEUPSDATA_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>

class PageUpsData : public QWidget
{
    Q_OBJECT
public:
    explicit PageUpsData(QWidget *parent = nullptr);
    ~PageUpsData();

private:
    void setupUi(void);

protected:
    void showEvent(QShowEvent *event);

private:
    QLabel          *m_lblUpsNumber;
    QLabel          *m_lblUpsVal;
    QLabel          *m_lblUpsStat;
    QLabel          *m_lblUPsStatVal;

    QTableWidget    *m_tabInfo_1;
    QTableWidget    *m_tabInfo_2;

};

#endif // PAGEUPSDATA_H
