#include "accesscontrol.h"
#include "ui_accesscontrol.h"
#include "tableWidgetDelegate.h"
#include "wgtmsgbox_h.h"
#include "cardopert.h"
#include "buzzer.h"
#include <QScrollBar>
#include <QGridLayout>
#include <QCheckBox>
#include <QSignalMapper>

AccessControl::AccessControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccessControl)
{
    setupUi();

    m_cardDb    =  FormMain::Instance()->getShareMemManage()->cardDb();
    m_rowCount  = 5;

    connect(ui->btnAdd, &QPushButton::clicked, this, [=](){
        ShortBeep();

        ui->wgtSet->setVisible(true);

        removeMode();
        addMode();
    });

    connect(ui->btnSet, &QPushButton::clicked, this, [=](){
        ShortBeep();

        m_dclist.clear();
        if(m_vector.count() != 0){
            for (int i = 0; i < m_vector.count(); i++) {
                if(m_vector.at(i)->isChecked()){
                    if(i >= m_dcDev.dcLen)
                        return;
                    else {
                        m_dclist.append(QString::number(m_dcDev.dcList.at(i)));
                    }
                }
            }
        }

        if(ui->lineCardNumVal->text().isEmpty() && ui->lineCardNameVal->text().isEmpty()){
            ui->lblCardTip->setVisible(true);
            ui->lblCareNameTip->setVisible(true);
            return;
        }else if (ui->lineCardNumVal->text().isEmpty() || ui->lineCardNameVal->text().isEmpty() || m_dclist.isEmpty()) {
            ui->lblCardTip->setVisible(false);
            ui->lblCareNameTip->setVisible(false);

            WgtMsgBox_H msgbox("设置失败！", MSGBOX_TYPE_OK);
            msgbox.exec();
            return;
        }

        QString cardNO, cardName;
        cardNO    = ui->lineCardNumVal->text();
        cardName  = ui->lineCardNameVal->text();
        emit cardChange(cardNO, cardName, m_dclist);

        WgtMsgBox_H msgbox("设置成功！", MSGBOX_TYPE_OK);
        msgbox.exec();
    });

    connect(ui->btnCancel, &QPushButton::clicked, this, [=](){
        ShortBeep();

        ui->wgtSet->setVisible(false);
        ui->lblCardTip->setVisible(false);
        ui->lblCareNameTip->setVisible(false);

        ui->lineCardNumVal->clear();
        ui->lineCardNameVal->clear();
    });

    //收到更新数据后，更新列表
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::sendChageCardDone, this, &AccessControl::updateTable);
    connect(FormMain::Instance()->getShareMemManage(),&ReadShareMem::removeCardDone,    this, &AccessControl::updateTable);

    //修改和移除卡号
    connect(this, &AccessControl::cardChange, FormMain::Instance()->getShareMemManage(),  &ReadShareMem::changeCard);
    connect(this, &AccessControl::cardRemove, FormMain::Instance()->getShareMemManage(),  &ReadShareMem::removeCard);

    updateTable();
}

AccessControl::~AccessControl()
{
    delete ui;
}

void AccessControl::setupUi(void)
{
    ui->setupUi(this);
    ui->wgtSet->setVisible(false);
    ui->lblCardTip->setVisible(false);
    ui->lblCareNameTip->setVisible(false);

    ui->tabInfo->setColumnCount(5);
    ui->tabInfo->setRowCount(m_rowCount);
    ui->tabInfo->setShowGrid(false);
    ui->tabInfo->setItemDelegate(new tableWidgetDelegate(ui->tabInfo));
    ui->tabInfo->horizontalHeader()->setVisible(false);
    ui->tabInfo->horizontalHeader()->setSectionsClickable(false);
    ui->tabInfo->horizontalHeader()->setStretchLastSection(true);
    ui->tabInfo->horizontalHeader()->setHighlightSections(false);
    ui->tabInfo->verticalHeader()->setVisible(false);
    ui->tabInfo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tabInfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tabInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabInfo->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tabInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tabInfo->setFocusPolicy(Qt::NoFocus);
    ui->tabInfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(44, 82, 153);color: #00ADE5; font:14pt;}");
    ui->tabInfo->setStyleSheet("QTableWidget{gridline-color: rgb(44, 82, 153);border:0px solid rgb(44, 82, 153);}");

    QPalette pll = ui->tabInfo->palette();
    pll.setBrush(QPalette::Base, QBrush(QColor(255,255,255,0)));
    ui->tabInfo->setPalette(pll);

    setHorizontalHeaderLabels();

    lieVector.append(ui->lineCardNumVal);
    lieVector.append(ui->lineCardNameVal);
    foreach (QLineEdit *lie, lieVector)
    {
        lie->installEventFilter(this);
    }
}

