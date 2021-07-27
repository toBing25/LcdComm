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

#ifndef WGTPARAITEM_AC_H
#define WGTPARAITEM_AC_H

#include <QWidget>
#include "loginmanage.h"
#include "Global/SysCfgMng.h"
#include "wgtparaitem_ups.h"

#define  AC_STATUS   0
#define  AC_PARA     1
#define  AC_SETTING  2

namespace Ui {
class wgtParaItem_ac;
}

//空调
class wgtDateTime;
class wgtParaItem_ac : public QWidget
{
    Q_OBJECT

public:
    enum AcRunStatusEnum
    {
        eAcStsIntegratedAlm, //综合告警:0：正常；1：故障
        eAcStsCoolingSts, //制冷状态:0：未制冷；1：制冷
        eAcStsMtrShutdown, //监控关机 0：开机运行 1：关机
        eAcStsFreezeProtect, //防冻保护 0：正常；1：故障
        eAcStsVFDCommBreak, //VFD通信失败 0：正常；1：故障
        eAcStsNTC1Fault, //(防冻)故障 0：正常；1：故障
        eAcStsVFDAlarm, //VFD报警 0：正常；1：故障
        eAcStsVFDFault, //VFD故障 0：正常；1：故障
        eAcStsOutletTempFault, //出风口温度故障 0：正常；1：故障
        eAcStsHighTempAlm, //高温告警 0：正常；1：故障

        eAcStsLowTempAlm, //低温告警 0：正常；1：故障
        eAcStsHighPressureAlm, //高压告警 0：正常；1：故障
        eAcStsHighPressureLock, //高压锁定 0：正常；1：故障
        eAcStsLowPressureAlm,//低压告警 0：正常；1：故障
        eAcStsLowPressureLock,//低压锁定 0：正常；1：故障
        eAcStsExhaustAlm, //排气报警 0：正常；1：故障
        eAcStsExhaustLock, //排气锁定 0：正常；1：故障
        eAcStsFanFault, //风机故障 0：正常；1：故障
        eAcStsInletTempFault, //进风口温度故障 0：正常；1：故障
        eAcStsAmbientTempFault, //环境温度故障 0：正常；1：故障

        eAcStsCondenserPressFault, //冷凝压力故障
        eAcStsRmtOffAlm, //远程关机报警
        eAcStsPowerLossAlm, //电源丢失报警
        eAcStsHeatSts, //加热器状态 0：未加热；1：加热
        eAcStsHumiSts, //加湿器状态 0：停止， 1：开启
        eAcStsDehumiSts, //除湿状态
        eAcStsHihumiAlm, //高湿告警
        eAcStsLowhumiAlm, //低湿报警
        eAcStsThunderAlm, //防雷告警
        eAcStsHumiOvFlow, //加湿过流

        eAcStsFloorOverflow, //地板溢水 0：正常；1：故障
        eAcStsAirflowLoss, //气流丢失
        eAcStsHumifault, //加湿器故障
        eAcStsHeaterFault, //加热器故障 0：正常；1：故障
        eAcStsInletHuimFault, //回风湿度故障
        eAcStsSmokeAlm, //烟感告警 0：正常；1：故障
        eAcStsPhaseLoss, //缺相告警 0：正常；1：故障
        eAcStsPhaseSequFault, //相序错误 0：正常；1：故障
        eAcStsNetworkFault, //组网失败报警 0：正常；1：故障
        eAcStsCompressorFault, //压缩机故障 0：正常；1：故障

        eAcStsDustFilterFault, //防尘网故障 0：正常；1：故障
        eAcStsRsvd42,
        eAcStsRsvd43,
        eAcStsRsvd44,
        eAcStsRsvd45,
        eAcStsRsvd46,
        eAcStsRsvd47,
        eAcStsRsvd48,
        eAcStsRsvd49,
        eAcStsRsvd50,

        eAcStsRsvd51,
        eAcStsRsvd52,
        eAcStsRsvd53,
        eAcStsRsvd54,
        eAcStsRsvd55,
        eAcStsRsvd56,
        eAcStsRsvd57,
        eAcStsRsvd58,
        eAcStsRsvd59,
        eAcStsRsvd60,

        eAcStsNum
    };

