#include "wgtmenuitem_ext.h"
#include <QPainter>
#include <QHBoxLayout>

WgtMenuItem_Ext::WgtMenuItem_Ext(QWidget *parent) : QToolButton(parent)
{
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(10, 0, 0, 0);
    hlayout->setSpacing(5);

    m_text = new QLabel;
    m_text->setStyleSheet("color: #00CCFF;font:12pt;");

    m_icon = new QLabel;

    hlayout->addWidget(m_icon);
    hlayout->addWidget(m_text);
    hlayout->addStretch();
    this->setLayout(hlayout);

    setMouseTracking(true);
    setCheckable(true);

    connect(this, &WgtMenuItem_Ext::clicked, this, &WgtMenuItem_Ext::onClicked);
}

void WgtMenuItem_Ext::onClicked(bool checked)
{
    emit itemClicked(checked);
}

void WgtMenuItem_Ext::setColor(QString color)
{
    m_text->setStyleSheet(QString("color: %1;font:12pt;").arg(color));
}

void WgtMenuItem_Ext::setText(const QString text)
{
    m_text->setText(text);
}

void WgtMenuItem_Ext::setIcon(const QString icon)
{
    m_icon->setPixmap(QPixmap(icon));
}
