#include "pagealarm.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QScrollBar>
#include <QHeaderView>
#include <QDebug>
#include <QEvent>
#include "wgtmenubar.h"
#include "formmain.h"
#include "buzzer.h"
#include "wgthistory.h"
#include "tableWidgetDelegate.h"

#define COL_NUM   4
#define ROW_COUNT 10


PageAlarm::PageAlarm(QWidget *parent)
    : QWidget(parent)
{
    windowInit();
    pageInit();
    //setTabWidgetStyle();

    m_tabWidget->setCurrentIndex(0);
    m_menuBar->setCurrIndex(0);
    connect(m_menuBar, SIGNAL(clicked(int)), m_stkWidget, SLOT(setCurrentIndex(int)));
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &PageAlarm::currentIndexChange);

    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeMtrAlmItemList,this,&PageAlarm::getAlarmList);
}

PageAlarm::~PageAlarm()
{

}

void PageAlarm::windowInit(void)
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
    m_menuBar->addItem(/*QObject::tr("当前告警")*/langN.show[langEnum::strCurAlm], "curAlm", langEnum::strCurAlm);
    m_wgtCurrentAlarm = new QWidget;
    m_stkWidget->addWidget(m_wgtCurrentAlarm);

    m_menuBar->addItem(/*QObject::tr("历史记录")*/langN.show[langEnum::strHistAlm], "histAlm", langEnum::strHistAlm);
    m_wgtHistory = new WgtHistory(this);
    m_stkWidget->addWidget(m_wgtHistory);

    m_menuBar->setAlignment(Qt::AlignLeft);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(m_menuBar);
    horizontalLayout->setMargin(0);
    m_wgtMenu->setLayout(horizontalLayout);
}

