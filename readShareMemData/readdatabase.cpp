#include "readdatabase.h"
#include "Global/AlarmMng.h"
#include <QDebug>
readDatabase::readDatabase():m_DBConn(false)

{
    if(QSqlDatabase::contains("alm_mysql")){
      db = QSqlDatabase::database("alm_mysql");
    }
    else{
      db = QSqlDatabase::addDatabase("QMYSQL","alm_mysql");
    }


    db.setHostName("127.0.0.1");
    db.setDatabaseName("eMtrDb");
    db.setUserName("root");    
    db.setPassword("invteMTR");
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1");
    bool ok = db.open();

    if (ok)
    {
        //qDebug() << "连接数据库ok";
        m_DBConn = true;
    }
    else
    {
        //qDebug() << "连接数据库失败";
        m_DBConn = false;
    }

    //初始化数据管理
   uDataList_Init();
   acDataList_Init();
   meterDataList_Init();
   thsDataList_Init();
   statsDataList_Init();
   pueDataList_Init();

}

bool readDatabase::readAlmHistoryData(int type, QVector<int> devNoList, QVector<int> levelList, QDateTime lower, QDateTime upper, int page)
{        
    if(!m_DBConn)
        return false;
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }
    bool ret = false;    
    int limitL=0;
    limitL =(page-1)*PAGENUM;
    QString upperDate,lowerDate,typeSql,levelSql,devNoSql;
    m_almData.almList.clear();
    m_almData.pageCount = 0;
    alarm_list almInfo;
    if(!upper.isNull() and !lower.isNull()){
        upperDate = upper.toString("yyyy-MM-dd hh:mm:ss");
        lowerDate = lower.toString("yyyy-MM-dd hh:mm:ss");
    }
    //以下参数值为-1为查询所有


    if(levelList.length() == 0 ){
        levelSql = "";
    }else{
        levelSql = " and(";
        foreach (int level, levelList) {
            levelSql += "level="+QString::number(level)+" or ";
        }
        levelSql = levelSql.left(levelSql.length()-4)+")";
    }

    if(devNoList.length() == 0){
        devNoSql = "";
    }else {
        devNoSql += " and(";
        foreach (int devNo, devNoList) {
            devNoSql += "devNo="+QString::number(devNo)+" or ";
        }
        devNoSql = devNoSql.left(devNoSql.length()-4)+")";
    }

    if(type == -1 ){
        typeSql = "";
        devNoSql ="";   //选中所有类型，就不区分设备编号
    }else{
        typeSql = " and devType="+QString::number(type);
    }

    QString sqlCount = QString("select count(id) as almCount from HislogTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3%4%5").arg(lowerDate,upperDate,typeSql,levelSql,devNoSql);

    QString sql = QString("select id,devType,logId,devNo,from_unixtime(datetime) as dt from HislogTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3%4%5 order by datetime desc limit %6,%7").arg(lowerDate,upperDate,typeSql,levelSql,devNoSql,QString::number(limitL),QString::number(PAGENUM));
    qDebug()<<"query historyData sql ="<<sql;
        QSqlQuery query(db);
        bool buscess = query.exec(sqlCount);
        if (!buscess){
            emit sendAlmHistoryData(m_almData);
            return ret;
        }

        //计算数据页数
        while(query.next()){
            m_almData.pageCount = (query.value("almCount").toInt()+PAGENUM-1)/PAGENUM;
        }

        //获取前20条

        if(m_almData.pageCount>0){
            QSqlQuery query(db);
            bool buscess = query.exec(sql);
            if (!buscess){
                emit sendAlmHistoryData(m_almData);
                return ret;
            }

            //遍历取得的所有结果
            int devType,logID,devNo;
            while(query.next()){
                //almInfo.id = query.value("id").toInt();
                devType = query.value("devType").toInt();
                logID = query.value("logId").toInt();
                devNo = query.value("devNo").toInt();
                almInfo.parts = QString::number(devNo)+"#"+AlarmDevTypetoString(devType);
                almInfo.alarm_code =AlarmLogIDtoString(logID,ExtDevAlmEnum(devType));
                almInfo.datetime = query.value("dt").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                m_almData.almList.append(almInfo);

            }

            //
        }
        ret = true;
        //qDebug()<<"query OK....";
        emit sendAlmHistoryData(m_almData);

        return ret;

}

