#include "loginui.h"
#include "ui_loginui.h"
#include "buzzer.h"
#include "gettext.h"
#include "languagePublic.h"
#include <QDebug>

LoginUI::LoginUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginUI)
{
    ui->setupUi(this);
    //setMinimumSize(350,200);
    //setMaximumSize(350,200);
    //setAttribute(Qt::WA_TranslucentBackground, true);

    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);
    //setAutoFillBackground(false);  //这个不设置的话就背景变黑

    ui->lEdit_passwd->setEchoMode(QLineEdit::Password);
    //this->setStyleSheet("QLabel{color: rgb(255, 255, 255);}");
    //this->setStyleSheet("QPushButton{color: rgb(255, 255, 255);}");

    ui->label->setText(langN.show[langEnum::strUsern] + ":");
    ui->label_2->setText(langN.show[langEnum::strPsw] + ":");
    ui->btn_submit->setText(langN.show[langEnum::strLogin]);
    ui->btn_quit->setText(langN.show[langEnum::strLogout]);


    connect(ui->btn_submit,&QPushButton::clicked,this,&LoginUI::btnSubmitSlot);
    connect(ui->btn_quit,&QPushButton::clicked,this,&LoginUI::btnQuitSlot);

    lieVector.append(ui->lEdit_user);
    lieVector.append(ui->lEdit_passwd);
    ui->lEdit_user->installEventFilter(this);
    ui->lEdit_passwd->installEventFilter(this);
}

LoginUI::~LoginUI()
{
    delete ui;
}

//void LoginUI::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event);
//    QPainter painter(this);
//    painter.fillRect(this->rect(), QColor(29, 138, 189, 180));
//    //QColor最后一个参数80代表背景的透明度，如果为0就是全透明
//}

void LoginUI::btnSubmitSlot()
{
    ShortBeep();

    QString account = ui->lEdit_user->text().simplified();
    QString passwd  = ui->lEdit_passwd->text().simplified();
    if(!account.isEmpty() and !passwd.isEmpty()){
        emit userInfoSignal(account,passwd);
    }
}

void LoginUI::btnQuitSlot()
{
    ShortBeep();

    this->close();
}

void LoginUI::loginSuccess(bool ok)
{
    QString info ="";
    if(ok){
        info = /*tr("登录成功")*/langN.show[langEnum::strLoginSuccess];
    }
    else {
        info = /*tr("登录失败,请检查用户名和密码")*/langN.show[langEnum::strLoginFail];
    }
    ui->label_info->setText(info);
}

bool LoginUI::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonRelease:
    {
        if(obj->inherits("QLineEdit"))
        {
            ShortBeep();

            QLineEdit *lineEdit = qobject_cast<QLineEdit *>(obj);
            connect(WgtKeyPad::Instance(), &WgtKeyPad::sendInputBufferAreaText, [=](QString text)
            {
                if(lineEdit != 0)
                {
                    lineEdit->setText(text);
                    return;
                }
            });

            foreach (QLineEdit *lie, lieVector) {
                if(lie == lineEdit)
                {
                    WgtKeyPad::Instance()->showInputBufferArea(lie->text());
                    WgtKeyPad::Instance()->show();
                    return  true;
                }
            }
        }
    }
    default:
        break;
    }
    return QWidget::eventFilter(obj, event);
}

