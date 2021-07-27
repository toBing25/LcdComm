#include "wgtparaitem_spds.h"
#include "ui_wgtparaitem_spds.h"
#include "Formmain/formmain.h"
#include "actparaitem_lineedit.h"
#include "actparaitem_combobox.h"
#include "actparaitem_bit.h"
#include "SpdsData/SpdsDataStruct.h"
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
#define SUPPORT_MAINITEM    2 //主路支持显示几个
#define SUPPORT_BRANCHITEM  3 //支路支持显示几个

#define min(a, b) ((a)>(b)?(b):(a))

wgtParaItem_spds::wgtParaItem_spds(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wgtParaItem_spds),
    m_curId(id),
    m_mainCurPage(1),
    m_mainTotalPage(1),
    m_branchCurPage(1),
    m_branchTotalPage(1)
{
    qRegisterMetaType<QList<spds_Info>>("QList<spds_Info>");

    setupUi();
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendeExtDevSpdsList,this,&wgtParaItem_spds::getSpdsInfo);
    connect(ui->btn_up, &QPushButton::clicked, this, [=](bool clicked){
        Q_UNUSED(clicked);
        ShortBeep();

        int index = ui->tabWidget->currentIndex();
        if(index == 0)
            ui->scrollArea_2->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
        else if (index == 1) {
            ui->scrollArea->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
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
        }
        setGuideBtntVisible();
    });

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index){
        Q_UNUSED(index);
        //setTabWidgetStyle();
        ShortBeep();

        setCurrentPage();
        setGuideBtntVisible();
    });

    connect(ui->scrollArea,     &MScrollArea::sigScrollVal, this, &wgtParaItem_spds::onScrollBarChange);
    connect(ui->scrollArea_2,   &MScrollArea::sigScrollVal, this, &wgtParaItem_spds::onScrollBarChange);

    connect(ui->btnPre,  &QPushButton::clicked,  this, &wgtParaItem_spds::onPageClicked);
    connect(ui->btnNext, &QPushButton::clicked,  this, &wgtParaItem_spds::onPageClicked);

    QTimer  *timer  = new QTimer();
    connect(timer, &QTimer::timeout, this, [=](){
        setGuideBtntVisible();
    });
    timer->start(500);
}

wgtParaItem_spds::~wgtParaItem_spds()
{
    delete ui;
}

void wgtParaItem_spds::setupUi(void)
{
    ui->setupUi(this);
    //m_lblDate = new wgtDateTime(ui->tabWidget);

    //状态
    ui->tabWidget->setTabText(0, langN.show[langEnum::strSpdsMain]);
    ui->lblSpdsNumberName->setText(langN.show[langEnum::strSpdsNumbe] + ":");
    ui->lblSpdsNumberVal->setText(QString::number(m_curId));

    ui->lblSpdsStaName->setText(langN.show[langEnum::strSpdsStatus] + ":");
    setConnectState(1);
    createInfo();

    //参数
    ui->tabWidget->setTabText(1, langN.show[langEnum::strSpdsBranch]);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    createParaWidget();

    //ui->tabWidget->setCurrentIndex(0);
    ui->wgtRender->setVisible(false);
    ui->lblCurpage->setText(QString("%1/%2").arg(m_mainCurPage).arg(m_mainTotalPage));
    ui->lblCurpage->installEventFilter(this);

    //setTabWidgetStyle();
}

void wgtParaItem_spds::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
}

void wgtParaItem_spds::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    //m_lblDate->move(ui->tabWidget->width() - m_lblDate->width(), 0);
}

