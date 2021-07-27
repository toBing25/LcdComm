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

#ifndef WGTPARAITEM_SMS_H
#define WGTPARAITEM_SMS_H

#include <QWidget>
#include "Global/SysCfgMng.h"


namespace Ui {
class wgtParaItem_sms;
}

//短信报警器
class wgtParaItem_sms : public QWidget
{
    Q_OBJECT

public:
    explicit wgtParaItem_sms(int id, QWidget *parent = nullptr);
    ~wgtParaItem_sms();

    void setCurrentId(int id);
private:
    void setupUi(void);
    void setConnectState(int state);

private slots:
    void getSmsInfo(QList<sms_Info> infoList);

private:
    Ui::wgtParaItem_sms *ui;
    int                 m_curId;
    QList<sms_Info>     sms_list;

};

#endif // WGTPARAITEM_SMS_H
