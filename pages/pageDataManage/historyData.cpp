#include "historyData.h"
#include "ui_historyData.h"
#include "buzzer.h"
#include "gettext.h"
#include "wgtwait.h"
#include "wgtkeypad.h"
#include "pageextend.h"
#include <QScrollBar>
#include <QDebug>
#include <QDateTime>
#include <QScreen>
#include <mask_widget.h>
#include <QListView>
#include "wgtmsgbox_h.h"
#include "Global/DataManage.h"
#include "wgtscrollbar.h"
#include "tableWidgetDelegate.h"

#define  ROW_COUNT     8
#define  COLUMN_COUNT  4

WgtHistoryData::WgtHistoryData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgtHistoryData)
{
    setupUi();
    qRegisterMetaType<statsDataStruct>("statsDataStruct");
    qRegisterMetaType<upsDataStruct>("upsDataStruct");
    qRegisterMetaType<meterDataStruct>("meterDataStruct");
    qRegisterMetaType<acDataStruct>("acDataStruct");
    qRegisterMetaType<pueDataStruct>("pueDataStruct");
    qRegisterMetaType<thsDataStruct>("thsDataStruct");
    //qRegisterMetaType<DevDataEnum>("DevDataEnum");

    //查询前显示等待动画
    dlg_wait = new wgtWait(this);
    dlg_wait->installEventFilter(this);  //过滤esc键
    dlg_wait->close();

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

    //最近一天/最近一周/最近一个月
    connect(ui->btnLastDay,   &QPushButton::clicked, this, &WgtHistoryData::onBtnLastHistory);
    connect(ui->btnLastMonth, &QPushButton::clicked, this, &WgtHistoryData::onBtnLastHistory);
    connect(ui->btnLastWeek,  &QPushButton::clicked, this, &WgtHistoryData::onBtnLastHistory);
    //前后页
    connect(ui->btnUp, &QPushButton::clicked, this, &WgtHistoryData::upPageAlmDatabase);
    connect(ui->btnDown, &QPushButton::clicked, this, &WgtHistoryData::downPageAlmDatabase);
    //查询数据库信号
    connect(this,&WgtHistoryData::sendStatsHistoryData,FormMain::Instance()->getShareMemManage()->getReadDataBase(),
            &readDatabase::readAllHistoryData,Qt::QueuedConnection);



    //收到数据处理
    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendStatsHistoryData,
            this,&WgtHistoryData::getStatsHistoryData);
    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendUpsHistoryData,
            this,&WgtHistoryData::getUpsHistoryData);
    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendMeterHistoryData,
            this,&WgtHistoryData::getMeterHistoryData);
    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendAcHistoryData,
            this,&WgtHistoryData::getAcHistoryData);
    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendThsHistoryData,
            this,&WgtHistoryData::getThsHistoryData);
    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendPueHistoryData,
            this,&WgtHistoryData::getPueHistoryData);

    //    connect(FormMain::Instance()->getShareMemManage()->getReadDataBase(),&readDatabase::sendStatsHistoryData,
    //                this,&WgtHistoryData::getOneDevHistoryData);

    connect(ui->cmboxType, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &WgtHistoryData::curIndexChanged);

    //查询按钮
    connect(ui->btnQuery, &QPushButton::clicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();
        //查询历史数据，显示第一页
        m_currentPage = 1;
        queryHistoryDatabase(1);
    });

    //日期框显示最近一天的日期
    QDateTime currT = QDateTime::currentDateTime();
    QString BeforeDaystr =currT.addDays(-1).toString("yyyy-MM-dd hh:mm:ss");//获取前一天时间
    ui->lineDate1->setText(BeforeDaystr);
    ui->lineDate2->setText(currT.toString("yyyy-MM-dd hh:mm:ss"));

    //蒙版
    //    MaskWidget::instance()->setMainWidget(this);
    MaskWidget *mask = new MaskWidget();
    mask->setMainWidget(this);
    QStringList listName;

    listName << "wgtWait";  //只要创建wgtWait类，显示蒙版效果
    mask->setNames(listName);
    //    MaskWidget::instance()->setNames(listName);
}

WgtHistoryData::~WgtHistoryData()
{
    delete ui;
}

