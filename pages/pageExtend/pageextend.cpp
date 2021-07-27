#include "pageextend.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QCoreApplication>
#include "Formmain/formmain.h"
#include "wgtmenubar.h"
#include "wgtmenubar_ext.h"
#include "wgtparaitem_ups.h"
#include "wgtparaitem_ac.h"
#include "wgtparaitem_eleak.h"
#include "wgtparaitem_meter.h"
#include "wgtparaitem_ths.h"
#include "wgtparaitem_spds.h"
#include "wgtparaitem_sms.h"
#include "Global/SysCfgMng.h"
#include "wgtequipmentoverview.h"
#include "languagePublic.h"

PageExtend *PageExtend::_instance = 0;

PageExtend::PageExtend(QWidget *parent)
    : QWidget(parent)
{
    setupUi();

    qRegisterMetaType<QList<ths_Info>>("QList<ths_Info>");
    qRegisterMetaType<QList<ac_Info>>("QList<ac_Info>");
    qRegisterMetaType<QList<ups_Info>>("QList<ups_Info>");
    qRegisterMetaType<QList<meter_Info>>("QList<meter_Info>");
    qRegisterMetaType<QList<leak_Info>>("QList<leak_Info>");
    qRegisterMetaType<QList<spds_Info>>("QList<spds_Info>");
    qRegisterMetaType<QList<sms_Info>>("QList<sms_Info>");

    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevThsList,this, [=](QList<ths_Info> list)
    {
        getThsInfo(list);
    });
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevAcList,this, [=](QList<ac_Info> list)
    {
        getAcInfo(list);
    });
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevUpsList,this, [=](QList<ups_Info> list)
    {
        getUpsInfo(list);
    });
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevMeterList,this, [=](QList<meter_Info> list)
    {
        getMeterInfo(list);
    });
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevLeakList,this, [=](QList<leak_Info> list)
    {
        getLeakInfo(list);
    });
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevSpdsList,this, [=](QList<spds_Info> list)
    {
        getSpdsInfo(list);
    });
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevSmsList,this, [=](QList<sms_Info> list)
    {
        getSmsInfo(list);
    });

    createDevice();
}
PageExtend::~PageExtend()
{

}

void PageExtend::setupUi(void)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setGeometry(0, 0, this->width(), this->height());

    mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(6, 6, 6, 6);

    m_wgtLeftMenu = new WgtMenuBar_Ext(this);
    m_wgtLeftMenu->setObjectName(QStringLiteral("m_leftNav"));

    mainLayout->addWidget(m_wgtLeftMenu);
    this->setLayout(mainLayout);

    _instance    = this;
    m_powNubmer  = 1;
    m_enviNubmer = 1;
    m_secuNubmer = 1;

    m_thstree    = -1;
    m_actree     = -1;
    m_upstree    = -1;
    m_metertree  = -1;
    m_leaktree   = -1;
    m_spdstree   = -1;
    m_smsstree   = -1;
}

PageExtend *PageExtend::Instance()
{
    return _instance;
}

void PageExtend::createDevice()
{
    m_wgtLeftMenu->removeWidget(SYSTEM_TYPE_DEVICE);
    WgtEquipmentOverview *tempWdgSts = new WgtEquipmentOverview(this);
    m_wgtLeftMenu->addWidget(tempWdgSts, langN.show[langEnum::strDevOveview], ":/image/home.png", ":/image/home2.png", 1, SYSTEM_TYPE_DEVICE, 0, 0);

    m_wgtLeftMenu->removeWidget(SYSTEM_TYPE_POW);
    m_wgtLeftMenu->addTitle(langN.show[langEnum::strDistSys], ":/image/power.png", ":/image/power2.png", 1, SYSTEM_TYPE_POW, 0);

    m_wgtLeftMenu->removeWidget(SYSTEM_TYPE_ENVI);
    m_wgtLeftMenu->addTitle(langN.show[langEnum::strEnvSys], ":/image/env.png", ":/image/env2.png", 1, SYSTEM_TYPE_ENVI, 0);

    m_wgtLeftMenu->removeWidget(SYSTEM_TYPE_SECURITY);
    m_wgtLeftMenu->addTitle(langN.show[langEnum::strSecuitrySys], ":/image/security.png", ":/image/security2.png", 1, SYSTEM_TYPE_SECURITY, 0);
}