void readDatabase::readAllHistoryData(int type, QVector<int> devNoList, QDateTime lower, QDateTime upper, int page)
{
    if(!m_DBConn)
        return ;
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }
    int limitL=0;
    QString upperDate,lowerDate,typeSql,levelSql,devNoSql;
    limitL =(page-1)*PAGENUM;


    if(!upper.isNull() and !lower.isNull()){
        upperDate = upper.toString("yyyy-MM-dd hh:mm:ss");
        lowerDate = lower.toString("yyyy-MM-dd hh:mm:ss");
    }
    //以下参数值为-1为查询所有

    if(devNoList.length() == 0){
        devNoSql = "";
    }else {
        devNoSql += " and(";
        foreach (int devNo, devNoList) {
            devNoSql += "devNo="+QString::number(devNo)+" or ";
        }
        devNoSql = devNoSql.left(devNoSql.length()-4)+")";
    }

    //根据设备类型查询
    //qDebug()<<type<<__LINE__;
    switch (type) {
        case DevDataEnum::SYSTEM:
            readStatsHistoryData(devNoSql,lowerDate,upperDate,limitL,PAGENUM);
            break;
        case DevDataEnum::PUE:
            readPueHistoryData(devNoSql,lowerDate,upperDate,limitL,PAGENUM);
            break;
        case DevDataEnum::THS:            
            readThsHistoryData(devNoSql,lowerDate,upperDate,limitL,PAGENUM);
            break;
        case DevDataEnum::UPS:
            readUpsHistoryData(devNoSql,lowerDate,upperDate,limitL,PAGENUM);
            break;
        case DevDataEnum::AC:
            readAcHistoryData(devNoSql,lowerDate,upperDate,limitL,PAGENUM);
            break;
        case DevDataEnum::METER:
            readMeterHistoryData(devNoSql,lowerDate,upperDate,limitL,PAGENUM);
            break;
    }


}

void readDatabase::dataSaveLocChange(bool isSDCard)
{
    qDebug()<<__LINE__<<"dataSaveLoc change "<<isSDCard;
    //关闭数据库
    if(db.isOpen())
        db.close();
    //重新设置数据库
    if(isSDCard == true)
        db.setDatabaseName("eMtrDbSD");
    else {
        db.setDatabaseName("eMtrDb");
    }
    //打开数据库
    bool ok = db.open();
    if (ok)
    {
        qDebug()<<__LINE__<<"open db ok";
        m_DBConn = true;
    }
    else
    {
        qDebug()<<__LINE__<<"open db fial";
        m_DBConn = false;
    }


}