void WgtHistoryData::setupUi(void)
{
    ui->setupUi(this);

    m_font.setFamily("Microsoft YaHei");
    m_font.setPixelSize(16);

    QStringList devlist;
    //devlist <<langN.show[langEnum::strAllDev],
    devlist <<langN.show[langEnum::strUps],
            devlist <<langN.show[langEnum::strAc],
            devlist <<langN.show[langEnum::strHumiture],
            devlist <<langN.show[langEnum::strVMeter],
            devlist <<langN.show[langEnum::strSystem],
            devlist <<langN.show[langEnum::strPue];

    ui->cmboxType->clear();
    ui->cmboxType->addItems(devlist);
    ui->cmboxType->setCurrentIndex(5);

    ui->lblDevType->setText(langN.show[langEnum::strDevType]);
    ui->lblEventLevel->setText(langN.show[langEnum::strDeviceNumber]);
    ui->btnLastDay->setText(langN.show[langEnum::strDayRpt]);
    ui->btnLastWeek->setText(langN.show[langEnum::strWeekRpt]);
    ui->btnLastMonth->setText(langN.show[langEnum::strMonthRpt]);
    ui->lblDateTime->setText(langN.show[langEnum::strDatetime]);
    ui->btnQuery->setText(langN.show[langEnum::strQuery]);


    ui->cmboxType->setView(new QListView());
    m_statsHeader.clear();
    m_statsHeader<<langEnum::strId
                 <<langEnum::strTmpAvg
                 <<langEnum::strFrontTmp
                 <<langEnum::strFrontHum
                 <<langEnum::strBackTmp;
                 //<<langEnum::strBackHum
                 //<<langEnum::strAmbientTemp
                 //<<langEnum::strAmbientHum;

    ui->tabHistory->setColumnCount(5/*m_statsHeader.length()*/);
    ui->tabHistory->setRowCount(ROW_COUNT);
    ui->tabHistory->setShowGrid(false);
    ui->tabHistory->horizontalHeader()->setVisible(false);
    ui->tabHistory->horizontalHeader()->setSectionsClickable(false);
    ui->tabHistory->horizontalHeader()->setStretchLastSection(true);
    ui->tabHistory->horizontalHeader()->setHighlightSections(false);
    ui->tabHistory->verticalHeader()->setVisible(false);
    ui->tabHistory->setItemDelegate(new tableWidgetDelegate(ui->tabHistory));
    //ui->tabHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tabHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tabHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabHistory->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tabHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabHistory->setFocusPolicy(Qt::NoFocus);
    ui->tabHistory->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(44, 82, 153);color: rgb(188, 207, 245);font:12pt;}");
    ui->tabHistory->setStyleSheet("QTableWidget{gridline-color: rgb(44, 82, 153);border:0px solid rgb(44, 82, 153);}");

    wgtScrollbar *vbar = new wgtScrollbar(Qt::Vertical, ui->tabHistory);
    vbar->setSliderLength(50);
    ui->tabHistory->setVerticalScrollBar(vbar);

    wgtScrollbar *hbar = new wgtScrollbar(Qt::Horizontal, ui->tabHistory);
    hbar->setSliderLength(50);
    ui->tabHistory->setHorizontalScrollBar(hbar);

    ui->tabHistory->horizontalScrollBar()->setVisible(true);
    //滚动条
    ui->tabHistory->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border:0px solid rgb(52, 72, 105);background:rgb(52, 72, 105);height:25px;margin:0 30 0 30px;}"
                                                         "QScrollBar::handle:horizontal{ background: rgb(41, 142, 214);border-radius:0px;min-width:20px; height: 10px; }"
                                                         "QScrollBar::add-line:horizontal {border:1px solid rgb(52, 134, 208);background:rgb(52, 134, 208);width:30px;subcontrol-position:right;subcontrol-origin:margin;}"
                                                         "QScrollBar::sub-line:horizontal {border:1px solid rgb(52, 134, 208);background:rgb(52, 134, 208);width:30px;subcontrol-position:left;subcontrol-origin:margin;}"
                                                         "QScrollBar::left-arrow:horizontal {width:30px;height:30px; border-image:url(:/image/left.png);}"
                                                         "QScrollBar::right-arrow:horizontal {width:30px;height:30px;border-image:url(:/image/right.png);}"
                                                         "QScrollBar::add-page:horizontal {background:rgb(52, 72, 105);}"
                                                         "QScrollBar::sub-page:horizontal {background:rgb(52, 72, 105);}");  //设置横向滚动条样式

    ui->tabHistory->verticalScrollBar()->setStyleSheet("QScrollBar:vertical {border-image: url(:/image/bar_bg.png);width:30px;margin:30 0 30 0px;}"
                                                       "QScrollBar::handle:vertical{border-image: url(:/image/handle.png); width:10px; height: 20px ; }"
                                                       "QScrollBar::add-line:vertical {border:1px solid rgb(52, 134, 208);background:rgb(52, 134, 208);height:30px;subcontrol-position:bottom;subcontrol-origin:margin;}"
                                                       "QScrollBar::sub-line:vertical {border:1px solid rgb(52, 134, 208);background:rgb(52, 134, 208);height:30px;subcontrol-position:top;subcontrol-origin:margin;}"
                                                       "QScrollBar::down-arrow:vertical {width:15px;height:12px; border-image:url(:/image/down_Arrow.png);}"
                                                       "QScrollBar::up-arrow:vertical {width:15px;height:12px;border-image:url(:/image/up_Arrow.png);}"
                                                       "QScrollBar::add-page:vertical {background:rgb(52, 72, 105);}"
                                                       "QScrollBar::sub-page:vertical {background:rgb(52, 72, 105);}");
    QPalette pll = ui->tabHistory->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    ui->tabHistory->setPalette(pll);

    //ui->tabHistory->setHorizontalHeaderLabels(m_statsHeader);
    setHeaderLabels(m_statsHeader);
    ui->wgtDeve->setVisible(false);
}