void AccessControl::removeMode(void)
{
    ui->lineCardNumVal->clear();
    ui->lineCardNameVal->clear();

    QList<QCheckBox*> wgts = ui->wgtMode->findChildren<QCheckBox *>();
    if(!wgts.isEmpty())
    {
        foreach(QCheckBox* wgt, wgts)
        {
            delete wgt;
        }
    }

    QList<QGridLayout*> glayout = ui->wgtMode->findChildren<QGridLayout *>();
    if(!glayout.isEmpty())
    {
        foreach(QGridLayout* layout, glayout)
        {
            delete layout;
        }
    }

    if(!m_vector.isEmpty())
        m_vector.clear();

    if(!m_vectorID.isEmpty())
        m_vectorID.clear();
}

void AccessControl::addMode(void)
{
    QGridLayout *gLayout = new QGridLayout;
    gLayout->setSpacing(15);
    gLayout->setObjectName(QStringLiteral("gLayout"));
    gLayout->setContentsMargins(0, 5, 10, 5);

    int row, col;
    int i, j, k;
    col   = 6;
    row   = m_dcDev.dcLen / col;
    if(m_dcDev.dcLen % col)
        row++;

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            k = i * col + j;
            if(k >= m_dcDev.dcLen)
                break;

            QCheckBox *checkbox = new QCheckBox(ui->wgtMode);
            checkbox->setText(QString("%1%2").arg("控制模块").arg(m_dcDev.dcList.at(k) + 1));
            checkbox->setFocusPolicy(Qt::NoFocus);
            checkbox->setFont(font);
            checkbox->setStyleSheet("QCheckBox::indicator:unchecked {"
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

            m_vector.append(checkbox);
            m_vectorID.append(m_dcDev.dcList.at(k));
            connect(checkbox, &QCheckBox::stateChanged, this, [=](int status){
                Q_UNUSED(status);
                ShortBeep();
            });

            gLayout->addWidget(checkbox, i, j);
        }
    }
    if(m_dcDev.dcLen < col)
    {
        gLayout->setColumnStretch(m_dcDev.dcLen, 1);
    }

    gLayout->setRowStretch(row, 1);
    ui->wgtMode->setLayout(gLayout);
    ui->wgtMode->resize(sizeHint());
}

void AccessControl::cardChangeSlot(CardDb card)
{
    ShortBeep();

    ui->wgtSet->setVisible(true);

    removeMode();
    addMode();

    if(m_vector.count() != 0){
        for (int i = 0; i < card.dcList.count(); i++) {
            for (int j = 0; j < m_vectorID.count(); j++) {
                if(card.dcList.at(i).toInt() == m_vectorID.at(j)){
                    m_vector.at(j)->setChecked(true);
                }
            }
        }
    }

    ui->lineCardNumVal->setText(card.cardNo);
    ui->lineCardNameVal->setText(card.cardName);
}

void AccessControl::cardRemoveSlot(CardDb card)
{
    ShortBeep();

    emit cardRemove(card.cardNo);
}

