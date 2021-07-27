#include "pagesysoverview.h"
#include "ui_pagesysoverview.h"
#include <QDebug>
#include <QEvent>
#include <QDateTime>
#include "formmain.h"
#include "util/conversiontool.h"
#include "alarmwidget.h"
#include "buzzer.h"
#include "SysCfgMng.h"
#include "gettext.h"


PageSysOverview::PageSysOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSysOverview)
{
    ui->setupUi(this);
    retranslateUi();

    qRegisterMetaType<QList<alarm_list>>("QList<alarm_list>&");
    qRegisterMetaType<AlmLevel>("AlmLevel");
    qRegisterMetaType<QList<meter_Info>>("QList<meter_Info>");
    qRegisterMetaType<QList<int>>("QList<int>");

    ui->load_body->setStepVertical(100);
    ui->load_body->setMin(0);
    ui->load_body->setMax(300);

    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeMtrPueValue, this, [=](double val)
    {
        if(val > 3 || val < 1){
            //ui->pue_body->valueChange(generateDiffNumber(1, 3, 1).at(0));
        }else {
            ui->pue_body->valueChange(val);
        }
    });

    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeMtrPerValue, this, [=](double val)
    {
        PointData data;
        data.pointValue = val;
        data.timeString = QDateTime::currentDateTime().toString("hh:mm");
        ui->load_body->addData(data);
    });
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeMtrColdOrHotHumAvg,this,&PageSysOverview::setColdOrHotHumAvg);
    //主页显示开门关门图片
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendDoorStsChange,ui->hostPage_widget,&HostPage::doorStsChange);

    //m_timerId2000ms = startTimer(500);
}

PageSysOverview::~PageSysOverview()
{
    delete ui;
}

void PageSysOverview::setColdOrHotHumAvg(bool isCold, double tmp, double hum)
{
    QString currTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    if(isCold){
        ui->lblColdHumVal->setText(QString::number(hum)+"%");
        ui->lblColdTmpVal->setText(QString::number(tmp)+"°C");
    }else{
        ui->lblHotHumVal->setText(QString::number(hum)+"%");
        ui->lblHotTmpVal->setText(QString::number(tmp)+"°C");
    }
}

void PageSysOverview::showEvent(QShowEvent */*event*/)
{
}

void PageSysOverview::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId2000ms)
    {
        static int mode = 0;
        if(mode == 0){
            ui->widget->setStyleSheet("border-radius:0px;"
                                      "border-color: rgb(52, 101, 164);"
                                      "border-image: url(:/image/cir1.png);");

            ui->widget_2->setStyleSheet("border-radius:0px;"
                                        "border-color: rgb(52, 101, 164);"
                                        "border-image: url(:/image/cir1.png);");
            mode = 1;
        }else if (mode == 1) {
            ui->widget->setStyleSheet("border-radius:0px;"
                                      "border-color: rgb(52, 101, 164);"
                                      "border-image: url(:/image/cir.png);");

            ui->widget_2->setStyleSheet("border-radius:0px;"
                                        "border-color: rgb(52, 101, 164);"
                                        "border-image: url(:/image/cir.png);");
            mode = 0;
        }
    }
}

void PageSysOverview::retranslateUi(void)
{
    ui->lblColdHum->setText(langN.show[langEnum::strHumi]);
    ui->lblHotHum->setText(langN.show[langEnum::strHumi]);

    ui->lblColdTmp->setText(langN.show[langEnum::strTemp]);
    ui->lblHotTmp->setText(langN.show[langEnum::strTemp]);

    ui->lblPue->setText(langN.show[langEnum::strPueDate]);
    ui->lblLoad->setText(langN.show[langEnum::strUpsParaLdPerPh]);

    ui->lblHotPass->setText(langN.show[langEnum::strHotChan]);
    ui->lblColdPass->setText(langN.show[langEnum::strColdChan]);
}

void PageSysOverview::changeEvent(QEvent* event)
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

std::vector<int> PageSysOverview::generateDiffNumber(int min, int max, int num)
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






