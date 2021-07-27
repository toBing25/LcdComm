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

#ifndef JSONFILE_H
#define JSONFILE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include "DCData/CabCtrlDataStructExt.h"
class JsonFile :public QObject
{
    Q_OBJECT
public:
    JsonFile(QString filepath);

    void init();
    void setValue(QString cardNo, QString cardName, QStringList dcList);
    void removeValue(QString cardNo);
    void getCardList(QJsonArray &, QVector<CardDb> &);
    QVector<CardDb>* cardDb(){return &m_cardDb;}

private:
    void save();

private:
    QJsonArray m_arr;
    QJsonDocument m_doc;
    QString m_file;
    QVector<CardDb>  m_cardDb;
};

#endif // JSONFILE_H
