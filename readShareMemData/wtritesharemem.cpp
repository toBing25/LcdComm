#include "wtritesharemem.h"
#include "ThsData/ThsDataStruct.h"
#include "RatioStruct.h"
#include <QDebug>
#include "formmain.h"
WtriteShareMem::WtriteShareMem(QObject *parent) : QObject(parent)
{

}

void WtriteShareMem::writeDevData(int devType, int devNo, int door)
{

    int openDoorVal = DryOpFncEnum::eDryOpFncFrontCtrl;
    if(door == 1){
        openDoorVal = DryOpFncEnum::eDryOpFncRearCtrl;
    }
    int frontCtrlOpValue = 0;
    //获取接线盒接口功能
    for(int i=0;i<eDryOpNum;i++){
        int opValue = sysCfg->eMtrMain.drySet[devNo].dryFncOp[i];        
        if(opValue == openDoorVal){
            frontCtrlOpValue = i;
            break;
        }
    }
    //开门
    if((pCabCtrlData+devNo)->connectFlag == 2){
        //qDebug()<<"open door OK";
        (pCabCtrlData+devNo)->opSetCabCtrl[frontCtrlOpValue].bits.sts = 1;
        (pCabCtrlData+devNo)->opSetCabCtrl[frontCtrlOpValue].bits.changed = 1;
        //
        (pCabCtrlData+devNo)->flag.bits.opCabCtrlChanged = 1;
    }

}

void WtriteShareMem::writeThsDevData(ths_set thsset)
{
    qDebug()<<"write Ths";
    //
    if (thsset.devNo <0 or thsset.devNo>= THS_MAX_NUM){ return; }

    uint16 flag = (pThsData+thsset.devNo)->connectFlag;
    qDebug()<<"flag"<<flag<<"devno"<<thsset.devNo<<"para num"<<thsset.para<<thsset.val;
    if(flag == CONNECTED){
        uint16 paraVal = thsset.val / extDevRatio.eThsRatio[thsset.para];
        qDebug()<<"paraval"<<paraVal;
        (pThsData+thsset.devNo)->para.setValue[thsset.para] = paraVal;
        (pThsData+thsset.devNo)->para.flag[thsset.para].all = 3;

    }else{
        qDebug()<<"ths dev is disconnect";
    }


}

void WtriteShareMem::wirteUpsDevData(ups_set upsset)
{
    if (upsset.devNo <0 or upsset.devNo>= UPS_MAX_NUM){ return; }

    uint16 flag = (pUpsData+upsset.devNo)->connectFlag;
    if(flag == CONNECTED){
        (pUpsData+upsset.devNo)->ctl.setValue[upsset.para] = upsset.val;
        (pUpsData+upsset.devNo)->ctl.flag[upsset.para].changed  = 1;
    }else{
        qDebug()<<"ups dev is disconnect";
    }
}

void WtriteShareMem::writeAcDevData(ac_set acset)
{
    return;
    //qDebug()<<__LINE__<<"ac type="<<acset.devNo <<acset.paraType << acset.val;
    if (acset.devNo <0 or acset.devNo>= AC_MAX_NUM)
        return;
    uint16 flag = pAcData->connectFlag;
        if (flag == 2){

            int16 paraVal = 0;
            //写状态，暂时不需要
            if(acset.paraType == TYPE_STAT_SETTING){

            }else if(acset.paraType == TYPE_PARA_SETTING){
                //日期    //低8位有效

                //十进制输入 文本框和按位输入

                qDebug()<<"write lineEdit or bit";
                double dVal = acset.val / extDevRatio.eAcParaRatio[acset.union_ac.para];
                (pAcData+acset.devNo)->acRunPara.setValue[acset.union_ac.para] = paraVal;
                (pAcData+acset.devNo)->acRunPara.flag[acset.union_ac.para].all = 3;

            }
        }

//    uint16 flag = (pAcData+acset.devNo)->connectFlag;
//    if(flag == CONNECTED){
//        if(acset.paraType == TYPE_PARA_SETTING){    //写参数
//            uint16 paraVal = acset.val / extDevRatio.eAcRatio[acset.union_ac.para];
//            (pAcData+acset.devNo)->acRunPara.setValue[acset.union_ac.para] = paraVal;
//            (pAcData+acset.devNo)->acRunPara.flag[acset.union_ac.para].all = 3;
//        }else if(acset.paraType == TYPE_STAT_SETTING) {     //写状态
//            (pAcData+acset.devNo)->acRunSts.flag[acset.union_ac.sta_para].bit.changed = 1;
//            (pAcData+acset.devNo)->acRunSts.flag[acset.union_ac.sta_para].bit.control = acset.val;  //1关机 0开机
//        }


//    }else{
//        qDebug()<<"ac dev is disconnect";
//    }
}

void WtriteShareMem::writeLeakDevData(leak_set leakset)
{
    if (leakset.devNo <0 or leakset.devNo>= LEAK_MAX_NUM){ return; }

    uint16 flag = (pLeakData+leakset.devNo)->connectFlag;
    if(flag == CONNECTED){
        (pLeakData+leakset.devNo)->para.setValue[leakset.para] = leakset.val;
        (pLeakData+leakset.devNo)->para.flag[leakset.para].all = 3;


    }else{
        qDebug()<<"leak dev is disconnect";
    }
}