void AccessControl::updateTable()
{
    m_dcDev  =  FormMain::Instance()->m_dcDev;
    if(m_cardDb->count() == 0){
        m_rowCount = 1;
        ui->tabInfo->clearContents();
        ui->tabInfo->setRowCount(m_rowCount);
        setHorizontalHeaderLabels();
        return;
    }

    m_rowCount = m_cardDb->count() + 1;

    ui->tabInfo->clearContents();
    ui->tabInfo->setRowCount(m_rowCount);
    setHorizontalHeaderLabels();
    setTabSize();

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    for (int i = 0, r = 1; i < m_cardDb->count(); i++, r++) {
        QTableWidgetItem *pItem;
        pItem = new QTableWidgetItem(QString::number(i+1));
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);
        ui->tabInfo->setItem(r, 0, pItem);

        pItem = new QTableWidgetItem(m_cardDb->at(i).cardNo);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);
        ui->tabInfo->setItem(r, 1, pItem);

        pItem = new QTableWidgetItem(m_cardDb->at(i).cardName);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);
        ui->tabInfo->setItem(r, 2, pItem);

        QString mode;
        if(!m_cardDb->at(i).dcList.isEmpty()){
            for (int j = 0; j < m_cardDb->at(i).dcList.length(); j++) {
                mode.append(m_cardDb->at(i).dcList.at(j));

                if(j != (m_cardDb->at(i).dcList.length()-1))
                    mode.append(",");
            }
        }

        pItem = new QTableWidgetItem(mode);
        pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        pItem->setForeground(QColor("#00ADE5"));
        pItem->setFont(font);
        ui->tabInfo->setItem(r, 3, pItem);

        CardOpert  *wgt = new CardOpert(m_cardDb->at(i));
        connect(wgt, &CardOpert::btnModifyClicked, this, &AccessControl::cardChangeSlot);
        connect(wgt, &CardOpert::btnDelClicked, this, &AccessControl::cardRemoveSlot);
        ui->tabInfo->setCellWidget(r, 4, wgt);
    }
    ui->tabInfo->resize(sizeHint());
    ui->wgtInfo->resize(sizeHint());
}

void AccessControl::setHorizontalHeaderLabels(void)
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
    ui->tabInfo->setItem(0, col, pItem);

    pItem = new QTableWidgetItem("门禁卡号");
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabInfo->setItem(0, col+1, pItem);

    pItem = new QTableWidgetItem("门禁卡名");
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabInfo->setItem(0, col+2, pItem);

    pItem = new QTableWidgetItem("控制模块");
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabInfo->setItem(0, col+3, pItem);

    pItem = new QTableWidgetItem("操作");
    pItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    pItem->setForeground(QColor("#00ADE5"));
    pItem->setFont(font);
    ui->tabInfo->setItem(0, col+4, pItem);
}

bool AccessControl::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLineEdit"))
        {
            ShortBeep();

            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj);
            connect(WgtKeyPad::Instance(), &WgtKeyPad::sendInputBufferAreaText, [=](QString text)
            {
                if(lineEdit == ui->lineCardNumVal)
                {
                    QRegExp regExp("[0-9]+$");
                    if (!regExp.exactMatch(text)) {
                        return;
                    }

                    lineEdit->setText(text);
                }else if (lineEdit == ui->lineCardNameVal) {
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

void AccessControl::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    setTabSize();
}

void AccessControl::setTabSize(void)
{
    ui->tabInfo->setMaximumHeight(m_rowCount *50);
    ui->tabInfo->setMinimumHeight(m_rowCount *50);

    ui->wgtInfo->setMaximumHeight(m_rowCount *50);
    ui->wgtInfo->setMinimumHeight(m_rowCount *50);

    int w, h, c_w, r_h;
    w = ui->tabInfo->width();
    h = ui->tabInfo->height();

    c_w = (w - ui->tabInfo->verticalScrollBar()->width())/ (5);
    r_h = 50;//h - ui->tabInfo->horizontalHeader()->height() + 5)/ m_rowCount;
    for(int i  = 0; i < m_rowCount; i++)
    {
        ui->tabInfo->setRowHeight(i, r_h);
    }

    for(int i = 0; i < (5); i++)
    {
        ui->tabInfo->setColumnWidth(i, c_w);
    }
}

void AccessControl::changeEvent(QEvent* event)
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

void AccessControl::retranslateUi(void)
{

}



















