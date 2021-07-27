#include "wgtequipmentoverview.h"
#include "ui_wgtequipmentoverview.h"
#include "wgtmenubar_ext.h"
#include "formmain.h"
#include "pageextend.h"
#include "flickeritem.h"
#include "languagePublic.h"
#include "actparawidget.h"
#include <QSignalMapper>

#define COL_NUM  7

WgtEquipmentOverview::WgtEquipmentOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgtEquipmentOverview)
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
}

WgtEquipmentOverview::~WgtEquipmentOverview()
{
    delete ui;
}

void WgtEquipmentOverview::setupUi(void)
{
    ui->setupUi(this);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //初始化时，默认没有设备设备控件先隐藏
    ui->wgt_ths->setVisible(false);
    ui->wgt_ac->setVisible(false);
    ui->wgt_meter->setVisible(false);
    ui->wgt_ups->setVisible(false);
    ui->wgt_leak->setVisible(false);
    ui->wgt_ups->setVisible(false);
    ui->wgt_spds->setVisible(false);
    ui->wgt_sms->setVisible(false);
}

void WgtEquipmentOverview::cleanThsDev(void)
{
    QList<QGridLayout*> glayout = ui->ths_item->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<flickerItem*> btn_list = ui->ths_item->findChildren<flickerItem *>();
    if(!btn_list.isEmpty())
    {
        foreach(flickerItem* btn, btn_list)
        {
            delete btn;
        }
    }

    if(!m_thsItemList.isEmpty())
        m_thsItemList.clear();
}

void WgtEquipmentOverview::cleanAcDev(void)
{
    QList<QGridLayout*> glayout = ui->ac_item->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<flickerItem*> btn_list = ui->ac_item->findChildren<flickerItem *>();
    if(!btn_list.isEmpty())
    {
        foreach(flickerItem* btn, btn_list)
        {
            delete btn;
        }
    }

    if(!m_acItemList.isEmpty())
        m_acItemList.clear();
}

void WgtEquipmentOverview::cleanUpsDev(void)
{
    QList<QGridLayout*> glayout = ui->ups_item->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<flickerItem*> btn_list = ui->ups_item->findChildren<flickerItem *>();
    if(!btn_list.isEmpty())
    {
        foreach(flickerItem* btn, btn_list)
        {
            delete btn;
        }
    }

    if(!m_upsItemList.isEmpty())
        m_upsItemList.clear();
}

void WgtEquipmentOverview::cleanMeterDev(void)
{
    QList<QGridLayout*> glayout = ui->meter_item->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<flickerItem*> btn_list = ui->meter_item->findChildren<flickerItem *>();
    if(!btn_list.isEmpty())
    {
        foreach(flickerItem* btn, btn_list)
        {
            delete btn;
        }
    }

    if(!m_meterItemList.isEmpty())
        m_meterItemList.clear();
}

void WgtEquipmentOverview::cleanLeakDev(void)
{
    QList<QGridLayout*> glayout = ui->leak_item->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<flickerItem*> btn_list = ui->leak_item->findChildren<flickerItem *>();
    if(!btn_list.isEmpty())
    {
        foreach(flickerItem* btn, btn_list)
        {
            delete btn;
        }
    }

    if(!m_leakItemList.isEmpty())
        m_leakItemList.clear();
}

void WgtEquipmentOverview::cleanSpdsDev(void)
{
    QList<QGridLayout*> glayout = ui->spds_item->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<flickerItem*> btn_list = ui->spds_item->findChildren<flickerItem *>();
    if(!btn_list.isEmpty())
    {
        foreach(flickerItem* btn, btn_list)
        {
            delete btn;
        }
    }

    if(!m_spdsItemList.isEmpty())
        m_spdsItemList.clear();
}

void WgtEquipmentOverview::cleanSmsDev(void)
{
    QList<QGridLayout*> glayout = ui->sms_item->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<flickerItem*> btn_list = ui->sms_item->findChildren<flickerItem *>();
    if(!btn_list.isEmpty())
    {
        foreach(flickerItem* btn, btn_list)
        {
            delete btn;
        }
    }

    if(!m_smsItemList.isEmpty())
        m_smsItemList.clear();
}

