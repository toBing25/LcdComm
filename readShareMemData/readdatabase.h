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

#ifndef READDATABASE_H
#define READDATABASE_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QDateTime>
#include <QPair>
#include <QVariant>
#include "Global/AlarmStruct.h"
#include "language/languagePublic.h"
#include "Global/DataManage.h"
#define PAGENUM     8

struct almDataStruct{
    QVector<alarm_list> almList;
    int pageCount;
};

class readDatabase : public QObject
{
    Q_OBJECT
public:
    readDatabase();
    bool getDBConnStatus(){return m_DBConn;}
private:
    //状态数据
    bool readStatsHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL,int pageNum);
    //PUE数据
    bool readPueHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL,int pageNum);
    //温湿度数据
    bool readThsHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL,int pageNum);
    //UPS数据
    bool readMeterHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL,int pageNum);
    //AC数据
    bool readAcHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL,int pageNum);
    //meter数据
    bool readUpsHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL,int pageNum);

    //
    QList<QPair<langEnum,QString>> getUpsHead(){return  uDataList;}
public slots:
    //告警历史数据查询
    bool readAlmHistoryData(int type, QVector<int>devNoList, QVector<int>levelList, QDateTime lower, QDateTime upper, int page);
    //数据管理历史数据查询
    void readAllHistoryData(int type, QVector<int>devNoList, QDateTime lower, QDateTime upper, int page);

signals:
    void sendAlmHistoryData(almDataStruct);

    void sendStatsHistoryData(statsDataStruct);
    void sendPueHistoryData(pueDataStruct);
    void sendThsHistoryData(thsDataStruct);
    void sendUpsHistoryData(upsDataStruct);
    void sendAcHistoryData(acDataStruct);
    void sendMeterHistoryData(meterDataStruct);
public slots:
    void dataSaveLocChange(bool isSDCard);
private:
    QSqlDatabase db;
    almDataStruct m_almData;

    statsDataStruct m_statsData;
    pueDataStruct   m_pueData;
    thsDataStruct   m_thsData;
    upsDataStruct   m_upsData;
    acDataStruct    m_acData;
    meterDataStruct m_meterData;
    bool        m_DBConn;

};



#endif // READDATABASE_H
