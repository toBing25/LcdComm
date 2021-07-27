
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "pagenetworkset.h"
#include "Global/SysCfgMng.h"
#include "wgtmsgbox_h.h"
#include "buzzer.h"
#include "gettext.h"
#include "formmain.h"
#include "languagePublic.h"
#include <QRegExpValidator>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QTimer>
#include <QEvent>
#include <QDebug>

PageNetWorkSet::PageNetWorkSet(QWidget *parent)
    : QWidget(parent)
{
    setupUi();

    lieVector.append(m_leIpVal1);
    lieVector.append(m_leMaskVal1);
    lieVector.append(m_leGatewayVal1);
    lieVector.append(m_leDns1Val);
    lieVector.append(m_leDns2Val);

    connect(m_btnSet, &QPushButton::clicked, this, &PageNetWorkSet::onNetworksettings);

    //设置IP地址校验过滤
    foreach (QLineEdit *lie, lieVector)
    {
        lie->installEventFilter(this);
    }
}

PageNetWorkSet::~PageNetWorkSet()
{

}

void PageNetWorkSet::setupUi(void)
{
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->setSpacing(6);
    vBoxLayout->setObjectName(QStringLiteral("vBoxLayout"));
    vBoxLayout->setContentsMargins(30, 8, 30, 22);

    m_wgtNet = new QWidget(this);
    m_wgtNet->setObjectName(QStringLiteral("m_wgtNet"));
    m_wgtNet->setMinimumSize(QSize(0, 200));
    m_wgtNet->setMaximumSize(QSize(16777215, 300));

    QVBoxLayout *wgtNetLayout = new QVBoxLayout(m_wgtNet);
    wgtNetLayout->setSpacing(20);
    wgtNetLayout->setObjectName(QStringLiteral("wgtNetLayout"));

    wgtNetLayout->setContentsMargins(0, 10, 0, 10);
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    //IP25
    QHBoxLayout *IpLayout = new QHBoxLayout;
    IpLayout->setSpacing(50);
    IpLayout->setObjectName(QStringLiteral("IpLayout"));
    IpLayout->setContentsMargins(10, 0, 10, 0);

    m_lblIpName = new QLabel;
    m_lblIpName->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    m_lblIpName->setLayoutDirection(Qt::RightToLeft);
    m_lblIpName->setMinimumSize(QSize(200, 35));
    m_lblIpName->setMaximumSize(QSize(200, 35));
    m_lblIpName->setFont(font);
    m_lblIpName->setStyleSheet("color: #00AEFF;");
    m_lblIpName->setText(langN.show[langEnum::strIpAddr]+":");

    m_leIpVal1 =  new QLineEdit;
    m_leIpVal1->setObjectName("m_leIpVal1");
    m_leIpVal1->setAlignment(Qt::AlignCenter);
    m_leIpVal1->setMinimumSize(QSize(100, 35));
    m_leIpVal1->setMaximumSize(QSize(180, 35));
    m_leIpVal1->setStyleSheet("border-image: url(:/image/border2.png);"
                              "color: #FFFFFF;"
                              "border:0px solid #68CBF2;"
                              "border-radius:0px;font:12pt;");

    IpLayout->addWidget(m_lblIpName);
    IpLayout->addWidget(m_leIpVal1);

    IpLayout->addStretch();
    wgtNetLayout->addLayout(IpLayout);
    wgtNetLayout->addStretch();

    //Mask
    QHBoxLayout *MaskLayout = new QHBoxLayout;
    MaskLayout->setSpacing(50);
    MaskLayout->setObjectName(QStringLiteral("MaskLayout"));
    MaskLayout->setContentsMargins(10, 0, 10, 0);

    m_lblMaskName = new QLabel;
    m_lblMaskName->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    m_lblMaskName->setLayoutDirection(Qt::RightToLeft);
    m_lblMaskName->setMinimumSize(QSize(200, 35));
    m_lblMaskName->setMaximumSize(QSize(200, 35));
    m_lblMaskName->setFont(font);
    m_lblMaskName->setStyleSheet("color: #00AEFF;");
    m_lblMaskName->setText(langN.show[langEnum::strNetMask]+":");

    m_leMaskVal1 =  new QLineEdit;
    m_leMaskVal1->setObjectName("m_leMaskVal1");
    m_leMaskVal1->setAlignment(Qt::AlignCenter);
    m_leMaskVal1->setMinimumSize(QSize(100, 35));
    m_leMaskVal1->setMaximumSize(QSize(180, 35));
    m_leMaskVal1->setStyleSheet("border-image: url(:/image/border2.png);"
                                "color: #FFFFFF;"
                                "border:0px solid #68CBF2;"
                                "border-radius:0px;font:12pt;");


    MaskLayout->addWidget(m_lblMaskName);
    MaskLayout->addWidget(m_leMaskVal1);
    MaskLayout->addStretch();
    wgtNetLayout->addLayout(MaskLayout);

    //Gateway
    QHBoxLayout *GatewayLayout = new QHBoxLayout;
    GatewayLayout->setSpacing(50);
    GatewayLayout->setObjectName(QStringLiteral("GatewayLayout"));
    GatewayLayout->setContentsMargins(10, 0, 10, 0);

    m_lblGatewayName = new QLabel;
    m_lblGatewayName->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    m_lblGatewayName->setLayoutDirection(Qt::RightToLeft);
    m_lblGatewayName->setMinimumSize(QSize(200, 35));
    m_lblGatewayName->setMaximumSize(QSize(200, 35));
    m_lblGatewayName->setFont(font);
    m_lblGatewayName->setStyleSheet("color: #00AEFF;");
    m_lblGatewayName->setText(langN.show[langEnum::strRoute]+":");

    m_leGatewayVal1 =  new QLineEdit;
    m_leGatewayVal1->setObjectName("m_leGatewayVal1");
    m_leGatewayVal1->setAlignment(Qt::AlignCenter);
    m_leGatewayVal1->setMinimumSize(QSize(100, 35));
    m_leGatewayVal1->setMaximumSize(QSize(180, 35));
    m_leGatewayVal1->setStyleSheet("border-image: url(:/image/border2.png);"
                                   "color: #FFFFFF;"
                                   "border:0px solid #68CBF2;"
                                   "border-radius:0px;font:12pt;");

    GatewayLayout->addWidget(m_lblGatewayName);
    GatewayLayout->addWidget(m_leGatewayVal1);

    GatewayLayout->addStretch();
    wgtNetLayout->addLayout(GatewayLayout);
    wgtNetLayout->addStretch();

    //DNS1
    QHBoxLayout *Dns1Layout = new QHBoxLayout;
    Dns1Layout->setSpacing(50);
    Dns1Layout->setObjectName(QStringLiteral("Dns1Layout"));
    Dns1Layout->setContentsMargins(10, 0, 10, 0);

    m_lblDns1Name = new QLabel;
    m_lblDns1Name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    m_lblDns1Name->setLayoutDirection(Qt::RightToLeft);
    m_lblDns1Name->setMinimumSize(QSize(200, 35));
    m_lblDns1Name->setMaximumSize(QSize(200, 35));
    m_lblDns1Name->setFont(font);
    m_lblDns1Name->setStyleSheet("color: #00AEFF;");
    m_lblDns1Name->setText(langN.show[langEnum::strPrefDNS]+":");

    m_leDns1Val =  new QLineEdit;
    m_leDns1Val->setObjectName("m_leDns1Val");
    m_leDns1Val->setAlignment(Qt::AlignCenter);
    m_leDns1Val->setMinimumSize(QSize(100, 35));
    m_leDns1Val->setMaximumSize(QSize(180, 35));
    m_leDns1Val->setStyleSheet("border-image: url(:/image/border2.png);"
                               "color: #FFFFFF;"
                               "border:0px solid #68CBF2;"
                               "border-radius:0px;font:12pt;");

    Dns1Layout->addWidget(m_lblDns1Name);
    Dns1Layout->addWidget(m_leDns1Val);

    Dns1Layout->addStretch();
    wgtNetLayout->addLayout(Dns1Layout);
    wgtNetLayout->addStretch();


    //DNS2
    QHBoxLayout *Dns2Layout = new QHBoxLayout;
    Dns2Layout->setSpacing(50);
    Dns2Layout->setObjectName(QStringLiteral("Dns2Layout"));
    Dns2Layout->setContentsMargins(10, 0, 10, 0);

    m_lblDns2Name = new QLabel;
    m_lblDns2Name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    m_lblDns2Name->setLayoutDirection(Qt::RightToLeft);
    m_lblDns2Name->setMinimumSize(QSize(200, 35));
    m_lblDns2Name->setMaximumSize(QSize(200, 35));
    m_lblDns2Name->setFont(font);
    m_lblDns2Name->setStyleSheet("color: #00AEFF;");
    m_lblDns2Name->setText(langN.show[langEnum::strAlterDNS]+":");

    m_leDns2Val =  new QLineEdit;
    m_leDns2Val->setObjectName("m_leDns2Val");
    m_leDns2Val->setAlignment(Qt::AlignCenter);
    m_leDns2Val->setMinimumSize(QSize(100, 35));
    m_leDns2Val->setMaximumSize(QSize(180, 35));
    m_leDns2Val->setStyleSheet("border-image: url(:/image/border2.png);"
                               "color: #FFFFFF;"
                               "border:0px solid #68CBF2;"
                               "border-radius:0px;font:12pt;");

    Dns2Layout->addWidget(m_lblDns2Name);
    Dns2Layout->addWidget(m_leDns2Val);

    Dns2Layout->addStretch();
    wgtNetLayout->addLayout(Dns2Layout);
    wgtNetLayout->addStretch();

    m_wgtSet = new QWidget(this);
    m_wgtSet->setObjectName(QStringLiteral("m_wgtSet"));
    m_wgtSet->setMinimumSize(QSize(0, 90));
    m_wgtSet->setMaximumSize(QSize(16777215, 90));

    QHBoxLayout *wgtSetLayout = new QHBoxLayout(m_wgtSet);
    wgtSetLayout->setSpacing(6);
    wgtSetLayout->setObjectName(QStringLiteral("wgtSetLayout"));
    wgtSetLayout->setContentsMargins(10, 10, 10, 10);

    QSpacerItem *horizontalSpacer = new QSpacerItem(600, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_btnSet = new QPushButton;
    m_btnSet->setObjectName("m_btnSet");
    m_btnSet->setText(langN.show[langEnum::strSet]);
    m_btnSet->setMinimumSize(QSize(90, 35));
    m_btnSet->setMaximumSize(QSize(90, 35));
    m_btnSet->setIcon(QIcon(":/image/setting.png"));
    m_btnSet->setIconSize(QSize(25, 25));
    m_btnSet->setFocusPolicy(Qt::NoFocus);
    m_btnSet->setStyleSheet(
                            "QPushButton:!pressed{"
                            "border-image: url(:/image/btn.png);"
                            "color: #FFFFFF;"
                            "border-radius:0px;font:13pt;"
                            "}"
                            "QPushButton:pressed{"
                            "border-image: url(:/image/btn.png);"
                            "color: #FFFFFF;"
                            "border:0px solid #68CBF2;"
                            "border-radius:0px;font:13pt;}"
                            );

    wgtSetLayout->addItem(horizontalSpacer);
    wgtSetLayout->addWidget(m_btnSet);
    wgtSetLayout->addStretch();

    vBoxLayout->addStretch();
    vBoxLayout->addWidget(m_wgtNet);
    vBoxLayout->addWidget(m_wgtSet);
    vBoxLayout->addStretch();
    this->setLayout(vBoxLayout);
    permissUpdate();
}

bool PageNetWorkSet::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLineEdit"))
        {
            ShortBeep();
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj);
            connect(WgtKeyPad::Instance(), &WgtKeyPad::sendInputBufferAreaText, [=](QString text)
            {
                QRegExp regExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
                if (!regExp.exactMatch(text)) {
                    return;
                }

                if(lineEdit != 0)
                {
                    QString ip, mask, gateway, dns1, dns2;
                    lineEdit->setText(text);

                    ip        = QString("%1").arg(m_leIpVal1->text());
                    if(!ip.isEmpty() && (ip != m_ip))
                        m_ip  = ip;

                    mask       = QString("%1").arg(m_leMaskVal1->text());
                    if(!mask.isEmpty() && (mask != m_mask))
                        m_mask = mask;

                    gateway   = QString("%1").arg(m_leGatewayVal1->text());
                    if(!gateway.isEmpty() && (gateway != m_gateway))
                        m_gateway = gateway;

                    dns1      = QString("%1").arg(m_leDns1Val->text());
                    if(!dns1.isEmpty() && (dns1 != m_dns1))
                        m_dns1 = dns1;

                    dns2      = QString("%1").arg(m_leDns2Val->text());
                    if(!dns2.isEmpty() && (dns2 != m_dns2))
                        m_dns2 = dns2;

                    return;
                }
            });

            foreach (QLineEdit *lie, lieVector) {
                if(lie == lineEdit)
                {
                    WgtKeyPad::Instance()->showInputBufferArea(lie->text());
                    WgtKeyPad::Instance()->show();
                    return  true;
                }
            }
        }
    }
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

