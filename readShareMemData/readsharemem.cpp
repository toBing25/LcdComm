#include "readsharemem.h"
//#include "Global/AlarmStruct.h"
#include "Global/AlarmMng.h"
#include "util/conversiontool.h"
#include "Global/SysCfgMng.h"
#include "ThsData/ThsDataStruct.h"
#include "ACData/ACDataStruct.h"
#include "UPSData/UPSDataStruct.h"
#include "DCData/CabCtrlDataStruct.h"
#include "Global/RatioStruct.h"
#include "formmain.h"
#include <QMetaType>
#include <QDebug>
#include <QMutex>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFileInfo>
#include<stdlib.h>

//注册，以便发送自定义结构体信号
Q_DECLARE_METATYPE(alarm_list);
Q_DECLARE_METATYPE(AlmLevelNumStruct);
Q_DECLARE_METATYPE(ths_Info);
Q_DECLARE_METATYPE(ac_Info);
Q_DECLARE_METATYPE(ups_Info);
Q_DECLARE_METATYPE(meter_Info);
Q_DECLARE_METATYPE(leak_Info);
Q_DECLARE_METATYPE(spds_Info);
Q_DECLARE_METATYPE(door_Info);

#define meterProtoPath "/opt/eMTR/uartExt/meter/"
ReadShareMem::ReadShareMem():
    isStop(false),
    m_dataSaveSDCard(false),
    m_iCurrMeterProto(-1),
    m_iCurrSpdsProto(-1)
{
    //初始化控制模块数据
    FormMain::m_dcDev.dcLen = 0;
    FormMain::m_dcDev.dcList.clear();



    //获取报警历史数据
    m_pThread_ReadData = new QThread();
    m_pReadData = new readDatabase();
    m_pReadData->moveToThread(m_pThread_ReadData);
    m_pThread_ReadData->start();
    //写数据类
    m_pThread_WriteData = new QThread();
    m_pWriteData = new WtriteShareMem();
    m_pWriteData->moveToThread(m_pThread_WriteData);
    m_pThread_WriteData->start();



    //初始化刷卡数据文件
    m_dcJson = new JsonFile("/etc/.eMTR/web/accessList.txt");
    //m_dcJson = new JsonFile("/home/xin/accessList.txt");
    m_initJsonOK = initCardAccessJson();
    pLcdData->para.LcdCfgCardUpdate = 0;
    if(m_initJsonOK){

    }else{
        //qDebug()<<__LINE__<<tr("门卡数据库文件错误");
    }

    //初始化电表协议文件
    bool ok = initMeterJsonData(QString(meterProtoPath)+"meter.cfg",m_meterProList);
    //初始化spds协议文件
    ok  = initMeterJsonData(QString(SPDS_CFG_PATH)+"spds.cfg",m_spdsProList);


    connect(this,&ReadShareMem::sendDataSaveLocChange,m_pReadData,&readDatabase::dataSaveLocChange);

}

void ReadShareMem::closeThread()
{
    isStop = true;
}

void ReadShareMem::run()
{
    int i = 0;
    while (1) {
        if(isStop)
            return;
        i++;
        //读主配置信息

        readDCDevData();

        msleep(1000);

        if(i >= 3){
            i=0;
            readMtrData();

        }


    }
}


void ReadShareMem::changeCard(QString cardNo, QString cardName, QStringList dcList)
{
    qDebug()<<__LINE__ <<cardNo <<  cardName<< dcList;
    m_dcJson->setValue(cardNo,cardName,dcList);
    pLcdData->para.LcdCfgCardUpdate = 1;    //通知门禁更新数据库
    m_dcJson->init();
    emit sendChageCardDone();

}

void ReadShareMem::removeCard(QString cardNo)
{
    m_dcJson->removeValue(cardNo);
    pLcdData->para.LcdCfgCardUpdate = 1;    //通知门禁更新数据库
    m_dcJson->init();
    emit sendChageCardDone();
}

void ReadShareMem::readMtrData()
{
    almItemList.clear();

    double anaPueV          = eMtrData->ana[eMtrAnaPue] * extDevRatio.PUERatio;
    double anaColdHumAvg    = eMtrData->ana[eMtrAnaColdHumAvg] * extDevRatio.eThsRatio[eThsParaHum];
    double anaHotHumAvg     = eMtrData->ana[eMtrAnaHotHumAvg] * extDevRatio.eThsRatio[eThsParaHum];

    double anaColdTmpAvg    = eMtrData->ana[eMtrAnaColdTmpAvg] * extDevRatio.eThsRatio[eThsParaTmp];
    double anaHotTmpAvg     = eMtrData->ana[eMtrAnaHotTmpAvg] * extDevRatio.eThsRatio[eThsParaTmp];

    int c_min, c_max, h_min, h_max;
    c_min  = eMtrData->ana[eMtrAnaColdTmpMin];
    c_max  = eMtrData->ana[eMtrAnaColdTmpMax];

    h_min  = eMtrData->ana[eMtrAnaHotTmpMin];
    h_max  = eMtrData->ana[eMtrAnaHotTmpMax];


    emit sendeMtrPueValue(anaPueV);
    emit sendeMtrColdOrHotHumAvg(true,  anaColdTmpAvg, anaColdHumAvg);
    emit sendeMtrColdOrHotHumAvg(false, anaHotTmpAvg, anaHotHumAvg);

    //qDebug()<<__LINE__<<"pue"<<anaPueV <<"coldavg"<<anaColdHumAvg <<"hotavg"<<anaHotHumAvg;
    readSyscfgData();
    readCurrentAlarm();
    readThsDevData();
    readACDevData();
    readUPSDevData();
    readMeterDevData();
    readLeakDevData();
    readSpdsDevData();
    readSMSDevData();
}

void ReadShareMem::readSyscfgData()
{
    if(m_dataSaveSDCard != sysCfg->eMtrMain.sysSet.dateSaveLoc){
        m_dataSaveSDCard = sysCfg->eMtrMain.sysSet.dateSaveLoc;
        emit sendDataSaveLocChange(m_dataSaveSDCard);
    }

}

void ReadShareMem::readCurrentAlarm()
{
    bool IsNewAlm = false;
    if(eMtrData->ctrl.muteOnOffSts == 1){   //没有新告警不用发送告警消息给界面
        //
    }
    //告警消息
    qint16 currAlmNum = eMtrData->currAlm.num;
    //qDebug()<<__LINE__<<"read currAlmnum"<<eMtrData->currAlm.num;
    for(int i=0;i<currAlmNum;i++){
        eMtrAlmItemStruct almItem = eMtrData->currAlm.item[i];

    }

    int almEvent = 0,almSame = 0,almCritical = 0,almTotalNum = 0;

    almTotalNum = eMtrData->currAlm.num;    
    AlmLevel almLevelNum;
    almLevelNum.almSame =0;
    almLevelNum.almEvent =0;
    almLevelNum.almCritical =0;
    almLevelNum.almTotalNum =0;
    QList<int> doorAlmList;
    if(almTotalNum > 0){
        for(int i=0;i<almTotalNum;i++){
            //统计不同级别报警数量
            int almlevel = eMtrData->currAlm.item[i].level;
            switch (almlevel) {
            case 0:
                almEvent++;
                break;
            case 1:
                almSame++;
                break;
            case 2:
                almCritical++;
                break;
            default:
                break;
            }

            //
            int Alarmlevel = eMtrData->currAlm.item[i].level;
            quint16 logID = eMtrData->currAlm.item[i].almId.logId;
            quint16 devNo = eMtrData->currAlm.item[i].almId.devNo;
            quint16 devType = eMtrData->currAlm.item[i].almId.devType;
            quint16 lowBit = eMtrData->currAlm.item[i].dAndT.lsw;
            quint16 hightBit = eMtrData->currAlm.item[i].dAndT.msw;

            //筛选接线盒前后门告警
            if(devType == ExtDevAlmEnum::eAlmDevDc and (logID >=4 and logID <=7)){
                  qDebug()<<"------------door"<<logID;
                  doorAlmList.append(logID);
            }

            alarm_list aList;
            aList.datetime      = ConversionTool::getTimeFrom16Bit(lowBit,hightBit);
            aList.alarm_code    = AlarmLogIDtoString(logID,ExtDevAlmEnum(devType));
            aList.parts         = QString::number(devNo)+"#"+AlarmDevTypetoString(devType);
            aList.Alarmlevel    = Alarmlevel;


            almItemList.append(aList);

        }
        std::sort(almItemList.begin(),almItemList.end(),[](alarm_list &date1,alarm_list &date2){
            return date1.datetime > date2.datetime;
        });


        almLevelNum.almSame = almSame;
        almLevelNum.almEvent = almEvent;
        almLevelNum.almCritical = almCritical;
        almLevelNum.almTotalNum = almTotalNum;

        //qDebug()<<"send almNum";

    }else { //告警为空也发送空list过去更新界面
        almItemList.clear();
    }
    //发送告警消息列表给主界面类
    emit sendAlmNumCount(almLevelNum);
    emit sendeMtrAlmItemList(almItemList);

    emit sendDoorStsChange(doorAlmList,almLevelNum.almTotalNum);
}

