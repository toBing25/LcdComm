#include "wgtbgscreen.h"
#include <QPainter>
#include <QEvent>

QPixmap *WgtBgScreen::g_pix = 0;
QStringList WgtBgScreen::tabBgList;

WgtBgScreen::WgtBgScreen(QWidget *parent) : QWidget(parent)
{
}

WgtBgScreen::~WgtBgScreen()
{
}

void WgtBgScreen::paintEvent(QPaintEvent *event)
{
    if(g_pix)
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, this->width(), this->height(), *g_pix);
    }
    else
        QWidget::paintEvent(event);
}

bool WgtBgScreen::event(QEvent *e)
{
    if(e->type() == QEvent::PaletteChange)
        update();

    return QWidget::event(e);
}