void WgtHistoryData::curIndexChanged(int index)
{
    ui->comboBox->cleanItems();
    ui->wgtDeve->setVisible(false);
    if(index != 4 && index != 5)
    {
        QStringList devlist;
        devlist<<langN.show[langEnum::strAllDev];
        if(index == 0)
        {
            QList<ups_Info> upsList = PageExtend::Instance()->m_upsList;
            if(!upsList.isEmpty())
            {
                for (int i = 0; i < upsList.count(); i++) {
                    devlist << QString("%1").arg(upsList.at(i).devID);
                }
            }
        }else if (index == 1) {
            QList<ac_Info> acList = PageExtend::Instance()->m_acList;
            if(!acList.isEmpty())
            {
                for (int i = 0; i < acList.count(); i++) {
                    devlist << QString("%1").arg(acList.at(i).devID);
                }
            }
        }else if (index == 2) {
            QList<ths_Info> thsList = PageExtend::Instance()->m_thsList;
            if(!thsList.isEmpty())
            {
                for (int i = 0; i < thsList.count(); i++) {
                    devlist << QString("%1").arg(thsList.at(i).devID);
                }
            }
        }else if (index == 3) {
            QList<meter_Info> meterList = PageExtend::Instance()->m_meterList;
            if(!meterList.isEmpty())
            {
                for (int i = 0; i < meterList.count(); i++) {
                    devlist << QString("%1").arg(meterList.at(i).devID);
                }
            }
        }

        if(!devlist.isEmpty())
        {
            ui->comboBox->addItems(devlist);
            ui->wgtDeve->setVisible(true);
            //ui->comboBox->showPopup();
        }
    }
}

void WgtHistoryData::onBtnLastHistory(bool clicked)
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

void WgtHistoryData::calculationTotalPage(int totalPage)
{
    m_totalPage  = totalPage;

    if(m_totalPage == 0)
        m_totalPage = 1;

    ui->lblPage->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
}