void ReadShareMem::readThsDevData()
{
    QList<int> id_list;
    //获取温湿度设备个数
    int ths_dev = sysCfg->uartExt.devNum[eExtDevThs];
    if(ths_dev > 0)
    {
        //获取每个设备的ID        
        for (int i = 0 ; i < ths_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevThs][i];
        }
    }else {
        return ;
    }


    //初始化空设备
    ths_Info nullthsInfo;
    initThsDev(nullthsInfo);

    QList<ths_Info> thsList;
    //获取温湿度值
    for(int i=0;i<id_list.length();i++){
        ths_Info thsInfo;

        //连接状态
        uint16 flag = (pThsData+id_list.at(i))->connectFlag;

        thsInfo.connect = flag;
        thsInfo.devID   = id_list.at(i) + 1;

        if (thsInfo.connect == CONNECTED){  //未连接不读取数据
            //温湿度数据
            for(int j=0;j<eThsParaNum;j++){
              //thsInfo.data[j] = (pThsData+id_list.at(i))->para.data[j] * extDevRatio.eThsRatio[j];  //乘以系数表
               uint16 thsPara = (pThsData+id_list.at(i))->para.data[j];

               if(j == eThsParaAlmSts){     //告警状态处理
                   //qDebug()<<__FILE__<<__LINE__<<thsInfo.devID  <<QString::number(thsPara,2);

                   for(int i=0;i<thsParabitMax;i++){
                       if(ConversionTool::getBit(thsPara,i)){   //有告警
                           if(i == 0){
                                thsInfo.almSts =1;
                           }else {
                               if(thsInfo.eThsParaAlmSts.length()>0){ thsInfo.eThsParaAlmSts += "/"; }
                                thsInfo.eThsParaAlmSts += extDevShow.eThsParaShow[eThsParaAlmSts][i];

                           }
                       }else {
                           if(i == 0){
                               thsInfo.eThsParaAlmSts = extDevShow.eThsParaShow[eThsParaAlmSts][thsParabitMax-1];  //无告警
                               thsInfo.almSts = 0;
                               break;
                           }
                       }
                   }

               }else{   //添加系数和单位
                   thsInfo.data[j] = thsPara * extDevRatio.eThsRatio[j];
                   thsInfo.dataUnit[j] = extDevShow.eThsParaUnit[j];
               }

            }
        }else {
            nullthsInfo.devID = thsInfo.devID;
            nullthsInfo.connect = thsInfo.connect;
            thsInfo = nullthsInfo;
        }


        thsList.append(thsInfo);

    }
    //发送温湿度列表消息
    emit sendeExtDevThsList(thsList);
}

void ReadShareMem::readACDevData()
{
    MenuAndClassList *pAc_List = &FormMain::Instance()->m_acList;
    QList<int> id_list;
    //获取空调设备个数
    int ac_dev = sysCfg->uartExt.devNum[eExtDevAc];
    if(ac_dev > 0)
    {
        //获取每个设备的ID
        for (int i = 0 ; i < ac_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevAc][i];
        }
    }else {
        return ;
    }
    //初始化空设备
    ac_Info nullacInfo;
    initACDev(nullacInfo);

    QList<ac_Info> acList;
    //读取空调设备设备数据
    for(int i=0;i<id_list.length();i++){
        //ExtDeviceHeadStruct acHead = sysCfg->uartExt.extDevHead[eExtDevAc][id_list.at(i)];
        //qDebug()<<"ac proto = "<<acHead.extProto;
            ac_Info acInfo;
            QString connectFlag = "";
            //连接状态
            uint16 flag = (pAcData+id_list.at(i))->connectFlag;
            acInfo.connect = flag;
            acInfo.devID   = id_list.at(i) + 1;
            acInfo.almSts  = 0; //告警正常
            if (acInfo.connect == CONNECTED){
                QString paraName;
                for(int x=0;x<pAc_List->menuList.size();++x){
                    paraName = pAc_List->menuList.at(i).parameter_name;

                    if(pAc_List->menuList.at(x).enumType == AcRunTypeEnum::AcRunStatus){
                        int16 acSts = (pAcData+id_list.at(i))->acRunSts.data[statusMap[paraName]]; //状态不需要乘以系数
                        if(acSts < acStsbitMax){
                            //防止越界，显示状态内容
                            acInfo.stsData[statusMap[paraName]] = extDevShow.eAcStsShow[statusMap[paraName]][acSts];
                        }else{
                            //显示原始状态值
                            acInfo.stsData[statusMap[paraName]] = QString::number(acSts);
                        }
                    }else if(pAc_List->menuList.at(x).enumType == AcRunTypeEnum::AcRunPara){
                        double acPara = (pAcData+id_list.at(i))->acRunPara.data[paraMap[paraName]] * extDevRatio.eAcParaRatio[paraMap[paraName]];
                        int showType = pAc_List->menuList.at(i).show_type;
                        if (SHOWTYPE_LINEEDIT == showType || SHOWTYPE_BIT == showType)
                            acInfo.paraData[statusMap[paraName]] = QString::number(acPara);
                        else if (SHOWTYPE_BTN == showType || SHOWTYPE_COMBOBOX == showType)
                            //acInfo.paraData[statusMap[paraName]] = QString::number(int16(acPara), 16);
                            acInfo.paraData[statusMap[paraName]] = QString::number(acPara);

                        acInfo.paraUnit[statusMap[paraName]] = extDevShow.eAcParaUnit[paraMap[paraName]];

                        //个别项目处理
                        //日期    //低8位有效
                        if(RU_DATE_YY == paraMap[paraName]){

                            int dYY = 2000 +  ConversionTool::get8BitL((pAcData + id_list.at(0))->acRunPara.data[RU_DATE_YY]) * extDevRatio.eAcParaRatio[RU_DATE_YY];
                            int dMM = ConversionTool::get8BitL((pAcData + id_list.at(0))->acRunPara.data[RU_DATE_MM]) * extDevRatio.eAcParaRatio[RU_DATE_MM];
                            int dDD = ConversionTool::get8BitL((pAcData + id_list.at(0))->acRunPara.data[RU_DATE_DD]) * extDevRatio.eAcParaRatio[RU_DATE_DD];
                            int tHH = ConversionTool::get8BitL((pAcData + id_list.at(0))->acRunPara.data[RU_TIME_HH]) * extDevRatio.eAcParaRatio[RU_TIME_HH];
                            int tMM = ConversionTool::get8BitL((pAcData + id_list.at(0))->acRunPara.data[RU_TIME_MM]) * extDevRatio.eAcParaRatio[RU_TIME_MM];
                            int tSS = ConversionTool::get8BitL((pAcData + id_list.at(0))->acRunPara.data[RU_TIME_SS]) * extDevRatio.eAcParaRatio[RU_TIME_SS];
                            QString timeStr = QString("%1-%2-%3 %4:%5:%6")
                                    .arg(dYY)
                                    .arg(dMM, 2, 10, QLatin1Char('0'))
                                    .arg(dMM, 2, 10, QLatin1Char('0'))
                                    .arg(dDD, 2, 10, QLatin1Char('0'))
                                    .arg(tHH, 2, 10, QLatin1Char('0'))
                                    .arg(tMM, 2, 10, QLatin1Char('0'))
                                    .arg(tSS, 2, 10, QLatin1Char('0'));
                            qDebug()<<"timeStr"<<timeStr;
                            acInfo.paraData[paraMap[paraName]] = timeStr;
                        //状态量
                        }else if(IR_CTL_WAYS == paraMap[paraName] ){
                            int16 acS = int16(acPara);
                            if(int16(acPara) < acParabitMax){
                               acInfo.paraData[paraMap[paraName]] = extDevShow.eAcParaShow[IR_CTL_WAYS_STS][acS];
                            }
                        }else if(IR_SYS_RUN_STS == paraMap[paraName] ){
                            int16 acS = int16(acPara);
                            if(int16(acPara) < acParabitMax){
                               acInfo.paraData[paraMap[paraName]] = extDevShow.eAcParaShow[IR_SYS_RUN_STS_STS][acS];
                            }
                        }else if(IR_SYS_RUN_MOD == paraMap[paraName] ){
                            int16 acS = int16(acPara);
                            if(int16(acPara) < acParabitMax){
                               acInfo.paraData[paraMap[paraName]] = extDevShow.eAcParaShow[IR_SYS_RUN_MOD_STS][acS];
                            }
                        }
                    }
                }

//                //空调状态数据
//                for(int j=0;j<eAcStsNum;j++){
//                    uint16 acSts = (pAcData+id_list.at(i))->acRunSts.data[j];  //状态不用乘以系数
//                    if(j == eAcStsIntegratedAlm){ acInfo.almSts = acSts; }  //告警状态

//                    QString showStr = "";
//                    if(acSts < acStsbitMax){
//                        showStr = extDevShow.eAcStsShow[j][acSts];
//                    }
//                    if(showStr == ""){
//                        acInfo.stsData[j] = QString::number(acSts);
//                    }else {
//                        acInfo.stsData[j] = showStr;
//                    }
//                }

//                //空调参数数据
//                for(int j=0;j<eAcParaNum;j++){
//                    double acPara = (pAcData+id_list.at(i))->acRunPara.data[j] * extDevRatio.eAcRatio[j];  //乘以系数
//                    if(j == eAcParaRunSts or j== eAcParaCntlWays){  //运行状态和控制方式特别处理
//                        if(acPara < acParabitMax)
//                            acInfo.paraData[j] = extDevShow.eAcParaShow[j][int(acPara)];
//                    }else{
//                        if(!extDevShow.eAcParaUnit[j].isEmpty()){
//                            acInfo.paraData[j] = QString::number(acPara);
//                            //单位
//                            acInfo.paraUnit[j] = extDevShow.eAcParaUnit[j];

//                        }
//                    }
//                }
//                //空调参数eAcParaRunSts属于状态
            }else {
                nullacInfo.devID = acInfo.devID;
                nullacInfo.connect = acInfo.connect;
                acInfo = nullacInfo;
            }

            acList.append(acInfo);
    }

