#include "wgthistory.h"
#include "ui_wgthistory.h"
#include "buzzer.h"
#include "gettext.h"
#include "wgtwait.h"
#include <QScrollBar>
#include <QDebug>
#include <QDateTime>
#include <QListView>
#include <QScreen>
#include <mask_widget.h>
#include "wgtmsgbox_h.h"
#include "wgtscrollbar.h"
#include "tableWidgetDelegate.h"

#define  ROW_COUNT     8
#define  COLUMN_COUNT  4


WgtHistory::WgtHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgtHistory)
{
    setupUi();
    qRegisterMetaType<almDataStruct>("almDataStruct");
    //查询前显示等待动画
    dlg = new wgtWait(this);
    dlg->installEventFilter(this);  //过滤esc键
    dlg->close();

    lieVector.append(ui->lineDate1);
    lieVector.append(ui->lineDate2);
    foreach (QLineEdit *lie, lieVector)
    {
        lie->installEventFilter(this);
    }
    ui->lblPage->installEventFilter(this);

    m_currentPage = 1;
    m_totalPage   = 1;
    m_tmpPage     = 1;
    m_status      = status_One;

    ui->lblPage->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));

    //当所有级别选中时，选中其他事件级别,反之亦然
    connect(ui->checkAllLevels, &QCheckBox::stateChanged, this, [=](int status){
        ShortBeep();

        if(status == Qt::Checked)
        {
            ui->checkSeriousAlarm->setChecked(true);
            ui->checkGeneralAlarm->setChecked(true);
            ui->checkNormalAlarm->setChecked(true);
        }
        else if(status == Qt::Unchecked)
        {
            if(m_status == status_All)
                return;

            ui->checkSeriousAlarm->setChecked(false);
            ui->checkGeneralAlarm->setChecked(false);
            ui->checkNormalAlarm->setChecked(false);
        }
    });

    //当其中一个事件级别取消选中时，所有级别设置为未选中状态
    connect(ui->checkSeriousAlarm, &QCheckBox::stateChanged, this, [=](int status){
        ShortBeep();

        m_status = status_One;
        if(status == Qt::Unchecked)
        {
            m_status = status_All;
            ui->checkAllLevels->setChecked(false);
        }
    });
    connect(ui->checkGeneralAlarm, &QCheckBox::stateChanged, this, [=](int status){
        ShortBeep();

        m_status = status_One;
        if(status == Qt::Unchecked)
        {
            m_status = status_All;
            ui->checkAllLevels->setChecked(false);
        }
    });
    connect(ui->checkNormalAlarm, &QCheckBox::stateChanged, this, [=](int status){
        ShortBeep();

        m_status = status_One;
        if(status == Qt::Unchecked)
        {
            m_status = status_All;
            ui->checkAllLevels->setChecked(false);
        }
    });

    //最近一天/最近一周/最近一个月
    connect(ui->btnLastDay,   &QPushButton::clicked, this, &WgtHistory::onBtnLastHistory);
    connect(ui->btnLastMonth, &QPushButton::clicked, this, &WgtHistory::onBtnLastHistory);
    connect(ui->btnLastWeek,  &QPushButton::clicked, this, &WgtHistory::onBtnLastHistory);
    //前后页
    connect(ui->btnUp, &QPushButton::clicked, this, &WgtHistory::upPageAlmDatabase);
    connect(ui->btnDown, &QPushButton::clicked, this, &WgtHistory::downPageAlmDatabase);
    //查询数据库信号
    connect(this,&WgtHistory::sendAlmHistoryData,FormMain::Instance()->getShareMemManage()->getReadDataBase(),
            &readDatabase::readAlmHistoryData,Qt::QueuedConnection);

    //收到数据处理
    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendAlmHistoryData,
            this,&WgtHistory::getHistoryList);
    //查询按钮
    connect(ui->btnQuery, &QPushButton::clicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();
        //查询历史数据，显示第一页
        m_currentPage = 1;
        queryAlmDatabase(1);
    });

    //日期框显示最近一天的日期
    QDateTime currT = QDateTime::currentDateTime();
    QString BeforeDaystr =currT.addDays(-1).toString("yyyy-MM-dd hh:mm:ss");//获取前一天时间
    ui->lineDate1->setText(BeforeDaystr);
    ui->lineDate2->setText(currT.toString("yyyy-MM-dd hh:mm:ss"));

    //默认选中所有级别
    ui->checkAllLevels->setChecked(true);

    //蒙版
    //MaskWidget::instance()->setMainWidget(this);
    MaskWidget *mask = new MaskWidget();
    mask->setMainWidget(this);
    QStringList listName;

    listName << "wgtWait";  //只要创建wgtWait类，显示蒙版效果

    mask->setNames(listName);
}

