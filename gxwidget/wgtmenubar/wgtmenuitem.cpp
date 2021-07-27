#include "wgtmenuitem.h"
#include <QPainter>

WgtMenuItem::WgtMenuItem(QWidget *parent) : QToolButton(parent)
{
    currId = 0;
    setMouseTracking(true);
    setCheckable(true);

    connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

void WgtMenuItem::onClicked()
{
    emit itemClicked(currId);
}