void WgtEquipmentOverview::getThsInfo(QList<ths_Info> ths_list)
{
    if(m_thsList.count() != ths_list.count())
    {
        m_thsList = ths_list;

        cleanThsDev();

        if(ths_list.isEmpty())
        {
            ui->wgt_ths->setVisible(false);
            return;
        }
        else {
            ui->wgt_ths->setVisible(true);
        }

        int rows, cols;
        cols   = COL_NUM;
        rows   = ths_list.count() / cols;
        if(ths_list.count() % cols)
            ++rows;

//        if(ths_list.count() < 8) {
//            ui->ths_item->setStyleSheet("QWidget#ths_item{"
//                                       "border-image: url(:/image/border_down3.png);"
//                                       "}");
//        }else if (ths_list.count() < 21) {
//            ui->ths_item->setStyleSheet("QWidget#ths_item{"
//                                       "border-image: url(:/image/border_down2.png);"
//                                       "}");
//        }else{
//            ui->ths_item->setStyleSheet("QWidget#ths_item{"
//                                       "border-image: url(:/image/border_down.png);"
//                                       "}");
//        }

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(10, 5, 10, 10);

        QSignalMapper *signalMapper = new QSignalMapper(this);
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                int k = i * cols + j + 1;
                if(k > ths_list.count())
                    break;

                QString text = langN.show[langEnum::strHumiture] + QString("%1").arg(ths_list.at(k - 1).devID);
                flickerItem *item = new flickerItem(text, getAlarmStatus_ths(ths_list.at(k - 1).devID), ui->ths_item);
                connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
                signalMapper->setMapping(item, QString("%1").arg(k));
                gridlayout->addWidget(item, i, j);
                m_thsItemList.append(item);
            }
        }

        if(ths_list.count() < cols)
        {
            gridlayout->setColumnStretch(ths_list.count(), 1);
        }
        connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onthsClicked(QString))); //将转发的信号连接到最终的槽函数

        ui->ths_item->setLayout(gridlayout);
    }

    if(m_thsList.isEmpty() || m_thsItemList.isEmpty())
        return;

    for (int i = 0; i < m_thsList.count(); i++) {
        m_thsItemList.at(i)->setState(getAlarmStatus_ths(m_thsList.at(i).devID));
    }
}

void WgtEquipmentOverview::getAcInfo(QList<ac_Info>   ac_list)
{
    if(m_acList.count() != ac_list.count())
    {
        m_acList = ac_list;

        cleanAcDev();

        if(ac_list.isEmpty())
        {
            ui->wgt_ac->setVisible(false);
            return;
        }
        else {
            ui->wgt_ac->setVisible(true);
        }

        int rows, cols;
        cols   = COL_NUM;
        rows   = ac_list.count() / cols;
        if( ac_list.count() % cols)
            ++rows;

//        if(ac_list.count() < 8) {
//            ui->ac_item->setStyleSheet("QWidget#ac_item{"
//                                       "border-image: url(:/image/border_down3.png);"
//                                       "}");
//        }else if (ac_list.count() < 21) {
//            ui->ac_item->setStyleSheet("QWidget#ac_item{"
//                                       "border-image: url(:/image/border_down2.png);"
//                                       "}");
//        }else{
//            ui->ac_item->setStyleSheet("QWidget#ac_item{"
//                                       "border-image: url(:/image/border_down.png);"
//                                       "}");
//        }

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(10, 5, 10, 10);

        QSignalMapper *signalMapper = new QSignalMapper(this);
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                int k = i * cols + j + 1;
                if(k > ac_list.count())
                    break;

                QString text = langN.show[langEnum::strAc] + QString("%1").arg(ac_list.at(k - 1).devID);
                flickerItem *item = new flickerItem(text, getAlarmStatus_ac(ac_list.at(k - 1).devID), ui->ac_item);
                connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
                signalMapper->setMapping(item, QString("%1").arg(k));
                gridlayout->addWidget(item, i, j);
                m_acItemList.append(item);
            }
        }

        if(ac_list.count() < cols)
        {
            gridlayout->setColumnStretch(ac_list.count(), 1);
        }

        connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onAcClicked(QString))); //将转发的信号连接到最终的槽函数

        ui->ac_item->setLayout(gridlayout);
    }

    if(m_acList.isEmpty() || m_acItemList.isEmpty())
        return;

    for (int i = 0; i < m_acList.count(); i++) {
        m_acItemList.at(i)->setState(getAlarmStatus_ac(m_acList.at(i).devID));
    }
}

