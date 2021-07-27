#include "pagetranslate.h"
#include "ui_pagetranslate.h"
#include "gettext.h"
#include "buzzer.h"
#include <QIcon>
#include <QCheckBox>
#include <QDebug>


#define PAGE_MAX_LANG 14 //一页显示多少语言
#define min(a, b) ((a)>(b)?(b):(a))
QString text_space("                           ");

PageTranslate::PageTranslate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageTranslate)
{
    setupUi();

    /*m_supportLang  = TEXTLANG_MAX;
    m_currentPage  = 1;
    m_tmpPage      = 1;
    m_totalPage    = 1;//m_supportLang/PAGE_MAX_LANG;
    if(m_supportLang%TEXTLANG_MAX)
        m_totalPage++;

    for(int i = 1; i <= min(m_supportLang, PAGE_MAX_LANG); i++)
    {
        m_lanGgroup->button(i)->setIcon(QIcon(GetText::getIcon(i)));
        m_lanGgroup->button(i)->setText(QString(QObject::tr(GetText::getString(i)).insert(0, text_space)));
    }

    QString strPage = "P" + QString::number(m_totalPage, 10) + "-" + QString::number(m_tmpPage, 10);
    ui->lblPage->setText(strPage);

    connect(m_lanGgroup, static_cast<void (QButtonGroup:: *)(int)>(&QButtonGroup::buttonClicked),
            this,  &PageTranslate::language_switch);
    connect(ui->btn_prePage,  &QPushButton::clicked, this, &PageTranslate::onPageClicked);
    connect(ui->btn_nextPage, &QPushButton::clicked, this, &PageTranslate::onPageClicked);*/

    if(GetText::getCurrentLangId() == TEXTLANG_CN){
        ui->checkChinese->setChecked(true);
        ui->checkChinese->setEnabled(false);
    }else if (GetText::getCurrentLangId() == TEXTLANG_EN) {
        ui->checkEnglish->setChecked(true);
        ui->checkEnglish->setEnabled(false);
    }

    connect(ui->checkChinese, &QCheckBox::stateChanged, this, [=](int status){
        if(status == Qt::Checked){
            ui->checkChinese->setEnabled(false);
            language_switch(TEXTLANG_CN);

            ui->checkEnglish->setEnabled(true);
            ui->checkEnglish->setChecked(false);
        }
    });

    connect(ui->checkEnglish, &QCheckBox::stateChanged, this, [=](int status){
        if(status == Qt::Checked){
            ui->checkEnglish->setEnabled(false);
            language_switch(TEXTLANG_EN);

            ui->checkChinese->setEnabled(true);
            ui->checkChinese->setChecked(false);
        }
    });
}

PageTranslate::~PageTranslate()
{
    delete ui;
}

void PageTranslate::setupUi(void)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    /*m_lanGgroup = new QButtonGroup(this);
    m_lanGgroup->addButton(ui->lan_1);
    m_lanGgroup->addButton(ui->lan_2);
    m_lanGgroup->addButton(ui->lan_3);
    m_lanGgroup->addButton(ui->lan_4);
    m_lanGgroup->addButton(ui->lan_5);
    m_lanGgroup->addButton(ui->lan_6);
    m_lanGgroup->addButton(ui->lan_7);
    m_lanGgroup->addButton(ui->lan_8);
    m_lanGgroup->addButton(ui->lan_9);
    m_lanGgroup->addButton(ui->lan_10);
    m_lanGgroup->addButton(ui->lan_11);
    m_lanGgroup->addButton(ui->lan_12);
    m_lanGgroup->addButton(ui->lan_13);
    m_lanGgroup->addButton(ui->lan_14);

    m_lanGgroup->setId(ui->lan_1,1);
    m_lanGgroup->setId(ui->lan_2,2);
    m_lanGgroup->setId(ui->lan_3,3);
    m_lanGgroup->setId(ui->lan_4,4);
    m_lanGgroup->setId(ui->lan_5,5);
    m_lanGgroup->setId(ui->lan_6,6);
    m_lanGgroup->setId(ui->lan_7,7);
    m_lanGgroup->setId(ui->lan_8,8);
    m_lanGgroup->setId(ui->lan_9,9);
    m_lanGgroup->setId(ui->lan_10,10);
    m_lanGgroup->setId(ui->lan_11,11);
    m_lanGgroup->setId(ui->lan_12,12);
    m_lanGgroup->setId(ui->lan_13,13);
    m_lanGgroup->setId(ui->lan_14,14);

    ui->lan_3->setEnabled(false);
    ui->lan_4->setEnabled(false);
    ui->lan_5->setEnabled(false);
    ui->lan_6->setEnabled(false);
    ui->lan_7->setEnabled(false);
    ui->lan_8->setEnabled(false);
    ui->lan_9->setEnabled(false);
    ui->lan_10->setEnabled(false);
    ui->lan_11->setEnabled(false);
    ui->lan_12->setEnabled(false);
    ui->lan_13->setEnabled(false);
    ui->lan_14->setEnabled(false);*/
}

void PageTranslate::onPageClicked(bool checked)
{
    Q_UNUSED(checked);
    ShortBeep();

    /*QPushButton *button = qobject_cast<QPushButton*>(sender());
    if(button == ui->btn_prePage)
    {
        m_tmpPage--;
        if(m_tmpPage < 1)
            m_tmpPage = m_totalPage;
    }
    else if(button == ui->btn_nextPage)
    {
        m_tmpPage++;
        if(m_tmpPage > m_totalPage)
            m_tmpPage = 1;
    }

    setCurrentPage(m_tmpPage);*/
}

int PageTranslate::setCurrentPage(int page)
{
    /*m_currentPage = page-1;
    for(int i = 1; i <= PAGE_MAX_LANG; i++)
    {
        m_lanGgroup->button(i)->setText("");
        m_lanGgroup->button(i)->setIcon(QIcon());
        m_lanGgroup->button(i)->show();
    }

    for(int i = 1; i <= min(m_supportLang - PAGE_MAX_LANG*m_currentPage, PAGE_MAX_LANG); i++)
    {
        m_lanGgroup->button(i)-> setIcon(QIcon(GetText::getIcon(i + PAGE_MAX_LANG * m_currentPage)));
        m_lanGgroup->button(i)->setText(QString(QObject::tr(GetText::getString(i + PAGE_MAX_LANG*m_currentPage))).insert(0, text_space));
    }

    QString strPage = "P" + QString::number(m_totalPage, 10) + "-" + QString::number(m_tmpPage, 10);
    ui->lblPage->setText(strPage);*/

    return 0;
}

void PageTranslate::language_switch(int id)
{
    ShortBeep();

    GetText::selectLang(id);
}