void PageExtend::clearDevice(void)
{
    QList<WgtMenuBar_Ext*> WgtMenuBar = this->findChildren<WgtMenuBar_Ext *>();
    if(!WgtMenuBar.isEmpty())
    {
        foreach(WgtMenuBar_Ext* wgt, WgtMenuBar)
        {
            delete wgt;
        }
    }

    QList<QHBoxLayout*> hlayout = this->findChildren<QHBoxLayout *>();
    if(!hlayout.isEmpty())
    {
        foreach(QHBoxLayout* layout, hlayout)
        {
            delete layout;
        }
    }

    m_thsList.clear();
    m_acList.clear();
    m_upsList.clear();
    m_meterList.clear();
    m_leakList.clear();
    m_spdsList.clear();
    m_smsList.clear();
}

void PageExtend::createThsDev(int &treeNubmer)
{
    //获取温湿度设备个数
    //if(m_thsList.count() > 0)
    {
        m_wgtLeftMenu->removeChildWidget(SYSTEM_TYPE_ENVI, treeNubmer);

        m_wgtLeftMenu->addTitle(langN.show[langEnum::strHumiture], ":/image/icon_ths.png", "", 2, SYSTEM_TYPE_ENVI, treeNubmer);
        if(m_thsList.isEmpty())
            return;

        //根据ID号去创建一个温湿度
        for (int i = 0; i < m_thsList.count(); i++){
            QString title = langN.show[langEnum::strHumiture] + QString::number(m_thsList.at(i).devID);
            m_wgtLeftMenu->addTitle(title, "", "", 3, SYSTEM_TYPE_ENVI, treeNubmer,  1+i);
        }
        //第一次创建后，把分配的编号进行赋值
        if(m_thstree == -1)
        {
            wgtParaItem_ths *tempWdgSts = new wgtParaItem_ths(0, this);
            m_wgtLeftMenu->bodyAddWidget(SYSTEM_TYPE_ENVI, m_enviNubmer, DEVTYPE_THS, tempWdgSts);

            m_thstree = m_enviNubmer;
            m_enviNubmer++;
            //if(m_enviNubmer > MAX_DEVICES)
            //    m_enviNubmer = 1;
        }
    }
}

void PageExtend::createAcDev(int &treeNubmer)
{
    //if(m_acList.count() > 0)
    {
        m_wgtLeftMenu->removeChildWidget(SYSTEM_TYPE_ENVI, treeNubmer);

        m_wgtLeftMenu->addTitle(langN.show[langEnum::strAc], ":/image/ac.png", "",  2, SYSTEM_TYPE_ENVI, treeNubmer);

        if(m_acList.isEmpty())
            return;

        for (int i = 0; i < m_acList.count(); i++)
        {
            QString title = langN.show[langEnum::strAc] + QString::number(m_acList.at(i).devID);
            m_wgtLeftMenu->addTitle(title, "",  "", 3, SYSTEM_TYPE_ENVI, treeNubmer, 1+i);
        }

        if(m_actree == -1)
        {

            wgtParaItem_ac *tempWdgSts = new wgtParaItem_ac(0, this);
            m_wgtLeftMenu->bodyAddWidget(SYSTEM_TYPE_ENVI, m_enviNubmer, DEVTYPE_AC, tempWdgSts);

            m_actree = m_enviNubmer;
            m_enviNubmer++;
            //if(m_enviNubmer > MAX_DEVICES)
            //    m_enviNubmer = 1;
        }
    }
}

void PageExtend::createMeterDev(int &treeNubmer)
{
    //if(m_meterList.count() > 0)
    {
        m_wgtLeftMenu->removeChildWidget(SYSTEM_TYPE_POW, treeNubmer);

        m_wgtLeftMenu->addTitle(langN.show[langEnum::strVMeter], ":/image/power.png",  "", 2, SYSTEM_TYPE_POW, treeNubmer);

        if(m_meterList.isEmpty())
            return;

        for (int i = 0; i < m_meterList.count(); i++)
        {
            QString title = langN.show[langEnum::strVMeter] + QString::number(m_meterList.at(i).devID);
            m_wgtLeftMenu->addTitle(title, "", "",  3, SYSTEM_TYPE_POW, treeNubmer, 1+i);
        }

        if(m_metertree == -1)
        {
            wgtParaItem_meter *tempWdgSts = new wgtParaItem_meter(0, this);
            m_wgtLeftMenu->bodyAddWidget(SYSTEM_TYPE_POW, m_powNubmer, DEVTYPE_METER, tempWdgSts);

            m_metertree = m_powNubmer;
            m_powNubmer++;
            //if(m_powNubmer > MAX_DEVICES)
            //    m_powNubmer = 1;
        }
    }
}

