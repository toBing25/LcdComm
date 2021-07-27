#include "mscrollarea.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>

MScrollArea::MScrollArea(QWidget *parent)
    : QScrollArea(parent)
{
    this->setMouseTracking(true);
}

MScrollArea::~MScrollArea()
{

}

void MScrollArea::mousePressEvent(QMouseEvent *e)
{
    m_point = e->pos();
}

void MScrollArea::mouseMoveEvent(QMouseEvent *e)
{
    QPoint pos = e->pos();

    //获取垂直滚动条
    QScrollBar *vscrollbar  = this->verticalScrollBar();

    int offset_up, offset_down, scroll_val, step, move, val;
    offset_up   = m_point.y() - pos.y();
    offset_down = pos.y() - m_point.y();

    scroll_val  = vscrollbar->value();
    step        = vscrollbar->pageStep();
    move        = (offset_up * step)/this->widget()->height();

    if((move + scroll_val < 0) || (move + scroll_val > this->widget()->height()))
        return;

    val  = move + scroll_val;
    vscrollbar->setValue(val);
    emit sigScrollVal();
}
