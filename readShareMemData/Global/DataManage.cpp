#include "DataManage.h"

struct QList<QPair<langEnum,QString>> uDataList;

struct QList<QPair<langEnum,QString>> acDataList;

struct QList<QPair<langEnum,QString>> meterDataList;

struct QList<QPair<langEnum,QString>> thsDataList;

struct QList<QPair<langEnum,QString>> statsDataList;

struct QList<QPair<langEnum,QString>> pueDataList;

void uDataList_Init()
{
    uDataList.clear();
    uDataList.append(qMakePair(langEnum::strDevType,QString("devNo")));

    uDataList.append(qMakePair(langEnum::strIpVoltA,QString("ipVoltA")));    //输入电压
    uDataList.append(qMakePair(langEnum::strIpVoltB,QString("ipVoltB")));
    uDataList.append(qMakePair(langEnum::strIpVoltC,QString("ipVoltC")));
    uDataList.append(qMakePair(langEnum::strIpFreqA,QString("ipFreqA")));    //输入频率
    uDataList.append(qMakePair(langEnum::strIpFreqB,QString("ipFreqB")));
    uDataList.append(qMakePair(langEnum::strIpFreqC,QString("ipFreqC")));
    uDataList.append(qMakePair(langEnum::strOpVoltA,QString("opVoltA")));    //输出电压
    uDataList.append(qMakePair(langEnum::strOpVoltB,QString("opVoltB")));
    uDataList.append(qMakePair(langEnum::strOpVoltC,QString("opVoltC")));
    uDataList.append(qMakePair(langEnum::strOpCurrA,QString("opCurrA")));    //输出电流
    uDataList.append(qMakePair(langEnum::strOpCurrB,QString("opCurrB")));
    uDataList.append(qMakePair(langEnum::strOpCurrC,QString("opCurrC")));
    uDataList.append(qMakePair(langEnum::strOpFreqA,QString("opFreqA")));    //输出频率
    uDataList.append(qMakePair(langEnum::strOpFreqB,QString("opFreqB")));
    uDataList.append(qMakePair(langEnum::strOpFreqC,QString("opFreqC")));
    uDataList.append(qMakePair(langEnum::strBypVoltA,QString("bypVoltA")));    //旁路电压
    uDataList.append(qMakePair(langEnum::strBypVoltB,QString("bypVoltB")));
    uDataList.append(qMakePair(langEnum::strBypVoltC,QString("bypVoltC")));
    uDataList.append(qMakePair(langEnum::strBypCurrA,QString("bypCurrA")));  //旁路电流
    uDataList.append(qMakePair(langEnum::strBypCurrB,QString("bypCurrB")));
    uDataList.append(qMakePair(langEnum::strBypCurrC,QString("bypCurrC")));
    uDataList.append(qMakePair(langEnum::strBypFreqA,QString("bypFreqA")));  //旁路频率
    uDataList.append(qMakePair(langEnum::strBypFreqB,QString("bypFreqB")));
    uDataList.append(qMakePair(langEnum::strBypFreqC,QString("bypFreqC")));
    uDataList.append(qMakePair(langEnum::strOpKvaA,QString("kvaA")));
    uDataList.append(qMakePair(langEnum::strOpKvaB,QString("kvaB")));
    uDataList.append(qMakePair(langEnum::strOpKvaC,QString("kvaC")));
    uDataList.append(qMakePair(langEnum::strOpKwA ,QString("kwA")));
    uDataList.append(qMakePair(langEnum::strOpKwB ,QString("kwB")));
    uDataList.append(qMakePair(langEnum::strOpKwC ,QString("kwC")));
    uDataList.append(qMakePair(langEnum::strLoadPerA,QString("loadA")));
    uDataList.append(qMakePair(langEnum::strLoadPerB,QString("loadB")));
    uDataList.append(qMakePair(langEnum::strLoadPerC,QString("loadC")));
    uDataList.append(qMakePair(langEnum::strBattVolt_p,QString("batVoltP")));
    uDataList.append(qMakePair(langEnum::strBattVolt_m,QString("batVoltM")));
    uDataList.append(qMakePair(langEnum::strBattCap,QString("batCap")));
    uDataList.append(qMakePair(langEnum::strBattRemT,QString("batRemTime")));

    uDataList.append(qMakePair(langEnum::strDatetime,QString("datetime")));
}

