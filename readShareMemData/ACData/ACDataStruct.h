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

#ifndef ACDATASTRUCT
#define ACDATASTRUCT

//#include "../DevInterface.h"
#include "GlobalPublic.h"
#include <QMap>
#include <QVector>
#include "languagePublic.h"

//Air conditioning run status, for eAcParaRunSts
#define AC_STS_LOCAL_SHUT   0 //Local shutdown
#define AC_STS_RMT_SHUT     1 //Remote shutdown
#define AC_STS_MTR_SHUT     2 //Monitor shutdown
#define AC_STS_STDBY        3 //Networking stdby
#define AC_STS_POWER_PROT   4 //Power protection
#define AC_STS_AIR_PROT     5 //Airflow protection
#define AC_STS_COMP_LOCK    6 //Compressor lock
#define AC_STS_SYS_RUN      7 //System run

/*----------------------------Enum----------------------------------------*/

enum AcRunStatusEnum
{
//用户参数***********************************************************************************
    IS_SYNT_ALM = 0, //综合告警; 0：正常；1：告警
    IS_ONOFF_STS, //开关机状态，0关机，1开机

    IS_IT_OH_ALM, //回风温度大于报警值; 0：正常；1：告警
    IS_IT_OL_ALM, //回风温度小于报警值; 0：正常；1：告警
    IS_IH_OH_ALM, //回风湿度大于报警值; 0：正常；1：告警
    IS_IH_OL_ALM, //回风湿度小于报警值; 0：正常；1：告警
    IS_OT_OH_ALM, //送风温度大于报警值; 0：正常；1：告警
    IS_OT_OL_ALM, //送风温度大于报警值; 0：正常；1：告警

    IS_SMK_ALM, //烟雾报警; 0：正常；1：告警
    IS_FSD_ALM, //水浸报警; 0：正常；1：告警
    IS_SYSDW_ALM, //系统排水故障; 0：正常；1：告警

    IS_LL_SV_STS, //液管电磁阀  //0 关 1 开 ?
    IS_CPUMP_STS, //冷凝排水泵; 0：关；1：开

    //输入电源------------------------------------------------------------
    IS_IPW_A_VOH_ALM, //A相电源过电压; 0：正常；1：告警
    IS_IPW_B_VOH_ALM, //B相电源过电压; 0：正常；1：告警
    IS_IPW_C_VOH_ALM, //C相电源过电压; 0：正常；1：告警
    IS_IPW_A_VOL_ALM, //A相电源欠电压; 0：正常；1：告警
    IS_IPW_B_VOL_ALM, //B相电源欠电压; 0：正常；1：告警
    IS_IPW_C_VOL_ALM, //C相电源欠电压; 0：正常；1：告警
    IS_IPW_PHUB_ALM, //三相不平衡; 0：正常；1：告警
    IS_IPW_FOL_ALM, //电源频率超下限; 0：正常；1：告警
    IS_IPW_FOH_ALM, //电源频率超上限; 0：正常；1：告警
    IS_IPW_A_PHL_ALM, //A电源缺相告警; 0：正常；1：告警
    IS_IPW_B_PHL_ALM, //B电源缺相告警; 0：正常；1：告警
    IS_IPW_C_PHL_ALM, //C电源缺相告警; 0：正常；1：告警
    IS_IPW_PHSE_ALM, //电源错相; 0：正常；1：告警


    //风机------------------------------------------------------------
    IS_EFAN_STS, //内风机运行状态; 0：关；1：开
    IS_EFAN1_ALM, //内风机1故障; 0：正常；1：告警
    IS_EFAN2_ALM, //内风机2故障; 0：正常；1：告警
    IS_EFAN3_ALM, //内风机3故障; 0：正常；1：告警
    IS_AF_LOST_ALM, //风机气流丢失告警; 0：正常；1：告警
    IS_EFAN_FLT_ALM, //过滤网堵塞; 0：正常；1：告警
    IS_CFAN_STS, //外风机运行状态; 0：关；1：开
    IS_CFAN_ALM, //外风机告警; 0：正常；1：告警

    //压缩机------------------------------------------------------------
    IS_CMPR_STS, //压缩机运行状态; 0：关；1：开
    IS_CMPR_HT_STS, //压缩机加热带; 0：关；1：开
    IS_CMPR_HP_ALM, //高压告警; 0：正常；1：告警
    IS_CMPR_HPLK_ALM, //高压告警锁定; 0：正常；1：告警
    IS_CMPR_LP_ALM, //低压告警; 0：正常；1：告警
    IS_CMPR_LPLK_ALM, //低压告警锁定; 0：正常；1：告警
    IS_CMPR_EXSU_HT_ALM, //排气高温告警; 0：正常；1：告警
    IS_CMPR_EXSU_HTLK_ALM, //排气高温锁定; 0：正常；1：告警
    IS_CMPR_F_ABN, //压缩机频率异常告警
    IS_CMPR_F_ABNLK, //压缩机频率异常告警锁定
    IS_CMPR_HP_LMTF, //高压限频输出

    //变频器------------------------------------------------------------
    IS_VFD_COMM_FLT, //变频器通讯故障; 0：正常；1：告警
    IS_VFD_AVOI, //加速过电流（OC1）; 0：正常；1：告警
    IS_VFD_DVOI, //减速过电流（OC2）; 0：正常；1：告警
    IS_VFD_SVOI, //恒速过电流（OC3）; 0：正常；1：告警
    IS_VFD_BAD_FLT, //压缩机变频器严重故障; 0：正常；1：告警
    IS_VFD_AVOV, //加速过电压（OV1）; 0：正常；1：告警
    IS_VFD_DVOV, //减速过电压（OV2）; 0：正常；1：告警
    IS_VFD_SVOV, //恒速过电压（OV3）; 0：正常；1：告警
    IS_VFD_BUS_LV, //母线欠压故障（UV）; 0：正常；1：告警
    IS_VFD_MOL, //电机过载（OL1）; 0：正常；1：告警
    IS_VFD_OL, //变频器过载（OL2）; 0：正常；1：告警
    IS_VFD_SPI, //输入侧缺相（SPI）; 0：正常；1：告警
    IS_VFD_SPO, //输出侧缺相（SPO）; 0：正常；1：告警
    IS_VFD_REV_OT, //整流模块过热（OH1）; 0：正常；1：告警
    IS_VFD_INV_OT, //逆变模块过热故障; 0：正常；1：告警
    IS_VFD_485_FLT, //485通讯故障（CE）; 0：正常；1：告警
    IS_VFD_IDTT_FLT, //电流检测故障（ItE）; 0：正常；1：告警
    IS_VFD_EEPROM_FLT, //EEPROM操作故障（EEP）; 0：正常；1：告警
    IS_VFD_ETH1, //对地短路故障1（ETH1）; 0：正常；1：告警
    IS_VFD_ETH2, //对地短路故障2（ETH2）; 0：正常；1：告警
    IS_VFD_DEU, //速度偏差故障（dEu）; 0：正常；1：告警
    IS_VFD_STO, //失调故障（STo）; 0：正常；1：告警
    IS_VFD_LL, //欠载故障（LL）; 0：正常；1：告警

