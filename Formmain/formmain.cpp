#include "formmain.h"
#include "loginmanage.h"
#include "wgtmsgbox_h.h"
#include "gettext.h"
#include "buzzer.h"
#include <QSignalMapper>
#include <QToolButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QApplication>
#include <QMessageBox>
#include <QLabel>
#include <QDir>
#include <QPoint>
#include <QAction>
#include <QTimer>
#include <QPropertyAnimation>
#include "mask_widget.h"
#include "sysfunction.h"
#include "languagePublic.h"
#include "Global/RatioStruct.h"


FormMain *FormMain::_instance = 0;
int       FormMain::m_login   = -1; //未登录

AcDevVector FormMain::m_acDev;
DcCardDevVector FormMain::m_dcDev;     //控制模块列表

FormMain::FormMain(QWidget *parent)
    : QWidget(parent),
      almIsHide(true),
      m_sysType(SysTypeEnum::eSysTypeISmart)
{
    this->setAttribute(Qt::WA_QuitOnClose,true);
    qRegisterMetaType<door_Info>("door_Info");
    _instance = this;
    m_alrmNum = 0;



    //初始化共享内存
    CFG_Init();

    //获取系统类型
    m_sysType = SysTypeEnum(sysCfg->sysInfo.sysType);

    //语言初始化  更换语言会再次调用
    language_init();

    //登录初始化
    login_init();

    //数据线程和数据库启动
    shareMem_init();

    //启动过程中先屏蔽蜂鸣器
    BuzzerStop();

    //创建界面
    setupUi();

    //注册权限更新事件
    registerEvent();

    //设置点击标题栏相关区域触发的事件函数
    //登录功能函数
    wgttitlebar::setLoginFunc(&setLoginFunc);

    //设置静音函数
    m_muter = status_noisy;
    m_titleBar->setMuteFunc(&setMetu);
    m_titleBar->setMuteIcon(":/image/voice");

    m_titleBar->setCurrIndex(0);
    connect(m_loginManage,&LoginManage::loginSuccessSignal,this,&FormMain::getMenuList);
    connect(m_readShareMem,&ReadShareMem::sendDoorList,this,&FormMain::opendoorWidget);

    connect(m_readShareMem, &ReadShareMem::sendeMtrAlmItemList, this, [=](QList<alarm_list>& list){
        if(!list.isEmpty()){//当收到告警信息时，将喇叭状态开启，显示第一条告警信息
            m_muter   = status_noisy;
            m_list    = list;
            m_alrmNum = 0;

            m_titleBar->setMuteIcon(":/image/voice");
            m_titleBar->setAlarmMsg(/*list.at(0).parts + */list.at(m_alrmNum).alarm_code, getAlarmColor(list.at(m_alrmNum).Alarmlevel));
        }else {
            m_titleBar->setAlarmMsg("", "");
        }

        //点亮背光
        gpio_write(SCREENSAVER_PIN, 1);
    });

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        if(!m_list.isEmpty()){//循环显示告警信息
            if(m_alrmNum >= m_list.count())
                m_alrmNum = 0;

            m_titleBar->setAlarmMsg(/*list.at(0).parts + */m_list.at(m_alrmNum).alarm_code, getAlarmColor(m_list.at(m_alrmNum).Alarmlevel));
            m_alrmNum++;
        }else {
            m_titleBar->setAlarmMsg("", "");
        }
    });

    //
    this->show();


    //启动蜂鸣器
    BuzzerStart();
    //三秒滚动一次告警信息
    timer->start(3000);
}

FormMain::~FormMain()
{
    CFG_Exit();
}

void FormMain::login_init(void)
{
    m_loginManage = new LoginManage();
    //m_loginManage->initIniToDatabase();
    {
        m_login = LOGIN_NOT_LOGGED;

        //设置标题栏登录信息
        SysFunction::setLoginInfo("", "", /*QObject::tr("未登录") */langN.show[langEnum::strLogin]);
        UserInfo defultUsr;
        defultUsr.usr = "user";
        defultUsr.auth = 2;
        defultUsr.nick ="user";
        m_loginInfo = m_loginManage->querySqlUserInfo(defultUsr);

        //m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "ac",        m_acList);
        //ac从json文件中获取数据
        //默认使用用户角色查询参数权限
        m_loginManage->roleAuthorityJson("user", FormMain::m_acDev); //获取ac数据写入全局数组
        //根据空调协议转换
        m_loginManage->convertToMenuList(FormMain::m_acDev,m_acList);

        m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "ups",       m_upsList);
        m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "meter",     m_meterList);

        m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "waterleak", m_eleakList);
        m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "ths",       m_thsList);
        m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "spds",      m_spdsList);
    }
    //m_loginManage->openModifydataUI();
    //m_loginManage->openLoginUI();
}