void WgtEquipmentOverview::getUpsInfo(QList<ups_Info> ups_list)
{
    if(m_upsList.count() != ups_list.count())
    {
        m_upsList = ups_list;

        cleanUpsDev();

        if(ups_list.isEmpty())
        {
            ui->wgt_ups->setVisible(false);
            return;
        }
        else {
            ui->wgt_ups->setVisible(true);
        }

        int rows, cols;
        cols   = COL_NUM;
        rows   = ups_list.count() / cols;
        if(ups_list.count() % cols)
            ++rows;

//        if(ups_list.count() < 8) {
//            ui->ups_item->setStyleSheet("QWidget#ups_item{"
//                                       "border-image: url(:/image/border_down3.png);"
//                                       "}");
//        }else if (ups_list.count() < 21) {
//            ui->ups_item->setStyleSheet("QWidget#ups_item{"
//                                       "border-image: url(:/image/border_down2.png);"
//                                       "}");
//        }else{
//            ui->ups_item->setStyleSheet("QWidget#ups_item{"
//                                       "border-image: url(:/image/border_down.png);"
//                                       "}");
//        }

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(10, 5, 10, 10);

        QSignalMapper *signalMapper = new QSignalMapper(this);
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                int k = i * cols + j + 1;
                if(k > ups_list.count())
                    break;

                QString text = langN.show[langEnum::strUps] + QString("%1").arg(ups_list.at(k - 1).devID);
                flickerItem *item = new flickerItem(text, getAlarmStatus_ups(ups_list.at(k - 1).devID), ui->ups_item);
                connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
                signalMapper->setMapping(item, QString("%1").arg(k));
                gridlayout->addWidget(item, i, j);
                m_upsItemList.append(item);
            }
        }

        if(ups_list.count() < cols)
        {
            gridlayout->setColumnStretch(ups_list.count(), 1);
        }

        connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onUpsClicked(QString))); //将转发的信号连接到最终的槽函数

        ui->ups_item->setLayout(gridlayout);
    }

    if(ups_list.isEmpty() || m_upsItemList.isEmpty())
        return;
    for (int i = 0; i < ups_list.count(); i++) {
        m_upsItemList.at(i)->setState(getAlarmStatus_ups(m_upsList.at(i).devID));
    }
}

void WgtEquipmentOverview::getMeterInfo(QList<meter_Info> meter_list)
{
    if(m_meterList.count() != meter_list.count())
    {
        m_meterList = meter_list;

        cleanMeterDev();

        if(meter_list.isEmpty())
        {
            ui->wgt_meter->setVisible(false);
            return;
        }
        else {
            ui->wgt_meter->setVisible(true);
        }

        int rows, cols;
        cols   = COL_NUM;
        rows   = meter_list.count() / cols;
        if(meter_list.count() % cols)
            ++rows;

//        if(meter_list.count() < 8) {
//            ui->meter_item->setStyleSheet("QWidget#meter_item{"
//                                       "border-image: url(:/image/border_down3.png);"
//                                       "}");
//        }else if (meter_list.count() < 21) {
//            ui->meter_item->setStyleSheet("QWidget#meter_item{"
//                                       "border-image: url(:/image/border_down2.png);"
//                                       "}");
//        }else{
//            ui->meter_item->setStyleSheet("QWidget#meter_item{"
//                                       "border-image: url(:/image/border_down.png);"
//                                       "}");
//        }

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(10, 5, 10, 10);

        QSignalMapper *signalMapper = new QSignalMapper(this);
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                int k = i * cols + j + 1;
                if(k > meter_list.count())
                    break;

                QString text = langN.show[langEnum::strVMeter] + QString("%1").arg(meter_list.at(k - 1).devID);
                flickerItem *item = new flickerItem(text, getAlarmStatus_meter(meter_list.at(k - 1).devID), ui->meter_item);
                connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
                signalMapper->setMapping(item, QString("%1").arg(k));
                gridlayout->addWidget(item, i, j);
                m_meterItemList.append(item);
            }
        }

        if(meter_list.count() < cols)
        {
            gridlayout->setColumnStretch(meter_list.count(), 1);
        }

        connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onMeterClicked(QString))); //将转发的信号连接到最终的槽函数

        ui->meter_item->setLayout(gridlayout);
    }

    if(m_meterList.isEmpty() || m_meterItemList.isEmpty())
        return;

    for (int i = 0; i < m_meterList.count(); i++) {
        m_meterItemList.at(i)->setState(getAlarmStatus_meter(m_meterList.at(i).devID));
    }
}

