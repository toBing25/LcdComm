#include "alarmwidget.h"
#include "wgtmenubar.h"
#include "tableWidgetDelegate.h"
#include "formmain.h"
#include "Gpio.h"
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QDateTime>

#define COL_NUM   4
#define ROW_COUNT 10


AlarmWidget::AlarmWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

AlarmWidget::~AlarmWidget()
{

}

void AlarmWidget::setupUi(void)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setGeometry(0, 0, this->width(), this->height());

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(5, 5, 5, 5);

    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setObjectName(QStringLiteral("tableWidget"));
    m_tableWidget->setColumnCount(COL_NUM);
    m_tableWidget->setRowCount(ROW_COUNT);
    m_tableWidget->setShowGrid(false);
    m_tableWidget->setItemDelegate(new tableWidgetDelegate(m_tableWidget));

    m_tableWidget->horizontalHeader()->setVisible(false);
    m_tableWidget->horizontalHeader()->setSectionsClickable(false);
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->horizontalHeader()->setHighlightSections(false);
    m_tableWidget->horizontalHeader()->setFixedHeight(20);
    m_tableWidget->verticalHeader()->setVisible(false);
    m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget->setFocusPolicy(Qt::NoFocus);
    m_tableWidget->setFrameStyle(QFrame::NoFrame | QFrame::Sunken);
    m_tableWidget->setMouseTracking(false);
    m_tableWidget->setStyleSheet("alternate-background-color: rgb(116, 116, 116);gridline-color: rgb(85, 170, 255);");

    setHorizontalHeaderLabels();

    mainLayout->addWidget(m_tableWidget);
    this->setLayout(mainLayout);
}

void AlarmWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    int w, h, c_w, r_h;
    w = m_tableWidget->width() - m_tableWidget->verticalScrollBar()->sizeHint().width();
    h = m_tableWidget->height() - m_tableWidget->horizontalScrollBar()->sizeHint().height() +13 ;

    c_w = 85;//(w - 45 ) / (COL_NUM - 1);
    r_h = h / ROW_COUNT;

    m_tableWidget->setColumnWidth(0, 35);

    for(int i  = 0; i < ROW_COUNT; i++)
    {
        m_tableWidget->setRowHeight(i, r_h);
    }

    //for(int i = 1; i < COL_NUM - 2; i++)
    {
        m_tableWidget->setColumnWidth(1, c_w);
        m_tableWidget->setColumnWidth(2, c_w);
    }

    c_w = w - 85*3;
    m_tableWidget->setColumnWidth(3, c_w);
}

void AlarmWidget::changeEvent(QEvent* event)
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

void AlarmWidget::retranslateUi(void)
{
    alarmListUpdate(m_list);
}

void AlarmWidget::setHorizontalHeaderLabels(void)
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(13);
    font.setBold(true);

    int col = 0;
    QTableWidgetItem *pItem;
    pItem = new QTableWidgetItem(/*QObject::tr("??????")*/langN.show[langEnum::strId]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor(188, 207, 245));
    pItem->setFont(font);
    m_tableWidget->setItem(0, col, pItem);

    pItem = new QTableWidgetItem(/*QObject::tr("??????")*/langN.show[langEnum::strDevType]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor(188, 207, 245));
    pItem->setFont(font);
    m_tableWidget->setItem(0, col+1, pItem);

    pItem = new QTableWidgetItem(/*QObject::tr("????????????")*/langN.show[langEnum::strEvent]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor(188, 207, 245));
    pItem->setFont(font);
    m_tableWidget->setItem(0, col+2, pItem);

    pItem = new QTableWidgetItem(/*QObject::tr("??????")*/langN.show[langEnum::strDatetime]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor(188, 207, 245));
    pItem->setFont(font);
    m_tableWidget->setItem(0, col+3, pItem);

    QPalette pll = m_tableWidget->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    m_tableWidget->setPalette(pll);
}

void AlarmWidget::getAlarmList(QList<alarm_list>& list)
{
    //alarmListUpdate(list);

    /*if(!list.isEmpty()){
        wgttitlebar::setAlarmMsg(list.at(0).parts + list.at(0).alarm_code, getAlarmColor(list.at(0).Alarmlevel));
    }else {
        wgttitlebar::setAlarmMsg("", "");
    }
    m_list = list;
    gpio_write(SCREENSAVER_PIN, 1);*/


#if 0
    //if(list.count() == 0)
    //{
        //?????????????????????????????????????????????????????????
        //alarmListUpdate(list);
        //if(FormMain::Instance()->getAlmIsShow() == false)
        //    FormMain::Instance()->popupAlmListWidget();
        //return;
    //}

    if((m_list.count() == 0)/* || ((m_list.count() != 0 )&&( m_list.at(0).datetime != list.at(0).datetime)) */)
    {
        m_list = list;
        //alarmListUpdate(list);
        //????????????????????????????????????????????????
        //FormMain::Instance()->setAlmIsShow(true);
        //FormMain::Instance()->popupAlmListWidget();

        //?????????????????????,????????????
        gpio_write(SCREENSAVER_PIN, 1);
    }
    else if ( m_list.at(0).datetime != list.at(0).datetime) {
        m_list = list;
        //alarmListUpdate(list);
        //????????????????????????????????????????????????
        //FormMain::Instance()->setAlmIsShow(true);
        //FormMain::Instance()->popupAlmListWidget();

        //?????????????????????,????????????
        gpio_write(SCREENSAVER_PIN, 1);
    }
#endif
}

void AlarmWidget::alarmListUpdate(QList<alarm_list>& list)
{
    //?????????????????????????????????
    m_tableWidget->clearContents();
    setHorizontalHeaderLabels();

    if(list.isEmpty())
        return;

    int msg_num;
    if(list.count() < 10)
        msg_num = list.count();
    else
        msg_num = 10; //????????????4???

    for (int i = 0, r = 1; i < msg_num; i++, r++)
    {
        QTableWidgetItem *pItem;

        //????????????????????????????????????????????????
        QColor color = QColor(getAlarmColor(list.at(i).Alarmlevel));
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(12);

        //??????
        QString  ser_number =  QString::number(i+1)/*.asprintf("%02d", i+1)*/;
        pItem = new QTableWidgetItem(ser_number);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(color);
        pItem->setFont(font);
        m_tableWidget->setItem(r, 0, pItem);

        //??????
        QString  parts = list.at(i).parts;
        pItem = new QTableWidgetItem(parts);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(color);
        pItem->setFont(font);
        m_tableWidget->setItem(r, 1, pItem);

        //????????????
        QString  alarm_code = list.at(i).alarm_code;
        pItem = new QTableWidgetItem(alarm_code);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(color);
        pItem->setFont(font);
        m_tableWidget->setItem(r, 2, pItem);

        //??????
        QString data, time;
        QStringList list_ = list.at(i).datetime.split(" ");
        data  = list_.at(0);
        time  = list_.at(1);

        QString  date = data + "\n" +time;
        pItem = new QTableWidgetItem(date);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(color);
        pItem->setFont(font);
        m_tableWidget->setItem(r, 3, pItem);
    }
}

QString AlarmWidget::getAlarmColor(int grade)
{
    QString color;
    switch (grade) {
    case GeneralAlarm://??????
        color = "#ffaa00";
        break;
    case NormalAlarm://??????
        color = "#ffff00";
        break;
    case CriticalAlarm://??????
        color = "#fc5328";
        break;
    default:
        break;
    }

    return  color;
}




