bool readDatabase::readStatsHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL, int pageNum)
{

    qDebug()<<"system in";
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }
    qDebug()<<"query start stats";
    stats_data statsdata;
    m_statsData.dataList.clear();
    m_statsData.pageCount = 0;

    QString sqlCount = QString("select count(id) as statsCount from StatsDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'").arg(lowerDate,upperDate);

    QString sqlQuery = QString("select id,tmpAvg,frontTmp,frontHum,backTmp,backHum,ambTmp,ambHum,from_unixtime(datetime) as dt from StatsDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2' order by datetime desc limit %3,%4").arg(lowerDate,upperDate,QString::number(limitL),QString::number(pageNum));
                                                                                                            //by id desc
    qDebug()<<"historySql = "<<sqlQuery;
    QSqlQuery query(db);
    bool buscess = query.exec(sqlCount);
    if (!buscess){
        emit sendStatsHistoryData(m_statsData);   //发送空数据
        return false;
    }

    //计算数据页数
    while(query.next()){
        m_statsData.pageCount = (query.value("statsCount").toInt()+pageNum-1)/pageNum;
    }

    //获取前20条
    if(m_statsData.pageCount > 0){
        QSqlQuery query(db);
        bool buscess = query.exec(sqlQuery);
        if (!buscess){
            emit sendStatsHistoryData(m_statsData);   //发送空数据
            return false;
        }

        //遍历取得的所有结果
        while(query.next()){

            if(statsDataList.length() == statsDataNum){
                for(int i=0;i<statsDataList.length();++i){  //0 = dev 1=datetime
                    if(i == StatsDataEnum::stats_datetime){
                        statsdata.data[i] = query.value("dt").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    }else {
                        statsdata.data[i] = query.value(statsDataList.at(i).second).toString();
                    }

                }
                m_statsData.dataList.append(statsdata);
            }


        }

        //
        emit sendStatsHistoryData(m_statsData);   //发送系统状态信息

        return true;
    }else{  //查询数据为空
        emit sendStatsHistoryData(m_statsData);   //发送空数据
        return false;
    }

}

bool readDatabase::readPueHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL, int pageNum)
{
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }

    pue_data puedata;
    m_pueData.dataList.clear();
    m_pueData.pageCount = 0;

    QString sqlCount = QString("select count(id) as dataCount from PueDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'").arg(lowerDate,upperDate);

    QString sqlQuery = QString("select id,pue,from_unixtime(datetime) as dt from PueDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2' order by datetime desc limit %3,%4").arg(lowerDate,upperDate,QString::number(limitL),QString::number(pageNum));
    qDebug()<<"historySql = "<<sqlQuery;
    QSqlQuery query(db);
    bool buscess = query.exec(sqlCount);
    if (!buscess){
        emit sendPueHistoryData(m_pueData);   //发送空数据
        return false;
    }

    //计算数据页数
    while(query.next()){
        m_pueData.pageCount = (query.value("dataCount").toInt()+pageNum-1)/pageNum;
    }

    //获取前20条
    if(m_pueData.pageCount > 0){
        QSqlQuery query(db);
        bool buscess = query.exec(sqlQuery);
        if (!buscess){
            emit sendPueHistoryData(m_pueData);   //发送空数据
            return false;
        }

        //遍历取得的所有结果
        while(query.next()){
            if(pueDataList.length() == pueDataNum){
                for(int i=0;i<pueDataList.length();++i){  //0 = dev 1=datetime
                    if(i == PueDataEnum::pue_datetime){
                        puedata.data[i] = query.value("dt").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    }else {
                        puedata.data[i] = query.value(pueDataList.at(i).second).toString();
                    }

                }
                m_pueData.dataList.append(puedata);
            }
        }


        emit sendPueHistoryData(m_pueData);   //发送系统状态信息
        return true;
    }else{  //查询数据为空
        emit sendPueHistoryData(m_pueData);   //发送空数据
        return false;
    }

}

