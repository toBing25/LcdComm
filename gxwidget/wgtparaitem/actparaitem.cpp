#include "actparaitem.h"
#include "ui_actparaitem.h"
#include <QPainter>
#include <QDebug>

ActParaItem::ActParaItem(QString name, QString val, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActParaItem),
    m_name(name),
    m_value(val)
{
    ui->setupUi(this);
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(14);

    ui->lblParaName->setFont(font);
    ui->lblParaVal->setFont(font);

    ui->lblParaName->setText(m_name);
    ui->lblParaVal->setText(m_value);
}

ActParaItem::~ActParaItem()
{
    delete ui;
}

void ActParaItem::setVal(QString val)
{
    m_value = val;
    ui->lblParaVal->setText(val);
}

void ActParaItem::setUnit(QString unit)
{
    if(unit != ""){
        QString str =m_name+ "("+unit+")";
        ui->lblParaName->setText(str);
    }

}

void ActParaItem::setWidgetStyle(int style)
{
    QString qss;
    //右边
    if(style == 1)
    {
        qss = ".ActParaItem{"
              "color: rgb(255, 255, 255);"
              "border-bottom:1px solid #01ADDE;"
              "border-right:1px solid #01ADDE;"
              "border-radius:opx;font:13pt;"
              "}";
    }//左边
    else if (style == 2) {
        qss = ".ActParaItem{"
              "color: rgb(255, 255, 255);"
              "border-bottom:1px solid #01ADDE;"
              "border-right:1px solid #01ADDE;"
              "border-left:1px solid #01ADDE;"
              "border-radius:opx;font:13pt;"
              "}";
    }else if (style == 3) {
        qss = ".ActParaItem{"
              "color: rgb(255, 255, 255);"
              "border:1px solid #01ADDE;"
              "}";
    } else {
        qss = ".ActParaItem{"
              "color: rgb(255, 255, 255);"
              "border-right:1px solid #01ADDE;"
              "}";
    }

    this->setStyleSheet(qss);
}

void ActParaItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
