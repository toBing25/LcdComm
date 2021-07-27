#include "wgtdatetime.h"
#include <QDateTime>
#include <QTimer>

wgtDateTime::wgtDateTime(QWidget *parent)
    : QLabel(parent)
{ 
    this->setStyleSheet("color:rgb(4, 199, 211);;font-size:15px;");
    this->setMinimumSize(QSize(140, 35));
    this->setMaximumSize(QSize(140, 35));
    //this->setAlignment(Qt::AlignRight|Qt::AlignVCenter);


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &wgtDateTime::updateTime);

    timer->start(1000);
    updateTime();
}

void wgtDateTime::updateTime(void)
{
    this->setText(QString("%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
}
