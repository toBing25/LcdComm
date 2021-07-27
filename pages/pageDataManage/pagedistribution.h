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

#ifndef PAGEEPDU_H
#define PAGEEPDU_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QLabel>

class PageDistribution : public QWidget
{
    Q_OBJECT
public:
    explicit PageDistribution(QWidget *parent = nullptr);
    ~PageDistribution();


protected:
    void showEvent(QShowEvent *event);

private:
    void setupUi(void);

private:
    QTabWidget      *m_tabWidget;
    QWidget         *m_wgtEPDU;
    QWidget         *m_wgtEleModules;
    QWidget         *m_wgtEleInstruments;

    QTableWidget    *m_tabEPDU;
    QTableWidget    *m_tabEleModules;
    QTableWidget    *m_tabEleInstruments;

    //ePDU
    QWidget         *m_wgtSplit1;
    QLabel          *m_lblePDEUNumber;
    QLabel          *m_lblePDEUNumberVal;
    QLabel          *m_lblePDEUStat;
    QLabel          *m_lblePDEUStatVal;
    QLabel          *m_lblRatCurrent;
    QLabel          *m_lblRatCurrentVal;
    QLabel          *m_lblOutCurrent;
    QLabel          *m_lblOutCurrentVal;


    //Electronic modules
    QWidget         *m_wgtSplit2;
    QLabel          *m_lblEleModNumber;
    QLabel          *m_lblEleModNumberVal;
    QLabel          *m_lblEleModStat;
    QLabel          *m_lblEleModStatVal;
    QLabel          *m_lblVoltage;
    QLabel          *m_lblVoltageVal;
    QLabel          *m_lblfrequency;
    QLabel          *m_lblfrequencyVal;

    //Electronic instruments
    QWidget         *m_wgtSplit3;
    QLabel          *m_lblEleInstrNumber;
    QLabel          *m_lblEleInstrNumberVal;
    QLabel          *m_lblEleInstrStat;
    QLabel          *m_lblEleInstrStatVal;
};

#endif // PAGEEPDU_H
