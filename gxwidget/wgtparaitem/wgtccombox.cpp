#include "wgtccombox.h"
#include "wgtcheckbox.h"
#include "wgtscrollbar.h"
#include <QListWidgetItem>
#include <QCheckBox>
#include <QEvent>
#include <QDebug>
#include <QSignalMapper>

wgtCCombox::wgtCCombox(QWidget *parent) :
    QComboBox(parent)
{
    m_status      = status_One;

    m_pListWidget = new QListWidget(this);
    m_pLineEdit   = new QLineEdit(this);
    m_pLineEdit->installEventFilter(this);

    wgtScrollbar *bar = new wgtScrollbar(Qt::Vertical, this);
    bar->setSliderLength(50);

    this->setModel(m_pListWidget->model());
    this->setView(m_pListWidget);
    this->setLineEdit(m_pLineEdit);
    this->view()->setVerticalScrollBar(bar);
    this->setMaxCount(10);
    this->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_pLineEdit->setReadOnly(true);                                                                                                                    //ui.comboBox->setEditable(true);
    connect(m_pLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));
}

wgtCCombox::~wgtCCombox()
{
}

void wgtCCombox::addItems(QStringList strItem)
{
    QSignalMapper *signalMapper = new QSignalMapper(this);

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    for (int i = 0; i < strItem.count(); ++i)
    {
        QListWidgetItem *pItem = new QListWidgetItem(m_pListWidget);
        m_pListWidget->addItem(pItem);
        pItem->setData(Qt::UserRole, i);

        wgtCheckBox *pCheckBox = new wgtCheckBox(this);
        pCheckBox->setText(QString("%1").arg(strItem.at(i)));
        pCheckBox->setFocusPolicy(Qt::NoFocus);
        pCheckBox->setFont(font);
        pCheckBox->installEventFilter(this);
        pCheckBox->setStyleSheet("QCheckBox::indicator:unchecked {"
                                 "image: url(:/image/checkfalse.png);"
                                 "}"
                                 "QCheckBox::indicator:checked {"
                                 "image: url(:/image/checktrue.png);"
                                 "}"
                                 "QCheckBox:checked{"
                                 "color: #FFFFFF;"
                                 "}"
                                 "QCheckBox{"
                                 "color: #00CCFF;"
                                 "}");

        m_pListWidget->addItem(pItem);
        m_pListWidget->setItemWidget(pItem, pCheckBox);

        signalMapper->setMapping(pCheckBox, i);
        connect(pCheckBox, SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));
    }

    //默认选择所有设备
    if(m_pListWidget->count() != 0)
    {
        QCheckBox *pCheckBox   = getCheckBox(0);
        pCheckBox->setChecked(true);
        stateChanged(0);
    }
    connect(signalMapper, SIGNAL(mapped(int )), this, SLOT(stateChanged(int))); //将转发的信号连接到最终的槽函数
}

void wgtCCombox::cleanItems(void)
{
    this->clear();
    m_pListWidget->clear();
    m_strSelectedText.clear();
    m_pLineEdit->setText("");
    m_pLineEdit->setToolTip("");
    m_bSelected = false;
}

void wgtCCombox::selectAllItem(int index)
{
    if(index == 0)
    {
        QCheckBox *pCheckBox   = getCheckBox(index);
        int checkState = pCheckBox->checkState();
        if(checkState == Qt::Checked){
            for (int i = 1; i < m_pListWidget->count();i++) {
                pCheckBox = getCheckBox(i);
                pCheckBox->setChecked(true);
            }
        }else if(checkState == Qt::Unchecked){
            if(m_status == status_All)
                return;

            for (int i = 1; i < m_pListWidget->count();i++) {
                pCheckBox = getCheckBox(i);
                pCheckBox->setChecked(false);
            }
        }
    }else {
        m_status = status_One;

        QCheckBox *pCheckBox = getCheckBox(index);
        int checkState = pCheckBox->checkState();
        if(checkState == Qt::Unchecked)
        {
            m_status = status_All;

            QCheckBox *pCheckBox = getCheckBox(0);
            pCheckBox->setChecked(false);
        }
    }
}

QCheckBox* wgtCCombox::getCheckBox(int index)
{
    QListWidgetItem *pItem;
    QWidget *pWidget;
    QCheckBox *pCheckBox;

    pItem     = m_pListWidget->item(index);
    pWidget   = m_pListWidget->itemWidget(pItem);
    pCheckBox = qobject_cast<QCheckBox *>(pWidget);

    return  pCheckBox;
}

void wgtCCombox::stateChanged(int index)
{
    selectAllItem(index);

    m_bSelected = true;
    QString strSelectedData;
    m_strSelectedText.clear();
    int nCount = m_pListWidget->count();
    for (int i = 1; i < nCount; ++i)
    {
        QCheckBox *pCheckBox = getCheckBox(i);
        if (pCheckBox->isChecked())
        {
            QString strText = pCheckBox->text();
            strSelectedData.append(strText).append(";");
        }
    }

    if (strSelectedData.endsWith(";"))
        strSelectedData.remove(strSelectedData.count() - 1, 1);

    if (!strSelectedData.isEmpty())
    {
        m_strSelectedText = strSelectedData;
        m_pLineEdit->setText(strSelectedData);
        m_pLineEdit->setToolTip(strSelectedData);
    }
    else
    {
        m_pLineEdit->clear();
    }

    m_bSelected = false;
}

QStringList wgtCCombox::getItemListId(void)
{
    QStringList list;
    list = m_strSelectedText.split(";");

    return  list;
}

void wgtCCombox::textChanged(const QString &text)
{
    Q_UNUSED(text);

    if (!m_bSelected)
        m_pLineEdit->setText(m_strSelectedText);
}

bool wgtCCombox::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLineEdit"))
        {
            this->showPopup();
        }
        if(obj->inherits("QCheckBox"))
        {
            this->hidePopup();
            this->showPopup();
        }
    }
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}
