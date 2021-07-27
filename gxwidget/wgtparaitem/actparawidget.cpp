#include "actparawidget.h"
#include "ui_actparawidget.h"
#include <QPainter>

ActParaWidget::ActParaWidget(const QString &title, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActParaWidget),
    m_title(title)
{
    ui->setupUi(this);
    ui->lbltitle->setText(m_title);

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPixelSize(15);
    ui->lbltitle->setFont(font);
}

ActParaWidget::~ActParaWidget()
{
    delete ui;
}

void ActParaWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    /*QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rect = this->rect();

    painter.setPen(QColor(28, 255 ,214));
    painter.drawLine(rect.bottomLeft().x(), rect.bottomLeft().y(), rect.bottomLeft().x()+10, rect.bottomLeft().y());
    painter.drawLine(rect.bottomLeft().x(), rect.bottomLeft().y(), rect.bottomLeft().x(), rect.bottomLeft().y()-10);*/

}

void ActParaWidget::setTitle(QString title)
{
    m_title = title;
    ui->lbltitle->setText(m_title);
}

void ActParaWidget::setStyle(int style)
{
    /*if(style == STYLE_MIN) {
        ui->wgtBody->setStyleSheet("QWidget#wgtBody{"
                                   "border-image: url(:/image/border_down3.png);"
                                   "}");
    }else if (style == STYLE_MID) {
        ui->wgtBody->setStyleSheet("QWidget#wgtBody{"
                                   "border-image: url(:/image/border_down2.png);"
                                   "}");
    }else if (style == STYLE_MAX) {
        ui->wgtBody->setStyleSheet("QWidget#wgtBody{"
                                   "border-image: url(:/image/border_down.png);"
                                   "}");
    }*/
}

void ActParaWidget::setTitleWidgetVisible(bool l)
{
    ui->widget->setVisible(l);
}
