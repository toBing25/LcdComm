#include "wgtparaitem_eleak.h"
#include "ui_wgtparaitem_eleak.h"
#include "Formmain/formmain.h"
#include "actparaitem_lineedit.h"
#include "actparaitem_combobox.h"
#include "actparaitem_bit.h"
#include "wgtmenubar.h"
#include "actparaitem.h"
#include "Eleak/LeakDataStruct.h"
#include "actparawidget.h"
#include "ui_actparawidget.h"
#include "actparaitem_btn.h"
#include "wgtdatetime.h"
#include "buzzer.h"
#include <QGroupBox>
#include <QPushButton>
#include <QDebug>
#include <QMetaEnum>
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

wgtParaItem_eLeak::wgtParaItem_eLeak(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wgtParaItem_eLeak),
    m_curId(id)
{
    qRegisterMetaType<QList<leak_Info>>("QList<leak_Info>");
    qRegisterMetaType<leak_set>("leak_set");

    setupUi();
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevLeakList,this,&wgtParaItem_eLeak::getLeakInfo);
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

    connect(ui->scrollArea,     &MScrollArea::sigScrollVal, this, &wgtParaItem_eLeak::onScrollBarChange);
    connect(ui->scrollArea_2,   &MScrollArea::sigScrollVal, this, &wgtParaItem_eLeak::onScrollBarChange);
    connect(ui->scrollArea_set, &MScrollArea::sigScrollVal, this, &wgtParaItem_eLeak::onScrollBarChange);

    QTimer  *timer  = new QTimer();
    connect(timer, &QTimer::timeout, this, [=](){
        setGuideBtntVisible();
    });
    timer->start(500);
}

wgtParaItem_eLeak::~wgtParaItem_eLeak()
{
    delete ui;
}

void wgtParaItem_eLeak::setupUi(void)
{
    ui->setupUi(this);
    //m_lblDate = new wgtDateTime(ui->tabWidget);

    //??????
    ui->tabWidget->setTabText(0, /*tr("????????????")*/langN.show[langEnum::strMtrSDSts]);
    ui->lblEleakNumberName->setText(/*tr("?????????????????????")*/langN.show[langEnum::strCurLeakNumber] + ":");
    ui->lblEleakNumberVal->setText(QString::number(m_curId));

    ui->lblEleakStaName->setText(/*tr("???????????????????????????")*/langN.show[langEnum::strCurConnectStatus] + ":");
    setConnectState(1);
    createInfo();

    //??????
    ui->tabWidget->setTabText(1, /*tr("????????????")*/langN.show[langEnum::strDevInfo]);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    createParaWidget();

    //??????
    ui->tabWidget->setTabText(2, /*"????????????"*/langN.show[langEnum::strParaSet]);
    ui->scrollArea_set->setWidgetResizable(true);
    ui->scrollArea_set->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_set->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    createSetParaWidget();

    //ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->removeTab(1);
    permissUpdate();
    ui->wgtRender->setVisible(false);
    //setTabWidgetStyle();
}

void wgtParaItem_eLeak::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
    setGuideBtntVisible();
}

void wgtParaItem_eLeak::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
}

