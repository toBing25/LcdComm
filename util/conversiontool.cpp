#include "conversiontool.h"
#include <QFile>
ConversionTool::ConversionTool()
{

}

QString ConversionTool::getTimeFrom16Bit(uint16 lowBit, uint16 hightBit)
{
    uint32 time32 = (hightBit<<16)+lowBit;
    return  QDateTime::fromTime_t(time32).toString("yyyy-MM-dd hh:mm:ss");

}

QString ConversionTool::mergeFrom16Bit(uint16 lowBit, uint16 hightBit, double ratio)
{    
    uint32 merge = (hightBit<<16)+lowBit;
    //qDebug()<<"merge"<<merge<<"ratio"<<ratio;
    double ret = merge * ratio;
    return QString::number(ret);
}

QString ConversionTool::mergeFrom16BitCard(uint16 lowBit, uint16 hightBit)
{
    uint32 merge = (hightBit<<16)+lowBit;

    return QString::number(merge);
}

QString ConversionTool::connectFlagToQString(uint16 flag)
{
    QString connectFlag = "";
    switch (flag) {
    case 0:
        connectFlag = ("无效");
        break;
    case 1:
        connectFlag = ("未连接");
        break;
    case 2:
        connectFlag = ("已连接");
        break;
    default:
        connectFlag = ("无效");
        break;
    }
    return  connectFlag;
}

uint8 ConversionTool::get8BitL(uint16 val)
{
    uint8 ret = val&0x00FF;
    return  ret;
}

uint8 ConversionTool::get8BitH(uint16 val)
{
    uint8 ret = (val&0xFF00)>>8;
    return ret;
}

QString ConversionTool::unicodeToQString(uint16 str[],int len)
{
    QString s;
    for(int i=0;i<len;i++){
        if(str[i] == 0x0000){break;}
        qint8 ctL = str[i];
        qint8 ctH = (str[i]&0xff00)>>8;
        s+=ctH;
        s+=ctL;
        //去除自动添加的\u0000
        QChar h0 = 0x00;
        if(s.contains(h0))
        {
            s.replace(h0,"");
        }

    }
    return  s;
}

QString ConversionTool::openJsonFile(QString filePath)
{
    QFile file(filePath);
    bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(ok){
       QString value = file.readAll();
       return  value;
    }
    return "";
}
//position从0开始
bool ConversionTool::getBit(int num, int position)
{
    bool bit = num & (1 << position);
    return bit;
}
//position从0开始
int ConversionTool::setBit(int num, int position, int value)
{
    int ret = 0;
    if(value == 1){
        int mask = (1 << position);
        ret = num | mask;
    }else if(value == 0){
        int mask = ~(1 << position);
        ret = num & mask;
    }
    return ret;
}

int ConversionTool::uint16Toint(uint16 num)
{
    return 0;

}

bool ConversionTool::initJsonDataFile(QString path, QJsonDocument &jsonData)
{
    QString value = ConversionTool::openJsonFile(path);
    QJsonParseError parseJsonErr;
    jsonData = QJsonDocument::fromJson(value.toUtf8(),&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError)){
        return false;
    }
    return true;
}

bool ConversionTool::jsonDocToArray(QJsonDocument &jsonData, QJsonArray &jsonArr)
{
    jsonArr = jsonData.array();
    if(jsonArr.isEmpty())
        return false;
    else
        return true;
}

int ConversionTool::generateRandomNumber()
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    return  qrand() % (999999 - 100000) + 100000;
}
