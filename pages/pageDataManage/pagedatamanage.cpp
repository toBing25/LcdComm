#include "pagedatamanage.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include "wgtmenubar.h"
#include "pageupsdata.h"
#include "alarmwidget.h"
#include "pagedistribution.h"
#include "historyData.h"
PageDataManage::PageDataManage(QWidget *parent)
    : QWidget(parent)
{
    setupUi();

    m_menuBar->setCurrIndex(0);
    connect(m_menuBar, SIGNAL(clicked(int)), m_stkWidget, SLOT(setCurrentIndex(int)));
}

PageDataManage::~PageDataManage()
{

}

void PageDataManage::setupUi(void)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setGeometry(0, 0, this->width(), this->height());

    //QHBoxLayout *mainLayout = new QHBoxLayout;
    //mainLayout->setSpacing(6);
    //mainLayout->setObjectName(QStringLiteral("mainLayout"));
    //mainLayout->setContentsMargins(6, 6, 6, 6);

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

    //mainLayout->addWidget(m_stkWidget);
    //mainLayout->addWidget(m_wgtMenu);
    //this->setLayout(mainLayout);

    m_menuBar = new WgtMenuBar(/*WgtMenuBar::VBoxLayout,*/ m_wgtMenu);
    m_menuBar->setLayouts(WgtMenuBar::VBoxLayout);

    m_menuBar->addItem(/*tr("历史数据")*/langN.show[langEnum::strHistData], "histData", langEnum::strHistData);
    m_wgtHistData = new WgtHistoryData(this);
    m_stkWidget->addWidget(m_wgtHistData);

    m_menuBar->setAlignment(Qt::AlignLeft);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(m_menuBar);
    horizontalLayout->setMargin(0);
    m_wgtMenu->setLayout(horizontalLayout);
}

bool PageDataManage::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        return true;
    }

    return QWidget::event(ev);
}

void PageDataManage::showEvent(QShowEvent */*event*/)
{
    m_stkWidget->move(6, 6);
    m_wgtMenu->move(m_stkWidget->width()- 5, 6);
}



