    //加湿器------------------------------------------------------------
    IS_HUMI_STS, //加湿器运行状态; 0：关；1：开
    IS_HUMI_ALM, //加湿器故障; 0：正常；1：告警
    IS_HUMI_AHI_DTT_ALM, //加湿电流检测故障; 0：正常；1：告警
    IS_HUMI_DWV_ALM, //加湿桶排水阀故障; 0：正常；1：告警
    IS_HUMI_WFLWL_ALM, //湿膜低水位告警; 0：正常；1：告警
    IS_HUMI_WLS_ALM, //水位开关故障; 0：正常；1：告警
    IS_HUMI_CWHWL_ALM, //冷凝水高水位告警; 0：正常；1：告警
    IS_EH_IW_STS, //电极加湿进水阀     //0 关 1 开
    IS_EH_OW_STS, //电极加湿排水阀
    IS_HPUMP_STS, //湿膜加湿循环水泵
    IS_WFH_IWSV_STS, //湿膜加湿进水阀

    //加热器------------------------------------------------------------
    IS_HEAT_STS, //加热器状态; 0：关；1：开
    IS_HEAT_ALM, //加热器故障; 0：正常；1：告警
    IS_HEAT_PTT_ALM, //电加热保护告警; 0：正常；1：告警
    IS_HEAT1_STS, //加热器状态; 0：关；1：开
    IS_HEAT2_STS, //加热器状态; 0：关；1：开
    IS_HEAT1_ALM, //加热器1故障; 0：正常；1：告警
    IS_HEAT2_ALM, //加热器2故障; 0：正常；1：告警

    //传感器故障------------------------------------------------------------
    IS_ITSD1_FLT, //回风温度探头1故障; 0：正常；1：告警
    IS_ITSD2_FLT, //回风温度探头2故障; 0：正常；1：告警
    IS_IHSD1_FLT, //回风湿度探头故障; 0：正常；1：告警
    IS_OTSD1_FLT, //送风温度探头1故障; 0：正常；1：告警
    IS_OTSD2_FLT, //送风温度探头2故障; 0：正常；1：告警
    IS_OTSD3_FLT, //送风温度探头3故障; 0：正常；1：告警
    IS_OHSD1_FLT, //送风湿度探头故障; 0：正常；1：告警
    IS_EXSU_TSD_FLT, //排气温度探头故障; 0：正常；1：告警
    IS_SUCT_TSD_FLT, //吸气温度探头故障; 0：正常；1：告警
    IS_SUCT_PSD_FLT, //吸气压力探头故障; 0：正常；1：告警
    IS_EXSU_PSD_FLT, //排气压力探头故障; 0：正常；1：告警
    IS_LEWYSD_FLT, //风压差传感器故障; 0：正常；1：告警
    IS_RTHSD0_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD1_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD2_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD3_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD4_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD5_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD6_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD7_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD8_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD9_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD10_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD11_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD12_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD13_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD14_FLT, //远程温湿度探头n故障; 0：正常；1：告警
    IS_RTHSD15_FLT, //远程温湿度探头n故障; 0：正常；1：告警

    //组网----------------------------------------------------------------
    IS_NET_COMM_FLT, //组网通讯故障; 0：正常；1：告警
    IS_NET_ADDR_CNF, //组网地址冲突; 0：正常；1：告警
    IS_NET_SFVER_ERR, //组网软件版本不匹配; 0：正常；1：告警
    IS_NET_S00_OFFLN, //从机00离线; 0：正常；1：告警
    IS_NET_S01_OFFLN, //从机01离线; 0：正常；1：告警
    IS_NET_S02_OFFLN, //从机02离线; 0：正常；1：告警
    IS_NET_S03_OFFLN, //从机03离线; 0：正常；1：告警
    IS_NET_S04_OFFLN, //从机04离线; 0：正常；1：告警
    IS_NET_S05_OFFLN, //从机05离线; 0：正常；1：告警
    IS_NET_S06_OFFLN, //从机06离线; 0：正常；1：告警
    IS_NET_S07_OFFLN, //从机07离线; 0：正常；1：告警
    IS_NET_S08_OFFLN, //从机08离线; 0：正常；1：告警
    IS_NET_S09_OFFLN, //从机09离线; 0：正常；1：告警
    IS_NET_S10_OFFLN, //从机10离线; 0：正常；1：告警
    IS_NET_S11_OFFLN, //从机11离线; 0：正常；1：告警
    IS_NET_S12_OFFLN, //从机12离线; 0：正常；1：告警
    IS_NET_S13_OFFLN, //从机13离线; 0：正常；1：告警
    IS_NET_S14_OFFLN, //从机14离线; 0：正常；1：告警
    IS_NET_S15_OFFLN, //从机15离线; 0：正常；1：告警
    IS_NET_S16_OFFLN, //从机16离线; 0：正常；1：告警
    IS_NET_S17_OFFLN, //从机17离线; 0：正常；1：告警
    IS_NET_S18_OFFLN, //从机18离线; 0：正常；1：告警
    IS_NET_S19_OFFLN, //从机19离线; 0：正常；1：告警
    IS_NET_S20_OFFLN, //从机20离线; 0：正常；1：告警
    IS_NET_S21_OFFLN, //从机21离线; 0：正常；1：告警
    IS_NET_S22_OFFLN, //从机22离线; 0：正常；1：告警
    IS_NET_S23_OFFLN, //从机23离线; 0：正常；1：告警
    IS_NET_S24_OFFLN, //从机24离线; 0：正常；1：告警
    IS_NET_S25_OFFLN, //从机25离线; 0：正常；1：告警
    IS_NET_S26_OFFLN, //从机26离线; 0：正常；1：告警
    IS_NET_S27_OFFLN, //从机27离线; 0：正常；1：告警
    IS_NET_S28_OFFLN, //从机28离线; 0：正常；1：告警
    IS_NET_S29_OFFLN, //从机29离线; 0：正常；1：告警
    IS_NET_S30_OFFLN, //从机30离线; 0：正常；1：告警
    IS_NET_S31_OFFLN, //从机31离线; 0：正常；1：告警
    IS_NET_S32_OFFLN, //从机32离线; 0：正常；1：告警
    IS_NET_S33_OFFLN, //从机33离线; 0：正常；1：告警
    IS_NET_S34_OFFLN, //从机34离线; 0：正常；1：告警
    IS_NET_S35_OFFLN, //从机35离线; 0：正常；1：告警
    IS_NET_S36_OFFLN, //从机36离线; 0：正常；1：告警
    IS_NET_S37_OFFLN, //从机37离线; 0：正常；1：告警
    IS_NET_S38_OFFLN, //从机38离线; 0：正常；1：告警
    IS_NET_S39_OFFLN, //从机39离线; 0：正常；1：告警
    IS_NET_S40_OFFLN, //从机40离线; 0：正常；1：告警
    IS_NET_S41_OFFLN, //从机41离线; 0：正常；1：告警
    IS_NET_S42_OFFLN, //从机42离线; 0：正常；1：告警
    IS_NET_S43_OFFLN, //从机43离线; 0：正常；1：告警
    IS_NET_S44_OFFLN, //从机44离线; 0：正常；1：告警
    IS_NET_S45_OFFLN, //从机45离线; 0：正常；1：告警
    IS_NET_S46_OFFLN, //从机46离线; 0：正常；1：告警
    IS_NET_S47_OFFLN, //从机47离线; 0：正常；1：告警
    IS_NET_S48_OFFLN, //从机48离线; 0：正常；1：告警
    IS_NET_S49_OFFLN, //从机49离线; 0：正常；1：告警
    IS_NET_S50_OFFLN, //从机50离线; 0：正常；1：告警
    IS_NET_S51_OFFLN, //从机51离线; 0：正常；1：告警
    IS_NET_S52_OFFLN, //从机52离线; 0：正常；1：告警
    IS_NET_S53_OFFLN, //从机53离线; 0：正常；1：告警
    IS_NET_S54_OFFLN, //从机54离线; 0：正常；1：告警
    IS_NET_S55_OFFLN, //从机55离线; 0：正常；1：告警
    IS_NET_S56_OFFLN, //从机56离线; 0：正常；1：告警
    IS_NET_S57_OFFLN, //从机57离线; 0：正常；1：告警
    IS_NET_S58_OFFLN, //从机58离线; 0：正常；1：告警
    IS_NET_S59_OFFLN, //从机59离线; 0：正常；1：告警
    IS_NET_S60_OFFLN, //从机60离线; 0：正常；1：告警
    IS_NET_S61_OFFLN, //从机61离线; 0：正常；1：告警
    IS_NET_S62_OFFLN, //从机62离线; 0：正常；1：告警
    IS_NET_S63_OFFLN, //从机63离线; 0：正常；1：告警