bool readDatabase::readThsHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL, int pageNum)
{
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }

    ths_data thsdata;
    m_thsData.dataList.clear();
    m_thsData.pageCount = 0;

    QString sqlCount = QString("select count(id) as dataCount from ThsHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3").arg(lowerDate,upperDate,devNoSql);

    QString sqlQuery = QString("select id,devNo,hum,tmp,from_unixtime(datetime) as dt from ThsHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3 order by datetime desc limit %4,%5").arg(lowerDate,upperDate,devNoSql,QString::number(limitL),QString::number(pageNum));
    qDebug()<<"historySql = "<<sqlCount;
    QSqlQuery query(db);
    bool buscess = query.exec(sqlCount);
    if (!buscess){
        emit sendThsHistoryData(m_thsData);   //发送空数据
        return false;
    }

    //计算数据页数
    while(query.next()){
        m_thsData.pageCount = (query.value("dataCount").toInt()+pageNum-1)/pageNum;
    }

    //获取前20条
    if(m_thsData.pageCount > 0){
        QSqlQuery query(db);
        bool buscess = query.exec(sqlQuery);
        if (!buscess){
            emit sendThsHistoryData(m_thsData);  //发送空数据
            return false;
        }

        //遍历取得的所有结果
        while(query.next()){
            if(thsDataList.length() == thsDataNum){
                for(int i=0;i<thsDataList.length();++i){  //0 = dev 1=datetime
                    if(i == ThsDataEnum::ths_devNo){
                        thsdata.data[i] = QString::number(query.value("devNo").toInt())+"#"+langN.show[langEnum::strHumiture];
                    }else if(i == ThsDataEnum::ths_datetime){
                        thsdata.data[i] = query.value("dt").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    }else {
                        thsdata.data[i] = query.value(thsDataList.at(i).second).toString();
                    }

                }

                m_thsData.dataList.append(thsdata);
            }

        }

        //
        emit sendThsHistoryData(m_thsData);   //发送系统状态信息
        return true;
    }else{  //查询数据为空
        emit sendThsHistoryData(m_thsData);   //发送空数据
        return false;
    }

}

bool readDatabase::readMeterHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL, int pageNum)
{
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }

    meter_data meterdata;
    m_meterData.dataList.clear();
    m_meterData.pageCount = 0;


    QString sqlCount = QString("select count(id) as dataCount from MeterHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3").arg(lowerDate,upperDate,devNoSql);

    QString sqlQuery = QString("select *,from_unixtime(datetime) as dt from MeterHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3 order by datetime desc limit %4,%5").arg(lowerDate,upperDate,devNoSql,QString::number(limitL),QString::number(pageNum));
    qDebug()<<"historySql = "<<sqlCount;
    QSqlQuery query(db);
    bool buscess = query.exec(sqlCount);
    if (!buscess){
        emit sendMeterHistoryData(m_meterData);   //发送空数据
        return false;
    }

    //计算数据页数
    while(query.next()){
        m_meterData.pageCount = (query.value("dataCount").toInt()+pageNum-1)/pageNum;
    }

    //获取前20条
    if(m_meterData.pageCount > 0){
        QSqlQuery query(db);
        bool buscess = query.exec(sqlQuery);
        if (!buscess){
            emit sendMeterHistoryData(m_meterData);  //发送空数据
            return false;
        }

        //遍历取得的所有结果他
        while(query.next()){
            if(meterDataList.length() == meterDataNum){
                for(int i=0;i<meterDataList.length();++i){  //0 = dev 1=datetime
                    if(i == MeterDataEnum::meter_devNo){
                        meterdata.data[i] = QString::number(query.value("devNo").toInt())+"#"+langN.show[langEnum::strVMeter];
                    }else if(i == MeterDataEnum::meter_datetime){
                        meterdata.data[i] = query.value("dt").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    }else {
                        meterdata.data[i] = query.value(meterDataList.at(i).second).toString();
                    }

                }

                m_meterData.dataList.append(meterdata);
            }
            //------------------------------------
        }

        //
        emit sendMeterHistoryData(m_meterData);   //发送系统状态信息
        return true;
    }else{  //查询数据为空
        emit sendMeterHistoryData(m_meterData);   //发送空数据
        return false;
    }

}