void wgtParaItem_spds::createInfo(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtInfo);
    vLayout->setSpacing(15);
    vLayout->setContentsMargins(10, 5, 10, 10);

    //类别个数
    int cateNum  = FormMain::Instance()->m_spdsList.classList.count();
    if(cateNum == 0)
        return;

    QString cateName = langN.show[FormMain::Instance()->m_spdsList.classList.at(0)];
    //筛选状态
    QList<Menu>  menuList;
    foreach (Menu menu, FormMain::Instance()->m_spdsList.menuList) {
        if(cateName == langN.show[menu.parameter_class])
        {
            if(menu.parameter_type == TYPE_STATUS)
                menuList << menu;
        }
    }

    if(menuList.isEmpty())
        return;

    for(int i = 0; i < SUPPORT_MAINITEM; i++)
    {
        QFlowLayout *flowlayout = new QFlowLayout(0,0,0);

        ActParaWidget *wgt  = new ActParaWidget(QString("%1%2").arg(cateName).arg(i), ui->wgtInfo);
        wgt->setObjectName(QStringLiteral("wgt"));
        wgt->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

        QVector<ActParaItem *> item_list;
        for (int j = 0; j < menuList.count(); ++j)
        {
            QString paraName  = langN.show[menuList.at(j).show_name];
            QString paraValue = QString::number(0);

            ActParaItem *actItem = new ActParaItem(paraName, paraValue, wgt->ui->wgtBody);
            actItem->setObjectName(QString("%1%2").arg(menuList.at(j).parameter_name).arg(i));
            actItem->setMinimumWidth((wgt->size().width() + 180)/2);
            actItem->setMaximumWidth((wgt->size().width() + 180)/2);
            actItem->setWidgetStyle(3);
            flowlayout->addWidget(actItem);
            item_list.append(actItem);
        }

        if((menuList.count()))
        {
            ActParaItem *actItem = new ActParaItem("", "", wgt->ui->wgtBody);
            actItem->setMinimumWidth((wgt->size().width() + 180)/2);
            actItem->setMaximumWidth((wgt->size().width() + 180)/2);
            actItem->setWidgetStyle(3);
            actItem->setVisible(false);
            flowlayout->addWidget(actItem);
            item_list.append(actItem);
        }

        //保存每个类别的参数
        SPDS_list list;
        list.para_type = TYPE_STATUS;
        list.menuList  = menuList;
        list.title     = cateName;
        list.wgt       = wgt;
        m_mainList.append(list);

        spds_item list2;
        list2.itemList = item_list;
        list2.wgt      = wgt;
        m_itemList.append(list2);

        wgt->ui->wgtBody->setLayout(flowlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();

    ui->wgtInfo->resize(sizeHint());
}

void wgtParaItem_spds::createParaWidget(void)
{
    QVBoxLayout *vLayout = new QVBoxLayout(ui->wgtContents);
    vLayout->setSpacing(10);
    vLayout->setContentsMargins(10, 10, 10, 10);

    //类别个数
    int cateNum  = FormMain::Instance()->m_spdsList.classList.count();
    if(cateNum == 0)
        return;

    QString cateName = langN.show[FormMain::Instance()->m_spdsList.classList.at(1)];

    //筛选只读的参数
    QList<Menu> menuList;
    foreach (Menu menu, FormMain::Instance()->m_spdsList.menuList) {
        if(cateName == langN.show[menu.parameter_class])
        {
            if(menu.parameter_type == TYPE_SETTING)
                menuList << menu;
        }
    }

    if(menuList.isEmpty())
        return;

    for(int i = 0; i < SUPPORT_BRANCHITEM; i++)
    {

        QFlowLayout *flowlayout = new QFlowLayout(0,0,0);
        ActParaWidget *wgt  = new ActParaWidget(QString("%1%2").arg(cateName).arg(i), ui->wgtContents);
        wgt->setObjectName(QStringLiteral("wgt"));
        wgt->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

        QVector<ActParaItem *> item_list;
        for (int j = 0; j < menuList.count(); ++j)
        {
            QString paraName  = langN.show[menuList.at(j).show_name];
            QString paraValue = QString::number(0);

            ActParaItem *actItem = new ActParaItem(paraName, paraValue, wgt->ui->wgtBody);
            actItem->setObjectName(QString("%1%2").arg(menuList.at(j).parameter_name).arg(i));
            actItem->setMinimumWidth((wgt->size().width() + 186)/2);
            actItem->setMaximumWidth((wgt->size().width() + 186)/2);
            actItem->setWidgetStyle(1);
            flowlayout->addWidget(actItem);
            item_list.append(actItem);
        }

        if((menuList.count()))
        {
            ActParaItem *actItem = new ActParaItem("", "", wgt->ui->wgtBody);
            actItem->setMinimumWidth((wgt->size().width() + 186)/2);
            actItem->setMaximumWidth((wgt->size().width() + 186)/2);
            actItem->setWidgetStyle(1);
            actItem->setVisible(false);
            flowlayout->addWidget(actItem);
            item_list.append(actItem);
        }

        //保存每个类别的参数
        SPDS_list list;
        list.para_type = TYPE_SETTING;
        list.menuList  = menuList;
        list.title     = cateName;
        list.wgt       = wgt;
        m_branchList.append(list);

        spds_item list2;
        list2.itemList = item_list;
        list2.wgt      = wgt;
        m_itemList.append(list2);

        wgt->ui->wgtBody->setLayout(flowlayout);
        vLayout->addWidget(wgt);
    }
    vLayout->addStretch();
    ui->wgtContents->resize(sizeHint());
}

void wgtParaItem_spds::clearAllWideget(void)
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

    if(!m_mainList.isEmpty())
        m_mainList.clear();

    if(!m_branchList.isEmpty())
        m_branchList.clear();

    if(!m_itemList.isEmpty())
        m_itemList.clear();
}