void WgtEquipmentOverview::getLeakInfo(QList<leak_Info> leak_list)
{
    if(m_leakList.count() != leak_list.count())
    {
        m_leakList = leak_list;

        cleanLeakDev();

        if(leak_list.isEmpty())
        {
            ui->wgt_leak->setVisible(false);
            return;
        }
        else {
            ui->wgt_leak->setVisible(true);
        }

        int rows, cols;
        cols   = COL_NUM;
        rows   = leak_list.count()/ cols;
        if(leak_list.count() % cols)
            ++rows;

//        if(leak_list.count() < 8) {
//            ui->leak_item->setStyleSheet("QWidget#leak_item{"
//                                       "border-image: url(:/image/border_down3.png);"
//                                       "}");
//        }else if (leak_list.count() < 21) {
//            ui->leak_item->setStyleSheet("QWidget#leak_item{"
//                                       "border-image: url(:/image/border_down2.png);"
//                                       "}");
//        }else{
//            ui->leak_item->setStyleSheet("QWidget#leak_item{"
//                                       "border-image: url(:/image/border_down.png);"
//                                       "}");
//        }

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(10, 5, 10, 10);

        QSignalMapper *signalMapper = new QSignalMapper(this);
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                int k = i * cols + j + 1;
                if(k > leak_list.count())
                    break;

                QString text = langN.show[langEnum::strWaterLeak] + QString("%1").arg(leak_list.at(k - 1).devID);
                flickerItem *item = new flickerItem(text, getAlarmStatus_leak(leak_list.at(k - 1).devID), ui->leak_item);
                connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
                signalMapper->setMapping(item, QString("%1").arg(k));
                gridlayout->addWidget(item, i, j);
                m_leakItemList.append(item);
            }
        }

        if(leak_list.count() < cols)
        {
            gridlayout->setColumnStretch(leak_list.count(), 1);
        }

        connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onLeakClicked(QString))); //将转发的信号连接到最终的槽函数
        ui->leak_item->setLayout(gridlayout);
    }

    if(m_leakList.isEmpty() || m_leakItemList.isEmpty())
        return;
    for (int i = 0; i < m_leakList.count(); i++) {
        m_leakItemList.at(i)->setState(getAlarmStatus_leak(m_leakList.at(i).devID));
    }
}

void WgtEquipmentOverview::getSpdsInfo(QList<spds_Info> spds_list)
{
    if(m_spdsList.count() != spds_list.count())
    {
        m_spdsList = spds_list;

        cleanSpdsDev();

        if(spds_list.isEmpty())
        {
            ui->wgt_spds->setVisible(false);
            return;
        }
        else {
            ui->wgt_spds->setVisible(true);
        }

        int rows, cols;
        cols   = COL_NUM;
        rows   = spds_list.count()/ cols;
        if(spds_list.count() % cols)
            ++rows;

//        if(spds_list.count() < 8) {
//            ui->spds_item->setStyleSheet("QWidget#spds_item{"
//                                       "border-image: url(:/image/border_down3.png);"
//                                       "}");
//        }else if (spds_list.count() < 21) {
//            ui->spds_item->setStyleSheet("QWidget#spds_item{"
//                                       "border-image: url(:/image/border_down2.png);"
//                                       "}");
//        }else{
//            ui->spds_item->setStyleSheet("QWidget#spds_item{"
//                                       "border-image: url(:/image/border_down.png);"
//                                       "}");
//        }

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(10, 5, 10, 15);

        QSignalMapper *signalMapper = new QSignalMapper(this);
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                int k = i * cols + j + 1;
                if(k > spds_list.count())
                    break;

                QString text = langN.show[langEnum::strSpds] + QString("%1").arg(spds_list.at(k - 1).devID);
                flickerItem *item = new flickerItem(text, getAlarmStatus_spds(spds_list.at(k - 1).devID), ui->spds_item);
                connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
                signalMapper->setMapping(item, QString("%1").arg(k));
                gridlayout->addWidget(item, i, j);
                m_spdsItemList.append(item);
            }
        }

        if(spds_list.count() < cols)
        {
            gridlayout->setColumnStretch(spds_list.count(), 1);
        }

        connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onSpdsClicked(QString))); //将转发的信号连接到最终的槽函数
        ui->spds_item->setLayout(gridlayout);
    }

    if(m_spdsList.isEmpty() || m_spdsItemList.isEmpty())
        return;
    for (int i = 0; i < m_spdsList.count(); i++) {
        m_spdsItemList.at(i)->setState(getAlarmStatus_spds(m_spdsList.at(i).devID));
    }
}