void PageAlarm::pageInit(void)
{
    QVBoxLayout *verticalLayout = new QVBoxLayout(m_wgtCurrentAlarm);
    verticalLayout->setSpacing(0);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(2, 5, 18, 2);

    m_tabWidget = new QTabWidget(m_wgtCurrentAlarm);
    m_tabWidget->setObjectName(QStringLiteral("tabWidget"));
    m_tabWidget->setFocusPolicy(Qt::NoFocus);
    m_tabWidget->setStyleSheet("QTabWidget::pane{"
                               "top:8px;"
                               "border:0px solid #00AEFF;"
                               "}"
                               "QTabBar::tab{"
                               "color:#333333;"
                               "background:transparent;"
                               "font-size:16px;"
                               "width:99px;"
                               "height:29px;"
                               "}"
                               "QTabBar::tab:selected{"
                               "color: #FFFFFF;"
                               "border-image: url(:/image/tab_select.png);"
                               "font-size:16px;"
                               "margin-top: 0; margin-left: 0; margin-right: 6;"
                               "}"
                               "QTabBar::tab:!selected{"
                               "color: #00CCFF;"
                               "border-image: url(:/image/tab_unselect.png);"
                               "font-size:16px;"
                               "margin-top: 0; margin-left: 0; margin-right: 6;"
                               "}"
                               "#detailWgt,#appealReasonWgt{border:none;}");


    //所有告警
    m_wgtAllAlarm = new QWidget();
    m_wgtAllAlarm->setObjectName(QStringLiteral("m_wgtAllAlarm"));
    m_tabWidget->addTab(m_wgtAllAlarm, /*QObject::tr("所有告警")*/langN.show[langEnum::strAllAlm]);

    QVBoxLayout *verticalLayout_1 = new QVBoxLayout;
    verticalLayout_1->setSpacing(6);
    verticalLayout_1->setObjectName(QStringLiteral("verticalLayout_1"));
    verticalLayout_1->setContentsMargins(4, 8, 4, 0);


    QVBoxLayout *verborder = new QVBoxLayout;
    verborder->setSpacing(0);
    verborder->setObjectName(QStringLiteral("verborder"));
    verborder->setContentsMargins(1, 4, 0, 0);

    QVBoxLayout *v = new QVBoxLayout;
    v->setSpacing(0);
    v->setObjectName(QStringLiteral("v"));
    v->setContentsMargins(0, 0, 0, 0);

    QWidget  *allAlarmTitle = new QWidget(m_wgtCurrentAlarm);
    allAlarmTitle->setMinimumHeight(35);
    allAlarmTitle->setMaximumHeight(35);
    allAlarmTitle->setObjectName(QStringLiteral("allAlarmTitle"));
    allAlarmTitle->setStyleSheet("QWidget#allAlarmTitle{color: rgb(255, 255, 255);"
                                 "border-image: url(:/image/border_up.png);"
                                 "}");

    QWidget  *allAlarmbody = new QWidget(m_wgtCurrentAlarm);
    allAlarmbody->setObjectName(QStringLiteral("allAlarmbody"));
    allAlarmbody->setStyleSheet("QWidget#allAlarmbody{color: rgb(255, 255, 255);"
                                "border-bottom:1px solid #01ADDE;"
                                "border-right:1px solid #01ADDE;"
                                "border-left:1px solid #01ADDE;"
                                "border-radius:opx;font:13pt;"
                                "}");

    m_tabAllAlarm = new QTableWidget(m_wgtCurrentAlarm);
    m_tabAllAlarm->setObjectName(QStringLiteral("tableWidget"));
    m_tabAllAlarm->setColumnCount(COL_NUM);
    m_tabAllAlarm->setRowCount(ROW_COUNT);
    m_tabAllAlarm->setShowGrid(false);
    m_tabAllAlarm->setItemDelegate(new tableWidgetDelegate(m_tabAllAlarm));
    m_tabAllAlarm->horizontalHeader()->setVisible(false);
    m_tabAllAlarm->horizontalHeader()->setSectionsClickable(false);
    m_tabAllAlarm->horizontalHeader()->setStretchLastSection(true);
    m_tabAllAlarm->horizontalHeader()->setHighlightSections(false);
    m_tabAllAlarm->verticalHeader()->setVisible(false);
    m_tabAllAlarm->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabAllAlarm->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabAllAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabAllAlarm->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabAllAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabAllAlarm->setFocusPolicy(Qt::NoFocus);
    m_tabAllAlarm->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(44, 82, 153);color: rgb(188, 207, 245);font:12pt;}");
    m_tabAllAlarm->setStyleSheet("QTableWidget{gridline-color: #01ADDE;border:0px solid #01ADDE;}");

    QPalette pll = m_tabAllAlarm->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tabAllAlarm->setPalette(pll);

    v->addWidget(m_tabAllAlarm);
    allAlarmbody->setLayout(v);
    verborder->addWidget(allAlarmTitle);
    verborder->addWidget(allAlarmbody);

    QHBoxLayout *horizontalLayout_1 = new QHBoxLayout;
    horizontalLayout_1->setSpacing(20);
    horizontalLayout_1->setObjectName(QStringLiteral("horizontalLayout_1"));
    horizontalLayout_1->setContentsMargins(0, 5, 10, 10);

    m_AlarmPageUp   = new QPushButton(m_wgtAllAlarm);
    m_AlarmPageUp->setObjectName("m_AlarmPageUp");
    m_AlarmPageUp->setMinimumSize(QSize(50, 30));
    m_AlarmPageUp->setMaximumSize(QSize(50, 30));
    //m_AlarmPageUp->setIcon(QIcon(":/image/PageUp.png"));
    //m_AlarmPageUp->setIconSize(QSize(70, 30));
    m_AlarmPageUp->setAutoRepeat(true);
    m_AlarmPageUp->setFocusPolicy(Qt::NoFocus);
    m_AlarmPageUp->setStyleSheet(
                                 "QPushButton:!pressed{"
                                 "border-image: url(:/image/left_normal.png);"
                                 "color: #FFFFFF;"
                                 "border-radius:0px;font:12pt;"
                                 "}"
                                 "QPushButton:pressed{"
                                 "border-image: url(:/image/left_check.png);"
                                 "color: #FFFFFF;"
                                 "border:0px solid #68CBF2;"
                                 "border-radius:0px;font:12pt;"
                                 "}"
                                 );

    connect(m_AlarmPageUp, &QPushButton::clicked, this, [=](){
        ShortBeep();
        m_tabAllAlarm->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
    });

    m_AlarmPageDown = new QPushButton(m_wgtAllAlarm);
    m_AlarmPageDown->setObjectName("m_AlarmPageDown");
    m_AlarmPageDown->setMinimumSize(QSize(50, 30));
    m_AlarmPageDown->setMaximumSize(QSize(50, 30));
    //m_AlarmPageDown->setIcon(QIcon(":/image/PageDn.png"));
    //m_AlarmPageDown->setIconSize(QSize(70, 30));
    m_AlarmPageDown->setAutoRepeat(true);
    m_AlarmPageDown->setFocusPolicy(Qt::NoFocus);
    m_AlarmPageDown->setStyleSheet(
                                    "QPushButton:!pressed{"
                                    "border-image: url(:/image/right_normal.png);"
                                    "color: #FFFFFF;"
                                    "border-radius:0px;font:12pt;"
                                    "}"
                                    "QPushButton:pressed{"
                                    "border-image: url(:/image/right_check.png);"
                                    "color: #FFFFFF;"
                                    "border:0px solid #68CBF2;"
                                    "border-radius:0px;font:12pt;"
                                    "}"
                                    );

    connect(m_AlarmPageDown, &QPushButton::clicked, this, [=](){
        ShortBeep();
        m_tabAllAlarm->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
    });

    horizontalLayout_1->addStretch();
    horizontalLayout_1->addWidget(m_AlarmPageUp);
    horizontalLayout_1->addWidget(m_AlarmPageDown);

    //verticalLayout_1->addWidget(m_tabAllAlarm);
    verticalLayout_1->addLayout(verborder);
    verticalLayout_1->addLayout(horizontalLayout_1);
    m_wgtAllAlarm->setLayout(verticalLayout_1);



    //一般告警
    m_wgtGeneralAlarm = new QWidget();
    m_wgtGeneralAlarm->setObjectName(QStringLiteral("m_wgtGeneralAlarm"));
    m_tabWidget->addTab(m_wgtGeneralAlarm, /*QObject::tr("一般告警")*/langN.show[langEnum::strGenAlm]);

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout;
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(4, 8, 4, 0);

    QVBoxLayout *verborder2 = new QVBoxLayout;
    verborder2->setSpacing(0);
    verborder2->setObjectName(QStringLiteral("verborder2"));
    verborder2->setContentsMargins(1, 4, 0, 0);

    QVBoxLayout *v2 = new QVBoxLayout;
    v2->setSpacing(0);
    v2->setObjectName(QStringLiteral("v2"));
    v2->setContentsMargins(0, 0, 0, 0);

    QWidget  *allAlarmTitle2 = new QWidget(m_wgtGeneralAlarm);
    allAlarmTitle2->setMinimumHeight(35);
    allAlarmTitle2->setMaximumHeight(35);
    allAlarmTitle2->setObjectName(QStringLiteral("allAlarmTitle2"));
    allAlarmTitle2->setStyleSheet("QWidget#allAlarmTitle2{color: rgb(255, 255, 255);"
                                 "border-image: url(:/image/border_up.png);"
                                 "}");

    QWidget  *allAlarmbody2 = new QWidget(m_wgtGeneralAlarm);
    allAlarmbody2->setObjectName(QStringLiteral("allAlarmbody2"));
    allAlarmbody2->setStyleSheet("QWidget#allAlarmbody2{color: rgb(255, 255, 255);"
                                 "border-bottom:1px solid #01ADDE;"
                                 "border-right:1px solid #01ADDE;"
                                 "border-left:1px solid #01ADDE;"
                                 "border-radius:opx;font:13pt;"
                                 "}");

    m_tabGeneralAlarm = new QTableWidget(m_wgtGeneralAlarm);
    m_tabGeneralAlarm->setObjectName(QStringLiteral("tableWidget"));
    m_tabGeneralAlarm->setColumnCount(COL_NUM);
    m_tabGeneralAlarm->setRowCount(ROW_COUNT);
    m_tabGeneralAlarm->setShowGrid(false);
    m_tabGeneralAlarm->setItemDelegate(new tableWidgetDelegate(m_tabGeneralAlarm));
    m_tabGeneralAlarm->horizontalHeader()->setVisible(false);
    m_tabGeneralAlarm->horizontalHeader()->setSectionsClickable(false);
    m_tabGeneralAlarm->horizontalHeader()->setStretchLastSection(true);
    m_tabGeneralAlarm->horizontalHeader()->setHighlightSections(false);
    m_tabGeneralAlarm->verticalHeader()->setVisible(false);
    m_tabGeneralAlarm->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabGeneralAlarm->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabGeneralAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabGeneralAlarm->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabGeneralAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabGeneralAlarm->setFocusPolicy(Qt::NoFocus);
    m_tabGeneralAlarm->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(44, 82, 153);color: rgb(188, 207, 245);font:12pt;}");
    m_tabGeneralAlarm->setStyleSheet("QTableWidget{gridline-color: #01ADDE;border:0px solid #01ADDE;}");

    QPalette pll2 = m_tabGeneralAlarm->palette();
    pll2.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tabGeneralAlarm->setPalette(pll2);
    //m_tabGeneralAlarm->setHorizontalHeaderLabels(headerLabels);

    v2->addWidget(m_tabGeneralAlarm);
    allAlarmbody2->setLayout(v2);
    verborder2->addWidget(allAlarmTitle2);
    verborder2->addWidget(allAlarmbody2);

    QHBoxLayout *horizontalLayout_2 = new QHBoxLayout;
    horizontalLayout_2->setSpacing(20);
    horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
    horizontalLayout_2->setContentsMargins(0, 5, 10, 10);

    m_GenAlarmPageUp   = new QPushButton(m_wgtGeneralAlarm);
    m_GenAlarmPageUp->setObjectName("m_GenAlarmPageUp");
    m_GenAlarmPageUp->setMinimumSize(QSize(50, 30));
    m_GenAlarmPageUp->setMaximumSize(QSize(50, 30));
    //m_GenAlarmPageUp->setIcon(QIcon(":/image/PageUp.png"));
    //m_GenAlarmPageUp->setIconSize(QSize(70, 30));
    m_GenAlarmPageUp->setAutoRepeat(true);
    m_GenAlarmPageUp->setFocusPolicy(Qt::NoFocus);
    m_GenAlarmPageUp->setStyleSheet(
                                    "QPushButton:!pressed{"
                                    "border-image: url(:/image/left_normal.png);"
                                    "color: #FFFFFF;"
                                    "border-radius:0px;font:12pt;"
                                    "}"
                                    "QPushButton:pressed{"
                                    "border-image: url(:/image/left_check.png);"
                                    "color: #FFFFFF;"
                                    "border:0px solid #68CBF2;"
                                    "border-radius:0px;font:12pt;"
                                    "}"
                                    );

    connect(m_GenAlarmPageUp, &QPushButton::clicked, this, [=](){
        ShortBeep();
        m_tabGeneralAlarm->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
    });

    m_GenAlarmPageDown = new QPushButton(m_wgtGeneralAlarm);
    m_GenAlarmPageDown->setObjectName("m_GenAlarmPageDown");
    m_GenAlarmPageDown->setMinimumSize(QSize(50, 30));
    m_GenAlarmPageDown->setMaximumSize(QSize(50, 30));
    //m_GenAlarmPageDown->setIcon(QIcon(":/image/PageDn.png"));
    //m_GenAlarmPageDown->setIconSize(QSize(70, 30));
    m_GenAlarmPageDown->setAutoRepeat(true);
    m_GenAlarmPageDown->setFocusPolicy(Qt::NoFocus);
    m_GenAlarmPageDown->setStyleSheet(
                                      "QPushButton:!pressed{"
                                      "border-image: url(:/image/right_normal.png);"
                                      "color: #FFFFFF;"
                                      "border-radius:0px;font:12pt;"
                                      "}"
                                      "QPushButton:pressed{"
                                      "border-image: url(:/image/right_check.png);"
                                      "color: #FFFFFF;"
                                      "border:0px solid #68CBF2;"
                                      "border-radius:0px;font:12pt;"
                                      "}"
                                      );

    connect(m_GenAlarmPageDown, &QPushButton::clicked, this, [=](){
        ShortBeep();
        m_tabGeneralAlarm->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
    });

    horizontalLayout_2->addStretch();
    horizontalLayout_2->addWidget(m_GenAlarmPageUp);
    horizontalLayout_2->addWidget(m_GenAlarmPageDown);

    //verticalLayout_2->addWidget(m_tabGeneralAlarm);
    verticalLayout_2->addLayout(verborder2);
    verticalLayout_2->addLayout(horizontalLayout_2);
    m_wgtGeneralAlarm->setLayout(verticalLayout_2);


    //严重告警
    m_wgtCriticalAlarm = new QWidget();
    m_wgtCriticalAlarm->setObjectName(QStringLiteral("m_wgtCriticalAlarm"));
    m_tabWidget->addTab(m_wgtCriticalAlarm, /*QObject::tr("严重告警")*/langN.show[langEnum::strUrgAlm]);

    QVBoxLayout *verticalLayout_3 = new QVBoxLayout;
    verticalLayout_3->setSpacing(6);
    verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
    verticalLayout_3->setContentsMargins(4, 8, 4, 0);

    QVBoxLayout *verborder3 = new QVBoxLayout;
    verborder3->setSpacing(0);
    verborder3->setObjectName(QStringLiteral("verborder3"));
    verborder3->setContentsMargins(1, 4, 0, 0);

    QVBoxLayout *v3 = new QVBoxLayout;
    v3->setSpacing(0);
    v3->setObjectName(QStringLiteral("v3"));
    v3->setContentsMargins(0, 0, 0, 0);

    QWidget  *allAlarmTitle3 = new QWidget(m_wgtCriticalAlarm);
    allAlarmTitle3->setMinimumHeight(35);
    allAlarmTitle3->setMaximumHeight(35);
    allAlarmTitle3->setObjectName(QStringLiteral("allAlarmTitle3"));
    allAlarmTitle3->setStyleSheet("QWidget#allAlarmTitle3{color: rgb(255, 255, 255);"
                                 "border-image: url(:/image/border_up.png);"
                                 "}");

    QWidget  *allAlarmbody3 = new QWidget(m_wgtCriticalAlarm);
    allAlarmbody3->setObjectName(QStringLiteral("allAlarmbody3"));
    allAlarmbody3->setStyleSheet("QWidget#allAlarmbody3{color: rgb(255, 255, 255);"
                                 "border-bottom:1px solid #01ADDE;"
                                 "border-right:1px solid #01ADDE;"
                                 "border-left:1px solid #01ADDE;"
                                 "border-radius:opx;font:13pt;"
                                 "}");

    m_tabCriticalAlarm = new QTableWidget(m_wgtCriticalAlarm);
    m_tabCriticalAlarm->setObjectName(QStringLiteral("tableWidget"));
    m_tabCriticalAlarm->setColumnCount(COL_NUM);
    m_tabCriticalAlarm->setRowCount(ROW_COUNT);
    m_tabCriticalAlarm->setShowGrid(false);
    m_tabCriticalAlarm->setItemDelegate(new tableWidgetDelegate(m_tabCriticalAlarm));
    m_tabCriticalAlarm->horizontalHeader()->setVisible(false);
    m_tabCriticalAlarm->horizontalHeader()->setSectionsClickable(false);
    m_tabCriticalAlarm->horizontalHeader()->setStretchLastSection(true);
    m_tabCriticalAlarm->horizontalHeader()->setHighlightSections(false);
    m_tabCriticalAlarm->verticalHeader()->setVisible(false);
    m_tabCriticalAlarm->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabCriticalAlarm->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabCriticalAlarm->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabCriticalAlarm->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabCriticalAlarm->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabCriticalAlarm->setFocusPolicy(Qt::NoFocus);
    m_tabCriticalAlarm->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(44, 82, 153);color: rgb(188, 207, 245);font:12pt;}");
    m_tabCriticalAlarm->setStyleSheet("QTableWidget{gridline-color: #01ADDE;border:0px solid #01ADDE;}");

    QPalette pll3 = m_tabCriticalAlarm->palette();
    pll3.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tabCriticalAlarm->setPalette(pll3);
    //m_tabCriticalAlarm->setHorizontalHeaderLabels(headerLabels);

    v3->addWidget(m_tabCriticalAlarm);
    allAlarmbody3->setLayout(v3);
    verborder3->addWidget(allAlarmTitle3);
    verborder3->addWidget(allAlarmbody3);

    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout;
    horizontalLayout_3->setSpacing(20);
    horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
    horizontalLayout_3->setContentsMargins(0, 5, 10, 10);

    m_CriAlarmPageUp   = new QPushButton(m_wgtCriticalAlarm);
    m_CriAlarmPageUp->setObjectName("m_CriAlarmPageUp");
    m_CriAlarmPageUp->setMinimumSize(QSize(50, 30));
    m_CriAlarmPageUp->setMaximumSize(QSize(50, 30));
    //m_CriAlarmPageUp->setIcon(QIcon(":/image/PageUp.png"));
    //m_CriAlarmPageUp->setIconSize(QSize(70, 30));
    m_CriAlarmPageUp->setAutoRepeat(true);
    m_CriAlarmPageUp->setFocusPolicy(Qt::NoFocus);
    m_CriAlarmPageUp->setStyleSheet(
                                    "QPushButton:!pressed{"
                                    "border-image: url(:/image/left_normal.png);"
                                    "color: #FFFFFF;"
                                    "border-radius:0px;font:12pt;"
                                    "}"
                                    "QPushButton:pressed{"
                                    "border-image: url(:/image/left_check.png);"
                                    "color: #FFFFFF;"
                                    "border:0px solid #68CBF2;"
                                    "border-radius:0px;font:12pt;"
                                    "}"
                                    );

    connect(m_CriAlarmPageUp, &QPushButton::clicked, this, [=](){
        ShortBeep();
        m_tabCriticalAlarm->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
    });

    m_CriAlarmPageDown = new QPushButton(m_wgtCriticalAlarm);
    m_CriAlarmPageDown->setObjectName("m_CriAlarmPageDown");
    m_CriAlarmPageDown->setMinimumSize(QSize(50, 30));
    m_CriAlarmPageDown->setMaximumSize(QSize(50, 30));
    //m_CriAlarmPageDown->setIcon(QIcon(":/image/PageDn.png"));
    //m_CriAlarmPageDown->setIconSize(QSize(70, 30));
    m_CriAlarmPageDown->setAutoRepeat(true);
    m_CriAlarmPageDown->setFocusPolicy(Qt::NoFocus);
    m_CriAlarmPageDown->setStyleSheet(
                                      "QPushButton:!pressed{"
                                      "border-image: url(:/image/right_normal.png);"
                                      "color: #FFFFFF;"
                                      "border-radius:0px;font:12pt;"
                                      "}"
                                      "QPushButton:pressed{"
                                      "border-image: url(:/image/right_check.png);"
                                      "color: #FFFFFF;"
                                      "border:0px solid #68CBF2;"
                                      "border-radius:0px;font:12pt;"
                                      "}"
                                      );

    connect(m_CriAlarmPageDown, &QPushButton::clicked, this, [=](){
        ShortBeep();
        m_tabCriticalAlarm->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
    });

    horizontalLayout_3->addStretch();
    horizontalLayout_3->addWidget(m_CriAlarmPageUp);
    horizontalLayout_3->addWidget(m_CriAlarmPageDown);

    //verticalLayout_3->addWidget(m_tabCriticalAlarm);
    verticalLayout_3->addLayout(verborder3);
    verticalLayout_3->addLayout(horizontalLayout_3);
    m_wgtCriticalAlarm->setLayout(verticalLayout_3);

    verticalLayout->addWidget(m_tabWidget);
    setHorizontalHeaderLabels();
}

