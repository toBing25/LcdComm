#include "wgtmenubar_ext.h"
#include "buzzer.h"
#include "wgtparaitem_ups.h"
#include "wgtparaitem_ac.h"
#include "wgtparaitem_eleak.h"
#include "wgtparaitem_meter.h"
#include "wgtparaitem_ths.h"
#include "wgtmenuitem_ext.h"
#include "wgtparaitem_spds.h"
#include "wgtparaitem_sms.h"
#include "wgtscrollbar.h"
#include "pageextend.h"
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QDebug>
#include <QTimer>

#define LAYOUT_LISTVIEW_STYLESHEET  \
    "QListView{background-color:transparent;border-radius:0px; border: 0px solid gray;}"\
    "QListView::item:focus{border-radius:0px; border: 5px solid rgb(255, 128, 0);outline: none;height:84px;}"\
    "QScrollBar:vertical {border-image: url(:/image/bar_bg.png);width:30px;margin:30 0 30 0px;}"\
    "QScrollBar::handle:vertical{border-image: url(:/image/handle.png); width:10px; height: 20px ; }"\
    "QScrollBar::add-line:vertical {border:1px solid rgb(52, 134, 208);background:rgb(52, 134, 208);height:30px;subcontrol-position:bottom;subcontrol-origin:margin;}"\
    "QScrollBar::sub-line:vertical {border:1px solid rgb(52, 134, 208);background:rgb(52, 134, 208);height:30px;subcontrol-position:top;subcontrol-origin:margin;}"\
    "QScrollBar::down-arrow:vertical {width:25px;height:25px; border-image:url(:/image/down_Arrow_2.png);}"\
    "QScrollBar::up-arrow:vertical {width:25px;height:25px;border-image:url(:/image/up_Arrow_1.png);}"\
    "QScrollBar::add-page:vertical {background:rgb(52, 72, 105);}"\
    "QScrollBar::sub-page:vertical {background:rgb(52, 72, 105);}"


WgtMenuBar_Ext::WgtMenuBar_Ext(QWidget *parent, int navMinWidth, int bodyMinWidth, int navInitWidth) :
    QWidget(parent), m_navMinWidth(navMinWidth), m_bodyMinWidth(bodyMinWidth), m_navInitWidth(navInitWidth),
    m_preiWgtId(-1)
{
    m_wgtMenu = new QWidget(this);;
    m_wgtMenu->setMinimumWidth(m_navInitWidth);
    m_wgtMenu->setMaximumWidth(m_navInitWidth);
    m_wgtMenu->setMinimumHeight(535);
    m_wgtMenu->setMaximumHeight(535);
    m_wgtMenu->setObjectName(QStringLiteral("wgtMenu"));
    m_wgtMenu->setStyleSheet(".QWidget{border-image: url(:/image/menu_right.png);}");

    m_menuListView = new QListView;
    m_menuListView->setObjectName(QStringLiteral("m_nav"));
    m_menuListView->setMinimumWidth(m_navInitWidth - 25);
    m_menuListView->setMaximumWidth(m_navInitWidth- 25);
    m_menuListView->setSpacing(5);
    m_menuListView->setStyleSheet(LAYOUT_LISTVIEW_STYLESHEET);
    m_menuListView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    m_menuListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_btn = new WgtMenuItem_Ext;
    m_btn->setObjectName("WgtMenuItem");
    m_btn->setFixedSize(m_navInitWidth - 35, 36);
    m_btn->setFocusPolicy(Qt::NoFocus);
    setBtnChecked(false);

    connect(m_btn, &WgtMenuItem_Ext::itemClicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();
        setBtnChecked(true);

        m_menuListView->clearSelection();
        setAllNodeCollapse();
        m_body->setCurrentIndex(0);
    });

    m_body = new QStackedWidget(this);
    m_body->setMinimumHeight(535);
    m_body->setMaximumHeight(535);
    m_body->setMinimumWidth(1024 - m_navInitWidth);
    m_body->setMaximumWidth(1024 - m_navInitWidth);

    m_model    = new wgtListViewModel(this);
    m_delegate = new wgtListViewDelegate(this);
    m_menuListView->setModel(m_model);
    m_menuListView->setItemDelegate(m_delegate);
    connect(m_menuListView, SIGNAL(clicked(const QModelIndex&)), m_model, SLOT(collapse(const QModelIndex&)));
    connect(m_model, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));
    connect(m_model, SIGNAL(widgetCharged(int, int, int)), this, SLOT(widgetChanged(int, int, int)));
    connect(m_menuListView, &QListView::pressed, this, [=](const QModelIndex &index){
        Q_UNUSED(index);
        setBtnChecked(false);
    });

    QVBoxLayout *vlayout1 = new QVBoxLayout;
    vlayout1->setContentsMargins(6, 0, 0, 5);
    vlayout1->addWidget(m_btn);

    m_btnUp = new QPushButton(this);
    m_btnUp->setMinimumSize(39, 26);
    m_btnUp->setMaximumSize(39, 26);
    m_btnUp->setFocusPolicy(Qt::NoFocus);
    m_btnUp->setStyleSheet("QPushButton:!pressed{"
                           "border-image: url(:/image/left_normal.png);"
                           "color: #FFFFFF;"
                           "border-radius:0px;font:12pt;"
                           "}"
                           "QPushButton:pressed{"
                           "border-image: url(:/image/left_check.png);"
                           "color: #FFFFFF;"
                           "border:0px solid #68CBF2;"
                           "border-radius:0px;font:12pt;"
                           "}");

    m_btnDown = new QPushButton(this);
    m_btnDown->setMinimumSize(39, 26);
    m_btnDown->setMaximumSize(39, 26);
    m_btnDown->setStyleSheet("QPushButton:!pressed{"
                             "border-image: url(:/image/right_normal.png);"
                             "color: #FFFFFF;"
                             "border-radius:0px;font:12pt;"
                             "}"
                             "QPushButton:pressed{"
                             "border-image: url(:/image/right_check.png);"
                             "color: #FFFFFF;"
                             "border:0px solid #68CBF2;"
                             "border-radius:0px;font:12pt;"
                             "}");

    connect(m_btnUp, &QPushButton::clicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();

        m_menuListView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
    });
    connect(m_btnDown, &QPushButton::clicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();

        m_menuListView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
    });

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSpacing(5);
    hlayout->setContentsMargins(0, 10, 0, 0);
    hlayout->addWidget(m_btnUp);
    hlayout->addWidget(m_btnDown);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(10, 15, 10, 6);
    vlayout->setSpacing(0);
    vlayout->addLayout(vlayout1);
    vlayout->addWidget(m_menuListView);
    vlayout->addLayout(hlayout);
    m_wgtMenu->setLayout(vlayout);

    for (int i = 0; i < DEVTYPE_NUM; i++) {
        QWidget *wgt = new QWidget(this);
        m_body->addWidget(wgt);
    }

    QTimer  *timer  = new QTimer();
    connect(timer, &QTimer::timeout, this, [=](){
        setGuideBtntVisible();
    });
    timer->start(200);
}