//    //发送空调列表消息
    emit sendeExtDevAcList(acList);
}

void ReadShareMem::readUPSDevData()
{
    QList<int> id_list;
    //获取UPS设备个数
    int ac_dev = sysCfg->uartExt.devNum[eExtDevUps];
    if(ac_dev > 0)
    {
        //获取每个设备的ID
        for (int i = 0 ; i < ac_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevUps][i];
        }
    }else {
        return ;
    }

    double currPer = 0;
    //初始化空设备
    ups_Info nullupsInfo;
    initUPSDev(nullupsInfo);

    QList<ups_Info> upsList;
    //获取UPS设备数据
    for(int i=0;i<id_list.length();i++){
        ups_Info upsInfo;
        //连接状态
        uint16 flag = (pUpsData+id_list.at(i))->connectFlag;        
        upsInfo.connect = flag;
        upsInfo.devID   = id_list.at(i) + 1;
        if (upsInfo.connect == CONNECTED){
            //UPS状态数据
            for(int j=0;j<eUpsStsNum;j++){
            uint16 upsSts = (pUpsData+id_list.at(i))->sts[j];  //状态不用乘以系数
            if(j == eUpsStsGeneralAlm){
                upsInfo.almSts = upsSts-1;
                //qDebug()<<"ups almSts"<<upsInfo.almSts;
            }     //告警状态

            QString showStr = "";
            if(upsSts < upsStsbitMax)
                showStr = extDevShow.eUpsStsShow[j][upsSts];
            if(showStr == ""){
                upsInfo.sts[j] = QString::number(upsSts);
            }else{
                upsInfo.sts[j] = extDevShow.eUpsStsShow[j][upsSts];
            }

            }
            //UPS参数数据
            for(int j=0;j<eUpsParaNum;j++){
                //电流有负数，用int16转换
                double upsPara = 0;
                if(j == eUpsParaBattVoltNeg or j == eUpsParaBattCurrNeg){
                    upsPara = int16((pUpsData+id_list.at(i))->para[j]) * extDevRatio.eUpsRatio[j];  //乘以系数
                }else{
                    upsPara = (pUpsData+id_list.at(i))->para[j] * extDevRatio.eUpsRatio[j];  //乘以系数
                }

                //ups负载放到首页
                if(j == eUpsParaLdPerPhA or j ==eUpsParaLdPerPhB or j ==eUpsParaLdPerPhC){
                    if(upsPara>currPer){
                        currPer = upsPara;
                    }
                    if(j == eUpsParaLdPerPhC and currPer >0){
                        emit sendeMtrPerValue(currPer);
                    }

                }

                upsInfo.para[j] = QString::number(upsPara)/*+ " "+extDevShow.eUpsParaUnit[j]*/;
                //单位
                upsInfo.paraUnit[j] = extDevShow.eUpsParaUnit[j];
            }
            //UPS配置参数
            for(int j=0;j<eUpsCfgNum;j++){
                uint16 upsPara = (pUpsData+id_list.at(i))->cfg.data[j];
                if(j == eUpsCfgSysMode){
                    if(upsPara < upsCfgbitMax)  //防止越界
                        upsInfo.cfgData[j]  = extDevShow.eUpsCfgShow[j][upsPara];
                }else{
                    if(!extDevShow.eUpsCfgUnit[j].isEmpty()){
                        upsInfo.cfgData[j] = QString::number(upsPara);
                        //单位
                        upsInfo.cfgUnit[j] = extDevShow.eUpsCfgUnit[j];

                    }
                }

            }
            //UPS信息
            upsInfo.inf.MtrVer = ConversionTool::unicodeToQString((pUpsData+id_list.at(i))->inf.MtrVer,MAX_INF_LEN);
            upsInfo.inf.series = ConversionTool::unicodeToQString((pUpsData+id_list.at(i))->inf.series,MAX_INF_LEN);
            upsInfo.inf.modelName = ConversionTool::unicodeToQString((pUpsData+id_list.at(i))->inf.modelName,MAX_INF_LEN);
            upsInfo.inf.manufacturer = ConversionTool::unicodeToQString((pUpsData+id_list.at(i))->inf.manufacturer,MAX_INF_LEN);
            upsInfo.inf.ioType  = (pUpsData+id_list.at(i))->inf.ioType;

            //ABC三相一起显示，合并参数
            upsInfo.meterD.eUpsParaBypVoltPh = upsInfo.para[eUpsParaBypVoltPhA]+"/"+upsInfo.para[eUpsParaBypVoltPhB]+"/"+upsInfo.para[eUpsParaBypVoltPhC];
            upsInfo.meterD.eUpsParaBypCurrPh = upsInfo.para[eUpsParaBypCurrPhA]+"/"+upsInfo.para[eUpsParaBypCurrPhB]+"/"+upsInfo.para[eUpsParaBypCurrPhC];
            upsInfo.meterD.eUpsParaBypFreqPh = upsInfo.para[eUpsParaBypFreqPhA]+"/"+upsInfo.para[eUpsParaBypFreqPhB]+"/"+upsInfo.para[eUpsParaBypFreqPhC];
            upsInfo.meterD.eUpsParaBypPfPh   = upsInfo.para[eUpsParaBypPfPhA]+"/"+upsInfo.para[eUpsParaBypPfPhB]+"/"+upsInfo.para[eUpsParaBypPfPhC];
            upsInfo.meterD.eUpsParaIpVoltPh  = upsInfo.para[eUpsParaIpVoltPhA]+"/"+upsInfo.para[eUpsParaIpVoltPhB]+"/"+upsInfo.para[eUpsParaIpVoltPhC];
            upsInfo.meterD.eUpsParaIpCurrPh  = upsInfo.para[eUpsParaIpCurrPhA]+"/"+upsInfo.para[eUpsParaIpCurrPhB]+"/"+upsInfo.para[eUpsParaIpCurrPhC];
            upsInfo.meterD.eUpsParaIpFreqPh  = upsInfo.para[eUpsParaIpFreqPhA]+"/"+upsInfo.para[eUpsParaIpFreqPhB]+"/"+upsInfo.para[eUpsParaIpFreqPhC];
            upsInfo.meterD.eUpsParaIpPfPh    = upsInfo.para[eUpsParaIpPfPhA]+"/"+upsInfo.para[eUpsParaIpPfPhB]+"/"+upsInfo.para[eUpsParaIpPfPhC];
            upsInfo.meterD.eUpsParaOpVoltPh  = upsInfo.para[eUpsParaOpVoltPhA]+"/"+upsInfo.para[eUpsParaOpVoltPhB]+"/"+upsInfo.para[eUpsParaOpVoltPhC];
            upsInfo.meterD.eUpsParaOpCurrPh  = upsInfo.para[eUpsParaOpCurrPhA]+"/"+upsInfo.para[eUpsParaOpCurrPhB]+"/"+upsInfo.para[eUpsParaOpCurrPhC];
            upsInfo.meterD.eUpsParaOpFreqPh  = upsInfo.para[eUpsParaOpFreqPhA]+"/"+upsInfo.para[eUpsParaOpFreqPhB]+"/"+upsInfo.para[eUpsParaOpFreqPhC];
            upsInfo.meterD.eUpsParaOpPfPh    = upsInfo.para[eUpsParaOpPfPhA]+"/"+upsInfo.para[eUpsParaOpPfPhB]+"/"+upsInfo.para[eUpsParaOpPfPhC];
            upsInfo.meterD.eUpsParaOpKvaPh   = upsInfo.para[eUpsParaOpKvaPhA]+"/"+upsInfo.para[eUpsParaOpKvaPhB]+"/"+upsInfo.para[eUpsParaOpKvaPhC];
            upsInfo.meterD.eUpsParaOpKwPh    = upsInfo.para[eUpsParaOpKwPhA]+"/"+upsInfo.para[eUpsParaOpKwPhB]+"/"+upsInfo.para[eUpsParaOpKwPhC];
            upsInfo.meterD.eUpsParaLdPerPh   = upsInfo.para[eUpsParaLdPerPhA]+"/"+upsInfo.para[eUpsParaLdPerPhB]+"/"+upsInfo.para[eUpsParaLdPerPhC];

            //1: 3in3out; 2: 3in1out; 3: 1in1out; 4: 1in3out; 5: 2in2out
            switch (upsInfo.inf.ioType) {
                case 1:
                    showInputOutputABC(upsInfo,3,3);;
                    break;
                case 2:
                    showInputOutputABC(upsInfo,3,1);
                    break;
                case 3:
                    showInputOutputABC(upsInfo,1,1);
                    break;
                case 4:
                    showInputOutputABC(upsInfo,1,3);
                    break;
                case 5:
                    showInputOutputABC(upsInfo,2,2);
                    break;
                default:
                    break;
                }


        }else{
            nullupsInfo.devID   = upsInfo.devID;
            nullupsInfo.connect = upsInfo.connect;
            upsInfo = nullupsInfo;
        }

        upsList.append(upsInfo);
    }

    //发送UPS列表消息
    emit sendeExtDevUpsList(upsList);
}