void WgtHistoryData::queryHistoryDatabase(int page)
{
    bool DBConnStatus = FormMain::Instance()->getShareMemManage()->getReadDataBase()->getDBConnStatus();
    if(!DBConnStatus){
        WgtMsgBox_H msgbox(/*"连接数据库失败！"*/langN.show[langEnum::strConnectDatabaseFail], MSGBOX_TYPE_OK);
        msgbox.exec();
        return;
    }

    dlg_wait->show();

    //调用查询数据库
    QVector<int>devNoList;

    QDateTime upperDate,lowerDate;
    int devType = ui->cmboxType->currentIndex();

    for (int i = 0; i <ui->comboBox->getItemListId().count();i++) {
        devNoList.append(ui->comboBox->getItemListId().at(i).toInt() - 1);
    }

    lowerDate = QDateTime::fromString(ui->lineDate1->text(),"yyyy-MM-dd hh:mm:ss");
    upperDate = QDateTime::fromString(ui->lineDate2->text(),"yyyy-MM-dd hh:mm:ss");
    //查询告警历史数据库
    //bool ok = FormMain::Instance()->getShareMemManage()->getReadDataBase()->readAlmHistoryData(devType,devNoList,levelList,lowerDate,upperDate,page);
    emit sendStatsHistoryData(devType,devNoList,lowerDate,upperDate,page);
}

void WgtHistoryData::upPageAlmDatabase()
{
    int page = m_currentPage - 1;
    if(page >= 1){
        m_currentPage = page;
        queryHistoryDatabase(page);
    }
}

void WgtHistoryData::downPageAlmDatabase()
{
    int page = m_currentPage + 1;
    if(page <= m_totalPage){
        m_currentPage = page;
        queryHistoryDatabase(page);
    }
}

void WgtHistoryData::setHeaderLabels(QList<langEnum> labels)
{
    if(labels.count() > ui->tabHistory->columnCount())
        return;

    for (int i = 0; i < labels.count(); i++) {
        QTableWidgetItem *pItem;
        pItem = new QTableWidgetItem(langN.show[labels.at(i)]);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(m_font);
        ui->tabHistory->setItem(0, i, pItem);
    }
}

void WgtHistoryData::getHistoryList(almDataStruct list)
{
    dlg_wait->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();
    for (int i = 0; i < list.almList.count(); i++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(i, 0, pItem);

        //部件
        QString  parts = list.almList.at(i).parts;
        pItem = new QTableWidgetItem(parts);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(i, 1, pItem);

        //事件
        QString  alarm_code = list.almList.at(i).alarm_code;
        pItem = new QTableWidgetItem(alarm_code);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(i, 2, pItem);

        //日期
        QString  date = list.almList.at(i).datetime;
        pItem = new QTableWidgetItem(date);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(i, 3, pItem);
    }
}

void WgtHistoryData::setTabColumnWidth()
{
    int h, r_h;
    h = ui->tabHistory->height();
    r_h = (h - ui->tabHistory->horizontalHeader()->height() /*+20 - ui->tabHistory->horizontalScrollBar()->height()*/)/ ROW_COUNT;
    for(int i  = 0; i < ROW_COUNT; i++)
    {
        ui->tabHistory->setRowHeight(i, r_h);
    }

    for (int i = 0; i < ui->tabHistory->columnCount(); i++) {
        QTableWidgetItem *item  =   ui->tabHistory->horizontalHeaderItem(i);
        if(item)
        {
            QString text;
            int len;
            QFontMetrics fm = ui->tabHistory->fontMetrics();
            text = item->text();
            if(i == ui->tabHistory->columnCount() - 1)
            {
                len  = fm.width(text) + 80;
            }else {
                len  = fm.width(text);
            }
            ui->tabHistory->setColumnWidth(i, len + 50);
        }
    }
}

void WgtHistoryData::getStatsHistoryData(statsDataStruct list)
{
    dlg_wait->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();

    //动态更新表头
    ui->tabHistory->setColumnCount(statsDataList.length()+1);
    ui->tabHistory->setRowCount(ROW_COUNT);

    m_statsHeader.clear();
    m_statsHeader<<langEnum::strId;  //序号
    for(int i=0;i<statsDataList.length();++i){
        m_statsHeader<<statsDataList.at(i).first;
    }
    //ui->tabHistory->setHorizontalHeaderLabels(headerLabels);
    setHeaderLabels(m_statsHeader);
    setTabColumnWidth();

    //显示内容
    for (int i = 0, r = 1; i < list.dataList.count(); i++, r++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(m_font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 0, pItem);

        //内容
        for(int j=0;j<statsDataList.length();++j){
            pItem = new QTableWidgetItem(list.dataList.at(i).data[j]);
            pItem->setForeground(QColor("#FFFFFF"));
            pItem->setFont(m_font);
            pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->tabHistory->setItem(r, j+1, pItem);
        }
    }
}

