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

#ifndef CONVERSIONTOOL_H
#define CONVERSIONTOOL_H
#include <QDateTime>
#include "Global/AlarmStruct.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

//------------------------------data type define----------------------------------------------------------------------
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long int uint32;
typedef  short int int16;
typedef  long int int32;

class ConversionTool
{

public:
    ConversionTool();
    static QString getTimeFrom16Bit(uint16 lowBit,uint16 hightBit);
    static QString connectFlagToQString(uint16 flag);
    static uint8   get8BitL(uint16 val);
    static uint8   get8BitH(uint16 val);

    static QString mergeFrom16Bit(uint16 lowBit,uint16 hightBit,double ratio);
    static QString mergeFrom16BitCard(uint16 lowBit,uint16 hightBit);
    static QString unicodeToQString(uint16 str[],int len);

    static QString openJsonFile(QString filePath);
    static bool    getBit(int num,int position);
    static int     setBit(int num,int position,int value);
    static int     uint16Toint(uint16 num);     //uint转为带正负的整数  最高为1为负

    //json操作
    static bool initJsonDataFile(QString path, QJsonDocument &jsonData);
    static bool jsonDocToArray(QJsonDocument &jsonData,QJsonArray &jsonArr);
    //随机数
    static int generateRandomNumber();



};

#endif // CONVERSIONTOOL_H
