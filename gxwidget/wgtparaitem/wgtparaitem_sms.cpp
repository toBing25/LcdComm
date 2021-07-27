#include "wgtparaitem_sms.h"
#include "ui_wgtparaitem_sms.h"
#include "formmain.h"

wgtParaItem_sms::wgtParaItem_sms(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wgtParaItem_sms),
    m_curId(id)
{
    qRegisterMetaType<QList<sms_Info>>("QList<sms_Info>");

    setupUi();
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevSmsList,this,&wgtParaItem_sms::getSmsInfo);
}

wgtParaItem_sms::~wgtParaItem_sms()
{
    delete ui;
}

void wgtParaItem_sms::setupUi(void)
{
    ui->setupUi(this);
}

void wgtParaItem_sms::setCurrentId(int id)
{
    if((id - 1) < 0)
        return;

    ui->lblSmsNumberVal->setText(QString::number(id));
    m_curId = id - 1;

    getSmsInfo(sms_list);
}

void wgtParaItem_sms::getSmsInfo(QList<sms_Info> infoList)
{
    sms_list = infoList;
    if((sms_list.count() == 0) || (infoList.count() == 0))
        return;

    if(m_curId >= infoList.count())
        return;

    setConnectState(infoList[m_curId].connect);
    ui->lblSmsNumberVal->setText(QString::number(infoList[m_curId].devID));
    ui->wgtSignal->setSigVal(infoList[m_curId].signalLV);
}

void wgtParaItem_sms::setConnectState(int state)
{
    QString connect;
    QColor  color;
    switch (state) {
    case 0:
        connect = /*("无效")*/langN.show[langEnum::strInvalid];
        color   = Qt::red;
        break;
    case 1:
        connect = langN.show[langEnum::strDisconn];
        color   = Qt::red;
        break;
    case 2:
        connect = /*("已连接")*/langN.show[langEnum::strConnect];
        color   = QColor(45, 214, 218);
        break;
    default:
        connect = /*("无效")*/langN.show[langEnum::strInvalid];
        color   = Qt::red;
        break;
    }
    QPalette pe = ui->lblSmsStaVal->palette();
    pe.setColor(QPalette::WindowText, color);
    ui->lblSmsStaVal->setPalette(pe);
    ui->lblSmsStaVal->setText(connect);
}