void PageAlarm::retranslateUi(void)
{
    m_tabWidget->addTab(m_wgtAllAlarm, langN.show[langEnum::strAllAlm]);
    m_tabWidget->addTab(m_wgtGeneralAlarm, langN.show[langEnum::strGenAlm]);
    m_tabWidget->addTab(m_wgtCriticalAlarm, langN.show[langEnum::strUrgAlm]);

    setHorizontalHeaderLabels();
}

void PageAlarm::changeEvent(QEvent* event)
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

bool PageAlarm::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        return true;
    }

    return QWidget::event(ev);
}

void PageAlarm::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    m_stkWidget->move(6, 6);
    m_wgtMenu->move(m_stkWidget->width()- 5, 6);

    int w, h, c_w, r_h;
    w = m_tabAllAlarm->width();
    h = m_tabAllAlarm->height();

    c_w = (w - m_tabAllAlarm->verticalScrollBar()->width() - 60)/ (COL_NUM - 1);
    r_h = (h - m_tabAllAlarm->horizontalHeader()->height() +5)/ ROW_COUNT;
    for(int i  = 0; i < ROW_COUNT; i++)
    {
        m_tabAllAlarm->setRowHeight(i, r_h);
        m_tabGeneralAlarm->setRowHeight(i, r_h);
        m_tabCriticalAlarm->setRowHeight(i, r_h);
    }

    for(int i = 1; i < (COL_NUM - 1); i++)
    {
        m_tabAllAlarm->setColumnWidth(i, c_w);
        m_tabGeneralAlarm->setColumnWidth(i, c_w);
        m_tabCriticalAlarm->setColumnWidth(i, c_w);
    }
}

