#include "sysfunction.h"
#include "ui_sysfunction.h"
#include "myapplication.h"
#include "gettext.h"
#include "buzzer.h"
#include "formmain.h"
#include "software_version.h"
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QStorageInfo>

#define SD_CARD_PATH   "/run/media/mmcblk0p1"

QString   SysFunction::m_strAccount;
QString   SysFunction::m_strUserName;
QString   SysFunction::m_strLoginSta;

SysFunction::SysFunction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysFunction)
{
    setupUi();

    lieVector.append(ui->lineEditTimeVal);
    ui->lineEditTimeVal->installEventFilter(this);
    connect(ui->btnScrSaverEabele, &SwitchButton::btnState, this, &SysFunction::onBtnClicked);
    connect(ui->btnLogin, &QPushButton::clicked, this, &SysFunction::onBtnLoginClicked);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]()
    {
        ui->lblUserNameVal->setText(m_strUserName);
        ui->lblAccountVal->setText(m_strAccount);
        ui->btnLogin->setText(m_strLoginSta);

    });
    timer->start(1000);

    QTimer *timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, [=](){
        memInfoQStorageInfo();
    });
}

SysFunction::~SysFunction()
{
    delete ui;
}

void SysFunction::setupUi(void)
{
    ui->setupUi(this);
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    ui->lblScreenSaver->setText(/*QObject::tr("屏保功能")*/langN.show[langEnum::strSceenSaveFun]);
    ui->lblScrSaverName->setText(/*QObject::tr("屏幕保护是否开启")*/langN.show[langEnum::strSceenSaveSwitch]);
    ui->lblTimeName->setText(/*QObject::tr("屏幕保护等待时间(分钟)")*/langN.show[langEnum::strSceenSaveWaitTime]);

    //读取屏保配置文件
    QString enable, timer_interval;
    QString fileName =  QString("/opt/%2").arg("qt_config.ini");
    QFile file(fileName);
    if(file.exists())
    {
        QSettings *config = new QSettings(fileName, QSettings::IniFormat);
        enable         = config->value("user_Info/screenSaver_enable").toString();
        timer_interval = config->value("user_Info/screenSaver_interval").toString();
        delete   config;
    }else {
        enable = "0";
        timer_interval = "3";
    }

    if(enable.isEmpty())
        enable = "0";

    if(timer_interval.isEmpty())
        timer_interval = "3";

    if(enable == "0")
        ui->btnScrSaverEabele->setSwitch(false);
    else if(enable == "1")
        ui->btnScrSaverEabele->setSwitch(true);

    ui->lineEditTimeVal->setText(timer_interval);

    ui->lbllogin->setText(/*QObject::tr("登录信息")*/langN.show[langEnum::strLoginInfo]);
    ui->lblUserName->setText(/*QObject::tr("用户:")*/langN.show[langEnum::strRole_USER]+ ":");
    ui->lblAccount->setText(/*QObject::tr("账号:")*/langN.show[langEnum::strAccount]);

    ui->lblMemName->setText(/*QObject::tr("内部存储")*/langN.show[langEnum::strMemory]);
    ui->lblmem->setText(/*QObject::tr("内存信息")*/langN.show[langEnum::strMemInfo]);
    ui->lblMemsurplus->setText(/*QObject::tr("剩余内存容量:")*/langN.show[langEnum::strMemAvailable] + ":");
    ui->lblMemtotal->setText(/*QObject::tr("总容量：")*/langN.show[langEnum::strTotal] + ":");
    ui->lblMemsurplusVal->setText("0");
    ui->lblMemtotalVal->setText("0");

    ui->lblSDName->setText(/*QObject::tr("SD卡存储")*/langN.show[langEnum::strSDCard]);
    ui->lblSDsurplus->setText(/*QObject::tr("剩余内存容量:")*/langN.show[langEnum::strMemAvailable] + ":");
    ui->lblSDtotal->setText(/*QObject::tr("总容量：")*/langN.show[langEnum::strTotal] + ":");
    ui->lblSDsurplusVal->setText("0");
    ui->lblSDtotalVal->setText("0");

    ui->lblsysInfo->setText(langN.show[langEnum::strHelp]);
    ui->lblSysName->setText(langN.show[langEnum::strSysName] + ":");
    ui->lblSysVer->setText(langN.show[langEnum::strFwVer] + ":");
    //ui->lblSysNameVal->setText("LcdComm");
    ui->lblSysVerVal->setText(SOFTWARE_VERSION);

    ui->lblScreenSaver->setFont(font);
    ui->lblScrSaverName->setFont(font);
    ui->lblTimeName->setFont(font);
    ui->lbllogin->setFont(font);
    ui->lblUserName->setFont(font);
    ui->lblAccount->setFont(font);
    ui->lblMemName->setFont(font);
    ui->lblmem->setFont(font);
    ui->lblMemsurplus->setFont(font);
    ui->lblMemtotal->setFont(font);
    ui->lblSDName->setFont(font);
    ui->lblSDsurplus->setFont(font);
    ui->lblSDtotal->setFont(font);
    ui->lblSDtotalVal->setFont(font);

    //linux版本信息
    FILE *stream;
    char  buf[100] = {0};
    stream = popen("uname -srm", "r");
    fread(buf, 1, sizeof(buf), stream);
    pclose(stream);

    QString str(buf);
    ui->lblSysNameVal->setText(str.trimmed());
    //qDebug()<<str;

    memInfoQStorageInfo();
}

