
#include "wgtmsgbox_h.h"
#include "ui_formwgtmsgbox_h.h"
#include "buzzer.h"
#include <QApplication>
#include <QStyleOption>
#include <QPainter>
#include <QDesktopWidget>


WgtMsgBox_H::WgtMsgBox_H(const QString &Message, int Type, QWidget *parent)
    : QDialog(parent),
      ui_c(new Ui::FormWgtMsgBox_H),
	  m_Type(Type)
{
    ui_c->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    //this->setAttribute(Qt::WA_DeleteOnClose);

    if(m_Type == MSGBOX_TYPE_OK)
    {
        ui_c->btnCancel->setVisible(false);
        connect(ui_c->btnOk,     SIGNAL(clicked()), this, SLOT(onBtnOkClicked()));
    }
    else if (m_Type == MSGBOX_TYPE_NOKEY)
    {
        ui_c->btnCancel->setVisible(false);
        ui_c->btnOk->setVisible(false);
    }
    else
    {
        connect(ui_c->btnCancel, SIGNAL(clicked()), this, SLOT(onBtnCancelClicked()));
        connect(ui_c->btnOk,     SIGNAL(clicked()), this, SLOT(onBtnOkClicked()));
    }
	
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(16);

    ui_c->lblMessage->setText(Message);
    ui_c->lblMessage->setFont(font);
}

WgtMsgBox_H::~WgtMsgBox_H()
{
}

int WgtMsgBox_H::onBtnOkClicked(void)
{
    ShortBeep();

    this->done(MSGBOX_YES_ID);

    return 0;
}

int WgtMsgBox_H::onBtnCancelClicked(void)
{
    ShortBeep();

    this->done(MSGBOX_NO_ID);

    return 0;
}

void WgtMsgBox_H::onAutoClose(void)
{
    onBtnOkClicked();
}

void WgtMsgBox_H::showEvent(QShowEvent *)
{
    QRect src;
    if(parent() == 0)
        src = QApplication::desktop()->screenGeometry();
    else {
        src = static_cast<QWidget *>(parent())->rect();
        src.setX(0);
        src.setY(0);
    }
    move(src.center() - rect().center());
}

//void WgtMsgBox_H::paintEvent(QPaintEvent *)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}