void PageNetWorkSet::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    QTimer::singleShot(100, this, &PageNetWorkSet::getNetInfo);
}

void PageNetWorkSet::getNetInfo()
{
    //从共享内存读取IP信息
    m_ip      = QString("%1.%2.%3.%4").arg(sysCfg->eMtrMain.netSet.ipByte1)
            .arg(sysCfg->eMtrMain.netSet.ipByte2)
            .arg(sysCfg->eMtrMain.netSet.ipByte3)
            .arg(sysCfg->eMtrMain.netSet.ipByte4);
    m_leIpVal1->setText(m_ip);

    m_mask    = QString("%1.%2.%3.%4").arg(sysCfg->eMtrMain.netSet.maskByte1)
            .arg(sysCfg->eMtrMain.netSet.maskByte2)
            .arg(sysCfg->eMtrMain.netSet.maskByte3)
            .arg(sysCfg->eMtrMain.netSet.maskByte4);
    m_leMaskVal1->setText(m_mask);

    m_gateway = QString("%1.%2.%3.%4").arg(sysCfg->eMtrMain.netSet.routeByte1)
            .arg(sysCfg->eMtrMain.netSet.routeByte2)
            .arg(sysCfg->eMtrMain.netSet.routeByte3)
            .arg(sysCfg->eMtrMain.netSet.routeByte4);
    m_leGatewayVal1->setText(m_gateway);

    if((sysCfg->eMtrMain.netSet.dns1Byte1 == 666) && (sysCfg->eMtrMain.netSet.dns1Byte2 == 666)&&
            (sysCfg->eMtrMain.netSet.dns1Byte3 == 666) && (sysCfg->eMtrMain.netSet.dns1Byte4 == 666))
    {
        m_dns1 = QString();
    }
    else
    {
        m_dns1  = QString("%1.%2.%3.%4").arg(sysCfg->eMtrMain.netSet.dns1Byte1)
                .arg(sysCfg->eMtrMain.netSet.dns1Byte2)
                .arg(sysCfg->eMtrMain.netSet.dns1Byte3)
                .arg(sysCfg->eMtrMain.netSet.dns1Byte4);
    }
    m_leDns1Val->setText(m_dns1);

    if((sysCfg->eMtrMain.netSet.dns2Byte1 == 666) && (sysCfg->eMtrMain.netSet.dns2Byte2 == 666)&&
            (sysCfg->eMtrMain.netSet.dns2Byte3 == 666) && (sysCfg->eMtrMain.netSet.dns2Byte4 == 666))
    {
        m_dns2 = QString();
    }
    else {
        m_dns2  = QString("%1.%2.%3.%4").arg(sysCfg->eMtrMain.netSet.dns2Byte1)
                .arg(sysCfg->eMtrMain.netSet.dns2Byte2)
                .arg(sysCfg->eMtrMain.netSet.dns2Byte3)
                .arg(sysCfg->eMtrMain.netSet.dns2Byte4);
    }
    m_leDns2Val->setText(m_dns2);
}

