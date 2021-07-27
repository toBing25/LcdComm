#include "wgtwait.h"
#include "ui_wgtwait.h"
#include <QDesktopWidget>
#include <QMovie>
#include <QDebug>
#include <QPixmap>
wgtWait::wgtWait(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wgtWait)
{
    ui->setupUi(this);
    //this->setAutoFillBackground(false);
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setAttribute(Qt::WA_TranslucentBackground, true); //用这种方式在板子上背景为黑色


    //QImage *img = new QImage;
    //img->load(":/image/environ.png");
    //ui->label->setPixmap(QPixmap::fromImage(*img));
    //QMovie * move = new QMovie(":/image/timg.gif");
    //ui->label->setMovie(move);
    //ui->label->setScaledContents(true);
    //move->start();

    ui->widget->startAnimation();

}

wgtWait::~wgtWait()
{
    ui->widget->stopAnimation();
    delete ui;
}

void wgtWait::showEvent(QShowEvent *)
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

