#include "wgtparaitem_meter.h"
#include "ui_wgtparaitem_meter.h"
#include "Formmain/formmain.h"
#include "actparaitem_lineedit.h"
#include "actparaitem_combobox.h"
#include "actparaitem_bit.h"
#include "actparaitem.h"
#include "MeterData/MeterDataStruct.h"
#include "actparawidget.h"
#include "ui_actparawidget.h"
#include "wgtdatetime.h"
#include "buzzer.h"
#include "flowlayout.h"
#include <QGroupBox>
#include <QPushButton>
#include "wgtmenubar.h"
#include <QDebug>
#include <QTimer>

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


wgtParaItem_meter::wgtParaItem_meter(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wgtParaItem_meter),
    m_curId(id)
{
    qRegisterMetaType<QList<meter_Info>>("QList<meter_Info>");

    setupUi();
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevMeterList,this,&wgtParaItem_meter::getMeterInfo);
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

    connect(ui->scrollArea,     &MScrollArea::sigScrollVal, this, &wgtParaItem_meter::onScrollBarChange);
    connect(ui->scrollArea_2,   &MScrollArea::sigScrollVal, this, &wgtParaItem_meter::onScrollBarChange);
    connect(ui->scrollArea_set, &MScrollArea::sigScrollVal, this, &wgtParaItem_meter::onScrollBarChange);

    QTimer  *timer  = new QTimer();
    connect(timer, &QTimer::timeout, this, [=](){
        setGuideBtntVisible();
    });
    timer->start(500);
}

wgtParaItem_meter::~wgtParaItem_meter()
{
    delete ui;
}

void wgtParaItem_meter::setupUi(void)
{
    ui->setupUi(this);
    //m_lblDate = new wgtDateTime(ui->tabWidget);

    //状态
    ui->tabWidget->setTabText(0, /*tr("运行状态")*/langN.show[langEnum::strMtrSDSts]);
    ui->lblMeterNumberName->setText(/*tr("当前配电编号：")*/langN.show[langEnum::strCurMeterNumber] + ":");
    ui->lblMeterNumberVal->setText(QString::number(m_curId));

    ui->lblMeterStaName->setText(/*tr("当前配电工作状态：")*/langN.show[langEnum::strCurMeterStatus] + ":");
    setConnectState(1);
    createInfo();

    //参数
    ui->tabWidget->setTabText(1, /*tr("设备信息")*/langN.show[langEnum::strDevInfo]);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //createParaWidget();

    //设置
    ui->tabWidget->setTabText(2, /*"参数设置"*/langN.show[langEnum::strParaSet]);
    ui->scrollArea_set->setWidgetResizable(true);
    ui->scrollArea_set->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_set->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //createSetParaWidget();

    //ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(1);
    //permissUpdate();
    ui->wgtRender->setVisible(false);
    //setTabWidgetStyle();
}

void wgtParaItem_meter::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
}

void wgtParaItem_meter::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
}