WgtHistory::~WgtHistory()
{
    delete ui;
}

void WgtHistory::setupUi(void)
{
    ui->setupUi(this);

    wgtScrollbar *vbar = new wgtScrollbar(Qt::Vertical, ui->cmboxType);
    vbar->setSliderLength(50);
    ui->cmboxType->setView(new QListView());
    ui->cmboxType->view()->setVerticalScrollBar(vbar);
    ui->cmboxType->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QStringList devlist;
    devlist <<langN.show[langEnum::strAllDev],
            devlist <<langN.show[langEnum::strSystem],
            devlist <<langN.show[langEnum::strUps],
            devlist <<langN.show[langEnum::strAc],
            devlist <<langN.show[langEnum::strHumiture],
            devlist <<langN.show[langEnum::strPdu],
            devlist <<langN.show[langEnum::strWaterLeak],
            devlist <<langN.show[langEnum::strSmsDev],
            devlist <<langN.show[langEnum::strSpds],
            devlist <<langN.show[langEnum::strVMeter],
            devlist <<langN.show[langEnum::strColumnCabinet],
            devlist <<langN.show[langEnum::strExtDc],
            devlist <<langN.show[langEnum::strBms],
            devlist <<langN.show[langEnum::strAts],
            devlist <<langN.show[langEnum::strMAUDev],
            devlist <<langN.show[langEnum::strDisplayMode],
            devlist <<langN.show[langEnum::strFanCtrlr],
            devlist <<langN.show[langEnum::strCtrlBox];
    ui->cmboxType->clear();
    ui->cmboxType->addItems(devlist);
    ui->cmboxType->setCurrentIndex(0);

    ui->lblDevType->setText(langN.show[langEnum::strDevType]);
    ui->lblEventLevel->setText(langN.show[langEnum::strEveLv]);
    ui->checkAllLevels->setText(langN.show[langEnum::strAllLv]);
    ui->checkSeriousAlarm->setText(langN.show[langEnum::strUrgAlm]);
    ui->checkGeneralAlarm->setText(langN.show[langEnum::strGenAlm]);
    ui->checkNormalAlarm->setText(langN.show[langEnum::strNorEve]);
    ui->btnLastDay->setText(langN.show[langEnum::strDayRpt]);
    ui->btnLastWeek->setText(langN.show[langEnum::strWeekRpt]);
    ui->btnLastMonth->setText(langN.show[langEnum::strMonthRpt]);
    ui->lblDateTime->setText(langN.show[langEnum::strDatetime]);
    ui->btnQuery->setText(langN.show[langEnum::strQuery]);

    ui->tabHistory->setColumnCount(COLUMN_COUNT);
    ui->tabHistory->setRowCount(ROW_COUNT);
    ui->tabHistory->setShowGrid(false);
    ui->tabHistory->setItemDelegate(new tableWidgetDelegate(ui->tabHistory));

    ui->tabHistory->horizontalHeader()->setVisible(false);
    ui->tabHistory->horizontalHeader()->setSectionsClickable(false);
    ui->tabHistory->horizontalHeader()->setStretchLastSection(true);
    ui->tabHistory->horizontalHeader()->setHighlightSections(false);
    ui->tabHistory->verticalHeader()->setVisible(false);
    ui->tabHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tabHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tabHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabHistory->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tabHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabHistory->setFocusPolicy(Qt::NoFocus);
    ui->tabHistory->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(44, 82, 153);color: #00ADE5; font:14pt;}");
    ui->tabHistory->setStyleSheet("QTableWidget{gridline-color: rgb(44, 82, 153);border:0px solid rgb(44, 82, 153);}");

    QPalette pll = ui->tabHistory->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    ui->tabHistory->setPalette(pll);

    QStringList headerLabels;
    headerLabels <</*QObject::tr("序号")*/langN.show[langEnum::strId]
            <</*QObject::tr("设备")*/langN.show[langEnum::strDevType]
            <</*QObject::tr("事件") */langN.show[langEnum::strEvent]
            <</*QObject::tr("日期")*/langN.show[langEnum::strDatetime];
    //ui->tabHistory->setHorizontalHeaderLabels(headerLabels);
    setHorizontalHeaderLabels();
}