void FormMain::shareMem_init()
{
    m_doorWgtIsOpen = false;
    m_readShareMem = new ReadShareMem();
    m_readShareMem->start();
}

void FormMain::almPopupList_init()
{
    //告警列表框
    m_almWidget = new  AlarmBorder(this);
    m_almWidgetPos = QPoint(this->width(),80);
    m_almWidget->move(m_almWidgetPos);

    //告警按钮
    m_almListBtn = new QToolButton(this);
    m_almListBtn->setFixedSize(30,220);
    m_almListBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_almListBtn->setStyleSheet("QToolButton{border-image: url(:/image/alarm1.png);color: rgb(114, 159, 207);}");

    QAction *pAction = new QAction(this);
    pAction->setIcon(QIcon(":/image/home.png"));
    m_almListBtn->setIconSize(QSize(20, 20));

    m_almListBtn->setDefaultAction(pAction);
    m_almListBtn->move(this->width()-m_almListBtn->width(),(this->height()-m_almListBtn->height())/2);
    m_almListBtn->setVisible(false);

    animation  = new QPropertyAnimation(m_almWidget, "geometry");
    animation2 = new QPropertyAnimation(m_almListBtn, "geometry");
    almIsHide  = true;
}

void FormMain::language_init()
{
    if(GetText::getCurrentLangId() == TEXTLANG_CN){
        LanguageCNInit();
    }else if(GetText::getCurrentLangId() == TEXTLANG_EN) {
        LanguageENInit();
    }

    //更换语言需要重新初始化赋值
    AlarmMngInit();
    ExtDevShowInit();
    ExtDevRatioInit();

}

void FormMain::setupUi(void)
{
    int XSize, YSize;
    //获取屏幕分辨率
    GetText::getLcdSize(XSize, YSize);
    this->setGeometry(0, 0, XSize, YSize);
    this->setMinimumSize(XSize, YSize);
    this->setMaximumSize(XSize, YSize);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setObjectName(QStringLiteral("mainLayoutayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    //背景更换
    if(m_sysType == eSysTypeISmart)
        WgtBgScreen::loadImage(":/image/bg_1.png");
    else if(m_sysType == eSysTypeIWit)
        WgtBgScreen::loadImage(":/image/bg_2.png");

    m_wgtMain = new WgtBgScreen(this);
    m_wgtMain->setObjectName(QStringLiteral("wgtMain"));
    m_wgtMain->setMinimumSize(XSize, YSize);
    m_wgtMain->setMaximumSize(XSize, YSize);

    QVBoxLayout *verticalLayout_2 = new QVBoxLayout(m_wgtMain);
    verticalLayout_2->setSpacing(0);
    verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
    verticalLayout_2->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(3);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

    m_titleBar = new wgttitlebar(m_wgtMain);
    m_titleBar->setLogo(":/image/logo.png");
    m_titleBar->setMinimumSize(QSize(0,50));
    m_titleBar->setMaximumSize(QSize(16777215, 50));
    verticalLayout->addWidget(m_titleBar);

    m_stkWidget = new QStackedWidget(m_wgtMain);
    m_stkWidget->setObjectName(QStringLiteral("stackedWidget"));
    verticalLayout->addWidget(m_stkWidget);

    verticalLayout_2->addLayout(verticalLayout);
    mainLayout->addWidget(m_wgtMain);

    if(m_sysType == eSysTypeISmart){
        PageSysOverview  *m_pageSysOverview = new PageSysOverview;
        m_stkWidget->addWidget(m_pageSysOverview);
    }else if(m_sysType == eSysTypeIWit){
        PageSysOverview_  *m_pageSysOverview = new PageSysOverview_;
        m_stkWidget->addWidget(m_pageSysOverview);
    }

    //Extend
    m_pageExtend = new PageExtend;
    m_stkWidget->addWidget(m_pageExtend);

    //数据管理
    m_pageDataManage = new PageDataManage;
    m_stkWidget->addWidget(m_pageDataManage);

    //告警管理
    m_pageAlarm = new PageAlarm;
    m_stkWidget->addWidget(m_pageAlarm);

    //系统设置
    m_pageSysSettings = new PageSystemSettings;
    m_stkWidget->addWidget(m_pageSysSettings);
}

FormMain *FormMain::Instance()
{
    return _instance;
}

void FormMain::getMenuList(bool flag)
{
    if(!flag)
        return;

    m_loginInfo = m_loginManage->getCurrentLogin();
    SysFunction::setLoginInfo(QString("%1").arg(m_loginInfo.user_name), m_loginInfo.user_account, /*QObject::tr("退出")*/langN.show[langEnum::strLogout]);
    m_titleBar->setLoginInfo(m_loginInfo.user_name, m_loginInfo.user_account);

    //更新登录信息
    m_login = LOGIN_LOGGED;

    m_loginManage->roleAuthorityJson(m_loginManage->m_acRoleIndex[m_loginInfo.from_role], FormMain::m_acDev); //获取ac数据写入全局数组
    //根据空调协议转换
    m_loginManage->convertToMenuList(FormMain::m_acDev,m_acList);


    m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "ups",   m_upsList);
    m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "meter", m_meterList);
    //m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "ac",    m_acList);
    m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "waterleak", m_eleakList);
    m_loginManage->sqlRoleAccessMenu(m_loginInfo.from_role, "ths",   m_thsList);

    //发送自定义事件提示相关界面更新显示
    postCustomEvents();
}