void WgtEquipmentOverview::getSmsInfo(QList<sms_Info> sms_list)
{
    if(m_spdsList.count() != sms_list.count())
    {
        m_smsList = sms_list;

        cleanSmsDev();

        if(sms_list.isEmpty())
        {
            ui->wgt_sms->setVisible(false);
            return;
        }
        else {
            ui->wgt_sms->setVisible(true);
        }

        int rows, cols;
        cols   = COL_NUM;
        rows   = sms_list.count()/ cols;
        if(sms_list.count() % cols)
            ++rows;

//        if(sms_list.count() < 8) {
//            ui->sms_item->setStyleSheet("QWidget#sms_item{"
//                                       "border-image: url(:/image/border_down3.png);"
//                                       "}");
//        }else if (sms_list.count() < 21) {
//            ui->sms_item->setStyleSheet("QWidget#sms_item{"
//                                       "border-image: url(:/image/border_down2.png);"
//                                       "}");
//        }else{
//            ui->sms_item->setStyleSheet("QWidget#sms_item{"
//                                       "border-image: url(:/image/border_down.png);"
//                                       "}");
//        }

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(10, 5, 10, 15);

        QSignalMapper *signalMapper = new QSignalMapper(this);
        for(int i = 0; i < rows; ++i)
        {
            for(int j = 0; j < cols; ++j)
            {
                int k = i * cols + j + 1;
                if(k > sms_list.count())
                    break;

                QString text = langN.show[langEnum::strSmsDev] + QString("%1").arg(sms_list.at(k - 1).devID);
                flickerItem *item = new flickerItem(text, getAlarmStatus_sms(sms_list.at(k - 1).devID), ui->sms_item);
                connect(item, SIGNAL(clicked()), signalMapper, SLOT(map()));
                signalMapper->setMapping(item, QString("%1").arg(k));
                gridlayout->addWidget(item, i, j);
                m_smsItemList.append(item);
            }
        }

        if(sms_list.count() < cols)
        {
            gridlayout->setColumnStretch(sms_list.count(), 1);
        }

        connect(signalMapper, SIGNAL(mapped(QString )), this, SLOT(onSmsClicked(QString))); //将转发的信号连接到最终的槽函数
        ui->sms_item->setLayout(gridlayout);
    }

    if(m_smsList.isEmpty() || m_smsList.isEmpty())
        return;
    for (int i = 0; i < m_smsList.count(); i++) {
        m_smsItemList.at(i)->setState(getAlarmStatus_sms(m_smsList.at(i).devID));
    }
}

void WgtEquipmentOverview::onthsClicked(QString id)
{
    PageExtend::Instance()->m_wgtLeftMenu->jumpChildItem(SYSTEM_TYPE_ENVI, PageExtend::Instance()->m_thstree, id.toInt());
}

void WgtEquipmentOverview::onAcClicked(QString id)
{
    PageExtend::Instance()->m_wgtLeftMenu->jumpChildItem(SYSTEM_TYPE_ENVI, PageExtend::Instance()->m_actree, id.toInt());
}

void WgtEquipmentOverview::onMeterClicked(QString id)
{
    PageExtend::Instance()->m_wgtLeftMenu->jumpChildItem(SYSTEM_TYPE_POW, PageExtend::Instance()->m_metertree, id.toInt());
}

void WgtEquipmentOverview::onUpsClicked(QString id)
{
    PageExtend::Instance()->m_wgtLeftMenu->jumpChildItem(SYSTEM_TYPE_POW, PageExtend::Instance()->m_upstree, id.toInt());
}

void WgtEquipmentOverview::onLeakClicked(QString id)
{
    PageExtend::Instance()->m_wgtLeftMenu->jumpChildItem(SYSTEM_TYPE_ENVI, PageExtend::Instance()->m_leaktree, id.toInt());
}

void WgtEquipmentOverview::onSpdsClicked(QString id)
{
    PageExtend::Instance()->m_wgtLeftMenu->jumpChildItem(SYSTEM_TYPE_POW, PageExtend::Instance()->m_spdstree, id.toInt());
}

void WgtEquipmentOverview::onSmsClicked(QString id)
{
    PageExtend::Instance()->m_wgtLeftMenu->jumpChildItem(SYSTEM_TYPE_SECURITY, PageExtend::Instance()->m_smsstree, id.toInt());
}

