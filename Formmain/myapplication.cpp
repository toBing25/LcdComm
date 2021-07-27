#include "myapplication.h"
#include "Gpio.h"

MyApplication::MyApplication(int &argc, char **argv)
    :QApplication(argc, argv)
{
    m_screenSaver_status = status_close;

    //读取屏保配置文件
    QString enable, timer_interval;
    readConfig(enable, timer_interval);
    if(enable == "0")
        m_screenSaver_enable = false;
    else if(enable == "1")
        m_screenSaver_enable = true;

    //int interval = timer_interval.toInt();
    int interval = timer_interval.toInt();
    if(interval == 0)
        interval = 1;

    m_timer  = new QTimer();
    m_timer->setInterval(interval*60*1000);
    connect(m_timer, &QTimer::timeout, this, [=](){
        if(m_screenSaver_status == status_close){
            m_screenSaver_status = status_open;
            //开启屏保
            gpio_write(SCREENSAVER_PIN, 0);
        }
    });

    if(m_screenSaver_enable)
        m_timer->start();
}

bool MyApplication::notify(QObject *obj, QEvent *e)
{
    if((e->type() == QEvent::MouseMove)||(e->type() == QEvent::MouseButtonPress)||
            (e->type() == QEvent::MouseButtonRelease) ||(e->type() == QEvent::MouseButtonDblClick))
    {
        if(m_screenSaver_enable)
        {
            //当有鼠标事件时，退出屏幕保护
            if(m_timer->isActive())
            {
                m_timer->stop();

                //退出屏保
                if(m_screenSaver_status == status_open)
                {
                    m_screenSaver_status = status_close;
                    gpio_write(SCREENSAVER_PIN, 1);
                }
            }
        }
    }
    else if(e->type() == PARA_UPDATE)
    {
        /*
         *  当前屏保参数更新时，重新读取配置参数，设置定时间隔
         *  屏保功能使能时，读取配置文件，重新设置定时时间
         *  当屏保功能不使能时，关掉定时器
         */
        QString enable, timer_interval;
        readConfig(enable, timer_interval);

        if(enable.isEmpty())
            enable = "0";

        if(timer_interval.isEmpty())
            timer_interval = "3";

        if(enable == "0")
            m_screenSaver_enable = false;
        else if(enable == "1")
            m_screenSaver_enable = true;
        else {
            m_screenSaver_enable = false;
        }

        //qDebug()<<__LINE__<<timer_interval.toInt() <<timer_interval.toFloat();
        if(m_screenSaver_enable){
            int interval = timer_interval.toInt();
            //int interval = timer_interval.toFloat();
            if(interval == 0)
                m_timer->setInterval(1000);
            else {
                m_timer->setInterval(interval*60*1000);
            }
        }
        else {
            m_timer->stop();
        }
    }
    else
    {
        if(m_screenSaver_enable)
        {
            if(!m_timer->isActive())
            {
                m_timer->start();
            }
        }
    }

    return QApplication::notify(obj,e);
}

void MyApplication::readConfig(QString &enable, QString &interval)
{
    QString fileName =  QString("/opt/%2").arg("qt_config.ini");

    QFile file(fileName);
    if(file.exists())
    {
        QSettings *config = new QSettings(fileName, QSettings::IniFormat);

        enable   = config->value("user_Info/screenSaver_enable").toString();
        interval = config->value("user_Info/screenSaver_interval").toString();
        delete   config;
    }
    else {
        enable   = "0";
        interval = "3";
    }
}
