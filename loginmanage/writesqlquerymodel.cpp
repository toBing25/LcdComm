#include "writesqlquerymodel.h"
#include <QDebug>
WritesqlQueryModel::WritesqlQueryModel(QSqlDatabase &db, QString dev, QObject *parent) :
    QSqlQueryModel(parent),
    m_dev(dev),
    m_database(db)
{

}

void WritesqlQueryModel::setDataBase(QSqlDatabase &db)
{
    m_database = db;
}



bool WritesqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    QModelIndex pkIndex = QSqlQueryModel::index(index.row(), 0);
    QString headData = this->headerData(index.column(),Qt::Horizontal).toString();
   //qDebug()<<headData;
    QString sParameter_name = data(pkIndex).toString();

    if(index.column() > 0)
    {
        //每个单元格数据修改都打开数据库和关闭数据库，
        m_database.open();
        //qDebug()<<"column = 3 "<<"id ="<<sParameter_name;
        QSqlQuery query(m_database);


        query.prepare(QString("UPDATE %1 SET %2 = :name WHERE parameter_name = :id").arg(m_dev,headData));
        query.bindValue(":name",value.toString());
        query.bindValue(":id",sParameter_name);
        bool isOk = query.exec();
        setQuery(QString("select * from %1").arg(m_dev));

        m_database.close();

    }
    else {
        //除第一列不能修改，其他列都能修改值
    }

}

Qt::ItemFlags WritesqlQueryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
        if (index.column() != 0)
        {
            flags |= Qt::ItemIsEditable;
        }
        return flags;
}