    //DI输入状态映射
    IS_DI_EFAN_OL_1, //风机过载1    0：正常；1：告警
    IS_DI_EFAN_OL_2, //风机过载2    0：正常；1：告警
    IS_DI_EFAN_OL_3, //风机过载3    0：正常；1：告警
    IS_DI_HP_ONOFF, //高压开关        0关 1开
    IS_DI_LP_ONOFF, //低压开关        0关 1开
    IS_DI_WFH_LWL, //湿膜加湿低水位    0：正常；1：告警
    IS_DI_CL_HWL, //冷凝高水位    0：正常；1：告警
    IS_DI_HEAT_OL, //电加热过载    0：正常；1：告警
    IS_DI_CFAN_ALM, //室外风机    0：正常；1：告警
    IS_DI_AF_LS, //气流流失        0：正常；1：告警
    IS_DI_FLT_BLK, //过滤网堵塞    0：正常；1：告警
    IS_DI_SMK_ALM, //烟雾报警    0：正常；1：告警
    IS_DI_ONOFF, //远程开关机    0关 1开

    IS_DI_RESV_194, //预留
    IS_DI_RESV_195, //预留
    IS_DI_RESV_196, //预留
    IS_DI_RESV_197, //预留
    IS_DI_RESV_198, //预留
    IS_DI_RESV_199, //预留

    eAcStsNum
};

enum AcRunParaEnum
{
//对应Modbus功能码0x03，此功能码对应读写模拟量
//如无特殊说明默认数据类型为int16
//用户参数***********************************************************************************
    //一般设置
    RU_ONOFF_CTL = 0, //开关机控制, 0x0000关机，0xFF00开机
    RU_CTL_WAYS, //控制方式
    RU_OTS_x10, //送风温度设定值
    RU_ITS_x10, //回风温度设定值
    RU_HS_x10, //湿度设定值
    RU_AS_DLY, //来电自启动延时 S
    RU_AS_DLY_EN, //来电自启动使能
    RU_USER_CFG_CLN, //用户参数复位
    RU_FLTL_CLEAR, //故障清除
    RU_CLCTL_PR_x10, //制冷控制比例带值
    RU_HTCTL_PR_x10, //制热控制比例带值
    RU_IAF_AHCTL_PR_x10, //回风加湿比例带值
    RU_IAF_DHCTL_PR_x10, //回风除湿比例带值
    RU_TCTL_DF_x10, //温度控制设定死区
    RU_HCTL_DF_x10, //湿度控制设定死区

    //时间设置
    RU_DATE_YY, //年
    RU_DATE_MM, //月
    RU_DATE_DD, //日
    RU_TIME_HH, //时
    RU_TIME_MM, //分
    RU_TIME_SS, //秒

    //远程通信设置
    RU_RMDBS_ADDR, //远程Modbus通讯地址
    RU_RMDBS_BAUD, //远程Modbus通讯波特率

    //组网设置
    RU_NET_EN, //组网功能使能
    RU_NET_ADDR, //组网地址
    RU_NET_NUMS, //组网数量
    RU_NET_SW_NUMS, //切换数量
    RU_NET_RTT_CYC, //轮值周期
    RU_NET_WK_NUMS, //运行数量
    RU_NET_RQ_SYNC, //需求同步
    RU_NET_OV_FUNC, //层叠功能
    RU_NET_HT_ALLON, //高温同开