void wgtParaItem_spds::setConnectState(int state)
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
    QPalette pe = ui->lblSpdsStaVal->palette();
    pe.setColor(QPalette::WindowText, color);
    ui->lblSpdsStaVal->setPalette(pe);
    ui->lblSpdsStaVal->setText(connect);
}

void wgtParaItem_spds::setCurrentId(int id)
{
    if((id - 1) < 0)
        return;

    ui->lblSpdsNumberVal->setText(QString::number(id));
    if(spds_list.isEmpty())
        spds_list = PageExtend::Instance()->m_spdsList;

    m_curId         = id - 1;
    m_mainCurPage   = 1;
    m_branchCurPage = 1;

    getSpdsInfo(spds_list);
    ui->tabWidget->setCurrentIndex(0);
}

void wgtParaItem_spds::onPageClicked(bool checked)
{
    Q_UNUSED(checked);
    ShortBeep();

    QPushButton *button = qobject_cast<QPushButton*>(sender());

    int cur_index = ui->tabWidget->currentIndex();
    if(cur_index == 0)
    {
        if(button == ui->btnPre)
        {
            m_mainCurPage--;
            if(m_mainCurPage < 1)
                m_mainCurPage = m_mainTotalPage;
        }
        else if(button == ui->btnNext)
        {
            m_mainCurPage++;
            if(m_mainCurPage > m_mainTotalPage)
                m_mainCurPage = 1;
        }
    }else if (cur_index == 1) {
        if(button == ui->btnPre)
        {
            m_branchCurPage--;
            if(m_branchCurPage < 1)
                m_branchCurPage = m_branchTotalPage;
        }
        else if(button == ui->btnNext)
        {
            m_branchCurPage++;
            if(m_branchCurPage > m_branchTotalPage)
                m_branchCurPage = 1;
        }
    }

    setCurrentPage();
    getSpdsInfo(spds_list);
}