void ReadShareMem::readMeterDevData()
{
    QList<int> id_list;
    //获取电量仪设备个数
    int ac_dev = sysCfg->uartExt.devNum[eExtDevMeter];
    if(ac_dev > 0)
    {
        //获取每个设备的ID
        for (int i = 0 ; i < ac_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevMeter][i];
        }
    }else {
        return ;
    }

    //获取电量仪协议
    //获取每个设备的基本信息
//    QList<ExtDeviceHeadStruct> thsHead_List;
//    foreach (int id, id_list) {
//        ExtDeviceHeadStruct thsHead = sysCfg->uartExt.extDevHead[eExtDevThs][id];
//        thsHead_List.append(thsHead);
//    }



    //初始化空设备
    meter_Info nullmeterInfo;
    initMeterDev(nullmeterInfo);

    QList<meter_Info> meterList;
    //获取电量仪设备数据
    for(int i=0;i<id_list.length();i++){
        //设备头信息,读取电表协议
        ExtDeviceHeadStruct meterHead = sysCfg->uartExt.extDevHead[eExtDevMeter][id_list.at(i)];
        if(meterHead.extProto != m_iCurrMeterProto){
            //加载协议文件
            if(m_meterProList.length()>0){
                m_currMeterProtoArr = findJsonDataArray(QString(meterProtoPath)+m_meterProList.at(meterHead.extProto),"para");
                //qDebug()<<m_meterProList.at(meterHead.extProto);
                m_iCurrMeterProto = meterHead.extProto;
            }

        }


        meter_Info meterInfo;
        //连接状态
        uint16 flag = (pMeterData+id_list.at(i))->connectFlag;        
        meterInfo.connect = flag;
        meterInfo.devID   = id_list.at(i) + 1;
        if (meterInfo.connect == CONNECTED){
            //电量仪参数数据
            double KWHHigh,KWHLow,KVarHHigh,KVarHLow;
            int meterArrLen = m_currMeterProtoArr.size();
            QJsonObject paraObj;
            for(int j=0;j<eMeterParaNum;j++){


                double meterPara = 0;
                //uint16转为int16
                if(extDevRatio.eMeterIsInt16[j]){
                    meterPara = int16((pMeterData+id_list.at(i))->para.data[j]) * extDevRatio.eMeterRatio[j];  //乘以系数
                }else{
                    meterPara = (pMeterData+id_list.at(i))->para.data[j] * extDevRatio.eMeterRatio[j];  //乘以系数
                }

                //按协议显示
                if(j<meterArrLen){
                    paraObj = m_currMeterProtoArr.at(j).toObject();
                    if(!paraObj["en"].toBool())
                        meterPara = int16(HIDEITEMVALUE);
                }

                //合并参数处理
                if(j== eMeterParaKWH_High){
                    KWHHigh = meterPara;
                }else if (j== eMeterParaKWH_Low) {
                    KWHLow = meterPara;
                }else if (j == eMeterParaKVarH_High) {
                    KVarHHigh = meterPara;
                }else if (j == eMeterParaKVarH_Low) {
                    KVarHLow = meterPara;
                }else {
                    meterInfo.data[j] = QString::number(meterPara);
                    //单位
                    meterInfo.dataUnit[j] = extDevShow.eMeterParaUnit[j];
                }
            }
            //总有功电量和总无功电量高地位合并
            meterInfo.eMeterParaKWH = ConversionTool::mergeFrom16Bit(KWHLow,KWHHigh,1);
            meterInfo.eMeterParaKVarH = ConversionTool::mergeFrom16Bit(KVarHLow,KVarHHigh,1);


        }else{
            nullmeterInfo.devID = meterInfo.devID;
            nullmeterInfo.connect = meterInfo.connect;
            meterInfo = nullmeterInfo;
        }



        meterList.append(meterInfo);
    }
    //发送电量仪列表消息
    emit sendeExtDevMeterList(meterList);

}

void ReadShareMem::readLeakDevData()
{
    QList<int> id_list;
    //获取水浸设备个数
    int ac_dev = sysCfg->uartExt.devNum[eExtDevLeak];
    if(ac_dev > 0)
    {
        //获取每个设备的ID
        for (int i = 0 ; i < ac_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevLeak][i];
        }
    }else {
        return ;
    }

    //初始化空设备
    leak_Info nullleakInfo;
    initLeakDev(nullleakInfo);

    QList<leak_Info> leakList;
    //获取水浸设备数据
    for(int i=0;i<id_list.length();i++){
        leak_Info leakInfo;
        //连接状态
        uint16 flag = (pLeakData+id_list.at(i))->connectFlag;
        leakInfo.connect = flag;
        leakInfo.devID   = id_list.at(i) + 1;
        if (leakInfo.connect == CONNECTED){
            //水浸状态数据
            for(int j=0;j<eLeakParaNum;j++){
                double leakSts = (pLeakData+id_list.at(i))->para.data[j] *extDevRatio.eLeakRatio[j];
                if(j == eLeakParaAlmSts){ leakInfo.almSts = leakSts; }
                QString showStr = "";
                if(leakSts < leakParabitMax)
                    showStr = extDevShow.eLeakParaShow[j][int(leakSts)];
                if(showStr == ""){
                    leakInfo.data[j] = QString::number(leakSts);
                }else {
                    leakInfo.data[j] = showStr;
                }
                //添加单位
                if(!extDevShow.eLeakParaUnit[j].isEmpty()){
                    leakInfo.dataUnit[j] = extDevShow.eLeakParaUnit[j];

                }

            }

        }else {
            nullleakInfo.devID = leakInfo.devID;
            nullleakInfo.connect = leakInfo.connect;
            leakInfo = nullleakInfo;
        }
       leakList.append(leakInfo);
    }

    //发送水浸列表消息
    emit sendeExtDevLeakList(leakList);
}

