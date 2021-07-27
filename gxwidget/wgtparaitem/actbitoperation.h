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

#ifndef ACTBITOPERATION_H
#define ACTBITOPERATION_H

#include <QWidget>
#include <QDialog>
#include <QVector>
#include <QCheckBox>

#include "actparaitem_define.h"

namespace Ui {
class ActBitOperation;
}

//位运算
class ActBitOperation : public QDialog
{
    Q_OBJECT

public:
    explicit ActBitOperation(QStringList list, QString val, QWidget *parent = nullptr);
    ~ActBitOperation();

protected:
    void showEvent(QShowEvent *event);

private slots:
    void onBtnClicked(bool checked);
    void onComBoBoxClicked(QString bit);

private:
    void setupUi(void);
    //二进制转十进制
    QString binToDec(QString strBin);
    //十进制转二进制
    QString decTobin(QString strDec);

private:
    Ui::ActBitOperation *ui;
    QVector<QCheckBox *> m_vector;
    //int                  m_valArray[16];
    int                 *m_valArray;

    QStringList          m_list;
    int                  m_bitNum;
    QString              m_result;
    QString              m_val;
};

#endif // ACTBITOPERATION_H
