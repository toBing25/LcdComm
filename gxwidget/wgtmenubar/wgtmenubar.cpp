#include "wgtmenubar.h"
#include "buzzer.h"
#include "wgtbgscreen.h"
#include "formmain.h"
#include "gettext.h"
#include <QDebug>

WgtMenuBar::WgtMenuBar(QWidget *parent)
    : QWidget(parent)
{
    m_curIndex = 0;
    m_layout   = HBoxLayout;

    this->resize(this->width(), this->height());
}

WgtMenuBar::~WgtMenuBar()
{

}

void WgtMenuBar::setLayouts(const BOX_LAYOUT &layout)
{
    m_layout   = layout;

    if(m_layout == HBoxLayout)
    {
        Hlayout = new QHBoxLayout;
        Hlayout->setContentsMargins(0, 0, 0, 0);
        Hlayout->setSpacing(0);
        this->setLayout(Hlayout);
    }
    else if(m_layout == VBoxLayout)
    {
        Vlayout = new QVBoxLayout;
        Vlayout->setContentsMargins(16, 20, 10, 10);
        Vlayout->setSpacing(10);
        this->setLayout(Vlayout);
    }
}

void WgtMenuBar::addItem_H(const QString &text, const QString &name, const langEnum textId)
{
    WgtMenuItem *toolButton = new WgtMenuItem(this);

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    toolButton->setFont(font);
    toolButton->setObjectName(name);
    toolButton->setIndex(Hlayout->count());
    toolButton->setText(text);
    toolButton->setStyleSheet("background-color: rgba(255, 255, 255, 0);"
                              "border-bottom:0px solid #68CBF2;"
                              "border-radius:0px;"
                              "border-image: url(:/image/menu_unselect.png);"
                              "color: #21CEFF;"
                              );

    toolButton->setFixedSize(108, 30);
    toolButton->setToolButtonStyle(Qt::ToolButtonTextOnly);

    connect(toolButton, SIGNAL(itemClicked(int)), this, SLOT(onItemClicked(int)));
    Hlayout->addWidget(toolButton, -10, Qt::AlignBottom);
    m_itemList.append(toolButton);
    m_itemText.append(textId);
}

void WgtMenuBar::addItem(const QString & text, const QString &name, const langEnum textId)
{
    WgtMenuItem *button = new WgtMenuItem(this);
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    button->setFont(font);
    button->setText(text);
    button->setObjectName(name);
    button->setFixedSize(145, 36);
    button->setIndex(Vlayout->count());
    button->setFocusPolicy(Qt::NoFocus);
    button->setIconSize(QSize(20, 22));
    button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    button->setStyleSheet("background-color: rgba(255, 255, 255, 0);"
                          "border-bottom:0px solid rgb(214, 216, 224);"
                          "border-radius:0px;"
                          "border-image: url(:/image/item_unselect.png);"
                          "color: #00CCFF;");

    connect(button, SIGNAL(itemClicked(int)), this, SLOT(onItemClicked(int)));

    Vlayout->addWidget(button, 0, Qt::AlignLeft);
    m_itemList.append(button);
    m_itemText.append(textId);
}

void WgtMenuBar::setAlignment(Qt::AlignmentFlag flag)
{
    switch (flag) {
    case Qt::AlignLeft:
        if(m_layout == HBoxLayout)
            Hlayout->addStretch();
        else if(m_layout == VBoxLayout)
            Vlayout->addStretch();
        break;
    case Qt::AlignRight:
        if(m_layout == HBoxLayout)
            Hlayout->insertStretch(0);
        else if(m_layout == VBoxLayout)
            Vlayout->insertStretch(0);
        break;
    case Qt::AlignCenter:
        if(m_layout == HBoxLayout)
        {
            Hlayout->insertStretch(0);
            Hlayout->addStretch(0);
        }
        else if(m_layout == VBoxLayout)
        {
            Vlayout->insertStretch(0);
            Vlayout->addStretch();
        }
        break;
    default:
        break;
    }
}

void WgtMenuBar::setCurrIndex(int id)
{
    id = (id < m_itemList.count()) ? id : 0;
    onItemClicked(id);
}

void WgtMenuBar::setItemEnable(ENABLE_VECTOR &vector)
{
    foreach (item_enable it, vector)
    {
        if((it.itemId < 0) || (it.itemId >= m_itemList.count()))
            continue;

        m_itemList[it.itemId]->setVisible(it.enable);
    }
}

int  WgtMenuBar::itemCount(void)
{
    return  m_itemList.count();
}

void WgtMenuBar::onItemClicked(int id)
{
    ShortBeep();

    if((id >= m_itemList.count()) || (id < 0))
    {
        id = 0;
    }

    if(m_curIndex >= m_itemList.count())
    {
        m_curIndex = 0;
    }

    QString pre_qss, cur_qss;
    if(m_layout == HBoxLayout){
        pre_qss = "background-color: rgba(255, 255, 255, 0);"
                  "border-bottom:0px solid #68CBF2;"
                  "border-radius:0px;"
                  "border-image: url(:/image/menu_unselect.png);"
                  "color: #21CEFF;";

        cur_qss = "background-color:  rgba(255, 255, 255, 0);"
                  "border-bottom:0px solid #68CBF2;"
                  "border-radius:2px;"
                  "border-image: url(:/image/menu_select.png);"
                  "color: #FFFFFF;";
    }else if(m_layout == VBoxLayout){
        pre_qss = "background-color: rgba(255, 255, 255, 0);"
                  "border-bottom:0px solid rgb(214, 216, 224);"
                  "border-radius:0px;"
                  "border-image: url(:/image/item_unselect.png);"
                  "color: #00CCFF;";

        cur_qss = "background-color: rgb(69, 121, 219);"
                  "border-bottom:0px solid rgb(214, 216, 224);"
                  "border-radius:0px;"
                  "border-image: url(:/image/item_select.png);"
                  "color: #FFFFFF;";
    }

    if(m_layout == HBoxLayout)
    {
        m_itemList[m_curIndex]->setSelect(false);
        m_itemList[m_curIndex]->setStyleSheet(pre_qss);
    }else {
        m_itemList[m_curIndex]->setSelect(false);
        m_itemList[m_curIndex]->setStyleSheet(pre_qss);
    }

    m_curIndex = id;

    if(m_layout == HBoxLayout)
    {
        m_itemList[m_curIndex]->setSelect(true);
        m_itemList[m_curIndex]->setStyleSheet(cur_qss);
    }
    else {
        m_itemList[m_curIndex]->setSelect(true);
        m_itemList[m_curIndex]->setStyleSheet(cur_qss);
    }

    if(FormMain::Instance()->getSysType() == eSysTypeISmart){
        static int status = 0;
        if(m_layout == HBoxLayout)
        {
            if(m_curIndex == 0)
            {
                WgtBgScreen::loadImage(":/image/bg_1.png");
                FormMain::Instance()->update();
                status = 0;
            }
            else {
                if(status == 0)
                {
                    WgtBgScreen::loadImage(":/image/bg_2.png");
                    FormMain::Instance()->update();
                    status = 1;
                }
            }
        }
    }

    emit clicked(m_curIndex);
}

void WgtMenuBar::changeEvent(QEvent* event)
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

void WgtMenuBar::retranslateUi(void)
{
    for (int i = 0; i < m_itemList.count(); i++) {
        m_itemList.at(i)->setText(langN.show[m_itemText.at(i)]);
    }
}