void ReadShareMem::readDCDevData()
{
//    QVector<int> v;// = findCardID(222222222, &tmp);
//    v.append(1);
//    v.append(2);
//    v.append(3);
//    v.append(4);

//    m_doorinfo.cardID = 222222222;
//    m_doorinfo.door = v;
//    emit sendDoorList(m_doorinfo);

    //网页更新门禁卡数据库，重新读取数据库
    if(1 == pLcdData->para.LcdCfgCardUpdate){
        m_initJsonOK = initCardAccessJson();
        //m_dcJson->init();
        pLcdData->para.LcdCfgCardUpdate = 0;
    }

    QList<int> id_list;
    //获取接线盒设备个数
    int ac_dev = sysCfg->uartExt.devNum[eExtDevDc];
    //qDebug()<<__LINE__<<"cabCtl num = "<<ac_dev;

    if(ac_dev > 0)
    {
        //获取每个设备的ID
        for (int i = 0 ; i < ac_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevDc][i];
            //控制模块列表更新
            if(ac_dev != FormMain::m_dcDev.dcLen){
                FormMain::m_dcDev.dcList.append(sysCfg->uartExt.onlineDev[eExtDevDc][i]);
            }

        }
    }else {
        return ;
    }
    //
    FormMain::m_dcDev.dcLen = ac_dev;

    door_Info doorinfo;
    uint16 idHigh, idLow, idChange;
    int idNo;
    //0号接线盒连接状态
    uint16 flag = (pCabCtrlData+id_list.at(0))->connectFlag;
    //qDebug()<<"0 cab is connect"<<flag;
    if (flag == 2){
        //读取ID号

        idHigh = (pCabCtrlData+id_list.at(0))->readId.id_h;
        idLow  = (pCabCtrlData+id_list.at(0))->readId.id_l;
        idChange = (pCabCtrlData+id_list.at(0))->IdChangedFlag.bits.lcd;

        if(idHigh >0 or idLow >0 ){
            idNo = ConversionTool::mergeFrom16BitCard(idLow,idHigh).toInt();
            //qDebug()<<"idNo"<<idNo;

            if(idChange == 1 and idNo > 0){
                //qDebug()<<__LINE__<<"read idNo ok";
                (pCabCtrlData+id_list.at(0))->IdChangedFlag.bits.lcd = 0;


                doorinfo.cardID = idNo;
                if(!m_initJsonOK){
                    qDebug()<<"card Database init fail";
                    doorinfo.door.clear();      //找不到数据库提示卡号不存在
                }else {
                    //查询json
                    findCardID(idNo,doorinfo.door);
                }

                //发送消息到界面
                //qDebug()<<doorinfo.cardID;
                emit sendDoorList(doorinfo);
            }
        }else {
            //qDebug()<<"card <0";            
        }

    }

}

