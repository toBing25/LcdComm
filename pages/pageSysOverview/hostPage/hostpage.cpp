#include "hostpage.h"
#include "ui_hostpage.h"
#include "conversiontool.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#define AlmImgString    ":/hostImg/alarm.png"
#define normalImgString ":/hostImg/normal1.png"
HostPage::HostPage(QWidget *parent) :
    QWidget(parent),
    switchTime(0),
    prevIsNormalImg(true),
    ui(new Ui::HostPage)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&HostPage::switchImg);

    //setAttribute(Qt::WA_TranslucentBackground, true);
    img = new QImage;    
    img->load(":/hostImg/normal1.png");
    //ui->hostImage_label->setPixmap(QPixmap::fromImage(*img));
    ui->hostImage_label->setPixmap(QPixmap::fromImage((*img).scaled(ui->hostImage_label->width(), ui->hostImage_label->height(),
                                                              Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));


}

HostPage::~HostPage()
{
    delete ui;
}



void HostPage::doorStsChange(QList<int> doorStsList, int almNum)
{
    if(doorStsList.count() == 0)
        return;

    //qDebug()<<"almNum"<<almNum;
    int stsLen = doorStsList.count();
    int stsV = 0;
    if(stsLen >0 and stsLen <=2){
        //转换为0 1号位状态
        for(int i=0;i<stsLen;i++){
            switch (doorStsList.at(i)) {
                case 4:
                    stsV = ConversionTool::setBit(stsV,0,1);
                    break;
                case 5:
                    stsV = ConversionTool::setBit(stsV,0,0);
                    break;
                case 6:
                    stsV = ConversionTool::setBit(stsV,1,1);
                    break;
                case 7:
                    stsV = ConversionTool::setBit(stsV,1,0);
                    break;

            }
        }
    }
    //平
    switch (stsV) {
        case 0:
            if(almNum>0){
                if(!timer->isActive()){timer->start(1000);}
                prevIsNormalImg = false;
            }else{
                //if(prevIsNormalImg){ return; }  //如果一直没有告警，图片不更新
                if(timer->isActive()){timer->stop();}
                img->load(normalImgString);
                prevIsNormalImg = true;
            }
            break;
        case 1:
            img->load(":/hostImg/frontOpen1.png");
            timer->stop();
            break;
        case 2:
            img->load(":/hostImg/frontOpen1.png");
            timer->stop();
            break;
        case 3:
            img->load(":/hostImg/frontOpen1.png");
            timer->stop();
            break;
        default:
            if(almNum>0){
                if(!timer->isActive()){timer->start(1000);}
            }else{
                timer->stop();
                img->load(normalImgString);
            }
            break;
    }

    //ui->hostImage_label->setPixmap(QPixmap::fromImage(*img));
    ui->hostImage_label->setPixmap(QPixmap::fromImage((*img).scaled(ui->hostImage_label->width(), ui->hostImage_label->height(),
                                                              Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
}

void HostPage::switchImg()
{

    if(switchTime == 0)
        img->load(normalImgString);
    else if(switchTime == 1)
        img->load(AlmImgString);
    //ui->hostImage_label->setPixmap(QPixmap::fromImage(*img));
    ui->hostImage_label->setPixmap(QPixmap::fromImage((*img).scaled(ui->hostImage_label->width(), ui->hostImage_label->height(),
                                                              Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));

    switchTime++;
    if(switchTime>1){ switchTime = 0; }

}
