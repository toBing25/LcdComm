#include "jsonfile.h"
#include <QFile>
#include <QDebug>

JsonFile::JsonFile(QString filepath):m_file(filepath)
{
    init();
}

void JsonFile::init()
{
    QFile file(m_file);
    if (!file.exists()){
        qDebug()<<"file does not exist";
        qDebug()<<m_file;
        return;
    }
    bool ok = file.open(QIODevice::Text|QIODevice::ReadOnly);
    qDebug()<<"file read "<<ok;
    QByteArray arr =  file.readAll();
    file.close();
    QJsonParseError parse;
    m_doc = QJsonDocument::fromJson(arr,&parse);
    if(parse.error != QJsonParseError::NoError){
        qDebug()<<"json parse err";
        return;
    }else{
        m_arr =  m_doc.array();
        getCardList(m_arr,m_cardDb);
        qDebug()<<"json parse ok";
    }
}

void JsonFile::save()
{
    QFile file(m_file);
    if (!file.exists()){
        qDebug()<<"file does not exist";
        return;
    }

    bool ok = file.open(QIODevice::Text|QIODevice::WriteOnly);
    if (ok){
        m_doc.setArray(m_arr);
        file.write(m_doc.toJson());
        file.close();
        qDebug()<<"save file";
    }


}

void JsonFile::setValue(QString cardNo, QString cardName, QStringList dcList)
{
    qDebug()<<dcList;
    qDebug()<<m_arr;
    QJsonArray boxArr = QJsonArray::fromStringList(dcList);
    QJsonObject cardObj;
    bool findOK = false;
    for(int i=0;i<m_arr.size();++i){
        cardObj = m_arr.at(i).toObject();
        if(cardObj["cardId"].toString() == cardNo){
            qDebug()<<"find ok";
            QJsonArray::iterator ArrayIterator = m_arr.begin();
            QJsonValueRef ElementOneValueRef = ArrayIterator[i];
            QJsonObject ElementOneObject = ElementOneValueRef.toObject();
            //修改原有的Object
            ElementOneObject["boxArr"] = boxArr;
            ElementOneObject["cardName"] = cardName;
            ElementOneValueRef = ElementOneObject;
            findOK = true;
            break;

        }
     }

    if (!findOK) {
        QJsonObject inObj;
        inObj.insert("cardId",cardNo);
        inObj.insert("cardName",cardName);
        inObj.insert("boxArr",QJsonValue(boxArr));
        m_arr.append(QJsonValue(inObj));
    }
        qDebug()<<m_arr;

        save();
}

void JsonFile::removeValue(QString cardNo)
{
    QJsonObject cardObj;
    bool findOK = false;
    for(int i=0;i<m_arr.size();++i){
        cardObj = m_arr.at(i).toObject();
        if(cardObj["cardId"].toString() == cardNo){
            qDebug()<<"remove ok";
            m_arr.removeAt(i);
            findOK = true;
            break;
        }
     }

        qDebug()<<m_arr;
        save();
}

void JsonFile::getCardList(QJsonArray &arr, QVector<CardDb> &cardDbVec)
{
    cardDbVec.clear();
    for(int i=0;i<arr.size();i++){
        CardDb cardDb;
        QJsonValue listArray = arr.at(i);
        QJsonObject card = listArray.toObject();
        cardDb.cardNo = card["cardId"].toString();
        cardDb.cardName = card["cardName"].toString();

        QJsonArray boxArr = card["boxArr"].toArray();
        for(int j=0;j<boxArr.size();j++){
            QJsonValue box = boxArr.at(j);
            cardDb.dcList<<box.toString();
        }
        cardDbVec.append(cardDb);
    }
}
