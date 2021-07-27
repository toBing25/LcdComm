#include "cardopert.h"
#include <QVBoxLayout>

CardOpert::CardOpert(CardDb card, QWidget *parent)
    : QWidget(parent),
      m_card(card)
{

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setSpacing(10);
    hLayout->setContentsMargins(10, 3, 10, 3);

    m_btnModify = new QPushButton;
    m_btnModify->setIcon(QIcon(":/image/modify.png"));
    m_btnModify->setIconSize(QSize(25, 25));
    m_btnModify->setFocusPolicy(Qt::NoFocus);
    m_btnModify->setStyleSheet("background-color: rgba(255, 255, 255, 0);"
                               "border:0px solid gray;"
                               "border-radius:0px;"
                               "color: rgb(255, 255, 255);");

    m_btnDel = new QPushButton;
    m_btnDel->setIcon(QIcon(":/image/del.png"));
    m_btnDel->setIconSize(QSize(35, 35));
    m_btnDel->setFocusPolicy(Qt::NoFocus);
    m_btnDel->setStyleSheet("background-color: rgba(255, 255, 255, 0);"
                               "border:0px solid gray;"
                               "border-radius:0px;"
                               "color: rgb(255, 255, 255);");


    hLayout->addWidget(m_btnModify);
    hLayout->addWidget(m_btnDel);
    this->setLayout(hLayout);

    connect(m_btnModify, &QPushButton::clicked, this, [=](bool checked){
        Q_UNUSED(checked);
        emit btnModifyClicked(m_card);
    });

    connect(m_btnDel, &QPushButton::clicked, this, [=](bool checked){
        Q_UNUSED(checked);
        emit btnDelClicked(m_card);
    });
}

CardOpert::~CardOpert()
{

}