void PageNetWorkSet::onNetworksettings(bool clicked)
{
    Q_UNUSED(clicked);
    ShortBeep();

    QStringList iplist = m_ip.split(".");
    sysCfg->eMtrMain.netSet.ipByte1  = iplist.at(0).toInt();
    sysCfg->eMtrMain.netSet.ipByte2  = iplist.at(1).toInt();
    sysCfg->eMtrMain.netSet.ipByte3  = iplist.at(2).toInt();
    sysCfg->eMtrMain.netSet.ipByte4  = iplist.at(3).toInt();

    QStringList masklist = m_mask.split(".");
    sysCfg->eMtrMain.netSet.maskByte1 = masklist.at(0).toInt();
    sysCfg->eMtrMain.netSet.maskByte2 = masklist.at(1).toInt();
    sysCfg->eMtrMain.netSet.maskByte3 = masklist.at(2).toInt();
    sysCfg->eMtrMain.netSet.maskByte4 = masklist.at(3).toInt();

    QStringList routelist = m_gateway.split(".");
    sysCfg->eMtrMain.netSet.routeByte1 = routelist.at(0).toInt();
    sysCfg->eMtrMain.netSet.routeByte2 = routelist.at(1).toInt();
    sysCfg->eMtrMain.netSet.routeByte3 = routelist.at(2).toInt();
    sysCfg->eMtrMain.netSet.routeByte4 = routelist.at(3).toInt();

    if(m_dns1.isEmpty())
        m_dns1 = "666.666.666.666";
    QStringList dns1list = m_dns1.split(".");
    sysCfg->eMtrMain.netSet.dns1Byte1 = dns1list.at(0).toInt();
    sysCfg->eMtrMain.netSet.dns1Byte2 = dns1list.at(1).toInt();
    sysCfg->eMtrMain.netSet.dns1Byte3 = dns1list.at(2).toInt();
    sysCfg->eMtrMain.netSet.dns1Byte4 = dns1list.at(3).toInt();

    if(m_dns2.isEmpty())
        m_dns2 = "666.666.666.666";
    QStringList dns2list = m_dns2.split(".");
    sysCfg->eMtrMain.netSet.dns2Byte1 = dns2list.at(0).toInt();
    sysCfg->eMtrMain.netSet.dns2Byte2 = dns2list.at(1).toInt();
    sysCfg->eMtrMain.netSet.dns2Byte3 = dns2list.at(2).toInt();
    sysCfg->eMtrMain.netSet.dns2Byte4 = dns2list.at(3).toInt();

    //设置修改标志位
    sysCfg->cfgChgedFlag.eMtrMain  = 1;

    WgtMsgBox_H msgbox(langN.show[langEnum::strSetNetWorkSuccess], MSGBOX_TYPE_OK);
    msgbox.exec();
}

void  PageNetWorkSet::permissUpdate(void)
{
    if(FormMain::Instance()->m_loginInfo.from_role != "role003")
    {
        m_btnSet->setVisible(false);
    }else {
        m_btnSet->setVisible(true);
    }
}

bool PageNetWorkSet::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        permissUpdate();

        return true;
    }

    return QWidget::event(ev);
}

void PageNetWorkSet::changeEvent(QEvent* event)
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

void PageNetWorkSet::retranslateUi(void)
{
    m_lblIpName->setText(langN.show[langEnum::strIpAddr]+":");
    m_lblMaskName->setText(langN.show[langEnum::strNetMask]+":");
    m_lblGatewayName->setText(langN.show[langEnum::strRoute]+":");
    m_lblDns1Name->setText(langN.show[langEnum::strPrefDNS]+":");
    m_lblDns2Name->setText(langN.show[langEnum::strAlterDNS]+":");
    m_btnSet->setText(langN.show[langEnum::strSet]);
}