void PageAlarm::setTabWidgetIndex(int index)
{
    ShortBeep();

    if(index < 0 || index > m_tabWidget->count())
        return;

    m_tabWidget->setCurrentIndex(index);
}

void PageAlarm::setMenuBarIndex(int index)
{
    m_menuBar->setCurrIndex(index);
}

void PageAlarm::currentIndexChange(int /*index*/)
{
    //setTabWidgetStyle();
    ShortBeep();
}

void PageAlarm::getAlarmList(QList<alarm_list>& list)
{
    //所有告警
    setAllAlarm(list);
    //一般告警
    setGeneralAlarm(list);
    //严重告警
    setCriticalAlarm(list);
}

void PageAlarm::setHorizontalHeaderLabels(void)
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    for (int i = 0; i < 3; i++) {
        int col = 0;
        QTableWidgetItem *pItem;
        pItem = new QTableWidgetItem(langN.show[langEnum::strId]);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);

        if(i == 0){
            m_tabAllAlarm->setItem(0, col, pItem);
        }else if (i == 1) {
            m_tabGeneralAlarm->setItem(0, col, pItem);
        }else {
            m_tabCriticalAlarm->setItem(0, col, pItem);
        }

        pItem = new QTableWidgetItem(langN.show[langEnum::strDevType]);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);

        if(i == 0){
            m_tabAllAlarm->setItem(0, col+1, pItem);
        }else if (i == 1) {
            m_tabGeneralAlarm->setItem(0, col+1, pItem);
        }else {
            m_tabCriticalAlarm->setItem(0, col+1, pItem);
        }

        pItem = new QTableWidgetItem(langN.show[langEnum::strEvent]);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);

        if(i == 0){
            m_tabAllAlarm->setItem(0, col+2, pItem);
        }else if (i == 1) {
            m_tabGeneralAlarm->setItem(0, col+2, pItem);
        }else {
            m_tabCriticalAlarm->setItem(0, col+2, pItem);
        }
        pItem = new QTableWidgetItem(langN.show[langEnum::strDatetime]);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);

        if(i == 0){
            m_tabAllAlarm->setItem(0, col+3, pItem);
        }else if (i == 1) {
            m_tabGeneralAlarm->setItem(0, col+3, pItem);
        }else {
            m_tabCriticalAlarm->setItem(0, col+3, pItem);
        }
    }
}