void PageExtend::createLakDev(int &treeNubmer)
{
    //if(m_leakList.count() > 0)
    {
        m_wgtLeftMenu->removeChildWidget(SYSTEM_TYPE_ENVI, treeNubmer);

        m_wgtLeftMenu->addTitle(langN.show[langEnum::strWaterLeak], ":/image/leak1.png",  "", 2, SYSTEM_TYPE_ENVI, treeNubmer);

        if(m_leakList.isEmpty())
            return;

        for (int i = 0; i < m_leakList.count(); i++)
        {
            QString title = langN.show[langEnum::strWaterLeak] + QString::number(m_leakList.at(i).devID);
            m_wgtLeftMenu->addTitle(title, "",  "", 3, SYSTEM_TYPE_ENVI, treeNubmer, 1+i);
        }

        if(m_leaktree == -1)
        {
            wgtParaItem_eLeak *tempWdgSts = new wgtParaItem_eLeak(0, this);
            m_wgtLeftMenu->bodyAddWidget(SYSTEM_TYPE_ENVI, treeNubmer, DEVTYPE_LEAK, tempWdgSts);

            m_leaktree = m_enviNubmer;
            m_enviNubmer++;
            //if(m_enviNubmer > MAX_DEVICES)
            //    m_enviNubmer = 1;
        }
    }
}

void PageExtend::createUpsDev(int &treeNubmer)
{
    //if(m_upsList.count() > 0)
    {
        m_wgtLeftMenu->removeChildWidget(SYSTEM_TYPE_POW, treeNubmer);

        m_wgtLeftMenu->addTitle(langN.show[langEnum::strUps], ":/image/ups.png",  "", 2, SYSTEM_TYPE_POW, treeNubmer);

        if(m_upsList.isEmpty())
            return;

        for(int i = 0; i < m_upsList.count(); i++)
        {
            QString title = langN.show[langEnum::strUps] + QString::number(m_upsList.at(i).devID);
            m_wgtLeftMenu->addTitle(title, "",  "", 3, SYSTEM_TYPE_POW, treeNubmer, 1+i);
        }

        if(m_upstree == -1)
        {
            wgtParaItem_ups *tempWdgSts = new wgtParaItem_ups(0, this);
            m_wgtLeftMenu->bodyAddWidget(SYSTEM_TYPE_POW, m_powNubmer, DEVTYPE_UPS, tempWdgSts);

            m_upstree = m_powNubmer;
            m_powNubmer++;
            //if(m_powNubmer > MAX_DEVICES)
            //    m_powNubmer = 1;
        }
    }
}

void PageExtend::createSpdsDev(int &treeNubmer)
{
    //if(m_spdsList.count() > 0)
    {
        m_wgtLeftMenu->removeChildWidget(SYSTEM_TYPE_POW, treeNubmer);

        m_wgtLeftMenu->addTitle(langN.show[langEnum::strSpds], ":/image/ups.png", "", 2, SYSTEM_TYPE_POW, treeNubmer);

        if(m_spdsList.isEmpty())
            return;

        for(int i = 0; i < m_spdsList.count(); i++)
        {
            QString title = langN.show[langEnum::strSpds] + QString::number(m_spdsList.at(i).devID);
            m_wgtLeftMenu->addTitle(title, "", "", 3, SYSTEM_TYPE_POW, treeNubmer, 1+i);
        }

        if(m_spdstree == -1)
        {
            wgtParaItem_spds *tempWdgSts = new wgtParaItem_spds(0, this);
            m_wgtLeftMenu->bodyAddWidget(SYSTEM_TYPE_POW, m_powNubmer, DEVTYPE_SPDS, tempWdgSts);

            m_spdstree = m_powNubmer;
            m_powNubmer++;
        }
    }
}

void PageExtend::createSmsDev(int &treeNubmer)
{
    //if(m_smsList.count() > 0)
    {
        m_wgtLeftMenu->removeChildWidget(SYSTEM_TYPE_SECURITY, treeNubmer);

        m_wgtLeftMenu->addTitle(langN.show[langEnum::strSmsDev], ":/image/sms.png", "", 2, SYSTEM_TYPE_SECURITY, treeNubmer);

        if(m_smsList.isEmpty())
            return;

        for(int i = 0; i < m_smsList.count(); i++)
        {
            QString title = langN.show[langEnum::strSmsDev] + QString::number(m_smsList.at(i).devID);
            m_wgtLeftMenu->addTitle(title, "", "", 3, SYSTEM_TYPE_SECURITY, treeNubmer, 1+i);
        }

        if(m_smsstree == -1)
        {
            wgtParaItem_sms *tempWdgSts = new wgtParaItem_sms(0, this);
            m_wgtLeftMenu->bodyAddWidget(SYSTEM_TYPE_SECURITY, m_secuNubmer, DEVTYPE_SMS, tempWdgSts);

            m_smsstree = m_secuNubmer;
            m_secuNubmer++;
        }
    }
}

