#include "gettext.h"
#include "Gpio.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QSettings>

int          GetText::_currLangId  = TEXTLANG_CN;
QTranslator* GetText::_pTranslator = NULL;

int  GetText::m_XSize  = 1024;
int  GetText::m_YSize  = 600;

GetText::GetText()
{
}

GetText::~GetText()
{
}

int  GetText::init(QTranslator *pTranslator)
{
    if(pTranslator)
    {
        _pTranslator = pTranslator;
        //selectLang(_currLangId);
    }

    return 1;
}

bool  GetText::selectLang(int landId)
{
    bool  ret = false;
    if((landId != _currLangId) && (landId != TEXTLANG_INVALID) && (landId <= TEXTLANG_MAX))
    {
        _currLangId = landId;
        qApp->removeTranslator(_pTranslator);
        ret =_pTranslator->load(getlandStr(landId));

        qApp->installTranslator(_pTranslator);

        QString fileName =  QString("/opt/%2").arg("qt_config.ini");

        QFile file_(fileName);
        if(file_.exists())
        {
            QSettings *settings = new  QSettings(fileName, QSettings::IniFormat);
            settings->setValue("user_Info/languageId", QString::number(landId));
            settings->sync();
            delete  settings;
        }
    }

    return ret;
}

QString  GetText::getlandStr(int id)
{
    QString str;
    switch (id) {
    case TEXTLANG_CN:
        str = ":/text_cn.qm";
        break;
    case TEXTLANG_EN:
        str = ":/text_en.qm";
        break;
    default:
        break;
    }

    return str;
}

const char* GetText::getString(int id)
{
    QString  str_text;
    switch (id) {
    case TEXTLANG_CN:
        str_text = "Chinese";
        break;
    case TEXTLANG_EN:
        str_text = "English";
    default:
        break;
    }

    return str_text.toLocal8Bit().data();
}

QString  GetText::getIcon(int id)
{
    QString str_ico;
    switch (id) {
    case TEXTLANG_CN:
        str_ico = ":/image/Chinese.png";
        break;
    case TEXTLANG_EN:
        str_ico = ":/image/English.png";
        break;
    default:
        break;
    }

    return str_ico;
}

int  GetText::getCurrentLangId(void)
{
    QString fileName =  QString("/opt/%2").arg("qt_config.ini");
    QFile file_(fileName);
    if(file_.exists())
    {
        QSettings *config = new  QSettings(fileName, QSettings::IniFormat);
        _currLangId   = config->value("user_Info/languageId").toInt();

        if((_currLangId < TEXTLANG_MIN) || (_currLangId > TEXTLANG_MAX))
            _currLangId = TEXTLANG_CN;
        delete  config;
    }
    return _currLangId;
}

void  GetText::createInitFile(void)
{
    QString fileName =  QString("/opt/%2").arg("qt_config.ini");

    QFile file_(fileName);
    if(!file_.exists())
    {
        QSettings *settings = new  QSettings(fileName, QSettings::IniFormat);
        settings->setValue("user_Info/screenSaver_enable", "1");
        settings->setValue("user_Info/screenSaver_interval", "3");
        settings->setValue("user_Info/languageId", "1");
        settings->sync();
        delete  settings;
    }
}

int  GetText::detectLcdSize(void)
{
    FILE *stream;
    char  buf[100];

    memset(buf, 0, sizeof(buf));
    //执行fbset命令获取屏幕分辨率
    stream = popen("/usr/sbin/fbset |grep geometry", "r");
    fread(buf, 1, sizeof(buf), stream);
    pclose(stream);

    if(buf[0] != '\0')
    {
        if(sscanf(buf, "%*[^0-9]%d%*[^0-9]%d", &m_XSize, &m_YSize) == 2)
            return 0;
        else
            return -1;
    }
    else
    {
        return -1;
    }
}

void GetText::getLcdSize(int &XSize, int &YSize)
{
    XSize = m_XSize;
    YSize = m_YSize;
}

void GetText::screenSaverInit(void)
{
    //注册引脚
    gpio_export(SCREENSAVER_PIN);
    //设置方向
    gpio_direction(SCREENSAVER_PIN, GPIO_OUT);
    gpio_write(SCREENSAVER_PIN, 1);

    //注册蜂鸣器
    gpio_export(BUZZER_PIN);
    gpio_direction(BUZZER_PIN, GPIO_OUT);
}