void SysFunction::memInfo(void)
{
    QFile file(SD_CARD_PATH);
    if(!file.exists())
        ui->wgtSD->setVisible(false);
    else {
        ui->wgtSD->setVisible(true);
    }

    FILE *stream;
    char  buf[100] = {0};

    stream = popen("df -h | grep ubi0  |awk '{print $2,$4,$5}'", "r");
    fread(buf, 1, sizeof(buf), stream);
    pclose(stream);
    if(buf[0] != '\0')
    {
        QString str(buf);
        QStringList list = str.split(" ");
        if(!list.isEmpty())
        {
            ui->lblMemsurplusVal->setText(list.at(1));
            ui->lblMemtotalVal->setText(list.at(0));
            QStringList list2 = list.at(2).split("%");
            ui->progressBar->setValue(list2.at(0).toInt());
        }
    }

    memset(buf, 0, sizeof(buf)/sizeof(char));
    stream = popen("df -m | awk '/\/dev\/mmcblk0p1 {print $2,$4,$5}'", "r");
    fread(buf, 1, sizeof(buf), stream);
    pclose(stream);
    if(buf[0] != '\0')
    {
        QString str(buf);
        QStringList list = str.split(" ");
        if(!list.isEmpty())
        {
            ui->lblSDsurplusVal->setText(list.at(1)+"M");
            ui->lblSDtotalVal->setText(list.at(0)+"M");
            QStringList list2 = list.at(2).split("%");
            ui->progressBar_2->setValue(list2.at(0).toInt());
        }
    }
}

void SysFunction::memInfoQStorageInfo()
{

    QFile file(SD_CARD_PATH);
    if(!file.exists())
        ui->wgtSD->setVisible(false);
    else {
        ui->wgtSD->setVisible(true);
    }

    double memTotal,memAVailable,memPercentage;
    double sdTotal,sdAVailable,sdPercentage;

    QStorageInfo storage = QStorageInfo::root();
    storage.refresh();
    memTotal = storage.bytesTotal()/1024/1024;
    memAVailable = storage.bytesAvailable()/1024/1024;
    memPercentage = ((memTotal-memAVailable)/memTotal)*100;
    ui->lblMemsurplusVal->setText(QString::number(memAVailable)+"M");
    ui->lblMemtotalVal->setText(QString::number(memTotal)+"M");
    ui->progressBar->setValue(memPercentage);

    storage.setPath(SD_CARD_PATH);
    sdTotal = storage.bytesTotal()/1024.0/1024.0/1024.0;
    sdAVailable = storage.bytesAvailable()/1024.0/1024.0/1024.0;
    sdPercentage = ((sdTotal-sdAVailable)/sdTotal)*100;

    ui->lblSDsurplusVal->setText(QString::number(sdAVailable,'f',1)+"G");
    ui->lblSDtotalVal->setText(QString::number(sdTotal,'f',1)+"G");
    ui->progressBar_2->setValue(int(sdPercentage));

}

