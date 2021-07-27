#include "alarmborder.h"
#include "ui_alarmborder.h"
#include "formmain.h"

AlarmBorder::AlarmBorder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlarmBorder)
{
    ui->setupUi(this);
    ui->label->setText(langN.show[langEnum::strCurAlm]);
    //connect(FormMain::Instance()->getShareMemManage(), &ReadShareMem::sendeMtrAlmItemList, ui->wgtAlarm, &AlarmWidget::getAlarmList);
}

AlarmBorder::~AlarmBorder()
{
    delete ui;
}

void AlarmBorder::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void AlarmBorder::changeEvent(QEvent* event)
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

void AlarmBorder::retranslateUi(void)
{
    ui->label->setText(langN.show[langEnum::strCurAlm]);
}