void wgtParaItem_meter::createInfo(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtInfo);
    vLayout->setSpacing(15);
    vLayout->setContentsMargins(10, 5, 10, 10);

    //类别个数
    int cateNum  = FormMain::Instance()->m_meterList.classList.count();
    if(cateNum == 0)
        return;

    for(int i = 0; i < cateNum; i++)
    {
        //QString cateName = QString("%1").arg(FormMain::Instance()->m_meterList.classList.at(i));
        QString cateName = langN.show[FormMain::Instance()->m_meterList.classList.at(i)];
        //筛选状态
        QList<Menu>  menuList;
        foreach (Menu menu, FormMain::Instance()->m_meterList.menuList) {
            if(cateName == langN.show[menu.parameter_class]/*menu.parameter_class*/)
            {
                if(menu.parameter_type == TYPE_STATUS)
                    menuList << menu;
            }
        }

        if(menuList.isEmpty())
            continue;

        QFlowLayout *flowlayout = new QFlowLayout(0,0,0);

        ActParaWidget *wgt  = new ActParaWidget(cateName, ui->wgtInfo);
        wgt->setObjectName(QStringLiteral("wgt"));
        wgt->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

        for (int j = 0; j < menuList.count(); ++j)
        {
            QString paraName  = langN.show[menuList.at(j).show_name];//menuList.at(j).show_name;
            QString paraValue = QString::number(0);

            ActParaItem *actItem = new ActParaItem(paraName, paraValue, wgt->ui->wgtBody);
            actItem->setObjectName(menuList.at(j).parameter_name);
            actItem->setMinimumWidth((wgt->size().width() + 186)/2);
            actItem->setMaximumWidth((wgt->size().width() + 186)/2);

            actItem->setWidgetStyle(1);
            flowlayout->addWidget(actItem);
            itemList.append(actItem);
        }

        if((menuList.count()))
        {
            ActParaItem *actItem = new ActParaItem("", "", wgt->ui->wgtBody);
            actItem->setMinimumWidth((wgt->size().width() + 186)/2);
            actItem->setMaximumWidth((wgt->size().width() + 186)/2);
            actItem->setWidgetStyle(1);
            actItem->setVisible(false);
            flowlayout->addWidget(actItem);
            itemList.append(actItem);
        }

        //保存每个类别的参数
        ups_list list;
        list.para_type = TYPE_STATUS;
        list.menuList  = menuList;
        m_meterList.append(list);

        wgt->ui->wgtBody->setLayout(flowlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtInfo->resize(sizeHint());
}

void wgtParaItem_meter::createParaWidget(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtContents);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(10, 10, 10, 10);

    //类别个数
    int cateNum  = FormMain::Instance()->m_meterList.classList.count();
    if(cateNum == 0)
        return;

    for(int i = 0; i < cateNum; i++)
    {
        //QString cateName = QString("%1").arg(FormMain::Instance()->m_meterList.classList.at(i));
        QString cateName = langN.show[FormMain::Instance()->m_meterList.classList.at(i)];

        //筛选只读的参数
        QList<Menu> menuList;
        foreach (Menu menu, FormMain::Instance()->m_meterList.menuList) {
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
            wgt->setStyle(STYLE_MIN);
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
            else {
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
        m_meterList.append(list);

        wgt->ui->wgtBody->setLayout(gridlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtContents->resize(sizeHint());
}

void wgtParaItem_meter::clearAllWideget(void)
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

    if(!m_meterList.isEmpty())
        m_meterList.clear();

    if(!itemList.isEmpty())
        itemList.clear();
}

void wgtParaItem_meter::createSetParaWidget(void)
{
    QList<Menu> setList;
    foreach (Menu menu, FormMain::Instance()->m_meterList.menuList) {
        if(menu.parameter_type == TYPE_PARA_SETTING)
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
        //QString cateName = QString("%1").arg(setList.at(i).parameter_class);
        QString cateName = langN.show[setList.at(i).parameter_class];

        QGridLayout *gridlayout = new QGridLayout();
        QGroupBox* groupBox     = new QGroupBox(cateName, ui->wgtSet);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding));
        groupBox->setStyleSheet("color:rgb(255, 255, 255);");

        //参数个数
        for (int j = 0; j < setList.count(); ++j)
        {
            QString paraName  = langN.show[setList.at(j).show_name];//setList.at(j).show_name;
            QString paraValue = QString::number(0/*(pMeterData + m_curId)->para.data[j]*/);

            ActParaItem *actItem = new ActParaItem(paraName, paraValue, groupBox);
            actItem->setObjectName(setList.at(j).parameter_name);

            /*if(j%2)
            {
                ActParaItem_lineEdit *actItem = new ActParaItem_lineEdit(DevTypeEnum::Meter, m_curId, j, paraName, paraValue, groupBox);
                gridlayout->addWidget(actItem, j/ROW_COUNT, j%ROW_COUNT);
            }
            else if(j%3)
            {
                ActParaItem_comboBox *actItem = new ActParaItem_comboBox(DevTypeEnum::Ths, m_curId, j, paraName, paraValue.toInt(), groupBox);
                gridlayout->addWidget(actItem, j/ROW_COUNT, j%ROW_COUNT);
            }
            else
            {
                ActParaItem_bit *actItem = new ActParaItem_bit(DevTypeEnum::Ths, m_curId, j, paraName, paraValue, groupBox);
                gridlayout->addWidget(actItem, j/ROW_COUNT, j%ROW_COUNT);
            }*/
            gridlayout->addWidget(actItem, j/ROW_COUNT, j%ROW_COUNT);
        }
        groupBox->setLayout(gridlayout);
        vLayout->addWidget(groupBox);
    }
    vLayout->addStretch();

    ui->wgtSet->resize(sizeHint());
}

void wgtParaItem_meter::clearSetWideget(void)
{
    QList<QGroupBox*> boxs = ui->wgtSet->findChildren<QGroupBox *>();
    if(!boxs.isEmpty())
    {
        foreach(QGroupBox* box, boxs)
        {
            delete box;
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

void wgtParaItem_meter::setConnectState(int state)
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
    QPalette pe = ui->lblMeterStaVal->palette();
    pe.setColor(QPalette::WindowText, color);
    ui->lblMeterStaVal->setPalette(pe);
    ui->lblMeterStaVal->setText(connect);
}

void wgtParaItem_meter::setCurrentId(int id)
{
    if((id - 1) < 0)
        return;

    ui->lblMeterNumberVal->setText(QString::number(id));
    m_curId = id - 1;

    getMeterInfo(meter_list);
    ui->tabWidget->setCurrentIndex(0);
}

void wgtParaItem_meter::getMeterInfo(QList<meter_Info> infoList)
{
    meter_list = infoList;
    if((m_meterList.count() == 0) || (infoList.count() == 0))
        return;

    if(m_curId >= infoList.count())
        return;

    //设置连接状态
    setConnectState(infoList[m_curId].connect);
    ui->lblMeterNumberVal->setText(QString::number(infoList[m_curId].devID));

#if 1
    //更新参数
    for (int i = 0; i < m_meterList.count(); i++) {
        if(m_meterList.at(i).para_type == TYPE_STATUS)
        {
            QList<Menu> menuList = m_meterList.at(i).menuList;
            for (int j = 0; j < menuList.count(); j++) {
                ActParaItem *item = ui->wgtInfo->findChild<ActParaItem *>(QString("%1").arg(menuList.at(j).parameter_name));
                if(item)
                {
                    //总有功电量
                    if(menuList.at(j).parameter_name == "eMeterParaKWH"){
                        if(infoList[m_curId].eMeterParaKWH.toInt() == HIDEITEMVALUE)
                            item->setVisible(false);
                        else
                        {
                            item->setVisible(true);
                            item->setVal(infoList[m_curId].eMeterParaKWH);
                        }
                    }else if (menuList.at(j).parameter_name == "eMeterParaKVarH") { //总无功电量
                        if(infoList[m_curId].eMeterParaKVarH == HIDEITEMVALUE)
                            item->setVisible(false);
                        else
                        {
                            item->setVisible(true);
                            item->setVal(infoList[m_curId].eMeterParaKVarH);
                        }
                    }else{
                        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_meter::MeterParaEnum>();
                        //把参数对应的值转换成枚举值
                        int index   = metaEnum.keysToValue(QString("%1").arg(menuList.at(j).parameter_name).toLatin1().data());
                        if(-1 != index)
                        {
                            QString val = infoList[m_curId].data[index];
                            if(val.toInt() == HIDEITEMVALUE)
                                item->setVisible(false);
                            else
                            {
                                item->setVisible(true);
                                item->setVal(val);
                                item->setUnit(infoList[m_curId].dataUnit[index]);
                            }
                        }
                    }
                }
            }
        }else if (m_meterList.at(i).para_type == TYPE_SETTING) {
            QList<Menu> menuList = m_meterList.at(i).menuList;
            for (int j = 0; j < menuList.count(); j++) {
                ActParaItem *item = ui->wgtContents->findChild<ActParaItem *>(QString("%1").arg(menuList.at(j).parameter_name));
                if(item)
                {
                    //总有功电量
                    if(menuList.at(j).parameter_name == "eMeterParaKWH"){
                        item->setVal(infoList[m_curId].eMeterParaKWH);
                    }else if (menuList.at(j).parameter_name == "eMeterParaKVarH") { //总无功电量
                        item->setVal(infoList[m_curId].eMeterParaKVarH);
                    }else{
                        QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_meter::MeterParaEnum>();
                        //把参数对应的值转换成枚举值
                        int index   = metaEnum.keysToValue(QString("%1").arg(menuList.at(j).parameter_name).toLatin1().data());
                        if(-1 != index)
                        {
                            QString val = infoList[m_curId].data[index];
                            item->setVal(val);
                            item->setUnit(infoList[m_curId].dataUnit[index]);
                        }
                    }
                }
            }
        }
    }

    if(itemList.isEmpty())
        return;

    int enable_num = 0;
    for (int i = 0; i < itemList.count() - 1; i++) {
        if(itemList.at(i)->isVisible() == true)
            enable_num++;
    }
    if(enable_num%2)
        itemList.last()->setVisible(true);
    else {
        itemList.last()->setVisible(false);
    }

    ActParaWidget *wgt = ui->wgtInfo->findChild<ActParaWidget *>(QString("%1").arg("wgt"));
    if(wgt){
        if(enable_num < 10)
            wgt->setStyle(STYLE_MIN);
        else if (enable_num < 30) {
            wgt->setStyle(STYLE_MID);
        }else {
            wgt->setStyle(STYLE_MAX);
        }
    }

#endif
}

void  wgtParaItem_meter::permissUpdate(void)
{
    if(FormMain::Instance()->m_loginInfo.from_role != "role003")
    {
        ui->tabWidget->removeTab(2);
    }else {
        ui->tabWidget->insertTab(2, ui->tab_set, /*QObject::tr("参数设置")*/langN.show[langEnum::strParaSet]);
    }
}

bool wgtParaItem_meter::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        clearAllWideget();
        createInfo();
        //createParaWidget();

        //clearSetWideget();
        //createSetParaWidget();

        //permissUpdate();
        return true;
    }

    return QWidget::event(ev);
}

void wgtParaItem_meter::onScrollBarChange(void)
{
    setGuideBtntVisible();
}

void  wgtParaItem_meter::setGuideBtntVisible()
{
    int scrollBar_val = 0, scrollBar_min = 0, scrollBar_max = 0;
    int cur_index = ui->tabWidget->currentIndex();
    if(cur_index == 0){
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
    }else if ((scrollBar_val > scrollBar_min) && (scrollBar_val == scrollBar_max)) {
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

void wgtParaItem_meter::setTabWidgetStyle(void)
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












