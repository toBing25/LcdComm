#include "flickeritem.h"
#include <QPixmap>
#include <QIcon>
#include <QTimer>
#include <QDebug>


flickerItem::flickerItem(QString name, int state, QWidget *parent) :
    QToolButton(parent)
{
    m_state = state;
    m_name  = name;
    m_alarmState = 0;

    m_list <<":/image/dev_close.png"
           <<":/image/dev_normal.png"
           <<":/image/dev_alarm.png";

    this->setText(m_name);
    this->setFixedSize(100, 35);
    this->setIconSize(QSize(35, 50));
    this->setFocusPolicy(Qt::NoFocus);
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    setAlarmColor("#00ADE5");

    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    connect(m_timer, &QTimer::timeout, this, [=](){
        if((m_state == ALARM_STATE_ALARM) && (m_alarmState == 0)){
            this->setIcon(QIcon(m_list.at(1)));
            m_alarmState = 1;
        }else if ((m_state == ALARM_STATE_ALARM) && (m_alarmState == 1)) {
            this->setIcon(QIcon(m_list.at(2)));
            m_alarmState = 0;
        }
        setAlarmColor("#D41136");
    });

    setIcons(m_state);
}

flickerItem::~flickerItem()
{

}

void flickerItem::setState(int state)
{
    if(m_state != state)
    {
        m_state  = state;
        setIcons(state);
    }
}

void flickerItem::setIcons(int state)
{
    //未连接
    if(state == ALARM_STATE_NOCON)
    {
        if(m_timer->isActive())
            m_timer->stop();

        this->setIcon(QIcon(m_list.at(0)));
        setAlarmColor("#696E74");
    }//正常
    else if (state == ALARM_STATE_NORNAL) {
        if((m_timer->isActive()) && (m_state != ALARM_STATE_ALARM))
            m_timer->stop();

        this->setIcon(QIcon(m_list.at(1)));
        setAlarmColor("#00ADE5");
    }//告警
    else if (state == ALARM_STATE_ALARM) {
        this->setIcon(QIcon(m_list.at(2)));
        setAlarmColor("#D41136");
        m_timer->start();
    }
}

void flickerItem::setAlarmColor(QString color)
{
    this->setStyleSheet(QString("background-color:transparent;;"
                                "border-bottom:0px solid #68CBF2;"
                                "border-radius:0px;"
                                "color: %1;"
                                "font:12pt;").arg(color));
}