void WgtHistoryData::getUpsHistoryData(upsDataStruct list)
{
    dlg_wait->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();

    //动态更新表头
    ui->tabHistory->setColumnCount(uDataList.length()+1);
    ui->tabHistory->setRowCount(ROW_COUNT);

    m_statsHeader.clear();
    m_statsHeader<<langEnum::strId;  //序号
    for(int i=0;i<uDataList.length();++i){
        m_statsHeader<<uDataList.at(i).first;
    }
    //ui->tabHistory->setHorizontalHeaderLabels(headerLabels);
    setHeaderLabels(m_statsHeader);
    setTabColumnWidth();

    //显示内容
    for (int i = 0, r = 1; i < list.dataList.count(); i++, r++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(m_font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 0, pItem);

        //内容
        for(int j=0;j<uDataList.length();++j){
            pItem = new QTableWidgetItem(list.dataList.at(i).data[j]);
            pItem->setForeground(QColor("#FFFFFF"));
            pItem->setFont(m_font);
            pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->tabHistory->setItem(r, j+1, pItem);
        }
    }
}

void WgtHistoryData::getMeterHistoryData(meterDataStruct list)
{
    dlg_wait->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();

    //动态更新表头
    ui->tabHistory->setColumnCount(meterDataList.length()+1);
    ui->tabHistory->setRowCount(ROW_COUNT);

    m_statsHeader.clear();
    m_statsHeader<<langEnum::strId;  //序号
    for(int i=0;i<meterDataList.length();++i){
        m_statsHeader<<meterDataList.at(i).first;
    }
    //ui->tabHistory->setHorizontalHeaderLabels(headerLabels);
    setHeaderLabels(m_statsHeader);
    setTabColumnWidth();

    //显示内容
    for (int i = 0, r = 1; i < list.dataList.count(); i++, r++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(m_font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 0, pItem);

        //内容
        for(int j=0;j<meterDataList.length();++j){
            pItem = new QTableWidgetItem(list.dataList.at(i).data[j]);
            pItem->setForeground(QColor("#FFFFFF"));
            pItem->setFont(m_font);
            pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->tabHistory->setItem(r, j+1, pItem);
        }
    }
}

void WgtHistoryData::getAcHistoryData(acDataStruct list)
{
    dlg_wait->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();

    //动态更新表头
    ui->tabHistory->setColumnCount(acDataList.length()+1);
    ui->tabHistory->setRowCount(ROW_COUNT);

    m_statsHeader.clear();
    m_statsHeader<<langEnum::strId;  //序号
    for(int i=0;i<acDataList.length();++i){
        m_statsHeader<<acDataList.at(i).first;
    }
    //ui->tabHistory->setHorizontalHeaderLabels(headerLabels);
    setHeaderLabels(m_statsHeader);
    setTabColumnWidth();

    //显示内容
    for (int i = 0, r = 1; i < list.dataList.count(); i++, r++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(m_font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 0, pItem);

        //内容
        for(int j=0;j<acDataList.length();++j){
            pItem = new QTableWidgetItem(list.dataList.at(i).data[j]);
            pItem->setForeground(QColor("#FFFFFF"));
            pItem->setFont(m_font);
            pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->tabHistory->setItem(r, j+1, pItem);
        }
    }
}

void WgtHistoryData::getThsHistoryData(thsDataStruct list)
{
    dlg_wait->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();

    //动态更新表头
    ui->tabHistory->setColumnCount(thsDataList.length()+1);
    ui->tabHistory->setRowCount(ROW_COUNT);

    m_statsHeader.clear();
    m_statsHeader<<langEnum::strId;  //序号
    for(int i=0;i<thsDataList.length();++i){
        m_statsHeader<<thsDataList.at(i).first;
    }
    //ui->tabHistory->setHorizontalHeaderLabels(headerLabels);
    setHeaderLabels(m_statsHeader);
    setTabColumnWidth();

    //显示内容
    for (int i = 0, r = 1; i < list.dataList.count(); i++, r++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(m_font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 0, pItem);

        //内容
        for(int j=0;j<thsDataList.length();++j){
            pItem = new QTableWidgetItem(list.dataList.at(i).data[j]);
            pItem->setForeground(QColor("#FFFFFF"));
            pItem->setFont(m_font);
            pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->tabHistory->setItem(r, j+1, pItem);
        }
    }
}