void PageAlarm::setAllAlarm(QList<alarm_list>& list)
{
    m_tabAllAlarm->clearContents();
    setHorizontalHeaderLabels();

    if(list.count() > ROW_COUNT)
        m_tabAllAlarm->setRowCount(list.count());

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    for(int i = 0, r = 1; i < list.count(); i++, r++)
    {
        QTableWidgetItem *pItem;

        //序号
        QString  ser_number = QString::number(i+1);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        m_tabAllAlarm->setItem(r, 0, pItem);

        //部件
        QString  parts = list.at(i).parts;
        pItem = new QTableWidgetItem(parts);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        m_tabAllAlarm->setItem(r, 1, pItem);

        //告警代码
        QString  alarm_code = list.at(i).alarm_code;
        pItem = new QTableWidgetItem(alarm_code);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        m_tabAllAlarm->setItem(r, 2, pItem);

        //日期
        QString  date = list.at(i).datetime;
        pItem = new QTableWidgetItem(date);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        m_tabAllAlarm->setItem(r, 3, pItem);
    }
}

void PageAlarm::setGeneralAlarm(QList<alarm_list> &list)
{
    m_tabGeneralAlarm->clearContents();
    setHorizontalHeaderLabels();

    if(list.count() > ROW_COUNT)
        m_tabGeneralAlarm->setRowCount(list.count());

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    int number = 0;
    for (int i = 0, r = 1; i < list.count(); i++, r++) {
        //筛选一般告警
        if(list.at(i).Alarmlevel != GeneralAlarm)
            continue;

        number++;
        QTableWidgetItem *pItem;

        //序号
        QString  ser_number = QString::number(number);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabGeneralAlarm->setItem(number, 0, pItem);

        //部件
        QString  parts = list.at(i).parts;
        pItem = new QTableWidgetItem(parts);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabGeneralAlarm->setItem(number, 1, pItem);

        //告警代码
        QString  alarm_code = list.at(i).alarm_code;
        pItem = new QTableWidgetItem(alarm_code);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabGeneralAlarm->setItem(number, 2, pItem);

        //日期
        QString  date = list.at(i).datetime;
        pItem = new QTableWidgetItem(date);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabGeneralAlarm->setItem(number, 3, pItem);
    }
}