void ReadShareMem::readSpdsDevData()
{
    QList<int> id_list;
    //获取水浸设备个数
    int ac_dev = sysCfg->uartExt.devNum[eExtDevSpds];
    if(ac_dev > 0)
    {
        //获取每个设备的ID
        for (int i = 0 ; i < ac_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevSpds][i];
        }
    }else {
        return ;
    }
    //qDebug()<<__FILE__<<"spds len "<<id_list.length();
    //初始化空设备
    spds_Info nullspdsInfo;
    initSpdsDev(nullspdsInfo);

    QList<spds_Info> spdsList;
    //获取spds设备数据
    for(int i=0;i<id_list.length();i++){
        //设备头信息,读取spds协议
        ExtDeviceHeadStruct Head = sysCfg->uartExt.extDevHead[eExtDevSpds][id_list.at(i)];
        if(Head.extProto != m_iCurrSpdsProto){
            //加载协议文件
            if(m_spdsProList.length()>0){
                m_currSpdsProto = findJsonDataSpdsProto(QString(SPDS_CFG_PATH)+m_spdsProList.at(Head.extProto));
                m_iCurrSpdsProto = Head.extProto;
                //qDebug()<<__LINE__ << QString(SPDS_CFG_PATH)+m_spdsProList.at(Head.extProto);
            }

        }

        spds_Info spdsInfo;
        //连接状态
        uint16 flag = (pSpdsData+id_list.at(i))->connectFlag;
        //qDebug()<<"spds connflag = "<<flag;
        spdsInfo.connect = flag;
        spdsInfo.devID   = id_list.at(i) + 1;

        spdsInfo.mainNum = m_currSpdsProto.mainNum;
        spdsInfo.branchNum = m_currSpdsProto.branchNum;
        //qDebug()<<__LINE__<<"main bran num"<<spdsInfo.mainNum<<spdsInfo.branchNum;
        if (spdsInfo.connect == CONNECTED){
            //qDebug()<<id_list.length();
            double mainData=0,branchData=0;
            uint16 int16SpdsMainKWhHi_A,int16SpdsMainKWhLo_A,int16SpdsMainKWhHi_B,int16SpdsMainKWhLo_B,int16SpdsMainKWhHi_C,int16SpdsMainKWhLo_C,int16SpdsMainKWhHi_all,int16SpdsMainKWhLo_all;

            //主路数据
            for(int m=0;m<m_currSpdsProto.mainNum;m++){
                int mainEn =sysCfg->uartExt.spdsSet[id_list.at(i)].mainEn[m];
                spdsInfo.mainEn[m] = mainEn;
                //qDebug()<<__LINE__<<"main en"<<mainEn;
                if(mainEn == 0){    //此主路不使能，
                    continue;
                }

                for(int mi=0;mi<eSpdsMainNum;mi++){

                    mainData = (pSpdsData+id_list.at(i))->main[m][mi] * extDevRatio.eSpdsMainRatio[mi];
                    if(!m_currSpdsProto.MainCfg[mi]){   //协议不显示
                        mainData = int16(HIDEITEMVALUE);
                    }
                    spdsInfo.main[m][mi] = QString::number(mainData);
                    spdsInfo.mainUnit[m][mi] = extDevShow.eSpdsMainUnit[mi];

                    if(mi == eSpdsMainVolt_AB or mi == eSpdsMainVolt_A){
                        qDebug()<<"eSpdsMainVolt"<<"Main"<<(pSpdsData+id_list.at(i))->main[m][mi]<<"ratio"<<extDevRatio.eSpdsMainRatio[mi];
                        qDebug()<<"Main+ ratio"<<mainData;
                    }

                    switch (mi) {
                        case eSpdsMainKWhHi_A:
                            int16SpdsMainKWhHi_A = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                        case eSpdsMainKWhLo_A:
                            int16SpdsMainKWhLo_A = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                        case eSpdsMainKWhHi_B:
                            int16SpdsMainKWhHi_B = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                        case eSpdsMainKWhLo_B:
                            int16SpdsMainKWhLo_B = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                        case eSpdsMainKWhHi_C:
                            int16SpdsMainKWhHi_C = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                        case eSpdsMainKWhLo_C:
                            int16SpdsMainKWhLo_C = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                        case eSpdsMainKWhHi_all:
                            int16SpdsMainKWhHi_all = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                        case eSpdsMainKWhLo_all:
                            int16SpdsMainKWhLo_all = (pSpdsData+id_list.at(i))->main[m][mi];
                            break;
                    }

                }
                //高低位合并,并赋值给高位，且把低位值设置为不显示
                if (spdsInfo.main[m][eSpdsMainKWhHi_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainKWhHi_A] = ConversionTool::mergeFrom16Bit(int16SpdsMainKWhLo_A,int16SpdsMainKWhHi_A,extDevRatio.eSpdsMainRatio[eSpdsMainKWhHi_A]);
                    spdsInfo.main[m][eSpdsMainKWhLo_A] = QString::number(int16(HIDEITEMVALUE));

                    spdsInfo.main[m][eSpdsMainKWhHi_B] = ConversionTool::mergeFrom16Bit(int16SpdsMainKWhLo_B,int16SpdsMainKWhHi_B,extDevRatio.eSpdsMainRatio[eSpdsMainKWhHi_B]);
                    spdsInfo.main[m][eSpdsMainKWhLo_B] = QString::number(int16(HIDEITEMVALUE));

                    spdsInfo.main[m][eSpdsMainKWhHi_C] = ConversionTool::mergeFrom16Bit(int16SpdsMainKWhLo_C,int16SpdsMainKWhHi_C,extDevRatio.eSpdsMainRatio[eSpdsMainKWhHi_C]);
                    spdsInfo.main[m][eSpdsMainKWhLo_C] = QString::number(int16(HIDEITEMVALUE));
                }

                if (spdsInfo.main[m][eSpdsMainKWhHi_all] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainKWhHi_all] = ConversionTool::mergeFrom16Bit(int16SpdsMainKWhLo_all,int16SpdsMainKWhHi_all,extDevRatio.eSpdsMainRatio[eSpdsMainKWhHi_all]);
                    spdsInfo.main[m][eSpdsMainKWhLo_all] = QString::number(int16(HIDEITEMVALUE));
                }

                //三相合并
                //线电压
                if (spdsInfo.main[m][eSpdsMainVolt_AB] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainVolt_AB] = spdsInfo.main[m][eSpdsMainVolt_AB]+"/"+spdsInfo.main[m][eSpdsMainVolt_BC]+"/"+spdsInfo.main[m][eSpdsMainVolt_CA];
                    spdsInfo.main[m][eSpdsMainVolt_BC] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainVolt_CA] = QString::number(int16(HIDEITEMVALUE));
                }

                //零地电压
                //相电压
                if (spdsInfo.main[m][eSpdsMainVolt_A] !=QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainVolt_A] =  spdsInfo.main[m][eSpdsMainVolt_A]+"/"+spdsInfo.main[m][eSpdsMainVolt_B]+"/"+spdsInfo.main[m][eSpdsMainVolt_C];
                    spdsInfo.main[m][eSpdsMainVolt_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainVolt_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //相电流
                if (spdsInfo.main[m][eSpdsMainCurr_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainCurr_A] =  spdsInfo.main[m][eSpdsMainCurr_A]+"/"+spdsInfo.main[m][eSpdsMainCurr_B]+"/"+spdsInfo.main[m][eSpdsMainCurr_C];
                    spdsInfo.main[m][eSpdsMainCurr_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainCurr_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //零地电流
                //相电流百分比
                if (spdsInfo.main[m][eSpdsMainLoad_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainLoad_A] =  spdsInfo.main[m][eSpdsMainLoad_A]+"/"+spdsInfo.main[m][eSpdsMainLoad_B]+"/"+spdsInfo.main[m][eSpdsMainLoad_C];
                    spdsInfo.main[m][eSpdsMainLoad_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainLoad_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //交流频率
                //相有功电量
                if (spdsInfo.main[m][eSpdsMainKWhHi_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainKWhHi_A] =  spdsInfo.main[m][eSpdsMainKWhHi_A]+"/"+spdsInfo.main[m][eSpdsMainKWhHi_B]+"/"+spdsInfo.main[m][eSpdsMainKWhHi_C];
                    spdsInfo.main[m][eSpdsMainKWhHi_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainKWhHi_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //相视在功率
                if (spdsInfo.main[m][eSpdsMainKva_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainKva_A] =  spdsInfo.main[m][eSpdsMainKva_A]+"/"+spdsInfo.main[m][eSpdsMainKva_B]+"/"+spdsInfo.main[m][eSpdsMainKva_C];
                    spdsInfo.main[m][eSpdsMainKva_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainKva_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //相有功功率
                if (spdsInfo.main[m][eSpdsMainKw_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainKw_A] =  spdsInfo.main[m][eSpdsMainKw_A]+"/"+spdsInfo.main[m][eSpdsMainKw_B]+"/"+spdsInfo.main[m][eSpdsMainKw_C];
                    spdsInfo.main[m][eSpdsMainKw_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainKw_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //相无功功率
                if (spdsInfo.main[m][eSpdsMainKVar_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainKVar_A] =  spdsInfo.main[m][eSpdsMainKVar_A]+"/"+spdsInfo.main[m][eSpdsMainKVar_B]+"/"+spdsInfo.main[m][eSpdsMainKVar_C];
                    spdsInfo.main[m][eSpdsMainKVar_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainKVar_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //相功率因数
                if (spdsInfo.main[m][eSpdsMainPF_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainPF_A] =  spdsInfo.main[m][eSpdsMainPF_A]+"/"+spdsInfo.main[m][eSpdsMainPF_B]+"/"+spdsInfo.main[m][eSpdsMainPF_C];
                    spdsInfo.main[m][eSpdsMainPF_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainPF_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //电压谐波百分比
                if (spdsInfo.main[m][eSpdsMainTHDU_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainTHDU_A] =  spdsInfo.main[m][eSpdsMainTHDU_A]+"/"+spdsInfo.main[m][eSpdsMainTHDU_B]+"/"+spdsInfo.main[m][eSpdsMainTHDU_C];
                    spdsInfo.main[m][eSpdsMainTHDU_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainTHDU_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //电流谐波百分比
                if (spdsInfo.main[m][eSpdsMainTHDI_A] != QString::number(int16(HIDEITEMVALUE))){
                    spdsInfo.main[m][eSpdsMainTHDI_A] =  spdsInfo.main[m][eSpdsMainTHDI_A]+"/"+spdsInfo.main[m][eSpdsMainTHDI_B]+"/"+spdsInfo.main[m][eSpdsMainTHDI_C];
                    spdsInfo.main[m][eSpdsMainTHDI_B] = QString::number(int16(HIDEITEMVALUE));
                    spdsInfo.main[m][eSpdsMainTHDI_C] = QString::number(int16(HIDEITEMVALUE));
                }

                //总有功电量

            }
            uint16 int16eSpdsBranchKWhHi,int16eSpdsBranchKWhLo;
            //支路数据
            for(int b=0;b<m_currSpdsProto.branchNum;b++){

                int branchEn = sysCfg->uartExt.spdsSet[id_list.at(i)].branchEn[b];
                spdsInfo.branchEn[b] = branchEn;
                //qDebug()<<__LINE__<<"branch en"<<branchEn;
                if(branchEn == 0){    //此支路不使能，
                    continue;
                }

                for(int bi=0;bi<eSpdsBranchNum;bi++){
                    if(!m_currSpdsProto.BranchCfg[bi]){  //协议不显示
                        spdsInfo.branch[b][bi] = QString::number(int16(HIDEITEMVALUE));
                        spdsInfo.branchUnit[b][bi] = "";
                        continue;
                    }

                    branchData = (pSpdsData+id_list.at(i))->branch[b][bi]* extDevRatio.eSpdsBranchRatio[bi];
//                    if(bi == eSpdsBranchVolt){
//                        qDebug()<<"eSpdsBranchVolt"<<"branch"<<(pSpdsData+id_list.at(i))->branch[b][bi]<<"ratio"<<extDevRatio.eSpdsBranchRatio[bi];
//                        qDebug()<<"branch+ ratio"<<branchData;
//                    }

                    spdsInfo.branch[b][bi] = QString::number(branchData);
                    spdsInfo.branchUnit[b][bi] = extDevShow.eSpdsBranchUnit[bi];
                    //状态处理
                    if(bi == eSpdsBranchSwSts or bi == eSpdsBranchCurrOvUp or bi ==eSpdsBranchCurrUnderLo){
                        if(branchData<spdsBranchbitMax){
                            spdsInfo.branch[b][bi] = extDevShow.eSpdsBranchShow[bi][int(branchData)];
                        }
                    }else if(bi == eSpdsBranchKWhHi){
                        int16eSpdsBranchKWhHi = (pSpdsData+id_list.at(i))->branch[b][bi];
                    }else if(bi == eSpdsBranchKWhLo){
                        int16eSpdsBranchKWhLo = (pSpdsData+id_list.at(i))->branch[b][bi];
                    }

                }
                //高低位合并
                spdsInfo.branch[b][eSpdsBranchKWhHi] = ConversionTool::mergeFrom16Bit(int16eSpdsBranchKWhLo,int16eSpdsBranchKWhHi,extDevRatio.eSpdsBranchRatio[eSpdsBranchKWhHi]);
                spdsInfo.branch[b][eSpdsBranchKWhLo] = QString::number(int16(HIDEITEMVALUE));
            }






        }else {
            nullspdsInfo.devID = spdsInfo.devID;
            nullspdsInfo.connect = spdsInfo.connect;
            spdsInfo = nullspdsInfo;
        }
        spdsList.append(spdsInfo);

    }

    //发送spds列表消息
    emit sendeExtDevSpdsList(spdsList);

//    qDebug()<<"spds ";
//    qDebug()<<spdsList.at(2).devID;
//    qDebug()<<spdsList.at(2).connect;

}

void ReadShareMem::readSMSDevData()
{
    QList<int> id_list;
    //获取接线盒设备个数
    int ac_dev = sysCfg->uartExt.devNum[eExtDevSms];
    //qDebug()<<__LINE__<<"cabCtl num = "<<ac_dev;
    if(ac_dev > 0)
    {
        //获取每个设备的ID
        for (int i = 0 ; i < ac_dev; i++) {
            id_list << sysCfg->uartExt.onlineDev[eExtDevSms][i];
        }
    }else {
        return ;
    }

    //初始化空设备
    sms_Info nullsmsInfo;
    initSmsDev(nullsmsInfo);

    QList<sms_Info> smsList;
    //获取sms设备数据
    for(int i=0;i<id_list.length();i++){
        sms_Info smsInfo;
        //连接状态
        uint16 flag = (pSmsData+id_list.at(i))->connectFlag;
        smsInfo.connect = flag;
        smsInfo.devID   = id_list.at(i) + 1;
        smsInfo.almSts = 1;
        if (smsInfo.connect == CONNECTED){
            uint16 signalVal = (pSmsData+id_list.at(i))->signalValue;
            smsInfo.almSts = signalVal>=16 ? 0 : (signalVal==0 ? 1 : 2);    //0-正常 1-无网络服务 2-信号弱
            //信号处理
            if(signalVal >=0 and signalVal <= 99){
                if (signalVal>20){
                    smsInfo.signalLV = signalVal>25 ? 5 : 4;
                }else if (signalVal<16){
                    smsInfo.signalLV = signalVal>10 ? 2 : (signalVal != 0 ? 1 : 0);
                }else{
                    smsInfo.signalLV = 3;
                }

                //读取csca内容
                //低8位有效，左边第一位位 s[0]
                int cscaLen = (pSmsData+id_list.at(i))->csca.valLen;
                QString cscaStr;
                int num;
                for(int j=0;j<cscaLen;++j){
                     num = ConversionTool::get8BitL((pSmsData+id_list.at(i))->csca.value[j]) - 0x30;
                     cscaStr += QString::number(num);
                }
                smsInfo.cscaNo = cscaStr;
            }
        }else {
            nullsmsInfo.devID = smsInfo.devID;
            nullsmsInfo.connect = smsInfo.connect;
            smsInfo = nullsmsInfo;
        }
        smsList.append(smsInfo);
        emit sendeExtDevSmsList(smsList);
    }
}

void ReadShareMem::showInputOutputABC(ups_Info &upsInfo, int in, int out)
{
    //输入
    switch (in) {
        case 1:
            {
                upsInfo.meterD.eUpsParaBypPfPh = upsInfo.meterD.eUpsParaBypPfPh.split("/").at(0);
                upsInfo.meterD.eUpsParaBypCurrPh = upsInfo.meterD.eUpsParaBypCurrPh.split("/").at(0);
                upsInfo.meterD.eUpsParaBypFreqPh = upsInfo.meterD.eUpsParaBypFreqPh .split("/").at(0);
                upsInfo.meterD.eUpsParaBypVoltPh = upsInfo.meterD.eUpsParaBypVoltPh.split("/").at(0);

                upsInfo.meterD.eUpsParaIpPfPh =   upsInfo.meterD.eUpsParaIpPfPh.split("/").at(0);
                upsInfo.meterD.eUpsParaIpCurrPh = upsInfo.meterD.eUpsParaIpCurrPh.split("/").at(0);
                upsInfo.meterD.eUpsParaIpFreqPh = upsInfo.meterD.eUpsParaIpFreqPh .split("/").at(0);
                upsInfo.meterD.eUpsParaIpVoltPh = upsInfo.meterD.eUpsParaIpVoltPh.split("/").at(0);

            }
            break;
        case 2:
            {
                upsInfo.meterD.eUpsParaBypPfPh   = upsInfo.meterD.eUpsParaBypPfPh  .left(upsInfo.meterD.eUpsParaBypPfPh  .lastIndexOf("/"));
                upsInfo.meterD.eUpsParaBypCurrPh = upsInfo.meterD.eUpsParaBypCurrPh.left(upsInfo.meterD.eUpsParaBypCurrPh.lastIndexOf("/"));
                upsInfo.meterD.eUpsParaBypFreqPh = upsInfo.meterD.eUpsParaBypFreqPh.left(upsInfo.meterD.eUpsParaBypFreqPh.lastIndexOf("/"));
                upsInfo.meterD.eUpsParaBypVoltPh = upsInfo.meterD.eUpsParaBypVoltPh.left(upsInfo.meterD.eUpsParaBypVoltPh.lastIndexOf("/"));

                upsInfo.meterD.eUpsParaIpPfPh   = upsInfo.meterD.eUpsParaIpPfPh  .left(upsInfo.meterD.eUpsParaIpPfPh  .lastIndexOf("/"));
                upsInfo.meterD.eUpsParaIpCurrPh = upsInfo.meterD.eUpsParaIpCurrPh.left(upsInfo.meterD.eUpsParaIpCurrPh.lastIndexOf("/"));
                upsInfo.meterD.eUpsParaIpFreqPh = upsInfo.meterD.eUpsParaIpFreqPh.left(upsInfo.meterD.eUpsParaIpFreqPh.lastIndexOf("/"));
                upsInfo.meterD.eUpsParaIpVoltPh = upsInfo.meterD.eUpsParaIpVoltPh.left(upsInfo.meterD.eUpsParaIpVoltPh.lastIndexOf("/"));
            }
        default:
            break;

    }
    //输出
    switch (out) {
        case 1:
            {
                upsInfo.meterD.eUpsParaOpPfPh   = upsInfo.meterD.eUpsParaOpPfPh  .split("/").at(0);
                upsInfo.meterD.eUpsParaOpCurrPh = upsInfo.meterD.eUpsParaOpCurrPh.split("/").at(0);
                upsInfo.meterD.eUpsParaOpFreqPh = upsInfo.meterD.eUpsParaOpFreqPh.split("/").at(0);
                upsInfo.meterD.eUpsParaOpVoltPh = upsInfo.meterD.eUpsParaOpVoltPh.split("/").at(0);
                upsInfo.meterD.eUpsParaOpKvaPh  = upsInfo.meterD.eUpsParaOpKvaPh .split("/").at(0);
                upsInfo.meterD.eUpsParaOpKwPh   = upsInfo.meterD.eUpsParaOpKwPh  .split("/").at(0);
                upsInfo.meterD.eUpsParaLdPerPh  = upsInfo.meterD.eUpsParaLdPerPh .split("/").at(0);

            }
            break;
        case 2:
            {
                upsInfo.meterD.eUpsParaOpPfPh   = upsInfo.meterD.eUpsParaOpPfPh  .left(upsInfo.meterD.eUpsParaOpPfPh  .lastIndexOf("/"));
                upsInfo.meterD.eUpsParaOpCurrPh = upsInfo.meterD.eUpsParaOpCurrPh.left(upsInfo.meterD.eUpsParaOpCurrPh.lastIndexOf("/"));
                upsInfo.meterD.eUpsParaOpFreqPh = upsInfo.meterD.eUpsParaOpFreqPh.left(upsInfo.meterD.eUpsParaOpFreqPh.lastIndexOf("/"));
                upsInfo.meterD.eUpsParaOpVoltPh = upsInfo.meterD.eUpsParaOpVoltPh.left(upsInfo.meterD.eUpsParaOpVoltPh.lastIndexOf("/"));
                upsInfo.meterD.eUpsParaOpKvaPh  = upsInfo.meterD.eUpsParaOpKvaPh .left(upsInfo.meterD.eUpsParaOpKvaPh .lastIndexOf("/"));
                upsInfo.meterD.eUpsParaOpKwPh   = upsInfo.meterD.eUpsParaOpKwPh  .left(upsInfo.meterD.eUpsParaOpKwPh  .lastIndexOf("/"));
                upsInfo.meterD.eUpsParaLdPerPh  = upsInfo.meterD.eUpsParaLdPerPh .left(upsInfo.meterD.eUpsParaLdPerPh .lastIndexOf("/"));

            }
        default:
            break;

    }
}

bool ReadShareMem::initCardAccessJson()
{
    QString value = ConversionTool::openJsonFile("/etc/.eMTR/web/accessList.txt");
    QJsonParseError parseJsonErr;
    m_cardAccessJsonDoc = QJsonDocument::fromJson(value.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError)){
        return false;
    }
    return true;
}

bool ReadShareMem::findCardID(int id,QVector<int>& dcList)
{   
    bool ret = false;
    QJsonArray jsonArray = m_cardAccessJsonDoc.array();
    for(int i=0;i<jsonArray.size();i++){
        QJsonValue listArray = jsonArray.at(i);
        QJsonObject card = listArray.toObject();
        int cardID = card["cardId"].toString().toInt();
        if(cardID == id){
            ret =true;
            QJsonArray boxArr = card["boxArr"].toArray();
            for(int j=0;j<boxArr.size();j++){
                QJsonValue box = boxArr.at(j);
                dcList<<box.toString().toInt();
            }

            break;
        }
    }
    return ret;
}

bool ReadShareMem::initMeterJsonData(QString path, QList<QString> & meterProto)
{
    meterProto.clear();
    QString value = ConversionTool::openJsonFile(path);
    QJsonParseError parseJsonErr;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(value.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError)){
        return false;
    }

    QJsonObject objList = jsonDoc.object();
    int proNum = objList["proNum"].toInt();
    QString proList = objList["protoName"].toString();
    QStringList proString = proList.split(",");
    if(proString.length()>0){
        for(int i=0;i<proString.length();i++){
            meterProto.append(QString::number(i)+"_"+proString.at(i)+".cfg");
        }
    }else {
        return false;
    }

    return  true;
}

bool ReadShareMem::initJsonDataFile(QString path, QJsonDocument &jsonData)
{
    QString value = ConversionTool::openJsonFile(path);
    QJsonParseError parseJsonErr;
    jsonData = QJsonDocument::fromJson(value.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError)){
        return false;
    }
    return true;
}

QJsonArray ReadShareMem::findJsonDataArray(QString path, QString objName)
{
    QJsonDocument jsonDoc;
    QJsonArray jsonArr;
    bool ok = initJsonDataFile(path,jsonDoc);
    if(ok){
        QJsonObject objList = jsonDoc.object();
        return  objList[objName].toArray();
    }

    return jsonArr;

}

QJsonObject ReadShareMem::findJsonDataObject(QJsonDocument &jsonDoc, QString objName, QString valType)
{
    QJsonObject objList = jsonDoc.object();
    return  objList[objName].toObject();
}

spds_proto ReadShareMem::findJsonDataSpdsProto(QString path)
{
    spds_proto spdsProto;
    spdsProto.mainNum = 0;
    spdsProto.branchNum = 0;
    QFileInfo file(path);
    if(file.exists() == false){
        qDebug()<<QString("spds %1 file does not exist!").arg(path);
        return spdsProto;
    }

    QJsonDocument jsonDoc;
    QJsonArray jsonArr;
    bool ok = initJsonDataFile(path,jsonDoc);
    if(ok){
        QJsonObject objList = jsonDoc.object();
        QJsonObject spdsNum = objList["ProCfg"].toObject();
        //主支路数量
        spdsProto.mainNum       = spdsNum["mainNum"].toInt();
        spdsProto.branchNum     = spdsNum["branchNum"].toInt();
        //主路使能
        QJsonArray mainArr = objList["MainCfg"].toArray();
        if(mainArr.size()>0){
            QJsonArray enArr = mainArr.at(0).toArray();
            QJsonObject enObj;
            for(int i=0;i<enArr.size();i++){
                enObj = enArr.at(i).toObject();
                spdsProto.MainCfg[i] = enObj["en"].toBool();
            }
        }
        //支路使能
        QJsonArray branchArr = objList["BranchCfg"].toArray();
        if(branchArr.size()>0){
            QJsonArray enArr = branchArr.at(0).toArray();
            QJsonObject enObj;
            for(int j=0;j<enArr.size();j++){
                enObj = enArr.at(j).toObject();
                spdsProto.BranchCfg[j] = enObj["en"].toBool();
            }
        }
    }
    return spdsProto;
}

void ReadShareMem::initThsDev(ths_Info &nullthsInfo)
{
    nullthsInfo.devID =0;
    nullthsInfo.connect=0;
    nullthsInfo.almSts=0;
    nullthsInfo.eThsParaAlmSts ="";
    for(int i =0;i<eThsParaNum;i++){
        nullthsInfo.data[i]=0;
        nullthsInfo.dataUnit[i]="";
    }
}

void ReadShareMem::initACDev(ac_Info &nullacInfo)
{
    nullacInfo.devID = 0;
    nullacInfo.connect = 0;
    nullacInfo.almSts = 0;
    for(int i =0;i<eAcStsNum;i++){
        nullacInfo.stsData[i] = "";
    }
    for(int i =0;i<eAcParaNum;i++){
        nullacInfo.paraData[i] = "";
        nullacInfo.paraUnit[i] = "";
    }

}

void ReadShareMem::initUPSDev(ups_Info &nullupsInfo)
{
    nullupsInfo.devID = 0;
    nullupsInfo.connect = 0;
    nullupsInfo.almSts = 0;

    for(int i =0;i<eUpsStsNum;i++){
        nullupsInfo.sts[i] = "";
    }
    for(int i =0;i<eUpsParaNum;i++){
        nullupsInfo.para[i] = "";
        nullupsInfo.paraUnit[i] = "";
    }
    for(int i =0;i<eUpsCfgNum;i++){
        nullupsInfo.cfgData[i] = "";
        nullupsInfo.cfgUnit[i] = "";
    }

    nullupsInfo.inf.manufacturer    ="";
    nullupsInfo.inf.MtrVer          ="";
    nullupsInfo.inf.series          ="";
    nullupsInfo.inf.modelName       ="";
    nullupsInfo.inf.ioType          =0;

    nullupsInfo.meterD.eUpsParaBypVoltPh   ="";
    nullupsInfo.meterD.eUpsParaBypCurrPh   ="";
    nullupsInfo.meterD.eUpsParaBypFreqPh   ="";
    nullupsInfo.meterD.eUpsParaBypPfPh     ="";
    nullupsInfo.meterD.eUpsParaIpVoltPh    ="";
    nullupsInfo.meterD.eUpsParaIpCurrPh    ="";
    nullupsInfo.meterD.eUpsParaIpFreqPh    ="";
    nullupsInfo.meterD.eUpsParaIpPfPh      ="";
    nullupsInfo.meterD.eUpsParaOpVoltPh    ="";
    nullupsInfo.meterD.eUpsParaOpCurrPh    ="";
    nullupsInfo.meterD.eUpsParaOpFreqPh    ="";
    nullupsInfo.meterD.eUpsParaOpPfPh      ="";
    nullupsInfo.meterD.eUpsParaOpKvaPh     ="";
    nullupsInfo.meterD.eUpsParaOpKwPh      ="";
    nullupsInfo.meterD.eUpsParaLdPerPh     ="";
}

void ReadShareMem::initMeterDev(meter_Info &nullmeterInfo)
{
    nullmeterInfo.devID = 0;
    nullmeterInfo.connect = 0;
    nullmeterInfo.almSts = 0;
    nullmeterInfo.eMeterParaKWH = "";
    nullmeterInfo.eMeterParaKVarH = "";
    for(int i =0;i<eMeterParaNum;i++){
        nullmeterInfo.data[i]       = "";
        nullmeterInfo.dataUnit[i]   = "";
    }
}

void ReadShareMem::initLeakDev(leak_Info &nullleakInfo)
{
    nullleakInfo.devID = 0;
    nullleakInfo.connect = 0;
    nullleakInfo.almSts = 0;

    for(int i =0;i<eLeakParaNum;i++){
        nullleakInfo.data[i] = "";
        nullleakInfo.dataUnit[i] ="";
    }

}

void ReadShareMem::initSpdsDev(spds_Info &nullspdsInfo)
{
    nullspdsInfo.devID = 0;
    nullspdsInfo.connect =0;
    nullspdsInfo.almSts =0;
}

void ReadShareMem::initSmsDev(sms_Info &nullsmsInfo)
{
    nullsmsInfo.devID = 0;
    nullsmsInfo.connect = 0;
    nullsmsInfo.almSts = 0;
    nullsmsInfo.cscaNo = "";
    nullsmsInfo.signalLV = 0;

}