    //预留
    RU_USER_RESV_032, //预留
    RU_USER_RESV_033, //预留
    RU_USER_RESV_034, //预留
    RU_USER_RESV_035, //预留
    RU_USER_RESV_036, //预留
    RU_USER_RESV_037, //预留
    RU_USER_RESV_038, //预留
    RU_USER_RESV_039, //预留
    RU_USER_RESV_040, //预留
    RU_USER_RESV_041, //预留
    RU_USER_RESV_042, //预留
    RU_USER_RESV_043, //预留
    RU_USER_RESV_044, //预留
    RU_USER_RESV_045, //预留
    RU_USER_RESV_046, //预留
    RU_USER_RESV_047, //预留
    RU_USER_RESV_048, //预留
    RU_USER_RESV_049, //预留
    RU_USER_RESV_050, //预留
    RU_USER_RESV_051, //预留
    RU_USER_RESV_052, //预留
    RU_USER_RESV_053, //预留
    RU_USER_RESV_054, //预留
    RU_USER_RESV_055, //预留
    RU_USER_RESV_056, //预留
    RU_USER_RESV_057, //预留
    RU_USER_RESV_058, //预留
    RU_USER_RESV_059, //预留
    RU_USER_RESV_060, //预留
    RU_USER_RESV_061, //预留
    RU_USER_RESV_062, //预留
    RU_USER_RESV_063, //预留
    RU_USER_RESV_064, //预留
    RU_USER_RESV_065, //预留
    RU_USER_RESV_066, //预留
    RU_USER_RESV_067, //预留
    RU_USER_RESV_068, //预留
    RU_USER_RESV_069, //预留
    RU_USER_RESV_070, //预留
    RU_USER_RESV_071, //预留
    RU_USER_RESV_072, //预留
    RU_USER_RESV_073, //预留
    RU_USER_RESV_074, //预留
    RU_USER_RESV_075, //预留
    RU_USER_RESV_076, //预留
    RU_USER_RESV_077, //预留
    RU_USER_RESV_078, //预留
    RU_USER_RESV_079, //预留
    RU_USER_RESV_080, //预留
    RU_USER_RESV_081, //预留
    RU_USER_RESV_082, //预留
    RU_USER_RESV_083, //预留
    RU_USER_RESV_084, //预留
    RU_USER_RESV_085, //预留
    RU_USER_RESV_086, //预留
    RU_USER_RESV_087, //预留
    RU_USER_RESV_088, //预留
    RU_USER_RESV_089, //预留
    RU_USER_RESV_090, //预留
    RU_USER_RESV_091, //预留
    RU_USER_RESV_092, //预留
    RU_USER_RESV_093, //预留
    RU_USER_RESV_094, //预留
    RU_USER_RESV_095, //预留
    RU_USER_RESV_096, //预留
    RU_USER_RESV_097, //预留
    RU_USER_RESV_098, //预留
    RU_USER_RESV_099, //预留

//维修参数***********************************************************************************
    //一般设置
    RM_VACUMD_EN, //抽真空模式使能
    RM_MMD_EN, //手动模式使能
    RM_MMD_FANRS_CTL_x10, //内风机转速控制
    RM_MMD_ODM_CTL, //室外机控制
    RM_MMD_HEAT_CTL, //电加热启停控制
    RM_MMD_HUMI_CTL, //加湿器启停控制
    RM_MMD_INWSV_CTL, //进水电磁阀控制
    RM_MMD_DEWSV_CTL, //排水电磁阀控制
    RM_MMD_LLSV_CTL, //液管电磁阀
    RM_MMD_CMPRF, //压缩机频率
    RM_MMD_EEV_OPEN_CTLMD, //EEV开度控制模式
    RM_MMD_EEV_OPEN, //EEV手动开度
    RM_MMD_DBTM, //手动模式调试时间

    //参数修正
    RM_AFV_CF_x100, //内风量修正系数a
    RM_CCP_CF_x100, //制冷量修正系数b
    RM_ITSD1_CF_x10, //回风温度1修正
    RM_ITSD2_CF_x10, //回风温度2修正
    RM_OTSD1_CF_x10, //送风温度1修正
    RM_OTSD2_CF_x10, //送风温度2修正
    RM_OTSD3_CF_x10, //送风温度3修正
    RM_IHSD1_CF_x10, //回风湿度修正
    RM_OHSD1_CF_x10, //送风湿度修正
    RM_SUCTT_CF_x10, //吸气温度修正
    RM_EXSUT_CF_x10, //排气温度修正
    RM_SUCTP_CF_x10, //吸气压力修正
    RM_EXSUP_CF_x10, //排气压力修正
    RM_LEWY_CF_x10, //风压差值修正
    RM_AHI_CF_x10, //加湿电流修正
    RM_RTSD01_CF_x10, //远程温度探头01修正
    RM_RTSD02_CF_x10, //远程温度探头02修正
    RM_RTSD03_CF_x10, //远程温度探头03修正
    RM_RTSD04_CF_x10, //远程温度探头04修正
    RM_RTSD05_CF_x10, //远程温度探头05修正
    RM_RTSD06_CF_x10, //远程温度探头06修正
    RM_RTSD07_CF_x10, //远程温度探头07修正
    RM_RTSD08_CF_x10, //远程温度探头08修正
    RM_RTSD09_CF_x10, //远程温度探头09修正
    RM_RTSD10_CF_x10, //远程温度探头10修正
    RM_RTSD11_CF_x10, //远程温度探头11修正
    RM_RTSD12_CF_x10, //远程温度探头12修正
    RM_RTSD13_CF_x10, //远程温度探头13修正
    RM_RTSD14_CF_x10, //远程温度探头14修正
    RM_RTSD15_CF_x10, //远程温度探头15修正
    RM_RTSD16_CF_x10, //远程温度探头16修正
    RM_RHSD01_CF_x10, //远程湿度探头01修正
    RM_RHSD02_CF_x10, //远程湿度探头02修正
    RM_RHSD03_CF_x10, //远程湿度探头03修正
    RM_RHSD04_CF_x10, //远程湿度探头04修正
    RM_RHSD05_CF_x10, //远程湿度探头05修正
    RM_RHSD06_CF_x10, //远程湿度探头06修正
    RM_RHSD07_CF_x10, //远程湿度探头07修正
    RM_RHSD08_CF_x10, //远程湿度探头08修正
    RM_RHSD09_CF_x10, //远程湿度探头09修正
    RM_RHSD10_CF_x10, //远程湿度探头10修正
    RM_RHSD11_CF_x10, //远程湿度探头11修正
    RM_RHSD12_CF_x10, //远程湿度探头12修正
    RM_RHSD13_CF_x10, //远程湿度探头13修正
    RM_RHSD14_CF_x10, //远程湿度探头14修正
    RM_RHSD15_CF_x10, //远程湿度探头15修正
    RM_RHSD16_CF_x10, //远程湿度探头16修正

    //清零控制
    RM_SYS_WKTM_CLN, //机组运行时间清零
    RM_EFAN_WKTM_CLN, //风机运行时间清零
    RM_HUMI_WKTM_CLN, //加湿运行时间清零
    RM_HEAT1_WKTM_CLN, //加热1运行时间清零
    RM_HEAT2_WKTM_CLN, //加热2运行时间清零
    RM_FLT_WKTM_CLN, //滤网运行时间清零
    RM_ODM_WKTM_CLN, //外机运行时间清零
    RM_CPUMP_WKTM_CLN, //冷凝水泵运行时间清零
    RM_CMPR_WKTM_CLN, //压机运行时间清零