void wgtParaItem_spds::getSpdsInfo(QList<spds_Info> infoList)
{
    spds_list = infoList;

    if(((m_mainList.count() == 0)&&(m_branchList.count() == 0)) || (infoList.count() == 0))
        return;

    if(m_curId >= infoList.count())
        return;

    //设置连接状态
    setConnectState(infoList[m_curId].connect);
    ui->lblSpdsNumberVal->setText(QString::number(infoList[m_curId].devID));

    //先隐藏所有窗体
    setWgtVisible();

    //更新主路数据
    int curMainNum  = 0;
    QList<int> enableNum;
    for (int i = 0; i < infoList[m_curId].mainNum; i++) {
        if(infoList[m_curId].mainEn[i] == 1)
        {
            curMainNum++;
            enableNum.append(i);
        }
    }
    m_mainTotalPage = curMainNum/SUPPORT_MAINITEM;
    if(curMainNum%SUPPORT_MAINITEM)
        m_mainTotalPage++;

    for(int i = 0; i < min(curMainNum - SUPPORT_MAINITEM*(m_mainCurPage - 1), SUPPORT_MAINITEM); i++)
    {
        if(i > m_mainList.count())
            return;

        int main_id = i + SUPPORT_MAINITEM * (m_mainCurPage - 1);
        if(main_id > curMainNum)
            return;
        if(enableNum.at(main_id) > infoList[m_curId].mainNum)
            return;

        QString title = m_mainList.at(i).title;
        m_mainList.at(i).wgt->setVisible(true);
        //qDebug()<<__LINE__ << enableNum.at(main_id) << main_id <<main_id+1;
        m_mainList.at(i).wgt->setTitle(QString("%1%2").arg(title).arg(enableNum.at(main_id)+1));

        QList<Menu> menuList = m_mainList.at(i).menuList;
        for (int j = 0; j < menuList.count(); j++) {
            ActParaItem *item = ui->wgtInfo->findChild<ActParaItem *>(QString("%1%2").arg(menuList.at(j).parameter_name).arg(i));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_spds::SpdsMainEnum>();
                //把参数对应的值转换成枚举值
                int index   = metaEnum.keysToValue(QString("%1").arg(menuList.at(j).parameter_name).toLatin1().data());
                if(-1 != index)
                {
                    QString val = infoList[m_curId].main[enableNum.at(main_id)][index];
                    if(val.toInt() == HIDEITEMVALUE)
                        item->setVisible(false);
                    else{
                        item->setVisible(true);
                        item->setVal(val);
                        item->setUnit(infoList[m_curId].mainUnit[enableNum.at(main_id)][index]);
                    }
                }
            }
        }
    }

    //更新支路数据
    enableNum.clear();
    int curBranchNum  = 0;
    for (int i = 0; i < infoList[m_curId].branchNum; i++) {
        if(infoList[m_curId].branchEn[i] == 1)
        {
            curBranchNum++;
            enableNum.append(i);

        }
    }
    m_branchTotalPage = curBranchNum/SUPPORT_BRANCHITEM;
    if(curBranchNum%SUPPORT_BRANCHITEM)
        m_branchTotalPage++;

    setCurrentPage();
    for(int i = 0; i < min(curBranchNum - SUPPORT_BRANCHITEM*(m_branchCurPage - 1), SUPPORT_BRANCHITEM); i++)
    {
        if(i > m_branchList.count())
            return;

        int branch_id = i + SUPPORT_BRANCHITEM * (m_branchCurPage - 1);
        if(branch_id > infoList[m_curId].branchNum)
            return;
        if(enableNum.at(branch_id) > infoList[m_curId].branchNum)
            return;

        QString title = m_branchList.at(i).title;
        m_branchList.at(i).wgt->setVisible(true);
        //qDebug()<<__LINE__ << enableNum.at(branch_id) << branch_id <<branch_id+1;
        m_branchList.at(i).wgt->setTitle(QString("%1%2").arg(title).arg(enableNum.at(branch_id)+1));

        QList<Menu> menuList = m_branchList.at(i).menuList;
        for (int j = 0; j < menuList.count(); j++) {
            ActParaItem *item = ui->wgtContents->findChild<ActParaItem *>(QString("%1%2").arg(menuList.at(j).parameter_name).arg(i));
            if(item)
            {
                QMetaEnum metaEnum = QMetaEnum::fromType<wgtParaItem_spds::SpdsBranchEnum>();
                //把参数对应的值转换成枚举值
                int index   = metaEnum.keysToValue(QString("%1").arg(menuList.at(j).parameter_name).toLatin1().data());
                if(-1 != index)
                {
                    QString val = infoList[m_curId].branch[enableNum.at(branch_id)][index];
                    if(val.toInt() == HIDEITEMVALUE)
                    {
                        item->setVisible(false);
                    }
                    else{
                        item->setVisible(true);
                        item->setVal(val);
                        item->setUnit(infoList[m_curId].branchUnit[enableNum.at(branch_id)][index]);
                    }
                }
            }
        }
    }

    if(m_itemList.isEmpty())
        return;

    for (int i = 0; i < m_itemList.count(); i++) {
        int enable_num = 0;
        for (int j = 0; j < m_itemList.at(i).itemList.count() - 1; j++) {
            if(m_itemList.at(i).itemList.at(j)->isVisible() == true)
                enable_num++;
        }
        if(enable_num%2)
            m_itemList.at(i).itemList.last()->setVisible(true);
        else {
            m_itemList.at(i).itemList.last()->setVisible(false);
        }

        if(enable_num < 10)
            m_itemList.at(i).wgt->setStyle(STYLE_MIN);
        else if (enable_num < 50) {
            m_itemList.at(i).wgt->setStyle(STYLE_MID);
        }else {
            m_itemList.at(i).wgt->setStyle(STYLE_MAX);
        }
    }
}

