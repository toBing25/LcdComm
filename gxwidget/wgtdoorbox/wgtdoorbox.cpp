#include "wgtdoorbox.h"
#include "ui_wgtdoorbox.h"
#include "buzzer.h"
#include "formmain.h"
WgtDoorBox::WgtDoorBox(QVector<int> doorlist, int devNo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WgtDoorBox)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    ui->label->setText(langN.show[langEnum::strOpenDoor]);

    int toolTotal = doorlist.length();
    //单行显示数量
    int rowNum = 2,toolW=60,toolH=40;
    int toolRow = (toolTotal + rowNum-1)/rowNum;
    //默认尺寸显示3行，超过3行，每增加一行高度增加3个按钮高度
    if(toolRow > 3 ){
        if(toolRow > 5){toolRow =5;}
        this->resize(this->width(),this->height()+(toolRow-3)*(toolH*3));
    }


    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(40,0,40,0);

    if(toolTotal>0)
    {
        int toolRow = (toolTotal + rowNum-1)/rowNum;
        int endRowNum = rowNum;
        for(int i=0;i<toolRow;i++){
            QHBoxLayout *oneHlay = new QHBoxLayout;
            oneHlay->setMargin(0);

            oneHlay->setContentsMargins(10,10,10,10);
            oneHlay->setSpacing(15);

            if(i == toolRow-1 and toolTotal%rowNum != 0)  //最后一页
            {
                endRowNum = toolTotal%rowNum;
            }
            for(int j=i*rowNum+0;j<i*rowNum+endRowNum;j++){


                //前门
                QToolButton * toolFront = new QToolButton(ui->mainWidget);
                toolFront->setText(QString::number(doorlist.at(j)+1) + "#" + langN.show[langEnum::strFrontDoor]);
                oneHlay->addWidget(toolFront);
                toolFront->setMinimumSize(80,40);
                toolFront->setObjectName(QString::number(doorlist.at(j))+"0");

                toolFront->setStyleSheet("border-image: url(:/image/btn.png);"
                                         "border-bottom:0px solid rgb(214, 216, 224);"
                                         "border-radius:0px;"
                                         "color: rgb(255, 255, 255);"
                                         "font-size:16px;");

                connect(toolFront,&QToolButton::clicked,this,&WgtDoorBox::openDoorHandle);
                //后门
                QToolButton * toolBack = new QToolButton(ui->mainWidget);
                toolBack->setText(QString::number(doorlist.at(j)+1) + "#" + langN.show[langEnum::strBackDoor]);
                oneHlay->addWidget(toolBack);
                toolBack->setMinimumSize(80,40);
                toolBack->setObjectName(QString::number(doorlist.at(j))+"1");
                toolBack->setStyleSheet("border-image: url(:/image/btn.png);"
                                         "border-bottom:0px solid rgb(214, 216, 224);"
                                         "border-radius:0px;"
                                         "color: rgb(255, 255, 255);"
                                         "font-size:16px;");
                connect(toolBack,&QToolButton::clicked,this,&WgtDoorBox::openDoorHandle);
            }

            vlay->addLayout(oneHlay);
            oneHlay->addStretch();


        }

    }else{
        //提示错误
        //qDebug()<<"card err";
        QFont font;
        font.setPointSize(13);
        font.setBold(true);

        QLabel *lab = new QLabel(ui->mainWidget);
        QHBoxLayout *labhlay = new QHBoxLayout;
        lab->setText(QString::number(devNo)+/*tr(" 为无效卡号")*/langN.show[langEnum::strRearDoor]);
        lab->setFont(font);
        lab->setStyleSheet("color: rgb(255, 255, 255);");
        lab->setAlignment(Qt::AlignCenter);
        labhlay->addStretch();
        labhlay->addWidget(lab);
        labhlay->addStretch();

        vlay->addLayout(labhlay);

    }
    vlay->addStretch();
    ui->mainWidget->setLayout(vlay);
    ui->mainWidget->resize(sizeHint());

    connect(ui->closeBtn,&QPushButton::clicked,[=](){
        ShortBeep();
        this->close();
    });
    connect(this,&WgtDoorBox::sendOpenDoor,FormMain::Instance()->getShareMemManage()->getWriteData(),
            &WtriteShareMem::writeDevData);

}

WgtDoorBox::~WgtDoorBox()
{
    delete ui;
}

void WgtDoorBox::openDoorHandle()
{
    qDebug()<<"in openDoorHandle";
    ShortBeep();

    QToolButton *tool = qobject_cast<QToolButton *>(sender());
    int totalVal = tool->objectName().toInt();
    int id, door;

    if(totalVal>=0){
        door = totalVal%10;
        id   = totalVal/10;
    }
    qDebug()<<"sendopenDoor id door"<<id<<door;
    emit sendOpenDoor(ExtDevEnum::eExtDevDc,id,door);
}