bool readDatabase::readAcHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL, int pageNum)
{
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }

    ac_data acdata;
    m_acData.dataList.clear();
    m_acData.pageCount = 0;


    QString sqlCount = QString("select count(id) as dataCount from AcHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3").arg(lowerDate,upperDate,devNoSql);

    QString sqlQuery = QString("select *,from_unixtime(datetime) as dt from AcHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3 order by datetime desc limit %4,%5").arg(lowerDate,upperDate,devNoSql,QString::number(limitL),QString::number(pageNum));
    qDebug()<<"historySql = "<<sqlQuery;
    QSqlQuery query(db);
    bool buscess = query.exec(sqlCount);
    if (!buscess){
        emit sendAcHistoryData(m_acData); //发送空数据
        return false;
    }

    //计算数据页数
    while(query.next()){
        m_acData.pageCount = (query.value("dataCount").toInt()+pageNum-1)/pageNum;
    }

    //获取前20条
    if(m_acData.pageCount > 0){
        QSqlQuery query(db);
        bool buscess = query.exec(sqlQuery);
        if (!buscess){
            emit sendAcHistoryData(m_acData);  //发送空数据
            return false;
        }

        //遍历取得的所有结果
        while(query.next()){
            if(acDataList.length() == acDataNum){
                for(int i=0;i<acDataList.length();++i){
                    if(i == AcDataEnum::ac_devNo){
                        acdata.data[i] = QString::number(query.value("devNo").toInt())+"#"+langN.show[langEnum::strAc];
                    }else if(i == AcDataEnum::ac_datetime){
                        acdata.data[i] = query.value("dt").toDateTime().toString("yyyy-MMdd hh:mm:ss");
                    }else {
                        acdata.data[i] = query.value(acDataList.at(i).second).toString();
                    }

                }

                m_acData.dataList.append(acdata);
            }
            //------------------------------------
        }

        //
        emit sendAcHistoryData(m_acData);   //发送系统状态信息
        return true;
    }else{  //查询数据为空
        emit sendAcHistoryData(m_acData);   //发送空数据
        return false;
    }
}

bool readDatabase::readUpsHistoryData(QString devNoSql, QString lowerDate, QString upperDate, int limitL, int pageNum)
{
    //长时间不查询数据库会断开连接，重新打开数据库
    if(!db.isOpen()){
        db.open();
    }

    ups_data upsdata;
    m_upsData.dataList.clear();
    m_upsData.pageCount = 0;


    QString sqlCount = QString("select count(id) as dataCount from UpsHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3").arg(lowerDate,upperDate,devNoSql);

    QString sqlQuery = QString("select *,from_unixtime(datetime) as dt from UpsHisDataTb "
                          "Where  from_unixtime(datetime) > '%1' and from_unixtime(datetime)<'%2'%3 order by datetime desc limit %4,%5").arg(lowerDate,upperDate,devNoSql,QString::number(limitL),QString::number(pageNum));
    qDebug()<<"historySql = "<<sqlCount;
    QSqlQuery query(db);
    bool buscess = query.exec(sqlCount);
    if (!buscess){
        emit sendUpsHistoryData(m_upsData);   //发送空数据
        return false;
    }

    //计算数据页数
    while(query.next()){
        m_upsData.pageCount = (query.value("dataCount").toInt()+pageNum-1)/pageNum;
    }

    //获取前20条
    if(m_upsData.pageCount > 0){
        QSqlQuery query(db);
        bool buscess = query.exec(sqlQuery);
        if (!buscess){
            emit sendUpsHistoryData(m_upsData);  //发送空数据
            return false;
        }

        //遍历取得的所有结果
        while(query.next()){
            if(uDataList.length() == upsDataNum){
                for(int i=0;i<uDataList.length();++i){  //0 = dev 1=datetime
                    if(i == UpsDataEnum::ups_devNo){
                        upsdata.data[i] = QString::number(query.value("devNo").toInt())+"#UPS";
                    }else if(i == UpsDataEnum::ups_datetime){
                        upsdata.data[i] = query.value("dt").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    }else {
                        upsdata.data[i] = query.value(uDataList.at(i).second).toString();
                    }

                }

                m_upsData.dataList.append(upsdata);
            }
            //------------------------------------
        }

        //
        emit sendUpsHistoryData(m_upsData);   //发送系统状态信息
        return true;
    }else{  //查询数据为空
        emit sendUpsHistoryData(m_upsData);   //发送空数据
        return false;
    }
}



