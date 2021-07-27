#include "wgtparaitem_ups.h"
#include "ui_wgtparaitem_ups.h"
#include "actparaitem.h"
#include "actparaitem_lineedit.h"
#include "actparaitem_combobox.h"
#include "actparaitem_bit.h"
#include "wgtmenubar.h"
#include "Formmain/formmain.h"
#include "actparawidget.h"
#include "ui_actparawidget.h"
#include "actparaitem_btn.h"
#include "wgtdatetime.h"
#include "buzzer.h"
#include <QTimer>
#include <QGroupBox>
#include <QPushButton>
#include <QDebug>
#include <math.h>
#include <QResizeEvent>
#include <QScrollBar>
#define ROW_COUNT 2

#define LAYOUT_STYLESHEET  \
    "QScrollArea{background-color:transparent;}"\
    "QListView{background:#5555ff;border-radius:0px; border: 0px solid gray;}" \
    "QListView::item:focus{border-radius:0px; border: 5px solid rgb(255, 128, 0);outline: none;}"\
    "QScrollBar:vertical {border:2px solid #a2a2a2;background:#F0F1EF;width:40px;margin:40 0 40 0px;}"\
    "QScrollBar::handle:vertical { border:1px solid lightgray;background:#f0f0f0;min-height:20px;}"\
    "QScrollBar::add-line:vertical {border:2px solid #a2a2a2;height:40px;subcontrol-position:bottom;subcontrol-origin:margin;}"\
    "QScrollBar::sub-line:vertical {border:2px solid #a2a2a2;height:40px;subcontrol-position:top;subcontrol-origin:margin;}"\
    "QScrollBar::down-arrow:vertical {width:12px;height:8px; border-image:url(:/image/down_Arrow.png);}"\
    "QScrollBar::up-arrow:vertical {width:12px;height:8px;border-image:url(:/image/up_Arrow.png);}"\
    "QScrollBar::add-page:vertical {background:lightgray;}"\
    "QScrollBar::sub-page:vertical {background:lightgray;}" \


wgtParaItem_ups::wgtParaItem_ups(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wgtParaItem_ups),
    m_curId(id)
{
    qRegisterMetaType<QList<ups_Info>>("QList<ups_Info>");
    qRegisterMetaType<ups_set>("ups_set");

    setupUi();
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevUpsList,this,&wgtParaItem_ups::getUpsInfo);
    connect(ui->btn_up, &QPushButton::clicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();

        int index = ui->tabWidget->currentIndex();
        if(index == 0)
            ui->scrollArea_2->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
        else if (index == 1) {
            ui->scrollArea->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
        }else if (index == 2) {
            ui->scrollArea_set->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
        }

        setGuideBtntVisible();
    });
    connect(ui->btn_down, &QPushButton::clicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();

        int index = ui->tabWidget->currentIndex();
        if(index == 0)
            ui->scrollArea_2->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
        else if (index == 1) {
            ui->scrollArea->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
        }else if (index == 2) {
            ui->scrollArea_set->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
        }

        setGuideBtntVisible();
    });

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index){
        Q_UNUSED(index);
        //setTabWidgetStyle();
        ShortBeep();

        setGuideBtntVisible();
    });

    connect(ui->scrollArea,     &MScrollArea::sigScrollVal, this, &wgtParaItem_ups::onScrollBarChange);
    connect(ui->scrollArea_2,   &MScrollArea::sigScrollVal, this, &wgtParaItem_ups::onScrollBarChange);
    connect(ui->scrollArea_set, &MScrollArea::sigScrollVal, this, &wgtParaItem_ups::onScrollBarChange);

    QTimer  *timer  = new QTimer();
    connect(timer, &QTimer::timeout, this, [=](){
        setGuideBtntVisible();
    });
    timer->start(500);
}

wgtParaItem_ups::~wgtParaItem_ups()
{
    delete ui;
}

