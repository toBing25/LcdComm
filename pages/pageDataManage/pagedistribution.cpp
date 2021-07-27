#include "pagedistribution.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QScrollBar>
#include <QHeaderView>
#include <QDebug>

#define TABEPDU_ROWCOUNT           11
#define TABEPDU_COLCOUNT           4

#define TABELEMODULES_ROWCOUNT     9
#define TABELEMODULES_COLCOUNT     5

#define TABELEINSTRUMENTS_ROWCOUNT 6
#define TABELEINSTRUMENTS_COLCOUNT 2

PageDistribution::PageDistribution(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

PageDistribution::~PageDistribution()
{

}

void PageDistribution::setupUi(void)
{
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setObjectName(QStringLiteral("tabWidget"));
    m_tabWidget->setStyleSheet("QTabWidget::pane{top:9px;border:none;}"
                               "QTabBar::tab{color:#333333;background:transparent;font-size:14px;width:120px;height:30px;}"
                               "QTabBar::tab:selected{color:#618BE5;background:transparent;font-size:14px;border-bottom:2px solid #618BE5;}"
                               "QTabBar::tab:!selected{color:rgb(255, 255, 255);background:transparent;font-size:14px;}"
                               "QTabBar::tab:hover{color:#618BE5;font-size:14px;}"
                               "#detailWgt,#appealReasonWgt{border:none;}");

    //ePDU
    m_wgtEPDU = new QWidget();
    m_wgtEPDU->setObjectName(QStringLiteral("m_wgtEPDU"));
    m_tabWidget->addTab(m_wgtEPDU, "ePDU");

    QVBoxLayout *verticalLayout_1 = new QVBoxLayout;
    verticalLayout_1->setSpacing(6);
    verticalLayout_1->setObjectName(QStringLiteral("verticalLayout_1"));
    verticalLayout_1->setContentsMargins(6, 3, 6, 6);

    m_wgtSplit1 = new QWidget(m_wgtEPDU);
    m_wgtSplit1->setObjectName(QStringLiteral("m_wgtSplit1"));
    m_wgtSplit1->setStyleSheet("border-bottom:1px solid rgb(255, 255, 255);");

    QHBoxLayout *horizontalLayout1 = new QHBoxLayout;
    horizontalLayout1->setSpacing(9);
    horizontalLayout1->setObjectName(QStringLiteral("horizontalLayout1"));
    horizontalLayout1->setContentsMargins(0, 0, 0, 9);

    m_lblePDEUNumber  = new QLabel(m_wgtSplit1);
    m_lblePDEUNumber->setObjectName(QStringLiteral("m_lblePDEUNumber"));
    m_lblePDEUNumber->setText(QObject::tr("当前ePDU编号："));
    m_lblePDEUNumber->setMinimumSize(QSize(120, 30));
    m_lblePDEUNumber->setMaximumSize(QSize(120, 30));
    m_lblePDEUNumber->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout1->addWidget(m_lblePDEUNumber);

    m_lblePDEUNumberVal  = new QLabel(m_wgtSplit1);
    m_lblePDEUNumberVal->setObjectName(QStringLiteral("m_lblePDEUNumberVal"));
    m_lblePDEUNumberVal->setText(QObject::tr("xxx"));
    m_lblePDEUNumberVal->setMinimumSize(QSize(110, 30));
    m_lblePDEUNumberVal->setMaximumSize(QSize(110, 30));
    m_lblePDEUNumberVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout1->addWidget(m_lblePDEUNumberVal);
    horizontalLayout1->addStretch();

    m_wgtSplit1->setLayout(horizontalLayout1);
    verticalLayout_1->addWidget(m_wgtSplit1);

    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setSpacing(3);
    horizontalLayout2->setObjectName(QStringLiteral("horizontalLayout2"));
    horizontalLayout2->setContentsMargins(0, 0, 0, 0);

    m_lblePDEUStat  = new QLabel(m_wgtEPDU);
    m_lblePDEUStat->setObjectName(QStringLiteral("m_lblePDEUStat"));
    m_lblePDEUStat->setText(QObject::tr("ePDU状态:"));
    m_lblePDEUStat->setMinimumSize(QSize(90, 30));
    m_lblePDEUStat->setMaximumSize(QSize(90, 30));
    m_lblePDEUStat->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout2->addStretch();
    horizontalLayout2->addWidget(m_lblePDEUStat);

    m_lblePDEUStatVal  = new QLabel(m_wgtEPDU);
    m_lblePDEUStatVal->setObjectName(QStringLiteral("m_lblePDEUStatVal"));
    m_lblePDEUStatVal->setText(QObject::tr("xxx"));
    m_lblePDEUStatVal->setMinimumSize(QSize(110, 30));
    m_lblePDEUStatVal->setMaximumSize(QSize(110, 30));
    m_lblePDEUStatVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout2->addWidget(m_lblePDEUStatVal);
    horizontalLayout2->addStretch();

    m_lblRatCurrent  = new QLabel(m_wgtEPDU);
    m_lblRatCurrent->setObjectName(QStringLiteral("m_lblRatCurrent"));
    m_lblRatCurrent->setText(QObject::tr("额定总电流(A):"));
    m_lblRatCurrent->setMinimumSize(QSize(110, 30));
    m_lblRatCurrent->setMaximumSize(QSize(110, 30));
    m_lblRatCurrent->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout2->addWidget(m_lblRatCurrent);

    m_lblRatCurrentVal  = new QLabel(m_wgtEPDU);
    m_lblRatCurrentVal->setObjectName(QStringLiteral("m_lblRatCurrentVal"));
    m_lblRatCurrentVal->setText(QObject::tr("xxx"));
    m_lblRatCurrentVal->setMinimumSize(QSize(110, 30));
    m_lblRatCurrentVal->setMaximumSize(QSize(110, 30));
    m_lblRatCurrentVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout2->addWidget(m_lblRatCurrentVal);
    horizontalLayout2->addStretch();

    m_lblOutCurrent  = new QLabel(m_wgtEPDU);
    m_lblOutCurrent->setObjectName(QStringLiteral("m_lblOutCurrent"));
    m_lblOutCurrent->setText(QObject::tr("输出总电流(A):"));
    m_lblOutCurrent->setMinimumSize(QSize(110, 30));
    m_lblOutCurrent->setMaximumSize(QSize(110, 30));
    m_lblOutCurrent->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout2->addWidget(m_lblOutCurrent);

    m_lblOutCurrentVal  = new QLabel(m_wgtEPDU);
    m_lblOutCurrentVal->setObjectName(QStringLiteral("m_lblOutCurrentVal"));
    m_lblOutCurrentVal->setText(QObject::tr("xxx"));
    m_lblOutCurrentVal->setMinimumSize(QSize(110, 30));
    m_lblOutCurrentVal->setMaximumSize(QSize(110, 30));
    m_lblOutCurrentVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout2->addWidget(m_lblOutCurrentVal);

    verticalLayout_1->addLayout(horizontalLayout2);

    m_tabEPDU = new QTableWidget(m_wgtEPDU);
    m_tabEPDU->setObjectName(QStringLiteral("m_tabEPDU"));
    m_tabEPDU->setColumnCount(TABEPDU_COLCOUNT);
    m_tabEPDU->setRowCount(TABEPDU_ROWCOUNT);
    m_tabEPDU->setShowGrid(true);
    m_tabEPDU->horizontalHeader()->setVisible(false);
    m_tabEPDU->horizontalHeader()->setSectionsClickable(false);
    m_tabEPDU->horizontalHeader()->setStretchLastSection(true);
    m_tabEPDU->horizontalHeader()->setHighlightSections(false);
    m_tabEPDU->verticalHeader()->setVisible(false);
    m_tabEPDU->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabEPDU->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabEPDU->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabEPDU->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabEPDU->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabEPDU->setFocusPolicy(Qt::NoFocus);

    int col = 1, row = 1;
    QTableWidgetItem *pItem;
    pItem = new QTableWidgetItem(QObject::tr("支路一"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(0, col, pItem);

    pItem = new QTableWidgetItem(QObject::tr("支路二"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(0, col+1, pItem);

    pItem = new QTableWidgetItem(QObject::tr("支路三"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(0, col+2, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电压(V)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电流(A)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+1, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("频率(HZ)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+2, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("功率因数"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+3, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("有功(KW)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+4, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("视在(KVA)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+5, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("耗电量(KW.h)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+6, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("负责百分比(%)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+7, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("排程关断前剩余时间(分钟)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+8, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("支路状态"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEPDU->setItem(row+9, 0, pItem);

    QPalette pll = m_tabEPDU->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tabEPDU->setPalette(pll);

    verticalLayout_1->addWidget(m_tabEPDU);
    m_wgtEPDU->setLayout(verticalLayout_1);


    //Electronic modules
    m_wgtEleModules = new QWidget();
    m_wgtEleModules->setObjectName(QStringLiteral("m_wgtEleModules"));
    m_tabWidget->addTab(m_wgtEleModules, QObject::tr("配电模块"));

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout;
    verticalLayout_2->setSpacing(6);
    verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(6, 3, 6, 6);

    m_wgtSplit2 = new QWidget(m_wgtEleModules);
    m_wgtSplit2->setObjectName(QStringLiteral("m_wgtSplit2"));
    m_wgtSplit2->setStyleSheet("border-bottom:1px solid rgb(255, 255, 255);");

    QHBoxLayout *horizontalLayout3 = new QHBoxLayout;
    horizontalLayout3->setSpacing(9);
    horizontalLayout3->setObjectName(QStringLiteral("horizontalLayout3"));
    horizontalLayout3->setContentsMargins(0, 0, 0, 9);

    m_lblEleModNumber  = new QLabel(m_wgtSplit2);
    m_lblEleModNumber->setObjectName(QStringLiteral("m_lblePDEUNumber"));
    m_lblEleModNumber->setText(QObject::tr("当前配电模块编号："));
    m_lblEleModNumber->setMinimumSize(QSize(140, 30));
    m_lblEleModNumber->setMaximumSize(QSize(140, 30));
    m_lblEleModNumber->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout3->addWidget(m_lblEleModNumber);

    m_lblEleModNumberVal  = new QLabel(m_wgtSplit2);
    m_lblEleModNumberVal->setObjectName(QStringLiteral("m_lblePDEUNumberVal"));
    m_lblEleModNumberVal->setText(QObject::tr("xxx"));
    m_lblEleModNumberVal->setMinimumSize(QSize(140, 30));
    m_lblEleModNumberVal->setMaximumSize(QSize(140, 30));
    m_lblEleModNumberVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout3->addWidget(m_lblEleModNumberVal);
    horizontalLayout3->addStretch();

    m_wgtSplit2->setLayout(horizontalLayout3);
    verticalLayout_2->addWidget(m_wgtSplit2);

    QHBoxLayout *horizontalLayout4 = new QHBoxLayout;
    horizontalLayout4->setSpacing(3);
    horizontalLayout4->setObjectName(QStringLiteral("horizontalLayout4"));
    horizontalLayout4->setContentsMargins(0, 0, 0, 0);

    m_lblEleModStat  = new QLabel(m_wgtEleModules);
    m_lblEleModStat->setObjectName(QStringLiteral("m_lblePDEUStat"));
    m_lblEleModStat->setText(QObject::tr("配电模块状态："));
    m_lblEleModStat->setMinimumSize(QSize(110, 30));
    m_lblEleModStat->setMaximumSize(QSize(110, 30));
    m_lblEleModStat->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout4->addStretch();
    horizontalLayout4->addWidget(m_lblEleModStat);

    m_lblEleModStatVal  = new QLabel(m_wgtEleModules);
    m_lblEleModStatVal->setObjectName(QStringLiteral("m_lblePDEUStatVal"));
    m_lblEleModStatVal->setText(QObject::tr("xxx"));
    m_lblEleModStatVal->setMinimumSize(QSize(110, 30));
    m_lblEleModStatVal->setMaximumSize(QSize(110, 30));
    m_lblEleModStatVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout4->addWidget(m_lblEleModStatVal);
    horizontalLayout4->addStretch();

    m_lblVoltage  = new QLabel(m_wgtEleModules);
    m_lblVoltage->setObjectName(QStringLiteral("m_lblRatCurrent"));
    m_lblVoltage->setText(QObject::tr("电压(V)："));
    m_lblVoltage->setMinimumSize(QSize(70, 30));
    m_lblVoltage->setMaximumSize(QSize(70, 30));
    m_lblVoltage->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout4->addWidget(m_lblVoltage);

    m_lblVoltageVal  = new QLabel(m_wgtEleModules);
    m_lblVoltageVal->setObjectName(QStringLiteral("m_lblRatCurrentVal"));
    m_lblVoltageVal->setText(QObject::tr("xxx"));
    m_lblVoltageVal->setMinimumSize(QSize(110, 30));
    m_lblVoltageVal->setMaximumSize(QSize(110, 30));
    m_lblVoltageVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout4->addWidget(m_lblVoltageVal);
    horizontalLayout4->addStretch();

    m_lblfrequency  = new QLabel(m_wgtEleModules);
    m_lblfrequency->setObjectName(QStringLiteral("m_lblOutCurrent"));
    m_lblfrequency->setText(QObject::tr("频率(HZ)："));
    m_lblfrequency->setMinimumSize(QSize(80, 30));
    m_lblfrequency->setMaximumSize(QSize(80, 30));
    m_lblfrequency->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout4->addWidget(m_lblfrequency);

    m_lblfrequencyVal  = new QLabel(m_wgtEleModules);
    m_lblfrequencyVal->setObjectName(QStringLiteral("m_lblOutCurrentVal"));
    m_lblfrequencyVal->setText(QObject::tr("xxx"));
    m_lblfrequencyVal->setMinimumSize(QSize(110, 30));
    m_lblfrequencyVal->setMaximumSize(QSize(110, 30));
    m_lblfrequencyVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout4->addWidget(m_lblfrequencyVal);
    verticalLayout_2->addLayout(horizontalLayout4);

    m_tabEleModules = new QTableWidget(m_wgtEleModules);
    m_tabEleModules->setObjectName(QStringLiteral("m_tabEleModules"));
    m_tabEleModules->setColumnCount(TABELEMODULES_COLCOUNT);
    m_tabEleModules->setRowCount(TABELEMODULES_ROWCOUNT);
    m_tabEleModules->setShowGrid(true);
    m_tabEleModules->horizontalHeader()->setVisible(false);
    m_tabEleModules->horizontalHeader()->setSectionsClickable(false);
    m_tabEleModules->horizontalHeader()->setStretchLastSection(true);
    m_tabEleModules->horizontalHeader()->setHighlightSections(false);
    m_tabEleModules->verticalHeader()->setVisible(false);
    m_tabEleModules->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabEleModules->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabEleModules->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabEleModules->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabEleModules->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabEleModules->setFocusPolicy(Qt::NoFocus);
    m_tabEleModules->setMinimumSize(840, 260);

    row = 0;
    pItem = new QTableWidgetItem(QObject::tr("支路"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电压(V)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+1, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电流(A)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+2, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("功率因数"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+3, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("视在(KVA)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+4, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("有功(KW)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+5, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("耗电量(KW.h)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+6, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("负载(%)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+7, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("状态"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleModules->setItem(row+8, 0, pItem);

    QPalette pll2 = m_tabEleModules->palette();
    pll2.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tabEleModules->setPalette(pll2);
    verticalLayout_2->addWidget(m_tabEleModules);
    verticalLayout_2->addStretch();

    //QSpacerItem *verticalSpacer = new QSpacerItem(5, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //verticalLayout_2->addSpacerItem(verticalSpacer);
    m_wgtEleModules->setLayout(verticalLayout_2);


    //Electronic instruments
    m_wgtEleInstruments = new QWidget();
    m_wgtEleInstruments->setObjectName(QStringLiteral("m_wgtEleInstruments"));
    m_tabWidget->addTab(m_wgtEleInstruments, QObject::tr("电量仪"));

    QVBoxLayout *verticalLayout_3 = new QVBoxLayout;
    verticalLayout_3->setSpacing(6);
    verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
    verticalLayout_3->setContentsMargins(6, 3, 6, 6);

    m_wgtSplit3 = new QWidget(m_wgtEleInstruments);
    m_wgtSplit3->setObjectName(QStringLiteral("m_wgtSplit3"));
    m_wgtSplit3->setStyleSheet("border-bottom:1px solid rgb(255, 255, 255);");

    QHBoxLayout *horizontalLayout5 = new QHBoxLayout;
    horizontalLayout5->setSpacing(3);
    horizontalLayout5->setObjectName(QStringLiteral("horizontalLayout5"));
    horizontalLayout5->setContentsMargins(0, 0, 0, 9);

    m_lblEleInstrNumber  = new QLabel(m_wgtSplit3);
    m_lblEleInstrNumber->setObjectName(QStringLiteral("m_lblEleInstrNumber"));
    m_lblEleInstrNumber->setText(QObject::tr("当前电量仪编号："));
    m_lblEleInstrNumber->setMinimumSize(QSize(130, 30));
    m_lblEleInstrNumber->setMaximumSize(QSize(130, 30));
    m_lblEleInstrNumber->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout5->addWidget(m_lblEleInstrNumber);

    m_lblEleInstrNumberVal  = new QLabel(m_wgtSplit3);
    m_lblEleInstrNumberVal->setObjectName(QStringLiteral("m_lblEleInstrNumberVal"));
    m_lblEleInstrNumberVal->setText(QObject::tr("xxx"));
    m_lblEleInstrNumberVal->setMinimumSize(QSize(110, 30));
    m_lblEleInstrNumberVal->setMaximumSize(QSize(110, 30));
    m_lblEleInstrNumberVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout5->addWidget(m_lblEleInstrNumberVal);
    horizontalLayout5->addStretch();

    m_wgtSplit3->setLayout(horizontalLayout5);
    verticalLayout_3->addWidget(m_wgtSplit3);

    QHBoxLayout *horizontalLayout6 = new QHBoxLayout;
    horizontalLayout6->setSpacing(3);
    horizontalLayout6->setObjectName(QStringLiteral("horizontalLayout6"));
    horizontalLayout6->setContentsMargins(0, 0, 0, 0);

    m_lblEleInstrStat  = new QLabel(m_wgtEleInstruments);
    m_lblEleInstrStat->setObjectName(QStringLiteral("m_lblEleInstrStat"));
    m_lblEleInstrStat->setText(QObject::tr("电量仪连接状态："));
    m_lblEleInstrStat->setMinimumSize(QSize(130, 30));
    m_lblEleInstrStat->setMaximumSize(QSize(130, 30));
    m_lblEleInstrStat->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    //horizontalLayout6->addStretch();
    horizontalLayout6->addWidget(m_lblEleInstrStat);

    m_lblEleInstrStatVal  = new QLabel(m_wgtEleInstruments);
    m_lblEleInstrStatVal->setObjectName(QStringLiteral("m_lblEleInstrStatVal"));
    m_lblEleInstrStatVal->setText(QObject::tr("xxx"));
    m_lblEleInstrStatVal->setMinimumSize(QSize(110, 30));
    m_lblEleInstrStatVal->setMaximumSize(QSize(110, 30));
    m_lblEleInstrStatVal->setStyleSheet("color:rgb(255, 255, 255);font-size:16px;");
    horizontalLayout6->addWidget(m_lblEleInstrStatVal);
    horizontalLayout6->addStretch();

    verticalLayout_3->addLayout(horizontalLayout6);

    m_tabEleInstruments = new QTableWidget(m_wgtEleInstruments);
    m_tabEleInstruments->setObjectName(QStringLiteral("m_tabEleInstruments"));
    m_tabEleInstruments->setColumnCount(TABELEINSTRUMENTS_COLCOUNT);
    m_tabEleInstruments->setRowCount(TABELEINSTRUMENTS_ROWCOUNT);
    m_tabEleInstruments->setShowGrid(true);
    m_tabEleInstruments->horizontalHeader()->setVisible(false);
    m_tabEleInstruments->horizontalHeader()->setSectionsClickable(false);
    m_tabEleInstruments->horizontalHeader()->setStretchLastSection(true);
    m_tabEleInstruments->horizontalHeader()->setHighlightSections(false);
    m_tabEleInstruments->verticalHeader()->setVisible(false);
    m_tabEleInstruments->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabEleInstruments->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabEleInstruments->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabEleInstruments->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabEleInstruments->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabEleInstruments->setFocusPolicy(Qt::NoFocus);
    m_tabEleInstruments->setMinimumSize(840, 200);

    pItem = new QTableWidgetItem(QObject::tr("电压(V)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleInstruments->setItem(row, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电流(A)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleInstruments->setItem(row+1, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("功率因数"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleInstruments->setItem(row+2, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("有功(KW)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleInstruments->setItem(row+3, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("频率(HZ)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleInstruments->setItem(row+4, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("Σ有功耗电量(KW.h)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabEleInstruments->setItem(row+5, 0, pItem);

    QPalette pll3 = m_tabEleInstruments->palette();
    pll3.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tabEleInstruments->setPalette(pll3);

    verticalLayout_3->addWidget(m_tabEleInstruments);
    verticalLayout_3->addStretch();
    m_wgtEleInstruments->setLayout(verticalLayout_3);

    verticalLayout->addWidget(m_tabWidget);
}

void PageDistribution::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    int w, h, c_w, r_h;
    w = m_tabEPDU->width();
    h = m_tabEPDU->height();

    c_w = (w - 200)  /(TABEPDU_COLCOUNT - 1);
    r_h = (h + 1) / TABEPDU_ROWCOUNT;
    m_tabEPDU->setColumnWidth(0, 200);
    for(int i  = 0; i < TABEPDU_ROWCOUNT; i++)
    {
        m_tabEPDU->setRowHeight(i, r_h);
    }

    for(int i = 1; i < (TABEPDU_COLCOUNT - 1); i++)
    {
        m_tabEPDU->setColumnWidth(i, c_w);
    }


    w = m_tabEleModules->width();
    h = m_tabEleModules->height();

    c_w = (w - 150)  / (TABELEMODULES_COLCOUNT - 1);
    r_h = (h + 1 ) / TABELEMODULES_ROWCOUNT;
    m_tabEleModules->setColumnWidth(0, 150);
    for(int i  = 0; i <TABELEMODULES_ROWCOUNT; i++)
    {
        m_tabEleModules->setRowHeight(i, r_h);
    }

    for(int i = 1; i < (TABELEMODULES_COLCOUNT - 1); i++)
    {
        m_tabEleModules->setColumnWidth(i, c_w);
    }


    w = m_tabEleInstruments->width();
    h = m_tabEleInstruments->height();

    c_w = (w - 200)  / (TABELEINSTRUMENTS_COLCOUNT - 1);
    r_h = (h + 1 ) / TABELEINSTRUMENTS_ROWCOUNT;
    m_tabEleInstruments->setColumnWidth(0, 200);
    for(int i  = 0; i < TABELEINSTRUMENTS_ROWCOUNT; i++)
    {
        m_tabEleInstruments->setRowHeight(i, r_h);
    }

    for(int i = 1; i < (TABELEINSTRUMENTS_COLCOUNT - 1); i++)
    {
        m_tabEleInstruments->setColumnWidth(i, c_w);
    }

    w = m_tabEleInstruments->width();
    h = m_tabEleInstruments->height();
}












