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

#ifndef PAGENETWORKSET_H
#define PAGENETWORKSET_H

#include <QWidget>
#include "wgtkeypad.h"


class QLineEdit;
class PageNetWorkSet : public QWidget
{
    Q_OBJECT
public:
    explicit PageNetWorkSet(QWidget *parent = nullptr);
    ~PageNetWorkSet();

protected:
    bool event(QEvent *ev);
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent* event);

private:
    void setupUi(void);
    //权限更新
    void permissUpdate(void);

    void retranslateUi(void);
private slots:
    void onNetworksettings(bool clicked);
    void getNetInfo();

private:
    QVector<QLineEdit *> lieVector;
    //WgtKeyPad    m_softKeyboard;
    QString      m_ip;
    QString      m_mask;
    QString      m_gateway;
    QString      m_dns1;
    QString      m_dns2;

    QWidget     *m_wgtNet;
    QWidget     *m_wgtSet;

    QLabel      *m_lblIpName;
    QLineEdit   *m_leIpVal1;

    QLabel      *m_lblMaskName;
    QLineEdit   *m_leMaskVal1;

    QLabel      *m_lblGatewayName;
    QLineEdit   *m_leGatewayVal1;


    QLabel      *m_lblDns1Name;
    QLineEdit   *m_leDns1Val;

    QLabel      *m_lblDns2Name;
    QLineEdit   *m_leDns2Val;

    QPushButton *m_btnSet;
};

#endif // PAGENETWORKSET_H