void PageExtend::getThsInfo(QList<ths_Info> ths_list)
{
    if(m_thsList.count() != ths_list.count())
    {
        m_thsList = ths_list;
        qDebug()<<__LINE__ <<"ths"<<QDateTime::currentDateTime().toString("hh:mm:ss");

        //判断是不是第一次创建设备，第一次创建使用默认分配的编号
        if(m_thstree == -1)
            createThsDev(m_enviNubmer);
        else {
            createThsDev(m_thstree);
        }
        qDebug()<<__LINE__ <<"ths"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    }
}

void PageExtend::getAcInfo(QList<ac_Info> ac_list)
{
    if(m_acList.count() != ac_list.count())
    {
        qDebug()<<__LINE__ <<"ac"<<QDateTime::currentDateTime().toString("hh:mm:ss");
        m_acList = ac_list;

        if(m_actree == -1)
            createAcDev(m_enviNubmer);
        else {
            createAcDev(m_actree);
        }
        qDebug()<<__LINE__ <<"ac"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    }
}

void PageExtend::getUpsInfo(QList<ups_Info> ups_list)
{
    if(m_upsList.count() != ups_list.count())
    {
        qDebug()<<__LINE__ <<"ups"<<QDateTime::currentDateTime().toString("hh:mm:ss");
        m_upsList = ups_list;

        if(m_upstree == -1)
            createUpsDev(m_powNubmer);
        else {
            createUpsDev(m_upstree);
        }
        qDebug()<<__LINE__ <<"ups"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    }
}

void PageExtend::getMeterInfo(QList<meter_Info> meter_list)
{
    if(m_meterList.count() != meter_list.count())
    {
        qDebug()<<__LINE__ <<"meter"<<QDateTime::currentDateTime().toString("hh:mm:ss");
        m_meterList = meter_list;

        if(m_metertree == -1)
            createMeterDev(m_powNubmer);
        else {
            createMeterDev(m_metertree);
        }
        qDebug()<<__LINE__ <<"meter"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    }
}

void PageExtend::getLeakInfo(QList<leak_Info> leak_list)
{
    if(m_leakList.count() != leak_list.count())
    {
        qDebug()<<__LINE__ <<"leak"<<QDateTime::currentDateTime().toString("hh:mm:ss");
        m_leakList = leak_list;

        if(m_leaktree == -1)
            createLakDev(m_enviNubmer);
        else {
            createLakDev(m_leaktree);
        }
        qDebug()<<__LINE__ <<"leak"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    }
}

void PageExtend::getSpdsInfo(QList<spds_Info> spds_list)
{
    if(m_spdsList.count() != spds_list.count())
    {
        qDebug()<<__LINE__ <<"spds"<<QDateTime::currentDateTime().toString("hh:mm:ss");
        m_spdsList = spds_list;

        if(m_spdstree == -1)
            createSpdsDev(m_powNubmer);
        else {
            createSpdsDev(m_spdstree);
        }
        qDebug()<<__LINE__ <<"spds"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    }
}

void PageExtend::getSmsInfo(QList<sms_Info> sms_list)
{
    if(m_smsList.count() != sms_list.count())
    {
        qDebug()<<__LINE__ <<"sms"<<QDateTime::currentDateTime().toString("hh:mm:ss");
        m_smsList = sms_list;

        if(m_smsstree == -1)
            createSmsDev(m_secuNubmer);
        else {
            createSmsDev(m_smsstree);
        }
        qDebug()<<__LINE__ <<"sms"<<QDateTime::currentDateTime().toString("hh:mm:ss");
    }
}

bool PageExtend::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        return true;
    }

    return QWidget::event(ev);
}

void PageExtend::showEvent(QShowEvent */*event*/)
{

}

void PageExtend::changeEvent(QEvent* event)
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

void PageExtend::retranslateUi(void)
{
    clearDevice();
    setupUi();

    createDevice();
}
















