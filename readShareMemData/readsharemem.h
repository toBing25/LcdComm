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

#ifndef READSHAREMEM_H
#define READSHAREMEM_H
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QMetaType>
#include <QVariant>
#include "Global/eMtrDataStruct.h"
#include "Global/AlarmStruct.h"
#include "SpdsData/SpdsDataStruct.h"
#include "Global/AlarmMng.h"
#include "readdatabase.h"
#include "wtritesharemem.h"
#include "jsonfile.h"
#include <QJsonDocument>
#include <QJsonArray>
class ReadShareMem : public QThread
{
    Q_OBJECT

public:
    ReadShareMem();
    void closeThread();
    readDatabase* getReadDataBase(){return m_pReadData;}
    WtriteShareMem* getWriteData(){return  m_pWriteData;}
    QVector<CardDb>* cardDb(){return m_dcJson->cardDb();}
    //bool    getDataSaveSDCard(){return m_dataSaveSDCard;}
protected:
    virtual void run();

public slots:
    //添加和修改门禁卡
    void changeCard(QString cardNo, QString cardName, QStringList dcList);
    void removeCard(QString cardNo);
signals:

    void sendChageCardDone();
    void removeCardDone();
private:
    void readMtrData();
    void readSyscfgData();
    void readCurrentAlarm();
    void readThsDevData();
    void readACDevData();
    void readUPSDevData();
    void readMeterDevData();    //获取电表协议，根据协议显示具体内容
    void readLeakDevData();
    void readDCDevData();
    void readSpdsDevData();
    void readBMSDevData();
    void readSMSDevData();

    //
    void showInputOutputABC(ups_Info &upsInfo, int in,int out);
    //
    bool initCardAccessJson();
    bool findCardID(int id,QVector<int>&);

    bool initMeterJsonData(QString path,QList<QString>&);
    bool initJsonDataFile(QString path, QJsonDocument &jsonData);
    QJsonArray findJsonDataArray(QString path, QString objName);
    QJsonObject findJsonDataObject(QJsonDocument &jsonDoc, QString objName,QString valType);
    spds_proto findJsonDataSpdsProto(QString path);

    //初始化空设备
    void initThsDev(ths_Info &nullthsInfo);
    void initACDev(ac_Info &nullacInfo);
    void initUPSDev(ups_Info &nullupsInfo);
    void initMeterDev(meter_Info &nullmeterInfo);
    void initLeakDev(leak_Info &nullleakInfo);
    void initSpdsDev(spds_Info &nullspdsInfo);
    void initSmsDev(sms_Info &nullsmsInfo);


private:
    volatile bool isStop;    
    QList<alarm_list>   almItemList;
    readDatabase        *m_pReadData;
    WtriteShareMem      *m_pWriteData;
    QThread             *m_pThread_ReadData;
    QThread             *m_pThread_WriteData;
    QJsonDocument       m_cardAccessJsonDoc;
    QJsonArray          m_currMeterProtoArr;
    QList<QString>      m_meterProList;
    int                 m_iCurrMeterProto;
    spds_proto          m_currSpdsProto;
    QList<QString>      m_spdsProList;
    int                 m_iCurrSpdsProto;

    door_Info           m_doorinfo;
    bool                m_initJsonOK;
    bool                m_dataSaveSDCard;

    //控制模块json
    JsonFile *m_dcJson;


signals:
    void sendeMtrPueValue(double val);
    void sendeMtrPerValue(double val);
    void sendeMtrColdOrHotHumAvg(bool isCold, double tmp, double hum);
    void sendeMtrAlmItemList(QList<alarm_list>&);
    void sendDoorStsChange(QList<int>,int almNum);
    void sendAlmNumCount(AlmLevel);
    void sendeExtDevThsList(QList<ths_Info>);
    void sendeExtDevAcList(QList<ac_Info>);
    void sendeExtDevUpsList(QList<ups_Info>);
    void sendeExtDevMeterList(QList<meter_Info>);
    void sendeExtDevLeakList(QList<leak_Info>);
    void sendeExtDevSpdsList(QList<spds_Info>);
    void sendeExtDevSmsList(QList<sms_Info>);

    //--------------------------------------------
    void sendDoorList(door_Info);
    void sendDataSaveLocChange(bool isSDCard);

};

#endif // READSHAREMEM_H