void wgtParaItem_ups::setupUi(void)
{
    ui->setupUi(this);
    //m_lblDate = new wgtDateTime(ui->tabWidget);

    //状态
    ui->tabWidget->setTabText(0, /*tr("运行状态")*/langN.show[langEnum::strMtrSDSts]);
    ui->lblUpsNumberName->setText(/*tr("当前UPS编号：")*/langN.show[langEnum::strCurUpsNumber] + ":");
    ui->lblUpsNumberVal->setText(QString::number(m_curId));

    ui->lblUpsStaName->setText(/*tr("当前UPS工作状态：")*/langN.show[langEnum::strCurUpsStatus] + ":");
    setConnectState(1);
    createInfo();

    //参数
    ui->tabWidget->setTabText(1, /*tr("设备信息")*/langN.show[langEnum::strDevInfo]);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    createParaWidget();

    //设置
    ui->tabWidget->setTabText(2, /*"参数设置"*/langN.show[langEnum::strParaSet]);
    ui->scrollArea_set->setWidgetResizable(true);
    ui->scrollArea_set->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_set->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    createSetParaWidget();

    permissUpdate();
    ui->wgtRender->setVisible(false);
    //ui->tabWidget->setCurrentIndex(0);
    //setTabWidgetStyle();
}

void wgtParaItem_ups::createInfo(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtInfo);
    vLayout->setSpacing(15);
    vLayout->setContentsMargins(10, 5, 10, 10);

    //类别个数
    int cateNum  = FormMain::Instance()->m_upsList.classList.count();
    if(cateNum == 0)
        return;

    for(int i = 0; i < cateNum; i++)
    {
        //QString cateName = QString("%1").arg(FormMain::Instance()->m_upsList.classList.at(i));
        QString cateName = langN.show[FormMain::Instance()->m_upsList.classList.at(i)];
        //筛选状态
        QList<Menu>  menuList;
        foreach (Menu menu, FormMain::Instance()->m_upsList.menuList) {
            if(cateName == langN.show[menu.parameter_class]/*menu.parameter_class*/)
            {
                if(menu.parameter_type == TYPE_STATUS)
                    menuList << menu;
            }
        }

        if(menuList.isEmpty())
            continue;

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(0);
        gridlayout->setContentsMargins(0, 0, 0, 0);

        ActParaWidget *wgt  = new ActParaWidget(cateName, ui->wgtInfo);
        wgt->setObjectName(QStringLiteral("wgt"));
        wgt->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

        /*if(menuList.count() < 20)
            wgt->setStyle(STYLE_MIN);
        else if (menuList.count() < 50) {
            wgt->setStyle(STYLE_MID);
        }else {
            wgt->setStyle(STYLE_MAX);
        }*/

        for (int j = 0; j < menuList.count(); ++j)
        {
            QString paraName  = langN.show[menuList.at(j).show_name];//menuList.at(j).show_name;
            QString paraValue = QString::number(0);

            ActParaItem *actItem = new ActParaItem(paraName, paraValue, wgt->ui->wgtBody);
            actItem->setObjectName(menuList.at(j).parameter_name);

            if(j%ROW_COUNT)
                actItem->setWidgetStyle(1);
            /*else if(j == menuList.count()-1){
                actItem->setWidgetStyle(3);
            }*/else {
                actItem->setWidgetStyle(2);
            }
            gridlayout->addWidget(actItem, j/ROW_COUNT, j%ROW_COUNT);
        }

        if((menuList.count()%2))
        {
            ActParaItem *actItem = new ActParaItem("", "", wgt->ui->wgtBody);
            actItem->setWidgetStyle(1);
            gridlayout->addWidget(actItem, menuList.count()/ROW_COUNT, menuList.count()%ROW_COUNT);
        }

        //保存每个类别的参数
        ups_list list;
        list.para_type = TYPE_STATUS;
        list.menuList  = menuList;
        m_upsList.append(list);

        wgt->ui->wgtBody->setLayout(gridlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtInfo->resize(sizeHint());
}

int wgtParaItem_ups::findParaValue(QString para, int &Type)
{
    //按顺序查找
    QVector<QMetaEnum> vMeta;
    int index;
    vMeta.append(QMetaEnum::fromType<wgtParaItem_ups::UpsStsEnum>());
    vMeta.append(QMetaEnum::fromType<wgtParaItem_ups::UpsParaEnum>());
    vMeta.append(QMetaEnum::fromType<wgtParaItem_ups::UpsCfgEnum>());

    for(int i=0;i<vMeta.length();i++){
        index = vMeta.at(i).keysToValue(para.toLatin1().data());
        if(-1 != index){
            Type = i;
            return index;
        }
    }
    return  -1;
}

void wgtParaItem_ups::createParaWidget(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtContents);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(10, 10, 10, 10);

    //类别个数
    int cateNum  = FormMain::Instance()->m_upsList.classList.count();
    if(cateNum == 0)
        return;

    for(int i = 0; i < cateNum; i++)
    {
        //QString cateName = QString("%1").arg(FormMain::Instance()->m_upsList.classList.at(i));
        QString cateName = langN.show[FormMain::Instance()->m_upsList.classList.at(i)];

        //筛选只读的参数
        QList<Menu> menuList;
        foreach (Menu menu, FormMain::Instance()->m_upsList.menuList) {
            if(cateName == langN.show[menu.parameter_class]/*menu.parameter_class*/)
            {
                if(menu.parameter_type == TYPE_SETTING)
                    menuList << menu;
            }
        }

        if(menuList.isEmpty())
            continue;

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(0);
        gridlayout->setContentsMargins(0, 0, 0, 0);

        ActParaWidget *wgt  = new ActParaWidget(cateName, ui->wgtContents);
        wgt->setObjectName(QStringLiteral("wgt"));
        wgt->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

        if(menuList.count() < 20)
            wgt->setStyle(STYLE_MIN);
        else if (menuList.count() < 50) {
            wgt->setStyle(STYLE_MID);
        }else {
            wgt->setStyle(STYLE_MAX);
        }

        for (int j = 0; j < menuList.count(); ++j)
        {
            QString paraName  = langN.show[menuList.at(j).show_name];//menuList.at(j).show_name;
            QString paraValue = QString::number(0);

            ActParaItem *actItem = new ActParaItem(paraName, paraValue, wgt->ui->wgtBody);
            actItem->setObjectName(menuList.at(j).parameter_name);

            if(j%ROW_COUNT)
                actItem->setWidgetStyle(1);
            /*else if(j == menuList.count()-1){
                actItem->setWidgetStyle(3);
            }*/else {
                actItem->setWidgetStyle(2);
            }

            gridlayout->addWidget(actItem, j/ROW_COUNT, j%ROW_COUNT);
        }

        if((menuList.count()%2))
        {
            ActParaItem *actItem = new ActParaItem("", "", wgt->ui->wgtBody);
            actItem->setWidgetStyle(1);
            gridlayout->addWidget(actItem, menuList.count()/2, menuList.count()%2);
        }

        //保存每个类别的参数
        ups_list list;
        list.para_type = TYPE_SETTING;
        list.menuList  = menuList;
        m_upsList.append(list);

        wgt->ui->wgtBody->setLayout(gridlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtContents->resize(sizeHint());
}

void wgtParaItem_ups::clearAllWideget(void)
{
    QList<ActParaWidget*> wgts = ui->wgtContents->findChildren<ActParaWidget *>();
    if(!wgts.isEmpty())
    {
        foreach(ActParaWidget* wgt, wgts)
        {
            delete wgt;
        }
    }

    QList<QGridLayout*> glayout = ui->wgtContents->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<QVBoxLayout*> vlayout = ui->wgtContents->findChildren<QVBoxLayout *>();
    if(!vlayout.isEmpty())
    {
        foreach(QVBoxLayout* layout, vlayout)
        {
            delete layout;
        }
    }

    QList<ActParaWidget*> wgts2 = ui->wgtInfo->findChildren<ActParaWidget *>();
    if(!wgts2.isEmpty())
    {
        foreach(ActParaWidget* wgt, wgts2)
        {
            delete wgt;
        }
    }

    QList<QGridLayout*> glayout2 = ui->wgtInfo->findChildren<QGridLayout *>();
    if(!glayout2.isEmpty())
    {
        foreach(QGridLayout* layout, glayout2)
        {
            delete layout;
        }
    }

    QList<QVBoxLayout*> vlayout2 = ui->wgtInfo->findChildren<QVBoxLayout *>();
    if(!vlayout2.isEmpty())
    {
        foreach(QVBoxLayout* layout, vlayout2)
        {
            delete layout;
        }
    }

    if(!m_upsList.isEmpty())
        m_upsList.clear();
}

void wgtParaItem_ups::createSetParaWidget(void)
{
    setList.clear();
    foreach (Menu menu, FormMain::Instance()->m_upsList.menuList) {
        if(TYPE_PARA_SETTING == menu.parameter_type)
        {
            setList << menu;
        }
    }
    if(setList.count() == 0)
        return;

    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtSet);
    vLayout->setContentsMargins(10, 10, 10, 10);

    //类别个数
    int cateNum  = 1;
    for(int i = 0; i < cateNum; i++)
    {
        QString cateName = langN.show[setList.at(i).parameter_class];//QString("%1").arg(setList.at(i).parameter_class);

        QGridLayout *gridlayout = new QGridLayout();
        gridlayout->setSpacing(10);
        gridlayout->setContentsMargins(0, 5, 0, 5);

        ActParaWidget *wgt  = new ActParaWidget(cateName, ui->wgtSet);
        wgt->setObjectName(QStringLiteral("wgt"));
        wgt->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
        wgt->setTitleWidgetVisible(false);

        //参数个数
        for (int j = 0; j < setList.count(); ++j)
        {
            int input = setList.at(j).show_type;
            if(input == 0)
            {
                ActParaItem_lineEdit *actItem_lin = new ActParaItem_lineEdit(Ups, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_lin->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_lin, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_lin, &ActParaItem_lineEdit::signal_ups,
                        FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::wirteUpsDevData);
            }
            else if (input == 1) {
                ActParaItem_btn *actItem_btn = new ActParaItem_btn(Ups, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_btn->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_btn, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_btn, &ActParaItem_btn::signal_ups,
                        FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::wirteUpsDevData);
            }
            else if (input == 2) {
                ActParaItem_comboBox *actItem_com = new ActParaItem_comboBox(Ups, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_com->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_com, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_com, &ActParaItem_comboBox::signal_ups,
                        FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::wirteUpsDevData);
            }
            else if (input == 3) {
                ActParaItem_bit *actItem_bit = new ActParaItem_bit(Ups, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_bit->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_bit, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_bit, &ActParaItem_bit::signal_ups,
                        FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::wirteUpsDevData);
            }
        }

        wgt->ui->wgtBody->setLayout(gridlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtSet->resize(sizeHint());
}

void wgtParaItem_ups::setConnectState(int state)
{
    QString connect;
    QColor  color;
    switch (state) {
    case 0:
        connect = /*("无效")*/langN.show[langEnum::strInvalid];
        color   = Qt::red;
        break;
    case 1:
        connect = langN.show[langEnum::strDisconn];
        color   = Qt::red;
        break;
    case 2:
        connect = /*("已连接")*/langN.show[langEnum::strConnect];
        color   = QColor(45, 214, 218);
        break;
    default:
        connect = /*("无效")*/langN.show[langEnum::strInvalid];
        color   = Qt::red;
        break;
    }
    QPalette pe = ui->lblUpsStaVal->palette();
    pe.setColor(QPalette::WindowText, color);
    ui->lblUpsStaVal->setPalette(pe);
    ui->lblUpsStaVal->setText(connect);
}

void wgtParaItem_ups::setCurrentId(int id)
{
    if((id - 1) < 0)
        return;

    ui->lblUpsNumberVal->setText(QString::number(id));
    m_curId = id - 1;

    getUpsInfo(Ups_list);
    ui->tabWidget->setCurrentIndex(0);
}

void wgtParaItem_ups::getUpsInfo(QList<ups_Info> infoList)
{
    Ups_list = infoList;
    if((m_upsList.count() == 0) || (infoList.count() == 0))
        return;

    if(m_curId >= infoList.count())
        return;

    //设置连接状态
    setConnectState(infoList[m_curId].connect);
    ui->lblUpsNumberVal->setText(QString::number(infoList[m_curId].devID));

    //更新参数
    for (int i = 0; i < m_upsList.count(); i++)
    {
        if(m_upsList.at(i).para_type == TYPE_STATUS)
        {
            QList<Menu> menuList = m_upsList.at(i).menuList;
            for (int j = 0; j < menuList.count(); j++) {
                ActParaItem *item = ui->wgtInfo->findChild<ActParaItem *>(QString("%1").arg(menuList.at(j).parameter_name));
                if(item)
                {
                    if(menuList.at(j).parameter_name == "eUpsParaBypVoltPh")
                    {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypVoltPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypVoltPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaBypCurrPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypCurrPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypCurrPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaBypFreqPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypFreqPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypFreqPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaBypPfPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypPfPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypPfPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpVoltPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpVoltPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpVoltPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpCurrPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpCurrPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpCurrPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpFreqPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpFreqPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpFreqPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpPfPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpPfPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpPfPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpVoltPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpVoltPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpVoltPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpCurrPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpCurrPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpCurrPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpFreqPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpFreqPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpFreqPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpPfPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpPfPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpPfPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpKvaPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpKvaPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpKvaPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpKwPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpKwPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpKwPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaLdPerPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaLdPerPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaLdPerPhA]);
                    }else {
                        //Type value 0=sts,1=para,2=cfg
                        QString val = "";
                        int paraType = 0;
                        int index = findParaValue(menuList.at(j).parameter_name,paraType);
                        if(index == -1 ){ continue; }
                        switch (paraType) {
                        case 0:
                            val = infoList[m_curId].sts[index];
                            break;
                        case 1:
                            val = infoList[m_curId].para[index];
                            item->setUnit(infoList[m_curId].paraUnit[index]);
                            break;
                        case 2:
                            val = infoList[m_curId].cfgData[index];
                            item->setUnit(infoList[m_curId].cfgUnit[index]);
                            break;
                        }
                        item->setVal(val);
                    }
                }
            }
        }
        else if(m_upsList.at(i).para_type == TYPE_SETTING)
        {
            QList<Menu> menuList = m_upsList.at(i).menuList;
            for (int j = 0; j < menuList.count(); j++) {
                ActParaItem *item = ui->wgtContents->findChild<ActParaItem *>(QString("%1").arg(menuList.at(j).parameter_name));
                if(item)
                {
                    if(menuList.at(j).parameter_name == "eUpsParaBypVoltPh")
                    {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypVoltPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypVoltPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaBypCurrPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypCurrPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypCurrPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaBypFreqPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypFreqPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypFreqPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaBypPfPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaBypPfPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaBypPfPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpVoltPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpVoltPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpVoltPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpCurrPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpCurrPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpCurrPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpFreqPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpFreqPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpFreqPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaIpPfPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaIpPfPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaIpPfPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpVoltPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpVoltPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpVoltPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpCurrPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpCurrPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpCurrPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpFreqPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpFreqPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpFreqPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpPfPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpPfPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpPfPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpKvaPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpKvaPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpKvaPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaOpKwPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaOpKwPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaOpKwPhA]);
                    }
                    else if (menuList.at(j).parameter_name == "eUpsParaLdPerPh") {
                        item->setVal(infoList[m_curId].meterD.eUpsParaLdPerPh);
                        item->setUnit(infoList[m_curId].paraUnit[eUpsParaLdPerPhA]);
                    }
                    else if(menuList.at(j).parameter_name == "manufacturer0")//厂家名称
                    {
                        item->setVal(infoList[m_curId].inf.manufacturer);
                    }//版本
                    else if (menuList.at(j).parameter_name == "MtrVer0") {
                        item->setVal(infoList[m_curId].inf.MtrVer);
                    }//序列号
                    else if (menuList.at(j).parameter_name == "series0") {
                        item->setVal(infoList[m_curId].inf.series);
                    }//UPS型号
                    else if (menuList.at(j).parameter_name == "modelName0") {
                        item->setVal(infoList[m_curId].inf.modelName);
                    }
                    else {
                        //Type value 0=sts,1=para,2=cfg
                        QString val = "";
                        int paraType = 1;
                        int index = findParaValue(menuList.at(j).parameter_name,paraType);
                        if(index == -1 ){ continue; }
                        switch (paraType) {
                        case 0:
                            val = infoList[m_curId].sts[index];
                            break;
                        case 1:
                            val = infoList[m_curId].para[index];
                            item->setUnit(infoList[m_curId].paraUnit[index]);
                            break;
                        case 2:
                            val = infoList[m_curId].cfgData[index];
                            item->setUnit(infoList[m_curId].cfgUnit[index]);
                            break;
                        }

                        item->setVal(val);
                    }
                }
            }
        }
        /*else if(m_upsList.at(i).cateName == TYPE_SETTING)
        {
            QList<Menu> menuList = m_upsList.at(i).menuList;
            for (int j = 0; j < menuList.count(); j++) {
                ActParaItem *item = ui->wgtContents->findChild<ActParaItem *>(QString("%1").arg(menuList.at(j).parameter_name));
                if(item)
                {
                    //厂家名称
                    if(menuList.at(j).parameter_name == "manufacturer0")
                    {
                        item->setVal(infoList[m_curId].inf.manufacturer);
                    }//版本
                    else if (menuList.at(j).parameter_name == "MtrVer0") {
                        item->setVal(infoList[m_curId].inf.MtrVer);
                    }//序列号
                    else if (menuList.at(j).parameter_name == "series0") {
                        item->setVal(infoList[m_curId].inf.series);
                    }//UPS型号
                    else if (menuList.at(j).parameter_name == "modelName0") {
                        item->setVal(infoList[m_curId].inf.modelName);
                    }
                    else{
                        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ups::UpsParaEnum>();
                        //把参数对应的值转换成枚举值
                        int index   = metaEnum.keysToValue(QString("%1").arg(menuList.at(j).parameter_name).toLatin1().data());
                        if(-1 == index)
                            continue;

                        QString val = infoList[m_curId].cfgData[index];
                        item->setVal(val);
                    }
                }
            }
        }*/
    }