bool wgtParaItem_spds::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        return true;
    }

    return QWidget::event(ev);
}

void wgtParaItem_spds::onScrollBarChange(void)
{
    setGuideBtntVisible();
}

void  wgtParaItem_spds::setGuideBtntVisible()
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

void wgtParaItem_spds::setWgtVisible()
{
    //int cur_index = ui->tabWidget->currentIndex();
    //if(cur_index == 0)
    //{
    for (int i = 0; i < m_mainList.count(); i++) {
        m_mainList.at(i).wgt->setVisible(false);
    }
    //}else if (cur_index == 1) {
    for (int i = 0; i < m_branchList.count(); i++) {
        m_branchList.at(i).wgt->setVisible(false);
    }
    //}
}

void wgtParaItem_spds::setCurrentPage(void)
{
    int cur_index = ui->tabWidget->currentIndex();
    if(cur_index == 0)
        ui->lblCurpage->setText(QString("%1/%2").arg(m_mainCurPage).arg(m_mainTotalPage));
    else if (cur_index == 1) {
        ui->lblCurpage->setText(QString("%1/%2").arg(m_branchCurPage).arg(m_branchTotalPage));
    }
}

bool wgtParaItem_spds::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLabel"))
        {
            ShortBeep();

            //页显示
            QLabel *label = qobject_cast<QLabel *>(obj);
            if(label == ui->lblCurpage)
            {
                connect(WgtKeyPad::Instance(), &WgtKeyPad::sendInputBufferAreaText, [=](QString text){
                    //页跳转输入的值必须为数字，并且大于0，小于总的页数
                    QRegExp regExp("[0-9]+$");
                    if (!regExp.exactMatch(text)) {
                        return;
                    }

                    int cur_index = ui->tabWidget->currentIndex();
                    if(cur_index == 0){
                        if((text.toInt() > m_mainTotalPage) || ( text.toInt() < 0))
                            return;

                        m_mainCurPage =  text.toInt();
                        ui->lblCurpage->setText(QString("%1/%2").arg(m_mainCurPage).arg(m_mainTotalPage));
                    }else if (cur_index == 1) {
                        if((text.toInt() > m_branchTotalPage) || ( text.toInt() < 0))
                            return;

                        m_branchCurPage =  text.toInt();
                        ui->lblCurpage->setText(QString("%1/%2").arg(m_branchCurPage).arg(m_branchTotalPage));
                    }
                    getSpdsInfo(spds_list);
                });

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

void wgtParaItem_spds::setTabWidgetStyle(void)
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