void WgtHistory::onBtnLastHistory(bool clicked)
{
    Q_UNUSED(clicked);
    ShortBeep();

    QDateTime currT = QDateTime::currentDateTime();
    QString BeforeDaystr;
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if(btn == NULL)
        return;

    if(btn == ui->btnLastDay){
        BeforeDaystr =currT.addDays(-1).toString("yyyy-MM-dd hh:mm:ss");//获取前一天时间
        ui->lineDate1->setText(BeforeDaystr);
        ui->lineDate2->setText(currT.toString("yyyy-MM-dd hh:mm:ss"));
    }else if(btn == ui->btnLastWeek){
        BeforeDaystr =currT.addDays(-7).toString("yyyy-MM-dd hh:mm:ss");//获取最近一周时间
        ui->lineDate1->setText(BeforeDaystr);
        ui->lineDate2->setText(currT.toString("yyyy-MM-dd hh:mm:ss"));
    }else if(btn == ui->btnLastMonth){
        BeforeDaystr = currT.addDays(-30).toString("yyyy-MM-dd hh:mm:ss");//获取最近一月时间
        ui->lineDate1->setText(BeforeDaystr);
        ui->lineDate2->setText(currT.toString("yyyy-MM-dd hh:mm:ss"));
    }
    //查询
    emit ui->btnQuery->clicked();
}

void WgtHistory::calculationTotalPage(int totalPage)
{
    m_totalPage  = totalPage;

    if(m_totalPage == 0)
        m_totalPage = 1;

    ui->lblPage->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
}

void WgtHistory::queryAlmDatabase(int page)
{

    bool DBConnStatus = FormMain::Instance()->getShareMemManage()->getReadDataBase()->getDBConnStatus();
    if(!DBConnStatus){
        WgtMsgBox_H msgbox(langN.show[langEnum::strConnectDatabaseFail], MSGBOX_TYPE_OK);
        msgbox.exec();
        return;
    }


    dlg->show();
    //调用查询数据库
    QVector<int> levelList,devNoList;
    QDateTime upperDate,lowerDate;
    int devType = ui->cmboxType->currentIndex()-1;
    if(ui->checkAllLevels->isChecked()==true){
        ;
    }else {
        if(ui->checkNormalAlarm->isChecked()==true){ levelList<<0; }
        if(ui->checkGeneralAlarm->isChecked()==true){ levelList<<1; }
        if(ui->checkSeriousAlarm->isChecked()==true){ levelList<<2; }
    }
    lowerDate = QDateTime::fromString(ui->lineDate1->text(),"yyyy-MM-dd hh:mm:ss");
    upperDate = QDateTime::fromString(ui->lineDate2->text(),"yyyy-MM-dd hh:mm:ss");
    //查询告警历史数据库
    //bool ok = FormMain::Instance()->getShareMemManage()->getReadDataBase()->readAlmHistoryData(devType,devNoList,levelList,lowerDate,upperDate,page);
    emit sendAlmHistoryData(devType,devNoList,levelList,lowerDate,upperDate,page);

}

void WgtHistory::upPageAlmDatabase()
{
    int page = m_currentPage - 1;
    if(page >= 1){
        m_currentPage = page;
        queryAlmDatabase(page);
    }
}

void WgtHistory::downPageAlmDatabase()
{
    int page = m_currentPage + 1;
    if(page <= m_totalPage){
        m_currentPage = page;
        queryAlmDatabase(page);
    }
}

void WgtHistory::setHorizontalHeaderLabels(void)
{
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    int col = 0;
    QTableWidgetItem *pItem;
    pItem = new QTableWidgetItem(langN.show[langEnum::strId]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabHistory->setItem(0, col, pItem);

    pItem = new QTableWidgetItem(langN.show[langEnum::strDevType]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabHistory->setItem(0, col+1, pItem);

    pItem = new QTableWidgetItem(langN.show[langEnum::strEvent]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabHistory->setItem(0, col+2, pItem);

    pItem = new QTableWidgetItem(langN.show[langEnum::strDatetime]);
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabHistory->setItem(0, col+3, pItem);
}

void WgtHistory::getHistoryList(almDataStruct list)
{

    dlg->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();
    setHorizontalHeaderLabels();

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    for (int i = 0, r = 1; i < list.almList.count(); i++, r++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 0, pItem);

        //部件
        QString  parts = list.almList.at(i).parts;
        pItem = new QTableWidgetItem(parts);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 1, pItem);

        //事件
        QString  alarm_code = list.almList.at(i).alarm_code;
        pItem = new QTableWidgetItem(alarm_code);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 2, pItem);

        //日期
        QString  date = list.almList.at(i).datetime;
        pItem = new QTableWidgetItem(date);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 3, pItem);
    }
}