WgtMenuBar_Ext::~WgtMenuBar_Ext()
{

}

void WgtMenuBar_Ext::setGuideBtntVisible(void)
{
    int scrollBar_val = 0, scrollBar_min = 0, scrollBar_max = 0;
    scrollBar_val = m_menuListView->verticalScrollBar()->value();
    scrollBar_min = m_menuListView->verticalScrollBar()->minimum();
    scrollBar_max = m_menuListView->verticalScrollBar()->maximum();

    if((scrollBar_val == scrollBar_min) && (scrollBar_val == scrollBar_max))
    {
        m_btnUp->setVisible(false);
        m_btnDown->setVisible(false);
    }else if (scrollBar_val == scrollBar_max) {
        m_btnUp->setVisible(true);
        m_btnDown->setVisible(false);
    }else if (scrollBar_val == scrollBar_min){
        m_btnUp->setVisible(false);
        m_btnDown->setVisible(true);
    }else {
        m_btnUp->setVisible(true);
        m_btnDown->setVisible(true);
    }
}

bool WgtMenuBar_Ext::addTitle(const QString &label, const QString &decorate, const QString &decorate2, int level, int titleId1, int titleId2, int titleId3)
{
    if(titleId1 == 0)
        return false;

    m_model->addItem(label, decorate, decorate2, -1, level, titleId1, titleId2, titleId3);
    m_model->refresh();

    return true;
}

bool WgtMenuBar_Ext::addWidget(QWidget *widget, const QString &label, const QString &decorate, const QString &decorate2, int level, int titleId1, int titleId2, int titleId3)
{
    QWidget *wgt = m_body->widget(0);
    if(wgt)
    {
        m_body->removeWidget(wgt);
        wgt->deleteLater();
    }
    m_body->insertWidget(0, widget);

    m_iconlist.append(decorate);
    m_iconlist.append(decorate2);

    m_btn->setText(label);
    m_btn->setIcon(decorate2);

    setBtnChecked(true);

    m_body->setCurrentIndex(0);
    return true;
}

bool WgtMenuBar_Ext::bodyAddWidget( int titleId1, int titleId2, int devtype, QWidget *widget)
{
    dev_ist list;
    list.titleId1 = titleId1;
    list.titleId2 = titleId2;
    list.devIndex = devtype;
    m_devList.append(list);

    QWidget *wgt = m_body->widget(devtype);
    if(wgt)
    {
        m_body->removeWidget(wgt);
        wgt->deleteLater();
    }

    m_body->insertWidget(devtype, widget);

    return  true;
}

