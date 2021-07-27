#include "pagesystemsettings.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include "wgtmenubar.h"
#include "pagenetworkset.h"
#include "pagetranslate.h"
#include "sysfunction.h"
#include "accesscontrol.h"
#include "languagePublic.h"
#include <QDebug>


PageSystemSettings::PageSystemSettings(QWidget *parent)
    : QWidget(parent)
{
    setupUi();

    m_menuBar->setCurrIndex(0);
    connect(m_menuBar, SIGNAL(clicked(int)), m_stkWidget, SLOT(setCurrentIndex(int)));
}

PageSystemSettings::~PageSystemSettings()
{
}

void PageSystemSettings::setupUi(void)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setGeometry(0, 0, this->width(), this->height());

    m_wgtMenu = new QWidget(this);
    m_wgtMenu->setObjectName(QStringLiteral("wgtMenu"));
    m_wgtMenu->setMinimumSize(QSize(180, 535));
    m_wgtMenu->setMaximumSize(QSize(180, 535));
    m_wgtMenu->setStyleSheet(".QWidget#wgtMenu{"
                             "border-image: url(:/image/menu_right.png);"
                             "border-top:0px solid  #00ffff;}");

    m_stkWidget = new QStackedWidget(this);
    m_stkWidget->setObjectName(QStringLiteral("stackedWidget"));
    m_stkWidget->setMinimumHeight(535);
    m_stkWidget->setMaximumHeight(535);
    m_stkWidget->setMinimumWidth(1024 - 180);
    m_stkWidget->setMaximumWidth(1024 - 180);
    m_stkWidget->setStyleSheet(".QStackedWidget#stackedWidget{"
                             "border-image: url(:/image/menu_left.png);"
                             "border-top:0px solid  #00ffff;}");

    m_menuBar = new WgtMenuBar(/*WgtMenuBar::VBoxLayout,*/ m_wgtMenu);
    m_menuBar->setLayouts(WgtMenuBar::VBoxLayout);
    m_menuBar->addItem(/*QObject::tr("语言设置")*/langN.show[langEnum::strLangSet], "translate", langEnum::strLangSet);
    m_wgtTranslate = new PageTranslate;
    m_stkWidget->addWidget(m_wgtTranslate);

    m_menuBar->addItem(/*QObject::tr("网络设置")*/langN.show[langEnum::strNetSet], "network", langEnum::strNetSet);
    m_wgtNetWork = new PageNetWorkSet;
    m_stkWidget->addWidget(m_wgtNetWork);

    m_menuBar->addItem(/*QObject::tr("系统功能")*/langN.show[langEnum::strSysFun], "sysfunction", langEnum::strSysFun);
    m_wgtSysFun = new SysFunction;
    m_stkWidget->addWidget(m_wgtSysFun);

    m_menuBar->addItem(/*QObject::tr("门禁管理")*/langN.show[langEnum::strDoorManage], "AccessControl", langEnum::strDoorManage);
    m_wgtAccessControl = new AccessControl;
    m_stkWidget->addWidget(m_wgtAccessControl);

    m_menuBar->setAlignment(Qt::AlignLeft);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(m_menuBar);
    horizontalLayout->setMargin(0);
    m_wgtMenu->setLayout(horizontalLayout);
}

void PageSystemSettings::restore(void)
{
    ENABLE_VECTOR vec;
    for (int i = 0; i < m_menuBar->itemCount(); i++)
    {
        item_enable  item;
        item.itemId = i;
        item.enable = true;

        vec.append(item);
    }

    m_menuBar->setItemEnable(vec);
}

void PageSystemSettings::shield(void)
{
   QList<int> list;
   list << 1 <<3 <<5;

   ENABLE_VECTOR vec;
   QList<int>::iterator it;
   for (it = list.begin(); it != list.end(); it++)
   {
       item_enable  item;
       item.itemId = *it;
       item.enable = false;

       vec.append(item);
   }

   m_menuBar->setItemEnable(vec);
}

bool PageSystemSettings::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        //restore();
        //shield();
        return true;
    }

    return QWidget::event(ev);
}

void PageSystemSettings::showEvent(QShowEvent */*event*/)
{
    m_stkWidget->move(6, 6);
    m_wgtMenu->move(m_stkWidget->width()- 5, 6);
}















