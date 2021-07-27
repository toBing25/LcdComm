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

#ifndef WGTEQUIPMENTOVERVIEW_H
#define WGTEQUIPMENTOVERVIEW_H

#include <QWidget>
#include "Global/SysCfgMng.h"

namespace Ui {
class WgtEquipmentOverview;
}

class flickerItem;
class WgtEquipmentOverview : public QWidget
{
    Q_OBJECT

public:
    explicit WgtEquipmentOverview(QWidget *parent = nullptr);
    ~WgtEquipmentOverview();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void getThsInfo(QList<ths_Info> ths_list);
    void getAcInfo(QList<ac_Info>   ac_list);
    void getUpsInfo(QList<ups_Info> ups_list);
    void getMeterInfo(QList<meter_Info> meter_list);
    void getLeakInfo(QList<leak_Info> leak_list);
    void getSpdsInfo(QList<spds_Info> spds_list);
    void getSmsInfo(QList<sms_Info> sms_list);

private slots:
    void onthsClicked(QString id);
    void onAcClicked(QString id);
    void onMeterClicked(QString id);
    void onUpsClicked(QString id);
    void onLeakClicked(QString id);
    void onSpdsClicked(QString id);
    void onSmsClicked(QString id);

private:
    void setupUi(void);

    void cleanThsDev(void);
    void cleanAcDev(void);
    void cleanUpsDev(void);
    void cleanMeterDev(void);
    void cleanLeakDev(void);
    void cleanSpdsDev(void);
    void cleanSmsDev(void);

    int  getAlarmStatus_ups(int devId);
    int  getAlarmStatus_ths(int devId);
    int  getAlarmStatus_ac(int devId);
    int  getAlarmStatus_leak(int devId);
    int  getAlarmStatus_meter(int devId);
    int  getAlarmStatus_spds(int devId);
    int  getAlarmStatus_sms(int devId);

private:
    Ui::WgtEquipmentOverview *ui;

    QList<ths_Info>    m_thsList;
    QList<ac_Info>     m_acList;
    QList<ups_Info>    m_upsList;
    QList<meter_Info>  m_meterList;
    QList<leak_Info>   m_leakList;
    QList<spds_Info>   m_spdsList;
    QList<sms_Info>    m_smsList;

    //item
    QList<flickerItem *>m_upsItemList;
    QList<flickerItem *>m_thsItemList;
    QList<flickerItem *>m_acItemList;
    QList<flickerItem *>m_leakItemList;
    QList<flickerItem *>m_meterItemList;
    QList<flickerItem *>m_spdsItemList;
    QList<flickerItem *>m_smsItemList;
};

#endif // WGTEQUIPMENTOVERVIEW_H