    enum AcRunParaEnum
    {
        eAcParaInletTemp,           //回风温度测量值, 0.1 ℃
        eAcParaAmbientTemp,         //环境温度测量值, 0.1 ℃
        eAcParaInnerFanRunTime,     //内风机运行时间, 1 小时
        eAcParaOuterFanRunTime,     //外风机运行时间, 1 小时
        eAcParaCompressorRunTime,   //压缩机运行时间, 1 小时
        eAcParaHighTempAlmVal,      //高温报警值, 0.1 ℃
        eAcParaLowTempAlmVal,       //低温报警值, 0.1 ℃
        eAcParaOutletTemp,          //送风温度测量值, 0.1 ℃
        eAcParaCoolingSetTemp,      //制冷设点, 0.1 ℃
        eAcParaCoolingRange,        //制冷范围, 0.1 ℃

        eAcParaCntlWays, //控制方式, 0:回风, 1:送风
        eAcParaDateYear, //
        eAcParaDateMonth,
        eAcParaDateDay,
        eAcParaDateHour,
        eAcParaDateMinute,
        eAcParaDateSecond,
        eAcParaRunSts, //运行状态:0：本地关机 1： 远程关机 2： 待机 3：监控关机 4：电源保护 5：气流保护 6：压机保护 7：系统开机
        eAcParaVFDFalutCode2,
        eAcParaVFDFalutCode1,

        eAcParaVFDAlmCode,
        eAcParaVFDStsCode,
        eAcParaReturnAirSet, //0.1 ℃
        eAcParaInletHumi, //回风湿度测量值, 1 %
        eAcParaHeaterRunTime, //电加热运行时间, 1 小时
        eAcParaHumiRunTime, //加湿器运行时间, 1 小时
        eAcParaPowerFreq, //电源频率, 0.1 Hz
        eAcParaHeaterSet, //加热设定, 0.1 ℃
        eAcParaHeatRange, //加热范围, 0.1 ℃
        eAcParaHiHumiAlmVal, //高湿报警值, 0.1

        eAcParaLowHumiAlmVal, //低湿报警值, 0.1
        eAcParaHumiSetVal, //湿度设点, 0.1 %
        eAcParaHumiRange, //湿度范围, 0.1
        eAcParaCoolCapacity, //制冷量, 0.1 KW
        eAcParaOutletHumi, //送风湿度测量值, 1 %
        eAcParaAmbientHumi, //环境湿度测量值, 1 %
        eAcParaInnerFanSpeed,     //内风机转速, 0.1 %
        eAcParaOuterFanSpeed,     //外风机转速, 0.1 %
        eAcParaCompressorSpeed,   //压缩机转速, 1 Hz (0-100)
        eAcParaSysTotalRunTime, //机组累计运行时间, 1 小时

        eAcParaRsvd41,
        eAcParaRsvd42,
        eAcParaRsvd43,
        eAcParaRsvd44,
        eAcParaRsvd45,
        eAcParaRsvd46,
        eAcParaRsvd47,
        eAcParaRsvd48,
        eAcParaRsvd49,
        eAcParaRsvd50,

        eAcParaNum
    };
    Q_ENUM(AcRunStatusEnum)
    Q_ENUM(AcRunParaEnum)

public:
    explicit wgtParaItem_ac(int id, QWidget *parent = nullptr);
    ~wgtParaItem_ac();

    void setCurrentId(int id);
private slots:
    void getAcInfo(QList<ac_Info> infoList);
    void onScrollBarChange(void);

protected:
    bool event(QEvent *ev);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    //创建参数界面的所有控件
    void createParaWidget(void);
    void clearAllWideget(void);

    void createSetParaWidget(void);
    void clearSetWideget(void);

    void createInfo(void);
    //匹配参数获取参数值
    int findParaValue(QString para,QString &Type/*out*/);

    //权限更新
    void permissUpdate(void);
    //使能指引按钮
    void setGuideBtntVisible();
    void setTabWidgetStyle(void);
private:
    void setupUi(void);
    void setConnectState(int state);

private:
    Ui::wgtParaItem_ac *ui;
    //wgtDateTime *m_lblDate;
    QList<Menu>  setList;
    UPS_List     m_acList;
    int          m_curId;
    QList<ac_Info> ac_list;
};

#endif // WGTPARAITEM_AC_H