void PageAlarm::setCriticalAlarm(QList<alarm_list> &list)
{
    m_tabCriticalAlarm->clearContents();
    setHorizontalHeaderLabels();

    if(list.count() > ROW_COUNT)
        m_tabCriticalAlarm->setRowCount(list.count());

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    int number = 0;
    for (int i = 0; i < list.count(); i++) {
        //严重告警
        if(list.at(i).Alarmlevel != CriticalAlarm)
            continue;

        number++;
        QTableWidgetItem *pItem;

        //序号
        QString  ser_number =  QString::number(number);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabCriticalAlarm->setItem(number/*-1*/, 0, pItem);

        //部件
        QString  parts = list.at(i).parts;
        pItem = new QTableWidgetItem(parts);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabCriticalAlarm->setItem(number/*-1*/, 1, pItem);

        //告警代码
        QString  alarm_code = list.at(i).alarm_code;
        pItem = new QTableWidgetItem(alarm_code);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabCriticalAlarm->setItem(number/*-1*/, 2, pItem);

        //日期
        QString  date = list.at(i).datetime;
        pItem = new QTableWidgetItem(date);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        m_tabCriticalAlarm->setItem(number/*-1*/, 3, pItem);
    }
}

void PageAlarm::setNormalAlarm(QList<alarm_list> &list)
{
#if 0
    //m_tabNormalAlarm->clearContents();
    if(list.count() > ROW_COUNT)
        //m_tabNormalAlarm->setRowCount(list.count());

        int number = 0;
    for (int i = 0; i < list.count(); i++) {
        //正常告警
        if(list.at(i).Alarmlevel != NormalAlarm)
            continue;

        number++;
        QTableWidgetItem *pItem;

        //序号
        QString  ser_number =  QString::number(number);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor(188, 207, 245));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        //m_tabNormalAlarm->setItem(number-1, 0, pItem);

        //部件
        QString  parts = list.at(i).parts;
        pItem = new QTableWidgetItem(parts);
        pItem->setForeground(QColor(188, 207, 245));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        //m_tabNormalAlarm->setItem(number-1, 1, pItem);

        //告警代码
        QString  alarm_code = list.at(i).alarm_code;
        pItem = new QTableWidgetItem(alarm_code);
        pItem->setForeground(QColor(188, 207, 245));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        //m_tabNormalAlarm->setItem(number-1, 2, pItem);

        //日期
        QString  date = list.at(i).datetime;
        pItem = new QTableWidgetItem(date);
        pItem->setForeground(QColor(188, 207, 245));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        //m_tabNormalAlarm->setItem(number-1, 3, pItem);
    }