void FormMain::popupAlmListWidget()
{
    ShortBeep();
    if(almIsHide){
        animation->setStartValue(QRect(this->width(), 80, m_almWidget->width(), m_almWidget->height()));
        animation->setEndValue(QRect(this->width()-m_almWidget->width(), 80, m_almWidget->width(), m_almWidget->height()));

        animation2->setStartValue(QRect(this->width()-m_almListBtn->width(),(this->height()-m_almListBtn->height())/2,30,220));
        animation2->setEndValue(QRect(this->width() - m_almWidget->width() - m_almListBtn->width() ,(this->height()-m_almListBtn->height())/2,30,220));

        almIsHide = false;
    }else {
        animation->setStartValue(QRect(this->width()-m_almWidget->width(), 80, m_almWidget->width(), m_almWidget->height()));
        animation->setEndValue(QRect(this->width(), 80, m_almWidget->width(), m_almWidget->height()));

        animation2->setStartValue(QRect(this->width() - m_almWidget->width() - m_almListBtn->width() ,(this->height()-m_almListBtn->height())/2,30,220));
        animation2->setEndValue(QRect(this->width()-m_almListBtn->width(),(this->height()-m_almListBtn->height())/2,30,220));

        almIsHide = true;
    }
    m_almWidgetPos = m_almWidget->pos();

    animation->setDuration(500);
    animation2->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation2->setEasingCurve(QEasingCurve::OutQuad);
    animation->start();
    animation2->start();

    if(almIsHide)
        m_almListBtn->setVisible(false);
    else {
        m_almListBtn->setVisible(true);
    }
}

void FormMain::opendoorWidget(door_Info doorinfo)
{

    //qDebug()<<"in opendoorwidgetSlot doorinfo.cardID"<<doorinfo.cardID;
    gpio_write(SCREENSAVER_PIN, 1);
    qDebug()<<"dooinff card"<<doorinfo.cardID;
    if(!m_doorWgtIsOpen){
        m_doorWgtIsOpen = true;
        WgtDoorBox door(doorinfo.door,doorinfo.cardID);
        pDoor = &door;
        QTimer::singleShot(20000, this,[&](){   //30秒自动关闭
            if(pDoor != nullptr)
                pDoor->close();
        });

        door.setWindowTitle(QString::number(doorinfo.cardID));
        door.exec();

        //m_doorCard = 0;
        m_doorWgtIsOpen = false;
        pDoor = nullptr;
    }
}

void FormMain::setMetu(void)
{
    ShortBeep();

    if(FormMain::Instance()->m_muter ==  status_noisy)
    {
        FormMain::Instance()->m_titleBar->setMuteIcon(":/image/escVoice");
        FormMain::Instance()->m_muter = status_mute;
        eMtrData->ctrl.muteOnOffSts = 0;
    }
    else if(FormMain::Instance()->m_muter ==  status_mute) {
        FormMain::Instance()->m_titleBar->setMuteIcon(":/image/voice");
        FormMain::Instance()->m_muter = status_noisy;
        eMtrData->ctrl.muteOnOffSts = 1;
    }
}

