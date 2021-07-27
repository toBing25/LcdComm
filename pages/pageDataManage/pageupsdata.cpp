#include "pageupsdata.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QScrollBar>



PageUpsData::PageUpsData(QWidget *parent)
    : QWidget(parent)
{
    setupUi();

}

PageUpsData::~PageUpsData()
{
}

void PageUpsData::setupUi(void)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setGeometry(0, 0, this->width(), this->height());

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10, 0, 10, 12);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setObjectName(QStringLiteral("horizontalLayout2"));
    horizontalLayout2->setSpacing(6);
    horizontalLayout2->setContentsMargins(0, 6, 0, 0);

    m_lblUpsNumber  = new QLabel(this);
    m_lblUpsNumber->setObjectName(QStringLiteral("m_lblUpsNumber"));
    m_lblUpsNumber->setText(QObject::tr("当前UPS编号："));
    m_lblUpsNumber->setMinimumSize(QSize(120, 45));
    m_lblUpsNumber->setMaximumSize(QSize(120, 45));
    m_lblUpsNumber->setStyleSheet("color:rgb(255, 255, 255);;font-size:16px;");
    horizontalLayout2->addWidget(m_lblUpsNumber);

    m_lblUpsVal     = new QLabel(this);
    m_lblUpsVal->setObjectName(QStringLiteral("m_lblUpsVal"));
    m_lblUpsVal->setMinimumSize(QSize(110, 45));
    m_lblUpsVal->setMaximumSize(QSize(110, 45));
    m_lblUpsVal->setStyleSheet("color:rgb(255, 255, 255);;font-size:16px;");
    horizontalLayout2->addWidget(m_lblUpsVal);

    QHBoxLayout *horizontalLayout3 = new QHBoxLayout;
    horizontalLayout3->setObjectName(QStringLiteral("horizontalLayout3"));
    horizontalLayout3->setSpacing(6);
    horizontalLayout3->setContentsMargins(0, 0, 10, 0);

    m_lblUpsStat    = new QLabel(this);
    m_lblUpsStat->setObjectName(QStringLiteral("m_lblUpsStat"));
    m_lblUpsStat->setText(QObject::tr("UPS工作状态："));
    m_lblUpsStat->setMinimumSize(QSize(120, 45));
    m_lblUpsStat->setMaximumSize(QSize(120, 45));
    m_lblUpsStat->setStyleSheet("color:rgb(255, 255, 255);;font-size:16px;");
    horizontalLayout3->addWidget(m_lblUpsStat);

    m_lblUPsStatVal = new QLabel(this);
    m_lblUPsStatVal->setObjectName(QStringLiteral("m_lblUPsStatVal"));
    m_lblUPsStatVal->setMinimumSize(QSize(110, 45));
    m_lblUPsStatVal->setMaximumSize(QSize(110, 45));
    m_lblUPsStatVal->setStyleSheet("color:rgb(255, 255, 255);;font-size:16px;");
    horizontalLayout3->addWidget(m_lblUPsStatVal);

    horizontalLayout->addLayout(horizontalLayout2);
    horizontalLayout->addStretch();
    horizontalLayout->addLayout(horizontalLayout3);
    mainLayout->addLayout(horizontalLayout);

    m_tabInfo_1 = new QTableWidget(this);
    m_tabInfo_1->setObjectName(QStringLiteral("tableWidget"));
    m_tabInfo_1->setColumnCount(4);
    m_tabInfo_1->setRowCount(4);
    m_tabInfo_1->setShowGrid(true);
    m_tabInfo_1->horizontalHeader()->setVisible(false);
    m_tabInfo_1->horizontalHeader()->setSectionsClickable(false);
    m_tabInfo_1->horizontalHeader()->setStretchLastSection(true);
    m_tabInfo_1->horizontalHeader()->setHighlightSections(false);
    m_tabInfo_1->verticalHeader()->setVisible(false);
    m_tabInfo_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabInfo_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabInfo_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabInfo_1->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabInfo_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabInfo_1->setFocusPolicy(Qt::NoFocus);

    int col = 1, row = 1;
    QTableWidgetItem *pItem;
    pItem = new QTableWidgetItem(QObject::tr("主路输入"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabInfo_1->setItem(0, col, pItem);

    pItem = new QTableWidgetItem(QObject::tr("输出"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabInfo_1->setItem(0, col+1, pItem);

    pItem = new QTableWidgetItem(QObject::tr("旁路输入"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabInfo_1->setItem(0, col+2, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电压(V)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabInfo_1->setItem(row, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电流(A)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabInfo_1->setItem(row+1, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("频率(HZ)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabInfo_1->setItem(row+2, 0, pItem);

    QPalette pll = m_tabInfo_1->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    pItem->setForeground(Qt::white);
    m_tabInfo_1->setPalette(pll);

    mainLayout->addWidget(m_tabInfo_1);

    m_tabInfo_2 = new QTableWidget(this);
    m_tabInfo_2->setObjectName(QStringLiteral("m_tabInfo_2"));
    m_tabInfo_2->setColumnCount(3);
    m_tabInfo_2->setRowCount(5);
    m_tabInfo_2->setShowGrid(true);
    m_tabInfo_2->horizontalHeader()->setVisible(false);
    m_tabInfo_2->horizontalHeader()->setSectionsClickable(false);
    m_tabInfo_2->horizontalHeader()->setStretchLastSection(true);
    m_tabInfo_2->horizontalHeader()->setHighlightSections(false);
    m_tabInfo_2->verticalHeader()->setVisible(false);
    m_tabInfo_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabInfo_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tabInfo_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tabInfo_2->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabInfo_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabInfo_2->setFocusPolicy(Qt::NoFocus);

    col = 0, row = 1;
    pItem = new QTableWidgetItem(QObject::tr("电池数据"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabInfo_2->setItem(0, col, pItem);

    pItem = new QTableWidgetItem(QObject::tr("负载数据"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabInfo_2->setItem(0, col+1, pItem);

    pItem = new QTableWidgetItem(QObject::tr("额定数据"));
    pItem->setTextAlignment(Qt::AlignCenter);
    pItem->setForeground(Qt::white);
    m_tabInfo_2->setItem(0, col+2, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电压(V)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabInfo_2->setItem(row, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("电流(A)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabInfo_2->setItem(row+1, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("容量(%)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabInfo_2->setItem(row+2, 0, pItem);

    pItem = new QTableWidgetItem(QObject::tr("剩余时间(分钟)"));
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    pItem->setForeground(Qt::white);
    m_tabInfo_2->setItem(row+3, 0, pItem);

    pll = m_tabInfo_2->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tabInfo_2->setPalette(pll);

    mainLayout->addWidget(m_tabInfo_2);

   this->setLayout(mainLayout);
}

void PageUpsData::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    int w, h, c_w, r_h;
    w = m_tabInfo_1->width();
    h = m_tabInfo_1->height();

    c_w = (w - 150)  / 3;
    r_h = (h - m_tabInfo_1->horizontalHeader()->height()) / 4;

    m_tabInfo_1->setColumnWidth(0, 150);
    for(int i  = 0; i < 4; i++)
    {
        m_tabInfo_1->setRowHeight(i, r_h);
    }

    for(int i = 1; i < 3; i++)
    {
        m_tabInfo_1->setColumnWidth(i, c_w);
    }

    w = m_tabInfo_2->width();
    h = m_tabInfo_2->height();

    c_w = w  / 3;
    r_h = (h - m_tabInfo_2->horizontalHeader()->height()+1) / 5;

    for(int i  = 0; i < 5; i++)
    {
        m_tabInfo_2->setRowHeight(i, r_h);
    }

    for(int i = 0; i < 3; i++)
    {
        m_tabInfo_2->setColumnWidth(i, c_w);
    }
}