#endif
}

void PageAlarm::setTabWidgetStyle(void)
{
    QString str;
    int index = m_tabWidget->currentIndex();

    if(index == 0)
        str = "url(:/image/tab_border.png)";
    else if (index == 1) {
        str = "url(:/image/tab_border1.png)";
    }
    else if (index == 2) {
        str = "url(:/image/tab_border2.png)";
    }

    m_tabWidget->setStyleSheet(QString("QTabWidget::pane{"
                                       "top:0px;"
                                       "border:0px solid #00AEFF;"
                                       "border-image: %1;"
                                       "}"
                                       "QTabBar::tab{"
                                       "color:#333333;"
                                       "background:transparent;"
                                       "font-size:16px;"
                                       "width:99px;"
                                       "height:29px;"
                                       "}"
                                       "QTabBar::tab:selected{"
                                       "color: #FFFFFF;"
                                       "border-image: url(:/image/tab_select.png);"
                                       "font-size:16px;"
                                       "margin-top: 0; margin-left: 0; margin-right: 6;"
                                       "}"
                                       "QTabBar::tab:!selected{"
                                       "color: #00CCFF;"
                                       "border-image: url(:/image/tab_unselect.png);"
                                       "font-size:16px;"
                                       "margin-top: 0; margin-left: 0; margin-right: 6;"
                                       "}"
                                       "#detailWgt,#appealReasonWgt{border:none;}").arg(str));
}












