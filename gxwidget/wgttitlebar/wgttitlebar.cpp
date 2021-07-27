#include "wgttitlebar.h"
#include "ui_wgttitlebar.h"
#include "buzzer.h"
#include <QDateTime>
#include <QStyleOption>
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include "formmain.h"

QString   wgttitlebar::m_strLogo;
pfunction wgttitlebar::pfMuteFunc  = 0;


pfunction wgttitlebar::pfLoginFunc = 0;
//QString   wgttitlebar::m_strAccount;
//QString   wgttitlebar::m_strUserName;
//QString   wgttitlebar::m_strMuteIcon;
//QString   wgttitlebar::m_strAlarmMsg;
//QString   wgttitlebar::m_AlarmColor;

wgttitlebar::wgttitlebar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wgttitlebar)
{
    ui->setupUi(this);

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    ui->lblUserName->setFont(font);
    ui->lblAccount->setFont(font);
    ui->lblTime->setFont(font);
    ui->lblDate->setFont(font);
    ui->lblMsg->setFont(font);

    ui->lblUserName->installEventFilter(this);
    ui->lblAccount->installEventFilter(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &wgttitlebar::updateTime);
    connect(ui->wgtMenu, &WgtMenuBar::clicked, this, [=](int index){
        FormMain::Instance()->setMenuIndex(index);
    });

    connect(ui->btnMute,   &QPushButton::clicked, this, &wgttitlebar::onButtonClick);
    connect(ui->btnLogin,  &QPushButton::clicked, this, &wgttitlebar::onButtonClick);

    ui->wgtMenu->setLayouts(WgtMenuBar::HBoxLayout);

    ui->wgtMenu->addItem_H(langN.show[langEnum::strHome],    "SystemOverview", langEnum::strHome);
    ui->wgtMenu->addItem_H(langN.show[langEnum::strDevMng],  "Extend",         langEnum::strDevMng);
    ui->wgtMenu->addItem_H(langN.show[langEnum::strDataMng], "DataManage",     langEnum::strDataMng);
    ui->wgtMenu->addItem_H(langN.show[langEnum::strAlmMng],  "Alarm",          langEnum::strAlmMng);
    ui->wgtMenu->addItem_H(langN.show[langEnum::strSysSet],  "SystemSettings", langEnum::strSysSet);
    ui->wgtMenu->setAlignment(Qt::AlignCenter);

    timer->start(1000);
    updateTime();
}

wgttitlebar::~wgttitlebar()
{
    delete ui;
}

void wgttitlebar::setLogo(QString strLogo)
{
    m_strLogo = strLogo;
}

void wgttitlebar::setMuteFunc(pfunction pf)
{
    pfMuteFunc = pf;
}

void wgttitlebar::setMuteIcon(QString strIcon)
{
    m_strMuteIcon = strIcon;

    ui->btnMute->setIcon(QIcon(m_strMuteIcon));
}

void wgttitlebar::setLoginFunc(pfunction pf)
{
    pfLoginFunc = pf;
}

void wgttitlebar::setLoginInfo(QString  username, QString account)
{
    m_strUserName = username;
    m_strAccount  = account;

    ui->lblUserName->setText(m_strUserName);
    ui->lblAccount->setText(m_strAccount);
}

void wgttitlebar::setAlarmMsg(QString msg, QString color)
{
    m_strAlarmMsg = msg;
    m_AlarmColor  = color;

    ui->lblMsg->setText(m_strAlarmMsg);
    ui->lblMsg->setStyleSheet(QString("color: %1;").arg(m_AlarmColor));
}

void wgttitlebar::setCurrIndex(int index)
{
    ui->wgtMenu->setCurrIndex(index);
}

void wgttitlebar::updateTime()
{
    ui->lblTime->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->lblDate->setText(QDate::currentDate().toString("yyyy/MM/dd"));

    if(FormMain::Instance()->getLoginStatus() == LOGIN_NOT_LOGGED){
        ui->widget->setVisible(false);
    }else {
        ui->widget->setVisible(true);
    }
}

void wgttitlebar::onButtonClick()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if(!button)
        return;

    ShortBeep();

    if( button == ui->btnLogin &&pfLoginFunc){
        pfLoginFunc();
    } else if( button == ui->btnMute && pfMuteFunc){
        pfMuteFunc();
    }

    return;
}

void wgttitlebar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void wgttitlebar::showEvent(QShowEvent *)
{
    if(m_strLogo != "")
    {
        QPixmap pix(m_strLogo);
        pix = pix.scaled(ui->lblTitle->width(), ui->lblTitle->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->lblTitle->setPixmap(pix);
    }
}

bool wgttitlebar::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLabel"))
        {
            QLabel *lbl = qobject_cast<QLabel *>(obj);
            if((lbl == ui->lblUserName) || (lbl == ui->lblAccount))
            {
                 ShortBeep();

                if(pfLoginFunc != 0)
                    pfLoginFunc();
                return true;
            }
        }
    }
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}
