#include "formtranslate.h"
#include "ui_formtranslate.h"
#include "upm.h"
#include "communicate.h"
#include "wbnotify_api.h"
#include "wgttitlebar_h.h"
#include "awnotify.h"
#include <QString>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gxtext_api.h"
#include "formdate.h"
#include "formlogo.h"

#define min(a,b) ((a)>(b)?(b):(a))
#define PAGE_MAX_LANG 14
QString text_space("                    ");
FormTranslate::FormTranslate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormTranslate)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    //title
    ui->widget_logo->setFlag(TITLEBAR_DISP_NO_TRAY | TITLEBAR_DISP_NO_TIME);
    ui->widget_time->setFlag(TITLEBAR_DISP_NO_TRAY | TITLEBAR_DISP_NO_TITLE);

    ui->LANGGroup->setId(ui->lan_1,1);
    ui->LANGGroup->setId(ui->lan_2,2);
    ui->LANGGroup->setId(ui->lan_3,3);
    ui->LANGGroup->setId(ui->lan_4,4);
    ui->LANGGroup->setId(ui->lan_5,5);
    ui->LANGGroup->setId(ui->lan_6,6);
    ui->LANGGroup->setId(ui->lan_7,7);
    ui->LANGGroup->setId(ui->lan_8,8);
    ui->LANGGroup->setId(ui->lan_9,9);
    ui->LANGGroup->setId(ui->lan_10,10);
    ui->LANGGroup->setId(ui->lan_11,11);
    ui->LANGGroup->setId(ui->lan_12,12);
    ui->LANGGroup->setId(ui->lan_13,13);
    ui->LANGGroup->setId(ui->lan_14,14);
    max_support_lang = GX_TEXTLANG_MAX;

    Current_page  = 0;
    tmp_page      = 1;
    Total_page    = max_support_lang/PAGE_MAX_LANG;
    if(max_support_lang%PAGE_MAX_LANG)
        Total_page++;

    QFont tfont;
    for(int i=1;i<=min(max_support_lang,PAGE_MAX_LANG);i++)
    {
        ui->LANGGroup->button(i)-> setIcon(QIcon(Upm::ITListDefValueGetIco(PARAID_LANGSEL,i))) ;
        ui->LANGGroup->button(i)->setText(QString(QObject::tr(TXT2STR(Upm::ITListDefValueGetText(PARAID_LANGSEL,i)))).insert(0,text_space));
        tfont = ui->LANGGroup->button(i)->font();
        if(Current_page == 0 && i == 1)
            tfont.setFamily("wenquanyi");
        else
            tfont.setFamily("Sun Batang");
        ui->LANGGroup->button(i)->setFont(tfont);
    }

    QString strPage = "P" + QString::number(Total_page, 10) + "-" + QString::number(tmp_page, 10);
    ui->lblPage->setText(strPage);

    connect(ui->btnQuit,SIGNAL(clicked(bool)),this,SLOT(onQuitClicked(bool)));
    connect(ui->LANGGroup,SIGNAL(buttonClicked(int)),this,SLOT(language_switch(int)));
    connect(ui->btn_prePage,SIGNAL(clicked(bool)),this,SLOT(onPageClicked(bool)));
    connect(ui->btn_nextPage,SIGNAL(clicked(bool)),this,SLOT(onPageClicked(bool)));
}

void FormTranslate::onPageClicked(bool checked)
{
    Q_UNUSED(checked);
    ShortBeep();

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button == ui->btn_prePage)
    {
        tmp_page--;
        if(tmp_page < 1)
            tmp_page = Total_page;
    }
    else if(button = ui->btn_nextPage)
    {
        tmp_page++;
        if(tmp_page > Total_page)
            tmp_page = 1;
    }

    setCurrentPage(tmp_page);
}

int FormTranslate::setCurrentPage(int page){
    Current_page=page-1;
    for(int i=1;i<=PAGE_MAX_LANG;i++)
    {
        ui->LANGGroup->button(i)->setText("");
        ui->LANGGroup->button(i)->setIcon(QIcon());
        ui->LANGGroup->button(i)->show();
    }

    QFont tfont;
    for(int i = 1; i <= min(max_support_lang-PAGE_MAX_LANG*Current_page,PAGE_MAX_LANG); i++)
    {
        ui->LANGGroup->button(i)-> setIcon(QIcon(Upm::ITListDefValueGetIco(PARAID_LANGSEL,i+PAGE_MAX_LANG*Current_page))) ;
        ui->LANGGroup->button(i)->setText(QString(QObject::tr(TXT2STR(Upm::ITListDefValueGetText(PARAID_LANGSEL,i+PAGE_MAX_LANG*Current_page)))).insert(0,text_space));
        tfont = ui->LANGGroup->button(i)->font();
        if(Current_page == 0 && i == 1)
            tfont.setFamily("wenquanyi");
        else
            tfont.setFamily("Sun Batang");
        ui->LANGGroup->button(i)->setFont(tfont);
    }

    QString strPage = "P" + QString::number(Total_page, 10) + "-" + QString::number(tmp_page, 10);
    ui->lblPage->setText(strPage);

    return 0;
}

void FormTranslate::language_switch(int id){
    ShortBeep();
    if( GxText::SelectLang(id+PAGE_MAX_LANG*Current_page) )
    {
        AW_Notify(WBNOTI_LANGUAGE_SWITCH_FAIL);
        return ;
    }

    QFont f ;
    f.setPointSize(14);
    if(Current_page == 0 && id == 1)
        f.setFamily("wenquanyi");
    else
        f.setFamily("Sun Batang");
    ((QApplication*)(QApplication::instance()))->setFont(f);

     WB_PutPara(WBPARA_APP_PATHNAME, PARAID_LANGSEL, id+PAGE_MAX_LANG*Current_page);
     this->done(1);
}

int FormTranslate::onQuitClicked(bool checked)
{
    Q_UNUSED(checked);
    ShortBeep();
    this->done(0);
    return 0;
}

FormTranslate::~FormTranslate()
{
    delete ui;
}