#if 1
    for (int i = 0; i < setList.count(); i++) {
        if(setList.at(i).show_type == 0)
        {
            ActParaItem_lineEdit *item = ui->wgtSet->findChild<ActParaItem_lineEdit *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ups::UpsCtlEnum>();
                //把参数对应的值转换成枚举值
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 == index)
                    continue;

                item->setDevId(infoList[m_curId].devID - 1);
                //QString val = infoList[m_curId].paraData[index];
                //item->setValue(val);
            }
        }
        else if (setList.at(i).show_type == 1) {
            ActParaItem_btn *item = ui->wgtSet->findChild<ActParaItem_btn *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ups::UpsCtlEnum>();
                //把参数对应的值转换成枚举值
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 == index)
                    continue;

                item->setDevId(infoList[m_curId].devID - 1);

                //QString val = infoList[m_curId].paraData[index];
                //item->setValue(val);
            }
        }
        else if (setList.at(i).show_type == 2) {
            ActParaItem_comboBox *item = ui->wgtSet->findChild<ActParaItem_comboBox *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ups::UpsCtlEnum>();
                //把参数对应的值转换成枚举值
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 == index)
                    continue;

                item->setDevId(infoList[m_curId].devID - 1);
                //QString val = infoList[m_curId].paraData[index];
                //item->setValue(val);
            }
        }
        else if (setList.at(i).show_type == 3) {
            ActParaItem_bit *item = ui->wgtSet->findChild<ActParaItem_bit *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_ups::UpsCtlEnum>();
                //把参数对应的值转换成枚举值
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 == index)
                    continue;

                item->setDevId(infoList[m_curId].devID - 1);
                //QString val = infoList[m_curId].paraData[index];
                //item->setValue(val);
            }
        }
    }
