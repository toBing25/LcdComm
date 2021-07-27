#include "wgtbgrender.h"
#include "ui_wgtbgrender.h"

wgtBgRender::wgtBgRender(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wgtBgRender)
{
    ui->setupUi(this);
}

wgtBgRender::~wgtBgRender()
{
    delete ui;
}