void wgtParaItem_eLeak::createInfo(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtInfo);
    vLayout->setSpacing(15);
    vLayout->setContentsMargins(10, 5, 10, 10);

    //????????????
    int cateNum  = FormMain::Instance()->m_eleakList.classList.count();
    if(cateNum == 0)
        return;

    for(int i = 0; i < cateNum; i++)
    {
        QString cateName = langN.show[FormMain::Instance()->m_eleakList.classList.at(i)];//QString("%1").arg(FormMain::Instance()->m_eleakList.classList.at(i));
        //????????????
        QList<Menu>  menuList;
        foreach (Menu menu, FormMain::Instance()->m_eleakList.menuList) {
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
            else {
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

        //???????????????????????????
        ups_list list;
        list.para_type = TYPE_STATUS;
        list.menuList  = menuList;
        m_leakList.append(list);

        wgt->ui->wgtBody->setLayout(gridlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtInfo->resize(sizeHint());
}

void wgtParaItem_eLeak::createParaWidget(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtContents);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(10, 10, 10, 10);

    //????????????
    int cateNum  = FormMain::Instance()->m_eleakList.classList.count();
    if(cateNum == 0)
        return;

    for(int i = 0; i < cateNum; i++)
    {
        //QString cateName = QString("%1").arg(FormMain::Instance()->m_eleakList.classList.at(i));
        QString cateName = langN.show[FormMain::Instance()->m_eleakList.classList.at(i)];//QString("%1").arg(FormMain::Instance()->m_eleakList.classList.at(i));
        //?????????????????????
        QList<Menu> menuList;
        foreach (Menu menu, FormMain::Instance()->m_eleakList.menuList) {
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

        //???????????????????????????
        ups_list list;
        list.para_type = TYPE_SETTING;
        list.menuList = menuList;
        m_leakList.append(list);

        wgt->ui->wgtBody->setLayout(gridlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtContents->resize(sizeHint());
}

void wgtParaItem_eLeak::clearAllWideget(void)
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

    if(!m_leakList.isEmpty())
        m_leakList.clear();
}

void wgtParaItem_eLeak::createSetParaWidget(void)
{
    setList.clear();
    foreach (Menu menu, FormMain::Instance()->m_eleakList.menuList) {
        if(menu.parameter_type == TYPE_PARA_SETTING)
        {
            setList << menu;
        }
    }
    if(setList.count() == 0)
        return;

    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtSet);
    vLayout->setContentsMargins(10, 10, 10, 10);

    //????????????
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

        //????????????
        for (int j = 0; j < setList.count(); ++j)
        {
            int input = setList.at(j).show_type;
            if(input == 0)
            {
                ActParaItem_lineEdit *actItem_lin = new ActParaItem_lineEdit(Leak, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_lin->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_lin, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_lin, &ActParaItem_lineEdit::signal_leak,
                    FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::writeLeakDevData);
            }
            else if (input == 1) {
                ActParaItem_btn *actItem_btn = new ActParaItem_btn(Leak, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_btn->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_btn, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_btn, &ActParaItem_btn::signal_leak,
                    FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::writeLeakDevData);
            }
            else if (input == 2) {
                ActParaItem_comboBox *actItem_com = new ActParaItem_comboBox(Leak, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_com->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_com, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_com, &ActParaItem_comboBox::signal_leak,
                    FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::writeLeakDevData);
            }
            else if (input == 3) {
                ActParaItem_bit *actItem_bit = new ActParaItem_bit(Leak, m_curId, setList.at(j), wgt->ui->wgtBody);
                actItem_bit->setObjectName(setList.at(j).parameter_name);
                gridlayout->addWidget(actItem_bit, j/ROW_COUNT, j%ROW_COUNT);
                connect(actItem_bit, &ActParaItem_bit::signal_leak,
                    FormMain::Instance()->getShareMemManage()->getWriteData(), &WtriteShareMem::writeLeakDevData);
            }
        }

        wgt->ui->wgtBody->setLayout(gridlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtSet->resize(sizeHint());
}

void wgtParaItem_eLeak::clearSetWideget(void)
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

void wgtParaItem_eLeak::setConnectState(int state)
{
    QString connect;
    QColor  color;
    switch (state) {
    case 0:
        connect = /*("??????")*/langN.show[langEnum::strInvalid];
        color   = Qt::red;
        break;
    case 1:
        connect = langN.show[langEnum::strDisconn];
        color   = Qt::red;
        break;
    case 2:
        connect = /*("?????????")*/langN.show[langEnum::strConnect];
        color   = QColor(45, 214, 218);
        break;
    default:
        connect = /*("??????")*/langN.show[langEnum::strInvalid];
        color   = Qt::red;
        break;
    }
    QPalette pe = ui->lblEleakStaVal->palette();
    pe.setColor(QPalette::WindowText, color);
    ui->lblEleakStaVal->setPalette(pe);
    ui->lblEleakStaVal->setText(connect);
}

void wgtParaItem_eLeak::setCurrentId(int id)
{
    if((id - 1) < 0)
        return;

    ui->lblEleakNumberVal->setText(QString::number(id));
    m_curId = id - 1;

    getLeakInfo(leak_list);
    ui->tabWidget->setCurrentIndex(0);
}

void wgtParaItem_eLeak::getLeakInfo(QList<leak_Info> infoList)
{
    leak_list = infoList;
    if((m_leakList.count() == 0) || (infoList.count() == 0))
        return;

    if(m_curId >= infoList.count())
        return;

    //??????????????????
    setConnectState(infoList[m_curId].connect);
    ui->lblEleakNumberVal->setText(QString::number(infoList[m_curId].devID));

    //????????????
    for (int i = 0; i < m_leakList.count(); i++)
    {
        if(m_leakList.at(i).para_type == TYPE_STATUS)
        {
            QList<Menu> menuList = m_leakList.at(i).menuList;
            for (int j = 0; j < menuList.count(); j++) {
                ActParaItem *item = ui->wgtInfo->findChild<ActParaItem *>(QString("%1").arg(menuList.at(j).parameter_name));
                if(item)
                {
                    QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                    //???????????????????????????????????????
                    int index   = metaEnum.keysToValue(QString("%1").arg(menuList.at(j).parameter_name).toLatin1().data());
                    if(-1 != index)
                    {
                        QString val = infoList[m_curId].data[index];
                        item->setVal(val);
                    }//?????????
                    /*else {
                        metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                        index    = metaEnum.keysToValue(QString("%1").arg(setList.at(j).parameter_name).toLatin1().data());
                        if(-1 == index)
                            continue;

                        QString val = infoList[m_curId].data[index];
                        item->setVal(val);
                    }*/

                }
            }
        }
    }

#if 1
    for (int i = 0; i < setList.count(); i++) {
        if(setList.at(i).show_type == 0)
        {
            ActParaItem_lineEdit *item = ui->wgtSet->findChild<ActParaItem_lineEdit *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                //???????????????????????????????????????
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 != index)
                {
                    QString val = infoList[m_curId].data[index];
                    item->setDevId(infoList[m_curId].devID);
                    item->setValue(val);
                    item->setUnit(infoList[m_curId].dataUnit[index]);
                }
                /*else {
                    metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                    index    = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                    if(-1 == index)
                        continue;

                    QString val = infoList[m_curId].data[index];
                    item->setValue(val);
                }*/

            }
        }
        else if (setList.at(i).show_type == 1) {
            ActParaItem_btn *item = ui->wgtSet->findChild<ActParaItem_btn *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                //???????????????????????????????????????
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 != index)
                {
                    QString val = infoList[m_curId].data[index];
                    item->setDevId(infoList[m_curId].devID);
                    item->setValue(val);
                    item->setUnit(infoList[m_curId].dataUnit[index]);
                }
                /*else {
                    metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                    index    = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                    if(-1 == index)
                        continue;

                    QString val = infoList[m_curId].data[index];
                    item->setValue(val);
                }*/
            }
        }
        else if (setList.at(i).show_type == 2) {
            ActParaItem_comboBox *item = ui->wgtSet->findChild<ActParaItem_comboBox *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                //???????????????????????????????????????
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 != index)
                {
                    QString val = infoList[m_curId].data[index];
                    item->setDevId(infoList[m_curId].devID);
                    item->setValue(val);
                    item->setUnit(infoList[m_curId].dataUnit[index]);
                }
                /*else {
                    metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                    index    = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                    if(-1 == index)
                        continue;

                    QString val = infoList[m_curId].data[index];
                    item->setValue(val);
                }*/
            }
        }
        else if (setList.at(i).show_type == 3) {
            ActParaItem_bit *item = ui->wgtSet->findChild<ActParaItem_bit *>(QString("%1").arg(setList.at(i).parameter_name));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                //???????????????????????????????????????
                int index   = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                if(-1 != index)
                {
                    QString val = infoList[m_curId].data[index];
                    item->setDevId(infoList[m_curId].devID - 1);
                    item->setValue(val);
                    item->setUnit(infoList[m_curId].dataUnit[index]);
                }
                /*else {
                    metaEnum = QMetaEnum::fromType<wgtParaItem_eLeak::LeakParaEnum>();
                    index    = metaEnum.keysToValue(QString("%1").arg(setList.at(i).parameter_name).toLatin1().data());
                    if(-1 == index)
                        continue;

                    QString val = infoList[m_curId].data[index];
                    item->setValue(val);
                }*/
            }
        }

    }
#endif
}

void  wgtParaItem_eLeak::permissUpdate(void)
{
    if(FormMain::Instance()->m_loginInfo.from_role != "role003")
    {
        ui->tabWidget->removeTab(1);
    }else {
        ui->tabWidget->insertTab(1, ui->tab_set, /*QObject::tr("????????????")*/langN.show[langEnum::strParaSet]);
    }
}

bool wgtParaItem_eLeak::event(QEvent *ev)
{
    //????????????
    if(ev->type() == PERMISSION_UPDATE)
    {
        clearAllWideget();
        createInfo();
        createParaWidget();

        clearSetWideget();
        createSetParaWidget();

        permissUpdate();
        return true;
    }

    return QWidget::event(ev);
}

void wgtParaItem_eLeak::onScrollBarChange(void)
{
    setGuideBtntVisible();
}

void  wgtParaItem_eLeak::setGuideBtntVisible()
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

void wgtParaItem_eLeak::setTabWidgetStyle(void)
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
