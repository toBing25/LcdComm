#include "formmain.h"
#include "gettext.h"
#include "myapplication.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>


int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    MyApplication a(argc, argv);

    QTranslator translator;
    GetText::init(&translator);
    GetText::selectLang(GetText::getCurrentLangId());
    a.installTranslator(&translator);

    //创建配置文件
    GetText::createInitFile();

    //获取屏幕分辨率
    GetText::detectLcdSize();

    //设置屏保初始化
    GetText::screenSaverInit();

    FormMain w;
    //w.show();

    return a.exec();
}