    //预留
    RM_RESV_169, //预留
    RM_RESV_170, //预留
    RM_RESV_171, //预留
    RM_RESV_172, //预留
    RM_RESV_173, //预留
    RM_RESV_174, //预留
    RM_RESV_175, //预留
    RM_RESV_176, //预留
    RM_RESV_177, //预留
    RM_RESV_178, //预留
    RM_RESV_179, //预留
    RM_RESV_180, //预留
    RM_RESV_181, //预留
    RM_RESV_182, //预留
    RM_RESV_183, //预留
    RM_RESV_184, //预留
    RM_RESV_185, //预留
    RM_RESV_186, //预留
    RM_RESV_187, //预留
    RM_RESV_188, //预留
    RM_RESV_189, //预留
    RM_RESV_190, //预留
    RM_RESV_191, //预留
    RM_RESV_192, //预留
    RM_RESV_193, //预留
    RM_RESV_194, //预留
    RM_RESV_195, //预留
    RM_RESV_196, //预留
    RM_RESV_197, //预留
    RM_RESV_198, //预留
    RM_RESV_199, //预留
    RM_RESV_200, //预留
    RM_RESV_201, //预留
    RM_RESV_202, //预留
    RM_RESV_203, //预留
    RM_RESV_204, //预留
    RM_RESV_205, //预留
    RM_RESV_206, //预留
    RM_RESV_207, //预留
    RM_RESV_208, //预留
    RM_RESV_209, //预留
    RM_RESV_210, //预留
    RM_RESV_211, //预留
    RM_RESV_212, //预留
    RM_RESV_213, //预留
    RM_RESV_214, //预留
    RM_RESV_215, //预留
    RM_RESV_216, //预留
    RM_RESV_217, //预留
    RM_RESV_218, //预留
    RM_RESV_219, //预留
    RM_RESV_220, //预留
    RM_RESV_221, //预留
    RM_RESV_222, //预留
    RM_RESV_223, //预留
    RM_RESV_224, //预留
    RM_RESV_225, //预留


//工厂参数***********************************************************************************
    //一般设置
    RF_AC_MODEL, //空调型号
    RF_REFRIG_TYPE, //制冷剂类型；0：R410A
    RF_T_CALC_WAYS, //温度计算方式设置
    RF_EFAN_CTL_WAYS, //风机控制方式设置
    RF_CTL_CYC, //需求控制计算周期
    RF_PRT_CYC, //保护调节计算周期
    RF_EFAN_FLT_MNTNTM, //滤网维护时间设置，未使用
    RF_LEWY_CTL_x10, //风压差控制设定值
    RF_DH_LEWY_CF_x10, //除湿风压差系数值
    RF_LEWYCTL_CF_x10, //风压差控制偏差值
    RF_CPUMP_MIN_WKTM, //冷凝水泵运行时间
    RF_WFWV_ON_TM, //湿膜水阀开启时间
    RF_WFWV_OFF_TM, //湿膜水阀关闭时间
    RF_DHLT_LMT_DF_x10, //除湿低温限制差值
    RF_IDM_AFV_DISP_EN, //室内机风量显示使能
    RF_SYS_CCP_DISP_EN, //机组制冷量显示使能
    RF_CLAHMD_EN, //制冷加湿模式使能
    RF_DHCTL_EN, //除湿控制使能
    RF_DEV_EN1, //设备使能1
    RF_DEV_EN2, //设备使能2
    RF_DEV_EN3, //设备使能3
    RF_IT_OH_x10, //回风高温告警值
    RF_IT_OL_x10, //回风低温告警值
    RF_OT_OH_x10, //送风高温告警值
    RF_OT_OL_x10, //送风低温告警值
    RF_H_OH_x10, //高湿告警值
    RF_H_OL_x10, //低湿告警值

    //压缩机设置
    RF_HPFLT_SET_x10, //高压报警设置值
    RF_LPFLT_SET_x10, //低压报警设置值
    RF_LLCL_EN, //轻载制冷功能使能
    RF_PRT_MIN_TM, //保护调节最短时间,未使用
    RF_CMPR_ON_TM, //压机启动特征时间,未使用
    RF_CMPR_OFF_TM1, //压机关闭特征时间1
    RF_CMPR_OFF_TM2, //压机关闭特征时间2
    RF_CLMD_INND_x10, //制冷模式进入需求
    RF_CLCMPR_OFFND_x10, //制冷压机停机需求
    RF_CMPR_EXSU_HPPRT_x10, //排气高压保护值
    RF_CMPR_SUCT_LPPRT_x10, //吸气低压保护值
    RF_CMPR_IPM_PRTT_x10, //IPM保护调节温度
    RF_CMPR_VFD_PRTI_x10, //变频器保护电流
    RF_CMPR_EXSU_HT_x10, //排气高温告警值
    RF_CMPR_PAHP_PC1_x10, //保护调节高压值Pc1
    RF_CMPR_PAHP_PC2_x10, //保护调节高压值Pc2
    RF_CMPR_PAHP_PC3_x10, //保护调节高压值Pc3
    RF_CMPR_PAHP_PC4_x10, //保护调节高压值Pc4
    RF_CMPR_PAHP_PC5_x10, //保护调节高压值Pc5
    RF_CMPR_PDF_MUL_x10, //压差倍数
    RF_CMPR_MINR_TM, //压机最短运行时间
    RF_CMPR_MINS_TM, //压机最短停止时间
    RF_CMPR_STLP_DLY, //压机启动低压延迟
    RF_CMPR_LPPRT_DLY, //压机低压保护延时，未使用
    RF_CMPR_SSTEP_F_x10, //压机启动台阶频率
    RF_CMPR_SHOLD_F_x10, //压机启动保持频率
    RF_CMPR_DWFPRT_F_x10, //压机降频保护频率
    RF_CMPR_SHOLD_TM, //压机启动保持时间
    RF_CMPR_VFD_STEP_x10, //变频压机调节步长
    RF_CMPR_VFD_REOF_x10, //变频压机回油频率
    RF_CMPR_REO_RISKF_x10, //压机回油风险频率
    RF_CMPR_CMPL_REOTM, //压机强制回油时间
    RF_CMPR_CP_x10, //制冷压机比例系数
    RF_CMPR_CI_x10, //制冷压机积分时间
    RF_CMPR_HP_x10, //除湿压机比例系数
    RF_CMPR_HI_x10, //除湿压机积分时间
    RF_CMPR_PAP, //保护调节比例系数,未使用
    RF_CMPR_PAI, //保护调节积分时间,未使用
    RF_CMPR_CTL_CYC, //变频压机控制周期
    RF_CMPR_RATE_F_x10, //变频压机额定频率
    RF_CMPR_MIN_F_x10, //变频压机最低频率
    RF_CMPR_CRS_NC1_x10, //压机特征转速Nc1
    RF_CMPR_CRS_NC2_x10, //压机特征转速Nc2
    RF_CMPR_CRS_NC3_x10, //压机特征转速Nc3
    RF_CMPR_CRS_NC4_x10, //压机特征转速Nc4
    RF_CMPR_CRS_NC5_x10, //压机特征转速Nc5
    RF_CMPR_CRS_NC6_x10, //压机特征转速Nc6
    RF_CMPR_RESN1_H_x10, //压机共振点1上限
    RF_CMPR_RESN1_L_x10, //压机共振点1下限
    RF_CMPR_RESN2_H_x10, //压机共振点2上限
    RF_CMPR_RESN2_L_x10, //压机共振点2下限
    RF_CMPR_RESN3_H_x10, //压机共振点3上限
    RF_CMPR_RESN3_L_x10, //压机共振点3下限
    RF_CMPR_RESN4_H_x10, //压机共振点4上限
    RF_CMPR_RESN4_L_x10, //压机共振点4下限
    RF_CMPR_RESN5_H_x10, //压机共振点5上限
    RF_CMPR_RESN5_L_x10, //压机共振点5下限
    RF_VFD_AVTM, //加速时间
    RF_VFD_DVTM, //减速时间
    RF_VFD_MAX_F_x10, //最大频率
    RF_VFD_MIN_F_x10, //最小频率

