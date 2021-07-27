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

#ifndef SYSFUNCTION_H
#define SYSFUNCTION_H

#include <QWidget>
#include <QVector>
#include "wgtkeypad.h"

namespace Ui {
class SysFunction;
}

class SysFunction : public QWidget
{
    Q_OBJECT

public:
    explicit SysFunction(QWidget *parent = nullptr);
    ~SysFunction();

    //设置登录信息
    static void setLoginInfo(QString  username, QString account, QString loginstatus);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void changeEvent(QEvent* event);

private:
    void setupUi(void);
    void postCustomEvents(void);
    void retranslateUi(void);

private slots:
    void onBtnClicked(bool statue);
    void onBtnLoginClicked(bool statue);
    void memInfo(void);
    void memInfoQStorageInfo(void);


private:
    Ui::SysFunction *ui;
    QVector<QLineEdit *> lieVector;

    static QString   m_strAccount;
    static QString   m_strUserName;
    static QString   m_strLoginSta;
};

#endif // SYSFUNCTION_H
