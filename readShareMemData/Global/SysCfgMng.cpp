#ifndef SYSCFGMNG_C
#define SYSCFGMNG_C

#include "SysCfgMng.h"
#include "eMtrDataStruct.h"
#include "ThsData/ThsDataStruct.h"
//#include "Version.h"
#include <QDebug>


void CFG_Init()
{
    PrintInfo("CFG_Init\n");    
    //Create shared memory
    if(!sysCfgShare.createShare(SYS_CFG_SHARE, sizeof(SysCfgMngStruct), ShareMemory::ReadWrite))
    {
        PrintAlm("Open SYS_CFG_SHARE fail\n");
        exit(1);
    }
    PrintInfo("Open SYS_CFG_SHARE ok\n");


    //Get the pointer of shared memory.
    sysCfg = (SysCfgMngStruct*)sysCfgShare.data();



    //Create shared memory
    if(!eMtrDataShare.createShare(EMTR_SHARE, sizeof(eMtrDataStruct), ShareMemory::ReadWrite))
    {
        PrintAlm("Open EMTR_SHARE fail\n");
        exit(1);
    }
    PrintInfo("Open EMTR_SHARE ok\n");
    eMtrData = (eMtrDataStruct*)eMtrDataShare.data();

    //创建温湿度共享内存
    if(!thsDataShare.createShare(THS_SHARE,sizeof(ThsDataStruct)*THS_MAX_NUM,ShareMemory::ReadWrite))
    {
        PrintAlm("Open THS_SHARE fail\n");
        exit(1);
    }
    pThsData = (ThsDataStruct*)thsDataShare.data();
    PrintInfo("Open THS_SHARE ok\n");

    //创建空调共享内存
    if(!acDataShare.createShare(AC_SHARE,sizeof(AcDataStruct)*AC_MAX_NUM,ShareMemory::ReadWrite))
    {
        PrintAlm("Open AC_SHARE fail\n");
        exit(1);
    }
    pAcData = (AcDataStruct *)acDataShare.data();

    //创建空调共享内存
    if(!upsDataShare.createShare(UPS_SHARE,sizeof(UpsDataStruct)*UPS_MAX_NUM,ShareMemory::ReadWrite))
    {
        PrintAlm("Open UPS_SHARE fail\n");
        exit(1);
    }
    pUpsData = (UpsDataStruct *)upsDataShare.data();
    PrintInfo("Open UPS_SHARE ok\n");

    //创建电量仪共享内存
    if(!meterDataShare.createShare(METER_SHARE,sizeof(MeterDataStruct)*METER_MAX_NUM,ShareMemory::ReadWrite))
    {
        PrintAlm("Open METER_SHARE fail\n");
        exit(1);
    }
    pMeterData = (MeterDataStruct *)meterDataShare.data();
    PrintInfo("Open METER_SHARE ok\n");

    //创建水浸共享内存
    if(!leakDataShare.createShare(LEAK_SHARE,sizeof(LeakDataStruct)*LEAK_MAX_NUM,ShareMemory::ReadWrite))
    {
        PrintAlm("Open LEAK_SHARE fail\n");
        exit(1);
    }
    pLeakData = (LeakDataStruct *)leakDataShare.data();
    PrintInfo("Open LEAK_SHARE ok\n");

    //创建接线盒共享内存
    if(!cabCtrDataShare.createShare(DC_SHARE,sizeof(CabCtrlDataStruct)*DC_MAX_NUM,ShareMemory::ReadWrite))
    {
        PrintAlm("Open DC_SHARE fail\n");
        exit(1);
    }
    pCabCtrlData = (CabCtrlDataStruct *)cabCtrDataShare.data();
    PrintInfo("Open DC_SHARE ok\n");

    //创建一体屏共享内存
    if(!lcdDataShare.createShare(LCD_CFG_SHARE, sizeof(LcdDataStruct), ShareMemory::ReadWrite))
    {
        PrintAlm("Open LCD_CFG_SHARE fail\n");
        exit(1);
    }
    PrintInfo("Open LCD_CFG_SHARE ok\n");
    pLcdData= (LcdDataStruct*)lcdDataShare.data();

    //创建spds共享内存
    if(!spdsDataShare.createShare(SPDS_SHARE,sizeof(SpdsDataStruct)*SPDS_MAX_NUM,ShareMemory::ReadWrite)){
        PrintAlm("Open SPDS_SHARE fail\n");
        exit(1);
    }
    PrintInfo("Open SPDS_SHARE Ok\n");
    pSpdsData = (SpdsDataStruct*)spdsDataShare.data();

    //创建短信报警器共享内存
    if(!smsDataShare.createShare(SMS_SHARE, sizeof(smsDataShare)*SMS_MAX_NUM,ShareMemory::ReadWrite)){
        PrintAlm("Open SMS_SHARE fail\n");
        exit(1);
    }
    PrintInfo("Open SMS_SHARE Ok\n");
    pSmsData = (SmsDataStruct*)smsDataShare.data();

    //创建电池巡检仪共享内存
    if(!bmsDataShare.createShare(BMS_SHARE, sizeof(bmsDataShare)*BMS_MAX_NUM,ShareMemory::ReadWrite)){
        PrintAlm("Open BMS_SHARE fail\n");
        exit(1);
    }
    PrintInfo("Open BMS_SHARE Ok\n");
    pBmsData = (BmsDataStruct*)bmsDataShare.data();
    //BMSTypeInit();
}

#endif //SYSCFGMNG_C

void CFG_Exit()
{
    qDebug()<<"Exit Share mem";
    sysCfgShare.releaseShare();
    eMtrDataShare.releaseShare();
    thsDataShare.releaseShare();
    acDataShare.releaseShare();
    upsDataShare.releaseShare();
    meterDataShare.releaseShare();
    leakDataShare.releaseShare();
    cabCtrDataShare.releaseShare();
    lcdDataShare.releaseShare();
    spdsDataShare.releaseShare();
    smsDataShare.releaseShare();
    bmsDataShare.releaseShare();

}