void acDataList_Init()
{
    acDataList.clear();
    acDataList.append(qMakePair(langEnum::strDevType,QString("devNo")));
    acDataList.append(qMakePair(langEnum::strAcInletTemp,QString("inletTmp")));
    acDataList.append(qMakePair(langEnum::strAcOutletTemp,QString("outletTmp")));
    acDataList.append(qMakePair(langEnum::strDatetime,QString("datetime")));
}

void meterDataList_Init()
{
    meterDataList.clear();
    //第一个值为显示表头名称，第二个值为数据库获取的值
    meterDataList.append(qMakePair(langEnum::strDevType,        QString("devNo")));

    meterDataList.append(qMakePair(langEnum::strTtlKwh,         QString("kwh")));
    meterDataList.append(qMakePair(langEnum::strMeterParaVoltAB,QString("lvoltUab")));
    meterDataList.append(qMakePair(langEnum::strMeterParaVoltBC,QString("lvoltUbc")));
    meterDataList.append(qMakePair(langEnum::strMeterParaVoltCA,QString("lvoltUca")));
    meterDataList.append(qMakePair(langEnum::strMeterParaCurrA, QString("currA")));
    meterDataList.append(qMakePair(langEnum::strMeterParaCurrB, QString("currB")));
    meterDataList.append(qMakePair(langEnum::strMeterParaCurrC, QString("currC")));
    meterDataList.append(qMakePair(langEnum::strMeterParaPfA,   QString("pfA")));
    meterDataList.append(qMakePair(langEnum::strMeterParaPfB,   QString("pfB")));
    meterDataList.append(qMakePair(langEnum::strMeterParaPfC,   QString("pfC")));
    meterDataList.append(qMakePair(langEnum::strFreq,           QString("freq")));
    meterDataList.append(qMakePair(langEnum::strMeterParaKwA,   QString("kwA")));
    meterDataList.append(qMakePair(langEnum::strMeterParaKwB,   QString("kwB")));
    meterDataList.append(qMakePair(langEnum::strMeterParaKwC,   QString("kwC")));
    meterDataList.append(qMakePair(langEnum::strMeterParaKvaA,  QString("kvaA")));
    meterDataList.append(qMakePair(langEnum::strMeterParaKvaB,  QString("kvaB")));
    meterDataList.append(qMakePair(langEnum::strMeterParaKvaC,  QString("kvaC")));
    meterDataList.append(qMakePair(langEnum::strMeterParaVoltABThd,    QString("lTHDvUab")));
    meterDataList.append(qMakePair(langEnum::strMeterParaVoltBCThd,    QString("lTHDvUbc")));
    meterDataList.append(qMakePair(langEnum::strMeterParaVoltCAThd,    QString("lTHDvUca")));
    meterDataList.append(qMakePair(langEnum::strMeterParaCurrAThd,     QString("phTHDiA")));
    meterDataList.append(qMakePair(langEnum::strMeterParaCurrBThd,     QString("phTHDiB")));
    meterDataList.append(qMakePair(langEnum::strMeterParaCurrCThd,     QString("phTHDiC")));

    meterDataList.append(qMakePair(langEnum::strDatetime,       QString("datetime")));
}

void thsDataList_Init()
{
    thsDataList.clear();
    thsDataList.append(qMakePair(langEnum::strDevType,QString("devNo")));

    thsDataList.append(qMakePair(langEnum::strTemp,QString("tmp")));
    thsDataList.append(qMakePair(langEnum::strHumi,QString("hum")));

    thsDataList.append(qMakePair(langEnum::strDatetime,QString("datetime")));

}

void statsDataList_Init()
{
    statsDataList.clear();
    statsDataList.append(qMakePair(langEnum::strAcTmpAvg,        QString("tmpAvg")));
    statsDataList.append(qMakePair(langEnum::strFrontTmp,        QString("frontTmp")));
    statsDataList.append(qMakePair(langEnum::strFrontHum,        QString("frontHum")));
    statsDataList.append(qMakePair(langEnum::strBackTmp,        QString("backTmp")));
    statsDataList.append(qMakePair(langEnum::strBackHum,        QString("backHum")));
    statsDataList.append(qMakePair(langEnum::strAmbientTemp,        QString("ambTmp")));
    statsDataList.append(qMakePair(langEnum::strAmbientHum,        QString("ambHum")));

    statsDataList.append(qMakePair(langEnum::strDatetime,       QString("datetime")));
}

void pueDataList_Init()
{
    pueDataList.clear();
    pueDataList.append(qMakePair(langEnum::strPue,       QString("pue")));
    pueDataList.append(qMakePair(langEnum::strDatetime,       QString("datetime")));

}