void SysFunction::setLoginInfo(QString  username, QString account, QString loginstatus)
{
    m_strUserName = username;
    m_strAccount  = account;
    m_strLoginSta = loginstatus;
}

bool SysFunction::eventFilter(QObject *obj, QEvent *event)
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
                QRegExp regExp("[0-9.0-9]*");
                if (!regExp.exactMatch(text)) {
                    return;
                }

                if(lineEdit != 0)
                {
                    if(text.toUInt() < 1 || text.startsWith("0"))
                        return;

                    lineEdit->setText(text);
                    QSettings *config = new QSettings(QString("/opt/%2").arg("qt_config.ini"), QSettings::IniFormat);

                    config->setValue("user_Info/screenSaver_interval", text);
                    delete  config;

                    //派发事件
                    postCustomEvents();
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

void SysFunction::onBtnLoginClicked(bool statue)
{
    Q_UNUSED(statue);
    ShortBeep();

    FormMain::Instance()->setLoginFunc();
}

void SysFunction::onBtnClicked(bool statue)
{
    ShortBeep();

    int val;
    if(statue)
        val = 1;
    else
        val = 0;

    //更新信息
    QSettings *config = new QSettings(QString("/opt/%2").arg("qt_config.ini"), QSettings::IniFormat);
    config->setValue("user_Info/screenSaver_enable",   val);
    delete  config;

    //派发事件
    postCustomEvents();
}

void SysFunction::postCustomEvents(void)
{
    QWidgetList list = QApplication::allWidgets();
    if(list.isEmpty())
        return;

    QWidget *w = list.at(0);
    QApplication::postEvent(w, new QEvent(PARA_UPDATE));
}

void SysFunction::changeEvent(QEvent* event)
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

void SysFunction::retranslateUi(void)
{
    ui->lblScreenSaver->setText(/*QObject::tr("屏保功能")*/langN.show[langEnum::strSysFun]);
    ui->lblScrSaverName->setText(/*QObject::tr("屏幕保护是否开启")*/langN.show[langEnum::strSceenSaveSwitch]);
    ui->lblTimeName->setText(/*QObject::tr("屏幕保护等待时间(分钟)")*/langN.show[langEnum::strSceenSaveWaitTime]);
    ui->lbllogin->setText(/*QObject::tr("登录信息")*/langN.show[langEnum::strLoginInfo]);
    ui->lblUserName->setText(/*QObject::tr("用户:")*/langN.show[langEnum::strRole_USER] + ":");
    ui->lblAccount->setText(/*QObject::tr("账号:")*/langN.show[langEnum::strAccount]);

    ui->lblMemName->setText(langN.show[langEnum::strMemory]);
    ui->lblmem->setText(/*QObject::tr("内存信息")*/langN.show[langEnum::strMemInfo]);
    ui->lblMemsurplus->setText(/*QObject::tr("剩余内存容量:")*/langN.show[langEnum::strMemAvailable] + ":");
    ui->lblMemtotal->setText(/*QObject::tr("总容量：")*/langN.show[langEnum::strTotal] + ":");
    ui->lblSDName->setText(/*QObject::tr("SD卡存储")*/langN.show[langEnum::strSDCard]);
    ui->lblSDsurplus->setText(/*QObject::tr("剩余内存容量:")*/langN.show[langEnum::strMemAvailable] + ":");
    ui->lblSDtotal->setText(/*QObject::tr("总容量：")*/langN.show[langEnum::strTotal] + ":");
    ui->lblsysInfo->setText(langN.show[langEnum::strHelp]);
    ui->lblSysName->setText(langN.show[langEnum::strSysName] + ":");
    ui->lblSysVer->setText(langN.show[langEnum::strFwVer] + ":");
}