    //风机
    RF_EFAN_UPSPEED_x10, //风机启动百分比
    RF_EFAN_NUM, //室内风机数量设置
    RF_EFAN_GRPS, //室内风机组数
    RF_EFAN_RATE_RS_x10, //室内风机额定转速
    RF_EFAN_MIN_RS_x10, //室内风机最低转速
    RF_EFAN_CTL_CYC, //室内风机控制周期
    RF_EFAN_RSP_x10, //风机转速比例系数
    RF_EFAN_RSI_x10, //风机转速积分时间
    RF_EFAN_CIOT_DF_x10, //制冷送回风温差值
    RF_EFAN_HIOT_DF_x10, //除湿送回风温差值
    RF_EFAN_IOT_AQ_CYC, //送回温差采集周期
    RF_EFAN_ON_DLY, //开机室内风机延时

    //膨胀阀设置
    RF_EEV_MAX_OPEN, //EEV最大开度设置
    RF_EEV_MIN_OPEN, //EEV最小开度设置
    RF_EEV_CTL_CYC, //EEV调节周期设置
    RF_EEV_INIT_OPEN, //EEV压机初始开度
    RF_EEV_BLNC_OPEN, //EEV压机平衡开度
    RF_EEV_BLNC_TM, //EEV压机平衡时间
    RF_EEV_INIT_TM, //EEV压机初始时间
    RF_EEV_OFF_DLY, //EEV关闭延迟时间
    RF_EEV_ET_MOP_x10, //蒸发温度MOP设定
    RF_EEV_MOPP_x10, //EEV—MOP比例值
    RF_EEV_MOPI_x10, //EEV—MOP积分值
    RF_EEV_KPC_x10, //EEV压机运行Kpc值
    RF_EEV_TIC_x10, //EEV压机运行Tic值
    RF_EEV_CSSHT_x10, //压机吸气过热度值
    RF_EEV_HSSHT_x10, //轻载除湿吸气过热

    //加湿器设置
    RF_HUMI_TYPE, //加湿器类型设置
    RF_HUMI_INH_KG_x10, //加湿公斤数设置
    RF_HUMI_IPV, //加湿器输入电压
    RF_HUMI_IPPS, //加湿器电源相数
    RF_HUMI_INH_PHTI, //加湿预热电流比
    RF_HUMI_INH_PHTTM, //加湿桶预热时间
    RF_HUMI_INH_INWI, //加湿注水电流比
    RF_HUMI_INH_INWSI, //加湿注水停止电流比
    RF_HUMI_INH_DEWI, //加湿排水电流比
    RF_HUMI_NDEWI, //停止排水电流比
    RF_HUMI_MIN_DEWTM, //最小排水时间值
    RF_HUMI_DEW_TIMS, //加湿桶排水次数
    RF_HUMI_SBDEM_CYC, //待机排水周期值
    RF_HUMI_DEM_TM, //加湿桶排水时间
    RF_HUMI_WKDEM_CYC, //加湿自排水间隔
    RF_HUMI_HWTV, //高水位阀值
    RF_HUMI_HWDF, //高水位偏差
    RF_HUMI_CLN_TIMS, //加湿桶清洁次数

    //加热器设置
    RF_HEAT_CLASS, //加热分级
    RF_HEATCLASS1_UP_x10, //1级制热上限
    RF_HEATCLASS1_DF_x10, //1级制热回差
    RF_HEATCLASS2_UP_x10, //2级制热上限
    RF_HEATCLASS2_DF_x10, //2级制热回差
    RF_HEATCLASS3_UP_x10, //3级制热上限
    RF_HEATCLASS3_DF_x10, //3级制热回差

    //电源设置
    RF_PHAS_SERR_ALW, //相序容错
    RF_IPW_VOL, //输入电压欠压
    RF_IPW_VOH, //输入电压过压
    RF_IPW_VDF, //电压回差
    RF_IPW_FOL, //机组输入频率过低
    RF_IPW_FOH, //机组输入频率过高
    RF_IPW_PHAV_UNB_x10, //输入电压三相不平衡
    RF_IPW_PHAV_UNBDF_x10, //输入电压三相不平衡回差

    //清除和恢复
    RF_HISALM_CLN, //历史告警清除
    RF_ALL_CFG_CLN, //恢复出厂设置

    //告警使能
    RF_EFAN1_ALM_EN, //室内风机1故障告警使能
    RF_EFAN2_ALM_EN, //室内风机2故障告警使能
    RF_EFAN3_ALM_EN, //室内风机3故障告警使能
    RF_AF_LOST_ALM_EN, //风机气流丢失告警
    RF_V_OL_ALM_EN, //电压过低报警使能
    RF_V_OH_ALM_EN, //电压过高报警使能
    RF_F_ABN_ALM_EN, //电源频率异常使能
    RF_PHAS_LOS_ALM_EN, //电源缺相报警使能
    RF_PHAS_UNB_ALM_EN, //三相不平衡报警使能
    RF_PHAS_SERR_ALM_EN, //电源错相报警使能
    RF_P_OH_ALM_EN, //高压告警使能
    RF_P_OL_ALM_EN, //低压告警使能
    RF_TP_OH_ALM_EN, //排气高温告警使能
    RF_TI_OH_ALM_EN, //回风高温告警使能
    RF_TI_OL_ALM_EN, //回风低温告警使能
    RF_HI_OH_ALM_EN, //回风高湿告警使能
    RF_HI_OL_ALM_EN, //回风低湿告警使能
    RF_TO_OH_ALM_EN, //送风高温告警使能
    RF_TO_OL_ALM_EN, //送风低温告警使能
    RF_HEAT_ALM_EN, //加热故障告警使能
    RF_SMOK_ALM_EN, //烟雾告警检测使能
    RF_FSD_ALM_EN, //水浸告警检测使能
    RF_FLT_ALM_EN, //滤网堵塞检测使能
    RF_HUMI_PRT_ALM_EN, //加湿保护检测使能
    RF_SYSDW_ALM_EN, //系统排水故障使能


    //DIO功能设置
    RF_DO1_FUNC, //DO1功能设置
    RF_DO2_FUNC, //DO2功能设置
    RF_DO3_FUNC, //DO3功能设置
    RF_DO4_FUNC, //DO4功能设置
    RF_DO5_FUNC, //DO5功能设置
    RF_DO6_FUNC, //DO6功能设置
    RF_DO7_FUNC, //DO7功能设置
    RF_DO8_FUNC, //DO8功能设置
    RF_DO9_FUNC, //DO9功能设置
    RF_DO10_FUNC, //DO10功能设置
    RF_DO11_FUNC, //DO11功能设置
    RF_DO12_FUNC, //DO12功能设置
    RF_DO13_FUNC, //DO13功能设置
    RF_DO14_FUNC, //DO14功能设置
    RF_DO15_FUNC, //DO15功能设置
    RF_DO16_FUNC, //DO16功能设置
    RF_DO17_FUNC, //DO17功能设置
    RF_DO18_FUNC, //DO18功能设置
    RF_DI1_FUNC, //DI1功能设置
    RF_DI2_FUNC, //DI2功能设置
    RF_DI3_FUNC, //DI3功能设置
    RF_DI4_FUNC, //DI4功能设置
    RF_DI5_FUNC, //DI5功能设置
    RF_DI6_FUNC, //DI6功能设置
    RF_DI7_FUNC, //DI7功能设置
    RF_DI8_FUNC, //DI8功能设置
    RF_DI9_FUNC, //DI9功能设置
    RF_DI10_FUNC, //DI10功能设置
    RF_DI11_FUNC, //DI11功能设置
    RF_DI12_FUNC, //DI12功能设置
    RF_DI13_FUNC, //DI13功能设置
    RF_DI14_FUNC, //DI14功能设置
    RF_DI15_FUNC, //DI15功能设置
    RF_DI16_FUNC, //DI16功能设置
    RF_DI17_FUNC, //DI17功能设置
    RF_DI18_FUNC, //DI18功能设置
    RF_DI19_FUNC, //DI19功能设置
    RF_DI20_FUNC, //DI20功能设置
    RF_AI_VIS, //通用模拟输入电压电流设置