bool WgtHistory::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLineEdit"))
        {
            ShortBeep();

            //日期时间
            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj);
            connect(WgtKeyPad::Instance(), &WgtKeyPad::sendInputBufferAreaText, [=](QString text)
            {
                if(lineEdit != 0)
                {
                    lineEdit->setText(text);
                }
            });

            foreach (QLineEdit *lie, lieVector) {
                if(lie == lineEdit)
                {
                    WgtKeyPad::Instance()->showInputBufferArea(lie->text());
                    WgtKeyPad::Instance()->show();
                    return  true;
                }
            }
        }
        else if(obj->inherits("QLabel"))
        {
            ShortBeep();

            //页显示
            QLabel *label = qobject_cast<QLabel *>(obj);
            if(label == ui->lblPage)
            {
                connect(WgtKeyPad::Instance(), &WgtKeyPad::sendInputBufferAreaText, [=](QString text){
                    //页跳转输入的值必须为数字，并且大于0，小于总的页数
                    QRegExp regExp("[0-9]+$");
                    if (!regExp.exactMatch(text)) {
                        return;
                    }

                    if((text.toInt() > m_totalPage) || ( text.toInt() < 0))
                        return;

                    m_currentPage = text.toInt();

                    //label->setFrameShape(QFrame::NoFrame);
                    //当设置要跳转的页时，先设置页的文本，然后查询数据库
                    label->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
                    //查询数据库
                    queryAlmDatabase(m_currentPage);
                });

                //label->setFrameShape(QFrame::Panel);
                WgtKeyPad::Instance()->showInputBufferArea("");
                WgtKeyPad::Instance()->show();
                return  true;
            }
        }
    }
    case QEvent::KeyPress:  //取消esc键
    {
        QDialog *pDialog = qobject_cast<QDialog *>(obj);
        if (pDialog != NULL)
        {
            QKeyEvent *pKeyEvent = static_cast<QKeyEvent*>(event);
            if (pKeyEvent->key() == Qt::Key_Escape)
            {
                return true;
            }
        }

    }
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

void WgtHistory::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    int w, h, c_w, r_h;
    w = ui->tabHistory->width();
    h = ui->tabHistory->height();

    c_w = (w - ui->tabHistory->verticalScrollBar()->width() - 60)/ (COLUMN_COUNT - 1);
    r_h = (h - ui->tabHistory->horizontalHeader()->height() + 5)/ ROW_COUNT;
    for(int i  = 0; i < ROW_COUNT; i++)
    {
        ui->tabHistory->setRowHeight(i, r_h);
    }

    for(int i = 1; i < (COLUMN_COUNT - 1); i++)
    {
        ui->tabHistory->setColumnWidth(i, c_w);
    }
}

void WgtHistory::changeEvent(QEvent* event)
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

void WgtHistory::retranslateUi(void)
{
    QStringList headerLabels;
    headerLabels <<langN.show[langEnum::strId]
            <<langN.show[langEnum::strDevType]
            <<langN.show[langEnum::strEvent]
            <<langN.show[langEnum::strDatetime];
    setHorizontalHeaderLabels();

    QStringList devlist;
    devlist <<langN.show[langEnum::strAllDev],
            devlist <<langN.show[langEnum::strSystem],
            devlist <<langN.show[langEnum::strUps],
            devlist <<langN.show[langEnum::strAc],
            devlist <<langN.show[langEnum::strHumiture],
            devlist <<langN.show[langEnum::strPdu],
            devlist <<langN.show[langEnum::strWaterLeak],
            devlist <<langN.show[langEnum::strSmsDev],
            devlist <<langN.show[langEnum::strSpds],
            devlist <<langN.show[langEnum::strVMeter],
            devlist <<langN.show[langEnum::strColumnCabinet],
            devlist <<langN.show[langEnum::strExtDc],
            devlist <<langN.show[langEnum::strBms],
            devlist <<langN.show[langEnum::strAts],
            devlist <<langN.show[langEnum::strMAUDev],
            devlist <<langN.show[langEnum::strDisplayMode],
            devlist <<langN.show[langEnum::strFanCtrlr],
            devlist <<langN.show[langEnum::strCtrlBox];
    ui->cmboxType->clear();
    ui->cmboxType->addItems(devlist);

    ui->lblDevType->setText(langN.show[langEnum::strDevType]);
    ui->lblEventLevel->setText(langN.show[langEnum::strEveLv]);
    ui->checkAllLevels->setText(langN.show[langEnum::strAllLv]);
    ui->checkSeriousAlarm->setText(langN.show[langEnum::strUrgAlm]);
    ui->checkGeneralAlarm->setText(langN.show[langEnum::strGenAlm]);
    ui->checkNormalAlarm->setText(langN.show[langEnum::strNorEve]);
    ui->btnLastDay->setText(langN.show[langEnum::strDayRpt]);
    ui->btnLastWeek->setText(langN.show[langEnum::strWeekRpt]);
    ui->btnLastMonth->setText(langN.show[langEnum::strMonthRpt]);
    ui->lblDateTime->setText(langN.show[langEnum::strDatetime]);
    ui->btnQuery->setText(langN.show[langEnum::strQuery]);
    ui->tabHistory->clearContents();

    m_currentPage = 1;
    m_totalPage   = 1;
    ui->lblPage->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
}