void WgtMenuBar_Ext::clicked(const QModelIndex& index)
{
    setBtnChecked(false);

    m_menuListView->setCurrentIndex(index);
    m_menuListView->clicked(index);
}

bool WgtMenuBar_Ext::removeWidget(int titleId1)
{
    m_model->removeItem(titleId1);
    m_model->refresh();
    return true;
}

bool WgtMenuBar_Ext::removeChildWidget(int titleId1, int titleId2)
{
    m_model->removeChildItem(titleId1, titleId2);
    m_model->refresh();
    return  true;
}

bool WgtMenuBar_Ext::jumpItem(int titleId1, int titleId2)
{
    m_model->queryItem(titleId1, titleId2);
    return  true;
}

bool WgtMenuBar_Ext::jumpChildItem(int titleId1, int titleId2, int titleId3)
{
    m_model->queryChildItem(titleId1, titleId2, titleId3);
    return true;
}

void WgtMenuBar_Ext::setNavStyleSheet(const QString style)
{
    m_menuListView->setStyleSheet(style);
}

void WgtMenuBar_Ext::setBodyStyleSheet(const QString style)
{
    m_body->setStyleSheet(style);
}

void WgtMenuBar_Ext::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void WgtMenuBar_Ext::showEvent(QShowEvent */*event*/)
{
    m_body->move(0, 0);
    m_wgtMenu->move(m_body->width()- 11, 0);
}

void  WgtMenuBar_Ext::setAllNodeCollapse(void)
{
    m_model->setAllNodeCollapse();
}

void WgtMenuBar_Ext::widgetChanged(int titleId1, int titleId2, int titleId3)
{
    ShortBeep();

    for (int i = 0; i < m_devList.count(); i++) {
        if((m_devList.at(i).titleId1 == titleId1) &&(m_devList.at(i).titleId2 == titleId2))
        {
            int index;
            index = m_devList.at(i).devIndex;
            if(index == DEVTYPE_THS){
                wgtParaItem_ths *ths = qobject_cast<wgtParaItem_ths *>(m_body->widget(index));
                if(ths)
                    ths->setCurrentId(titleId3);
            }
            else if (index == DEVTYPE_UPS) {
                wgtParaItem_ups *ups = qobject_cast<wgtParaItem_ups *>(m_body->widget(index));
                if(ups)
                    ups->setCurrentId(titleId3);
            }
            else if (index == DEVTYPE_AC) {
                wgtParaItem_ac *ac = qobject_cast<wgtParaItem_ac *>(m_body->widget(index));
                if(ac)
                    ac->setCurrentId(titleId3);
            }
            else if (index == DEVTYPE_METER) {
                wgtParaItem_meter *meter = qobject_cast<wgtParaItem_meter *>(m_body->widget(index));
                if(meter)
                    meter->setCurrentId(titleId3);
            }
            else if (index == DEVTYPE_LEAK) {
                wgtParaItem_eLeak *leak = (wgtParaItem_eLeak *)m_body->widget(index);
                if(leak)
                    leak->setCurrentId(titleId3);
            }
            else if (index == DEVTYPE_SPDS) {
                wgtParaItem_spds *spds = (wgtParaItem_spds *)m_body->widget(index);
                if(spds)
                    spds->setCurrentId(titleId3);
            }else if (index == DEVTYPE_SMS) {
                wgtParaItem_sms *sms = (wgtParaItem_sms *)m_body->widget(index);
                if(sms)
                    sms->setCurrentId(titleId3);
            }

            m_body->setCurrentIndex(index);
            break;
        }
    }
}

void WgtMenuBar_Ext::setBtnChecked(bool clicked)
{
    m_btn->setChecked(clicked);
    if(clicked){
        m_btn->setStyleSheet("QToolButton#WgtMenuItem{background-color: rgba(255, 255, 255, 0);"
                             "border-bottom:0px solid #68CBF2;"
                             "border-radius:0px;"
                             "border-image: url(:/image/item_select.png);"
                             "color: #FFFFFF;"
                             "font:16pt;}"
                             );
        m_btn->setColor("#FFFFFF");

        if(!m_iconlist.isEmpty())
            m_btn->setIcon(m_iconlist.at(0));
    }else {
        m_btn->setStyleSheet("QToolButton{border-image: url(:/image/item_unselect.png);"
                             "border-bottom:0px solid rgb(214, 216, 224);"
                             "border-radius:0px;"
                             "color: #00CCFF;"
                             "font:16pt;}");
        m_btn->setColor("#00CCFF");

        if(!m_iconlist.isEmpty())
            m_btn->setIcon(m_iconlist.at(1));
    }
}
