#endif
}



void wgtParaItem_ups::clearSetWideget(void)
{
    QList<ActParaWidget*> wgts = ui->wgtSet->findChildren<ActParaWidget *>();
    if(!wgts.isEmpty())
    {
        foreach(ActParaWidget* wgt, wgts)
        {
            delete wgt;
        }
    }

    QList<QGridLayout*> glayout = ui->wgtSet->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    QList<QVBoxLayout*> vlayout = ui->wgtSet->findChildren<QVBoxLayout *>();
    if(!vlayout.isEmpty())
    {
        foreach(QVBoxLayout* layout, vlayout)
        {
            delete layout;
        }
    }
}

void  wgtParaItem_ups::permissUpdate(void)
{
    if(FormMain::Instance()->m_loginInfo.from_role != "role003")
    {
        ui->tabWidget->removeTab(2);
    }else {
        ui->tabWidget->insertTab(2, ui->tab_set, /*QObject::tr("参数设置")*/langN.show[langEnum::strParaSet]);
    }
}

bool wgtParaItem_ups::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        //只读参数
        clearAllWideget();
        createInfo();
        createParaWidget();

        //设置参数
        clearSetWideget();
        createSetParaWidget();

        permissUpdate();
        return true;
    }

    return QWidget::event(ev);
}