    //
    RF_RESV_440, //预留
    RF_RESV_441, //预留
    RF_RESV_442, //预留
    RF_RESV_443, //预留
    RF_RESV_444, //预留
    RF_RESV_445, //预留
    RF_RESV_446, //预留
    RF_RESV_447, //预留
    RF_RESV_448, //预留
    RF_RESV_449, //预留

/*-------------------------------------------------------------------*/
    //对应Modbus功能码0x04，此功能码对应只读模拟量
    //如无特殊说明默认数据类型为int16
    IR_T_x10, //当前控制温度，0.1℃
    IR_H_x10, //当前控制湿度，0.1%

    IR_TS_x10, //控制温度设定值，0.1℃
    IR_HS_x10, //控制湿度设定值，0.1%
    IR_CTL_WAYS, //实际控制方式；如果设置的控制方式探头故障，会自动切换控制方式
                 //0：回风控制；1：送风控制

    IR_IT_x10, //室内回风温度；综合  0.1℃
    IR_IH_x10, //室内回风湿度1; 0.1%

    IR_OT_x10, //室内送风温度；综合 0.1℃
    IR_OH_x10, //室内送风湿度     0.1%

    IR_IT1_x10, //室内回风温度1；0.1℃
    IR_IT2_x10, //室内回风温度2；0.1℃

    IR_OT1_x10, //室内送风温度1   0.1℃
    IR_OT2_x10, //室内送风温度2   0.1℃
    IR_OT3_x10, //室内送风温度3   0.1℃


    IR_NET_AT_x10, //室内平均温度; 群组模式时的平均值  0.1℃
    IR_NET_AH_x10, //室内平均湿度; 群组模式时的平均值  0.1%

    IR_SYS_RUN_STS, //系统运行状态：0关机；1正常开机；2故障关机；3群组待机  1
    IR_SYS_RUN_MOD, //系统运行模式：AcRunModEnum_t 1

    IR_EXSU_T_x10, //排气温度  0.1℃
    IR_SUCT_T_x10, //吸气温度  0.1℃
    IR_EVAP_T_x10, //蒸发温度,对应吸气压力  0.1℃

    IR_SUCT_P_x10, //吸气压力，低压    0.1bar
    IR_EXSU_P_x10, //排气压力,高压    0.1bar
    IR_CMPR_PAHP_PC_x10, //保护调节高压值  0.1bar

    IR_SYSA_COND_T_x10, //系统A冷凝温度  0.1℃
    IR_SYSB_COND_T_x10, //系统B冷凝温度  0.1℃
    IR_SYSA_SUCT_SHT_x10, //系统A吸气过热度  0.1℃
    IR_SYSB_SUCT_SHT_x10, //系统B吸气过热度  0.1℃


    IR_ETH_I_x10, //电极加湿电流, 0.1A
    IR_ETH_HWL_V, //电极加湿高水位电压，V

    IR_IPWV_A_x10, //电源电压,0.1V
    IR_IPWV_B_x10, //电源电压,0.1V
    IR_IPWV_C_x10, //电源电压,0.1V

    IR_IPWF_A_x10, //电源频率,0.1Hz
    IR_IPWF_B_x10, //电源频率,0.1Hz
    IR_IPWF_C_x10, //电源频率,0.1Hz

    IR_SYS_TYPE, //机组型号 1
    IR_EFAN_AFV_x10, //内风机风量    1
    IR_SYS_CCP, //机组制冷量     1

    IR_CMPR_F_x10, //压机实际频率     0.1Hz
    IR_CMPR_SET_F_x10, //压机设置频率     0.1Hz
    IR_INV_DC_VOLT_x10, //变频器母线电压   0.1V
    IR_INV_OUT_VOLT_x10, //变频器输出电压   0.1V
    IR_INV_OUT_CURR_x10, //变频器输出电流  0.1A
    IR_INV_TEMP_x10, //逆变模块温度  0.1℃
    IR_INV_VER, //控制板软件版本   1

    IR_EEVA_OPEN, //EEVA运行开度    1
    IR_EEVB_OPEN, //EEVB运行开度    1

    IR_CWVA_OPEN, //冷却水阀A开度    1
    IR_CWVB_OPEN, //冷却水阀B开度     1

    IR_SYS_WKTM, //机组累计运行时间     1H
    IR_EFAN_WKTM, //风机累计运行时间     1H
    IR_HUMI_WKTM, //加湿累计运行时间     1H
    IR_HRAT1_WKTM, //加热1累计时间值     1H
    IR_HRAT2_WKTM, //加热2累计时间值     1H
    IR_ODMA_WKTM, //外机A累计运行时间     1H
    IR_ODMB_WKTM, //外机B累计运行时间     1H
    IR_CMPRA_WKTM, //压机A累计时间值     1H
    IR_CMPRB_WKTM, //压机B累计时间值     1H
    IR_FLT_WKTM, //滤网累计运行时间       1H
    IR_TOTAL_KWH, //历史电量统计数值      1kWh
    IR_SYS_CLCN, //系统制冷计算需求     1
    IR_SYS_HTCN, //系统制热计算需求     1
    IR_SYS_IHCN, //系统加湿计算需求     1
    IR_SYS_DHCN, //系统除湿计算需求     1
    IR_EFAN_RS_x10, //内风机转速百分比  0.1%
    IR_SYS_LEWY_x10, //系统风压差检测值 0.1
    IR_CMPRA_CTL_F, //压机A控制频率值  1Hz
    IR_CMPRB_CTL_F, //压机B控制频率值  1Hz

    IR_RESV_69,
    IR_RESV_70,
    IR_RESV_71,
    IR_RESV_72,
    IR_RESV_73,
    IR_RESV_74,
    IR_RESV_75,
    IR_RESV_76,