void WgtHistoryData::getPueHistoryData(pueDataStruct list)
{
    dlg_wait->close();
    //计算总的页数，更新页信息
    calculationTotalPage(list.pageCount);
    ui->tabHistory->clearContents();
    qDebug()<<pueDataList.length();
    //动态更新表头
    ui->tabHistory->setColumnCount(pueDataList.length()+1);
    ui->tabHistory->setRowCount(ROW_COUNT);

    m_statsHeader.clear();
    m_statsHeader<<langEnum::strId;  //序号
    for(int i=0;i<pueDataList.length();++i){
        m_statsHeader<<pueDataList.at(i).first;
    }
    //ui->tabHistory->setHorizontalHeaderLabels(headerLabels);
    setHeaderLabels(m_statsHeader);
    setTabColumnWidth();

    //显示内容
    for (int i = 0, r = 1; i < list.dataList.count(); i++, r++) {
        QTableWidgetItem *pItem;

        //序号
        int currPageNo = (m_currentPage-1)*ROW_COUNT +1;
        QString  ser_number =  QString::number(currPageNo+i);
        pItem = new QTableWidgetItem(ser_number);
        pItem->setForeground(QColor("#FFFFFF"));
        pItem->setFont(m_font);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->tabHistory->setItem(r, 0, pItem);

        //内容
        for(int j=0;j<pueDataList.length();++j){
            pItem = new QTableWidgetItem(list.dataList.at(i).data[j]);
            pItem->setForeground(QColor("#FFFFFF"));
            pItem->setFont(m_font);
            pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->tabHistory->setItem(r, j+1, pItem);
        }
    }
}

bool WgtHistoryData::eventFilter(QObject *obj, QEvent *event)
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
                    queryHistoryDatabase(m_currentPage);
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

void WgtHistoryData::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    /*int w, h, c_w, r_h;
    w = ui->tabHistory->width();
    h = ui->tabHistory->height();

    c_w = (w - ui->tabHistory->verticalScrollBar()->width())/ COLUMN_COUNT;
    r_h = (h - ui->tabHistory->horizontalHeader()->height() +20 - ui->tabHistory->horizontalScrollBar()->height())/ ROW_COUNT;
    for(int i  = 0; i < ROW_COUNT; i++)
    {
        ui->tabHistory->setRowHeight(i, r_h);
    }*/

    /*for(int i = 0; i < COLUMN_COUNT; i++)
    {
        ui->tabHistory->setColumnWidth(i, c_w);
    }*/
    setTabColumnWidth();
}

void WgtHistoryData::changeEvent(QEvent* event)
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

void WgtHistoryData::retranslateUi(void)
{
    QStringList devlist;
    //devlist <<langN.show[langEnum::strAllDev],
    devlist <<langN.show[langEnum::strUps],
            devlist <<langN.show[langEnum::strAc],
            devlist <<langN.show[langEnum::strHumiture],
            devlist <<langN.show[langEnum::strVMeter],
            devlist <<langN.show[langEnum::strSystem],
            devlist <<langN.show[langEnum::strPue];

    ui->cmboxType->clear();
    ui->cmboxType->addItems(devlist);

    ui->lblDevType->setText(langN.show[langEnum::strDevType]);
    ui->lblEventLevel->setText(langN.show[langEnum::strDeviceNumber]);
    ui->btnLastDay->setText(langN.show[langEnum::strDayRpt]);
    ui->btnLastWeek->setText(langN.show[langEnum::strWeekRpt]);
    ui->btnLastMonth->setText(langN.show[langEnum::strMonthRpt]);
    ui->lblDateTime->setText(langN.show[langEnum::strDatetime]);
    ui->btnQuery->setText(langN.show[langEnum::strQuery]);

    ui->tabHistory->clearContents();
    setHeaderLabels(m_statsHeader);
    setTabColumnWidth();

    m_currentPage = 1;
    m_totalPage   = 1;
    ui->lblPage->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
}