void wgtParaItem_ups::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
}

void wgtParaItem_ups::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
}

void wgtParaItem_ups::onScrollBarChange(void)
{
    setGuideBtntVisible();
}

void  wgtParaItem_ups::setGuideBtntVisible()
{
    int scrollBar_val = 0, scrollBar_min = 0, scrollBar_max = 0;
    int cur_index = ui->tabWidget->currentIndex();
    if(cur_index == 0)
    {
        scrollBar_val = ui->scrollArea_2->verticalScrollBar()->value();
        scrollBar_min = ui->scrollArea_2->verticalScrollBar()->minimum();
        scrollBar_max = ui->scrollArea_2->verticalScrollBar()->maximum();
    }else if (cur_index == 1) {
        scrollBar_val = ui->scrollArea->verticalScrollBar()->value();
        scrollBar_min = ui->scrollArea->verticalScrollBar()->minimum();
        scrollBar_max = ui->scrollArea->verticalScrollBar()->maximum();
    }else if (cur_index == 2) {
        scrollBar_val = ui->scrollArea_set->verticalScrollBar()->value();
        scrollBar_min = ui->scrollArea_set->verticalScrollBar()->minimum();
        scrollBar_max = ui->scrollArea_set->verticalScrollBar()->maximum();
    }

    if((scrollBar_val == scrollBar_min) && (scrollBar_val == scrollBar_max))
    {
        ui->btn_up->setVisible(false);
        ui->btn_down->setVisible(false);
    }else if (scrollBar_val == scrollBar_max) {
        ui->btn_up->setVisible(true);
        ui->btn_down->setVisible(false);
    }else if (scrollBar_val == scrollBar_min){
        ui->btn_up->setVisible(false);
        ui->btn_down->setVisible(true);
    }else {
        ui->btn_up->setVisible(true);
        ui->btn_down->setVisible(true);
    }
}

void wgtParaItem_ups::setTabWidgetStyle(void)
{
    QString str;
    int index = ui->tabWidget->currentIndex();

    if(index == 0)
        str = "url(:/image/tab_border.png)";
    else if (index == 1) {
        str = "url(:/image/tab_border1.png)";
    }
    else if (index == 2) {
        str = "url(:/image/tab_border2.png)";
    }

    ui->tabWidget->setStyleSheet(QString("QTabWidget::pane{"
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














