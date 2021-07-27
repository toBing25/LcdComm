#include "BMSDataStructExt.h"

struct ExtValueTypeStruct extBMSType;


void BMSTypeInit()
{
    //默认类型为状态
    for(int i=0;i<eBmsGroupNum;++i){
        extBMSType.BMSGroupType[i] = BMSTypeEnum::BMSSts;
    }

    //数值参数类型
    extBMSType.BMSGroupType[eBmsGroupBattNum      ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupBattAH       ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupVolt         ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupCurr         ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupAvgCellVolt  ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMaxCellVolt  ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMaxCellVoltID] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMinCellVolt  ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMinCellVoltID] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupAvgCellRes   ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMaxCellRes   ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMaxCellResID ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMinCellRes   ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMinCellResID ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupAvgCellTemp  ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMaxCellTemp  ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMaxCellTempID] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMinCellTemp  ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupMinCellTempID] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupBattSOC      ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupBattSOH      ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupBalanceID1   ] = BMSTypeEnum::BMSPara;
    extBMSType.BMSGroupType[eBmsGroupBalanceID2   ] = BMSTypeEnum::BMSPara;



    //默认类型为状态
    for(int i=0;i<eBmsCellNum;++i){
        extBMSType.BMSCellType[i] = BMSTypeEnum::BMSSts;
    }

    //数值参数类型
    extBMSType.BMSCellType[eBmsCellVolt] = BMSTypeEnum::BMSPara;
    extBMSType.BMSCellType[eBmsCellTemp] = BMSTypeEnum::BMSPara;
    extBMSType.BMSCellType[eBmsCellRes ] = BMSTypeEnum::BMSPara;


}