    IR_RT01, //远程温度01；0.1℃
    IR_RH01, //远程湿度01；0.1%
    IR_RT02, //远程温度02；0.1℃
    IR_RH02, //远程湿度02；0.1%
    IR_RT03, //远程温度03；0.1℃
    IR_RH03, //远程湿度03；0.1%
    IR_RT04, //远程温度04；0.1℃
    IR_RH04, //远程湿度04；0.1%
    IR_RT05, //远程温度05；0.1℃
    IR_RH05, //远程湿度05；0.1%
    IR_RT06, //远程温度06；0.1℃
    IR_RH06, //远程湿度06；0.1%
    IR_RT07, //远程温度07；0.1℃
    IR_RH07, //远程湿度07；0.1%
    IR_RT08, //远程温度08；0.1℃
    IR_RH08, //远程湿度08；0.1%
    IR_RT09, //远程温度09；0.1℃
    IR_RH09, //远程湿度09；0.1%
    IR_RT10, //远程温度10；0.1℃
    IR_RH10, //远程湿度10；0.1%
    IR_RT11, //远程温度11；0.1℃
    IR_RH11, //远程湿度11；0.1%
    IR_RT12, //远程温度12；0.1℃
    IR_RH12, //远程湿度12；0.1%
    IR_RT13, //远程温度13；0.1℃
    IR_RH13, //远程湿度13；0.1%
    IR_RT14, //远程温度14；0.1℃
    IR_RH14, //远程湿度14；0.1%
    IR_RT15, //远程温度15；0.1℃
    IR_RH15, //远程湿度15；0.1%
    IR_RT16, //远程温度16；0.1℃
    IR_RH16, //远程湿度16；0.1%

    IR_SFVER, //软件版本

    eAcParaNum
};

//参数里面的状态量单独提出来
enum DevStsEnum{
    IR_CTL_WAYS_STS,
    IR_SYS_RUN_STS_STS,
    IR_SYS_RUN_MOD_STS,
    devStsNum

};

/*----------------------------Struct----------------------------------------*/
struct PollTaskStruct
{
    quint8 func;
    quint16 startAddr;
    quint16 regNum;
    uint16 *pBuf;
};

struct AcRunStsBitStruct
{
    uint16 changed:1; 	//Bit0
    uint16 settable:1;	//Bit1

    uint16 control:1;	//Bit2, if this value is 0 and bit0 is 1, will send 0000,
                        //if this value is 1 and bit0 is 1, will send FF00
    uint16 rsvd:13;	//Bit3-Bit15
};

union AcRunStsBitUnion
{
    uint16 all;
    AcRunStsBitStruct bit;
};

struct AcRunStsStruct
{
    uint16 data[eAcStsNum]; //改
    AcRunStsBitUnion flag[eAcStsNum];//改
};

struct AcRunStsInfStruct
{
    uint16 regAddr;
    bool settable; //If this variant is settable
    bool used; //If this variant is used
};

struct AcRunParaBitStruct
{
    uint16 changed:1; 	//Bit0
    uint16 settable:1;	//Bit1
    uint16 rsvd:14;	//Bit2-Bit15
};

union AcRunParaBitUnion
{
    uint16 all;
    AcRunParaBitStruct bit;
};

struct AcRunParaStruct
{
    int16 data[eAcParaNum];
    int16 setValue[eAcParaNum];
    AcRunParaBitUnion flag[eAcParaNum];
};

struct AcRunParaInfStruct
{
    uint16 regAddr;
    int16 coeff;
    int16 lmtDn;
    int16 lmtUp;
    int16 defaultValue;
    bool settable;
    bool used;
};

struct AcDataStruct
{
    uint16 connectFlag; //0:invalid; 1:disconnect; 2:connected
    AcRunStsStruct acRunSts;
    AcRunParaStruct acRunPara;
};
//----------------------------------------------------------------------
enum AcRunTypeEnum{
    AcRunStatus = 0,
    AcRunPara
};

//参数容器
struct onePara{
    int mainClass;
    int subClass;
    int enumType;
    int showType;   //只对设置有效 0 -文本框 1-按钮 2-下拉框 3-位设置

    int16   srcVal;     //源值
    QString enumName;   //转化后的值
    langEnum languageName;

    QString data;
    QString unit;

    QList<QPair<QString,langEnum>> typeStrList;

};

struct WritePara{
    int enumType;
    QString enumName;
    QString data;
    int showType;
};




struct AcDevVector{
    QVector<QVector<QVector<onePara>>>    acVec;     //三维数组 第一维主菜单(运行状态、用户设置、维修设置..) 第二维度（传感器、远程） 第三维 具体值
    QVector<QVector<QString>> towLevelName;
    QVector<QString>    firtLevelName;

};

typedef enum AcRunModEnum
{
    MOD_IDLE = 0, //空闲模式
    MOD_CL = 0x01, //制冷模式
    MOD_DH = 0x02, //除湿模式
    MOD_HT = 0x04, //加热模式
    MOD_AH = 0x08, //加湿模式
    MOD_CLAH = 0x09, //制冷加湿模式
    MOD_HTAH = 0x0C, //加热加湿模式
    MOD_DHHT = 0x06, //加热除湿模式
    MOD_AF = 0x10, //送风模式
}AcRunModEnum_t;

typedef enum AcSysStsEnum
{
    SYS_OFF = 0, //系统关机
    SYS_ON, //系统开机
    SYS_FLT_OFF, //故障关机
    SYS_STBY, //待机
    SYS_OFFING, //关机中
}AcSysStsEnum_t;

//-------old ac---------哦
typedef  struct AC_INFO{
     int           devID;
     int           connect;  //连接状态 0:invalid; 1:disconnect; 2:connected
     int           almSts;
     QString stsData[eAcStsNum];
     QString paraData[eAcParaNum];
     QString paraUnit[eAcParaNum];

}ac_Info;

typedef  struct AC_SET{
    int             devNo;
    int             paraType;  //2:写参数 3：写状态
    union{
        AcRunParaEnum    para;
        AcRunStatusEnum  sta_para;
    }union_ac;
    uint16          val;
}ac_set;

//----------------------------------------------------------------
/************************************local used*************************************************/
//struct AcLocalDateStruct
//{
//    uint32 disconnctTimer;
//    uint16 noRespTimes;
//    CntAffirmFlagUnion cntAffirmFlag;
//    uint16 UpsType;
//    bool AddedCompAlm;
//};


typedef AcRunStsInfStruct AcRunStsInfArray[eAcStsNum]; //改
typedef AcRunParaInfStruct AcRunParaInfArray[eAcParaNum]; //改

//extern AcDataStruct *pAcData;
//extern AcLocalDateStruct *pAcLocalData;

extern QMap<QString,AcRunStatusEnum> statusMap;
extern QMap<QString,AcRunParaEnum> paraMap;
void acMap_Init();

//void CreatAcShareMemery(void);
//void InitAcData(uint8 id);
//void AddAcPollTask(UartIdEnum uart,SerialTaskSchedule *serial);
//void CheckAcActiveTask(UartIdEnum uart,SerialTaskSchedule *serial);
//void AcConnectSts(SerialTaskSchedule::SerialTaskStruct &task, bool sts);
//void AcUnpackData(SerialTaskSchedule::SerialTaskStruct &task, QByteArray &rxFrame);


#endif // ACDATASTRUCT












