#include "PageSysOverview_.h"
#include "ui_PageSysOverview_.h"
#include <QDebug>
#include <QEvent>
#include <QDateTime>
#include "formmain.h"
#include "util/conversiontool.h"
#include "alarmwidget.h"
#include "buzzer.h"
#include "SysCfgMng.h"
#include "gettext.h"


PageSysOverview_::PageSysOverview_(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSysOverview_)
{
    setupUi();

    qRegisterMetaType<QList<alarm_list>>("QList<alarm_list>&");
    qRegisterMetaType<AlmLevel>("AlmLevel");
    qRegisterMetaType<QList<meter_Info>>("QList<meter_Info>");
    qRegisterMetaType<QList<int>>("QList<int>");


    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeMtrPueValue, ui->lblPueVal, [=](double val)
    {
        ui->lblPueVal->setText(QString::number(val));
    });

    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeMtrPerValue, ui->lblPueVal, [=](double val)
    {
        ui->lblLoadRateVal->setText(QString::number(val)+"%");
    });

    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeMtrColdOrHotHumAvg,this,&PageSysOverview_::setColdOrHotHumAvg);
    //主页显示开门关门图片
    //connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendDoorStsChange,ui->hostPage_widget,&HostPage::doorStsChange);


    //m_timerId2000ms = startTimer(2000);
}

PageSysOverview_::~PageSysOverview_()
{
    delete ui;
}

void PageSysOverview_::setupUi(void)
{
    ui->setupUi(this);
    retranslateUi();
}

void PageSysOverview_::getAlarmList(QList<alarm_list>& list)
{
    if(m_list.count() != list.count())
    {
        m_muter = status_noisy;
    }
}

void PageSysOverview_::setColdOrHotHumAvg(bool isCold, double tmp, double hum)
{
    //QString currTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    if(isCold){
        ui->lblColdTmpVal->setText(QString::number(tmp));
        ui->lblColdHumVal->setText(QString::number(hum));
    }else{
        ui->lblHotTmpVal->setText(QString::number(tmp));
        ui->lblHotHumVal->setText(QString::number(hum));
    }
    setProSyle(isCold, tmp, hum);
}

QString PageSysOverview_::getProVal(bool isTmp, double val)
{
    int  ret = 0;
    if(val <= 1)
        ret = 0;
    else if (val <= 30)
        ret = 30;
    else if (val <= 50)
        ret = 50;
    else if (val <= 65)
        ret = 65;
    else if (val <= 80)
        ret = 80;
    else if (val <= 100){
        ret = 100;
    }

    return  QString(":/image/w_pro%1%2.png").arg(isTmp? "L":"R").arg(ret);
}

void PageSysOverview_::setProSyle(bool isCold, double tmp, double hum)
{
    if(isCold){
        QString str;
        str = getProVal(true, tmp);
        ui->wgtColdTmp->setStyleSheet(QString(".QWidget#wgtColdTmp"
                                              "{"
                                              "border-radius:0px;"
                                              "border-color: rgb(52, 101, 164);"
                                              "border-image: url(%1);"
                                              "}").arg(str));

        str = getProVal(false, hum);
        ui->wgtColdHum->setStyleSheet(QString(".QWidget#wgtColdHum"
                                              "{"
                                              "border-radius:0px;"
                                              "border-color: rgb(52, 101, 164);"
                                              "border-image: url(%1);"
                                              "}").arg(str));
    }else {
        QString str;
        str = getProVal(true, tmp);
        ui->wgtHotTmp->setStyleSheet(QString(".QWidget#wgtHotTmp"
                                              "{"
                                              "border-radius:0px;"
                                              "border-color: rgb(52, 101, 164);"
                                              "border-image: url(%1);"
                                              "}").arg(str));

        str = getProVal(false, hum);
        ui->wgtHotHum->setStyleSheet(QString(".QWidget#wgtHotHum"
                                              "{"
                                              "border-radius:0px;"
                                              "border-color: rgb(52, 101, 164);"
                                              "border-image: url(%1);"
                                              "}").arg(str));
    }
}

void PageSysOverview_::showEvent(QShowEvent */*event*/)
{
}

void PageSysOverview_::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId2000ms)
    {

    }
}

void PageSysOverview_::retranslateUi(void)
{
    ui->lbPueName->setText(langN.show[langEnum::strPueDate]);
    ui->lblLoadName->setText(langN.show[langEnum::strUpsParaLdPerPh]);

    ui->lblHot->setText(langN.show[langEnum::strHotChan]);
    ui->lblCold->setText(langN.show[langEnum::strColdChan]);
}

void PageSysOverview_::changeEvent(QEvent* event)
{
    if(0 != event)
    {
        switch(event->type())
        {
        case QEvent::LanguageChange:
            retranslateUi();
            break;
        default:
            break;
        }
    }
}

std::vector<int> PageSysOverview_::generateDiffNumber(int min, int max, int num)
{
    int rnd;
    std::vector<int> diff;
    std::vector<int> tmp;//存储剩余的数
    //初始化
    for(int i = min;i < max+1 ; i++ )
    {
        tmp.push_back(i);
    }
    srand((unsigned)time(0)); //初始化随机数种子
    for(int i = 0 ; i < num ; i++)
    {
        do
        {
            rnd = min+rand()%(max-min+1);
        }
        while(tmp.at(rnd - min) == -1);
        diff.push_back(rnd);
        tmp.at(rnd-min) = -1;
    }
    return diff;
}