void FormMain::setLoginFunc(void)
{
    if(LOGIN_NOT_LOGGED == m_login)
    {
        FormMain::Instance()->m_loginManage->openLoginUI();
    }
    else if(m_login == LOGIN_LOGGED)
    {
        WgtMsgBox_H *msgbox = new WgtMsgBox_H(/*QObject::tr("是否退出登录？")*/langN.show[langEnum::strQuitLogout], MSGBOX_TYPE_ALL);
        msgbox->setAttribute(Qt::WA_DeleteOnClose);
        int ret = msgbox->exec();
        if(ret == MSGBOX_NO_ID)
            return;

        //设置标题栏登录信息
        SysFunction::setLoginInfo("", "", /*QObject::tr("未登录")*/langN.show[langEnum::strLogin]);
        //wgttitlebar::setLoginInfo("", "");
        FormMain::Instance()->m_titleBar->setLoginInfo("", "");

        //清除登录信息
        m_login = LOGIN_NOT_LOGGED;

        //使用默认账号登录，发送更新权限事件
        UserInfo defultUsr;
        defultUsr.usr = "user";
        defultUsr.auth = 2;
        defultUsr.nick ="user";
        //m_loginInfo = m_loginManage->querySqlUserInfo(defultUsr);

        FormMain::Instance()->m_loginManage->roleAuthorityJson(FormMain::Instance()->m_loginManage->m_acRoleIndex[FormMain::Instance()->m_loginInfo.from_role], FormMain::m_acDev); //获取ac数据写入全局数组
        //根据空调协议转换
        FormMain::Instance()->m_loginManage->convertToMenuList(FormMain::m_acDev,FormMain::Instance()->m_acList);

        FormMain::Instance()->m_loginInfo = FormMain::Instance()->m_loginManage->querySqlUserInfo(defultUsr);
        FormMain::Instance()->m_loginManage->sqlRoleAccessMenu(FormMain::Instance()->m_loginInfo.from_role, "ups",   FormMain::Instance()->m_upsList);
        FormMain::Instance()->m_loginManage->sqlRoleAccessMenu(FormMain::Instance()->m_loginInfo.from_role, "meter", FormMain::Instance()->m_meterList);
        //FormMain::Instance()->m_loginManage->sqlRoleAccessMenu(FormMain::Instance()->m_loginInfo.from_role, "ac",    FormMain::Instance()->m_acList);
        FormMain::Instance()->m_loginManage->sqlRoleAccessMenu(FormMain::Instance()->m_loginInfo.from_role, "waterleak", FormMain::Instance()->m_eleakList);
        FormMain::Instance()->m_loginManage->sqlRoleAccessMenu(FormMain::Instance()->m_loginInfo.from_role, "ths",   FormMain::Instance()->m_thsList);

        FormMain::Instance()->postCustomEvents();
    }
}

QString FormMain::getAlarmColor(int grade)
{
    QString color;
    switch (grade) {
    case GeneralAlarm://一般
        color = "#ffaa00";
        break;
    case NormalAlarm://正常
        color = "#ffff00";
        break;
    case CriticalAlarm://严重
        color = "#fc5328";
        break;
    default:
        break;
    }

    return  color;
}

void FormMain::retranslateUi(void)
{
    language_init();

    if(m_login == LOGIN_NOT_LOGGED)
    {
        SysFunction::setLoginInfo("", "", langN.show[langEnum::strLogin]);
        m_titleBar->setLoginInfo("", "");
    }
    else if (m_login == LOGIN_LOGGED) {
        SysFunction::setLoginInfo(QString("%1").arg(m_loginInfo.user_name), m_loginInfo.user_account, langN.show[langEnum::strLogout]);
        m_titleBar->setLoginInfo(m_loginInfo.user_name, m_loginInfo.user_account);
    }
}

void FormMain::changeEvent(QEvent* event)
{
    switch(event->type())
    {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
}

void FormMain::registerEvent(void)
{
    QEvent::registerEventType(PERMISSION_UPDATE);
    QEvent::registerEventType(PARA_UPDATE);
}

void FormMain::postCustomEvents(void)
{
    QWidgetList list = QApplication::allWidgets();
    if(list.isEmpty())
        return;

    //boardcast event
    for (int i = 0; i < list.size(); ++i)
    {
        QWidget *w = list.at(i);
        QApplication::postEvent(w, new QEvent(PERMISSION_UPDATE));
    }
}

bool FormMain::event(QEvent *ev)
{
    //权限更新
    if(ev->type() == PERMISSION_UPDATE)
    {
        return true;
    }

    return QWidget::event(ev);
}