int  WgtEquipmentOverview::getAlarmStatus_ups(int devId)
{
    if(m_upsList.isEmpty())
        return  ALARM_STATE_NOCON;

    for (int i = 0; i < m_upsList.count(); i++)
    {
        if(m_upsList.at(i).devID == devId)
        {
            if(m_upsList.at(i).connect != 2)
            {
                return  ALARM_STATE_NOCON;
            }
            else
            {
                if(m_upsList.at(i).almSts == 0)
                    return  ALARM_STATE_NORNAL;
                else
                    return  ALARM_STATE_ALARM;
            }
        }
    }

    return  ALARM_STATE_NOCON;
}

int  WgtEquipmentOverview::getAlarmStatus_ths(int devId)
{
    if(m_thsList.isEmpty())
        return  ALARM_STATE_NOCON;

    for (int i = 0; i < m_thsList.count(); i++)
    {
        if(m_thsList.at(i).devID == devId)
        {
            if(m_thsList.at(i).connect != 2)
            {
                return  ALARM_STATE_NOCON;
            }
            else
            {
                if(m_thsList.at(i).almSts == 0)
                    return  ALARM_STATE_NORNAL;
                else
                    return  ALARM_STATE_ALARM;
            }
        }
    }

    return  ALARM_STATE_NOCON;
}

int  WgtEquipmentOverview::getAlarmStatus_ac(int devId)
{
    if(m_acList.isEmpty())
        return  ALARM_STATE_NOCON;

    for (int i = 0; i < m_acList.count(); i++)
    {
        if(m_acList.at(i).devID == devId)
        {
            if(m_acList.at(i).connect != 2)
            {
                return  ALARM_STATE_NOCON;
            }
            else
            {
                if(m_acList.at(i).almSts == 0)
                    return  ALARM_STATE_NORNAL;
                else
                    return  ALARM_STATE_ALARM;
            }
        }
    }

    return  ALARM_STATE_NOCON;
}

int  WgtEquipmentOverview::getAlarmStatus_leak(int devId)
{
    if(m_leakList.isEmpty())
        return  ALARM_STATE_NOCON;

    for (int i = 0; i < m_leakList.count(); i++)
    {
        if(m_leakList.at(i).devID == devId)
        {
            if(m_leakList.at(i).connect != 2)
            {
                return  ALARM_STATE_NOCON;
            }
            else
            {
                if(m_leakList.at(i).almSts == 0)
                    return  ALARM_STATE_NORNAL;
                else
                    return  ALARM_STATE_ALARM;
            }
        }
    }

    return  ALARM_STATE_NOCON;
}

int  WgtEquipmentOverview::getAlarmStatus_meter(int devId)
{
    if(m_meterList.isEmpty())
        return  ALARM_STATE_NOCON;

    for (int i = 0; i < m_meterList.count(); i++)
    {
        if(m_meterList.at(i).devID == devId)
        {
            if(m_meterList.at(i).connect != 2)
                return  ALARM_STATE_NOCON;
            else
                return  ALARM_STATE_NORNAL;
        }
    }

    return  ALARM_STATE_NOCON;
}

int  WgtEquipmentOverview::getAlarmStatus_spds(int devId)
{
    if(m_spdsList.isEmpty())
        return  ALARM_STATE_NOCON;

    for (int i = 0; i < m_spdsList.count(); i++)
    {
        if(m_spdsList.at(i).devID == devId)
        {
            if(m_spdsList.at(i).connect != 2)
            {
                return  ALARM_STATE_NOCON;
            }
            else
            {
                if(m_spdsList.at(i).almSts == 0)
                    return  ALARM_STATE_NORNAL;
                else
                    return  ALARM_STATE_ALARM;
            }
        }
    }

    return  ALARM_STATE_NOCON;
}

int  WgtEquipmentOverview::getAlarmStatus_sms(int devId)
{
    if(m_smsList.isEmpty())
        return  ALARM_STATE_NOCON;

    for (int i = 0; i < m_smsList.count(); i++)
    {
        if(m_smsList.at(i).devID == devId)
        {
            if(m_smsList.at(i).connect != 2)
            {
                return  ALARM_STATE_NOCON;
            }
            else
            {
                if(m_smsList.at(i).almSts == 0)
                    return  ALARM_STATE_NORNAL;
                else
                    return  ALARM_STATE_ALARM;
            }
        }
    }

    return  ALARM_STATE_NOCON;
}

void WgtEquipmentOverview::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}











