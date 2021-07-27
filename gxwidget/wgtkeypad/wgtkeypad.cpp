
#include "wgtkeypad.h"
#include "gettext.h"
#include <QBoxLayout>
#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include "buzzer.h"


WgtKeyPad* WgtKeyPad::_instance = 0;

WgtKeyPad::WgtKeyPad(QWidget *parent) :
    QWidget(parent),
    cursorGlobalPos(0,0),
    isMousePress(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);

    int XSize, YSize;
    GetText::getLcdSize(XSize, YSize);
    this->setMinimumSize(XSize, 300);
    this->setMaximumSize(XSize, 300);
    this->move(0, YSize - 300);

    this->initStyleSheet();
    this->initInputBufferArea();
    this->initFunctionAndCandidateArea();
    this->initKeysArea();
    this->selectKeyboardStyle(1);
    this->setMoveEnabled(false);

    QVBoxLayout *globalVLayout = new QVBoxLayout(this);
    globalVLayout->setMargin(0);
    globalVLayout->setSpacing(0);
    globalVLayout->addWidget(inputBufferArea, 1);
    //globalVLayout->addWidget(functionAndCandidateArea, 1);
    globalVLayout->addWidget(keysArea, 5);

    //readDictionary(); //拼音包
    showInputBufferArea();
}

WgtKeyPad::~WgtKeyPad()
{

}

/*
 *@brief:   选择键盘样式，所有按键及功能候选区域背景的样式在此设置
 *@param:   num:确定用哪一套样式（皮肤）
 */
void WgtKeyPad::selectKeyboardStyle(int num)
{
    if(num >= keysAreaStyle.size())
    {
        return;
    }
    skinNum = num;
    hideSoftKeyboard->setStyleSheet(hideSoftKeyboardStyle.at(num));
    inputBufferArea->setStyleSheet(inputBufferAreaStyle.at(num));
    keysArea->setStyleSheet(keysAreaStyle.at(num));
    functionAndCandidateArea->setStyleSheet(functionAndCandidateAreaStyle.at(num));
}
/*
 *@brief:   设置无边框窗口是否可以移动
 *@param:   moveEnabled:移动使能
 */
void WgtKeyPad::setMoveEnabled(bool moveEnabled)
{
    isMoveEnabled = moveEnabled;
}
/*
 *@brief:   显示输入缓存区
 * 该接口主要应用场景：点击某一个编辑框，弹出软键盘(可能会遮挡点击的编辑框)，键盘的输入内容会临时
 * 存放到内置的缓存区(标题和初始内容可通过该接口设置)，在点击回车时把缓存区的内容通过信号的形式发
 * 送出来，传递给外部的编辑框。
 *@param:   inputTitle:输入缓存区标题
 *@param:   inputContent:输入缓存区的初始内容
 */
void WgtKeyPad::showInputBufferArea(QString inputContent)
{
    inputBufferArea->setVisible(true);
    inputContentEdit->setText(inputContent);
    inputContentEdit->setFocus();
    currentLineEdit = inputContentEdit;
}
/*
 *@brief:   隐藏输入缓存区
 * 该接口主要应用场景：点击某一个编辑框，编辑框预计不会被软键盘遮挡，在弹出键盘之前调用该接口传递点击
 * 的编辑框指针，这样键盘输入的内容会直接显示在外部的编辑框中，不再需要使用信号传递
 *@param:   currLineEdit:外部的编辑框指针
 */
void WgtKeyPad::hideInputBufferArea(QLineEdit *currLineEdit)
{
    inputBufferArea->setVisible(false);
    currentLineEdit = currLineEdit;
}
/*
 *@brief:   鼠标按下事件处理
 */
void WgtKeyPad::mousePressEvent(QMouseEvent *event)
{
    if(event->button()  == Qt::LeftButton)
    {
        isMousePress    = true;
        cursorGlobalPos = event->globalPos();
    }
}
/*
 *@brief:   鼠标移动事件处理
 */
void WgtKeyPad::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton && isMousePress && isMoveEnabled)
    {
        QPoint position = this->pos() + event->globalPos() - cursorGlobalPos;
        move(position.x(), position.y());
        cursorGlobalPos = event->globalPos();
    }
}
/*
 *@brief:   鼠标释放事件处理
 */
void WgtKeyPad::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMousePress = false;
    }
}
/*
 *@brief:   初始化样式表，即用于界面皮肤选择
 */
void WgtKeyPad::initStyleSheet()
{
    /*皮肤1:雅黑*/
    keysAreaStyle.append(
                ".QWidget{background-color:#1E1E1E;}"
                "QToolButton:!pressed{background-color:#4E4E4E;color:#E6E6E6;border-radius:4px;font:12pt;}"
                "QToolButton:pressed{background-color:#4E4E4E;color:#E6E6E6;	border:2px solid #E6E6E6;border-radius:4px;font:12pt;}"
                "QToolButton#specialKeyStyle{background-color:#2D2D2D;border-radius:4px;font:12pt;}");
     functionAndCandidateAreaStyle.append(
                "QWidget{background-color:#1E1E1E;color:#E6E6E6;}"
                "QToolButton{border-style:none;}"
                "QToolButton#pageBtn{font-weight:bold;}"
                "QToolButton#pageBtn:disabled{color:#6c97b4;}"
                "QLineEdit{background-color:#4E4E4E;}");
    inputBufferAreaStyle.append(".QWidget{background-color:#1E1E1E;}");
    hideSoftKeyboardStyle.append(
                "QToolButton:!pressed{background-color:#1E1E1E;border-radius:4px;}"
                "QToolButton:pressed{background-color:#1E1E1E;border:2px solid #E6E6E6;border-radius:4px;}");

    /*皮肤2:简白*/
    keysAreaStyle.append(
                ".QWidget{background-color:#D8D8D8;}"
                "QToolButton:!pressed{background-color:#FFFFFF;color:black;border-radius:4px;font:12pt;}"
                "QToolButton:pressed{background-color:#FFFFFF;color:black;	border:2px solid rgb(255, 0, 255);border-radius:4px;font:12pt;}"
                "QToolButton#specialKeyStyle{background-color:#EBEBEB;border-radius:4px;font:12pt;}");
    functionAndCandidateAreaStyle.append(
                "QWidget{background-color:#D8D8D8;color:black;}"
                "QToolButton{border-style:none;}"
                "QToolButton#pageBtn{font-weight:bold;}"
                "QToolButton#pageBtn:disabled{color:#6c97b4;}"
                "QLineEdit{background-color:white;}");
    inputBufferAreaStyle.append(".QWidget{background-color:#D8D8D8;}");
    hideSoftKeyboardStyle.append(
                "QToolButton:!pressed{background-color:#D8D8D8;border-radius:4px;}"
                "QToolButton:pressed{background-color:#D8D8D8;border:2px solid rgb(255, 0, 255);border-radius:4px;}");

    /*皮肤3:魅紫*/
    keysAreaStyle.append(
                ".QWidget{background-color:#190724;}"
                "QToolButton:!pressed{background-color:#272A5E;color:#68CBF2;border-radius:4px;font:12pt;}"
                "QToolButton:pressed{background-color:#272A5E;color:#68CBF2;	border:2px solid #68CBF2;border-radius:4px;font:12pt;}"
                "QToolButton#specialKeyStyle{background-color:#241D48;border-radius:4px;font:12pt;}");
    functionAndCandidateAreaStyle.append(
                "QWidget{background-color:#190724;color:#68CBF2;}"
                "QToolButton{border-style:none;}"
                "QToolButton#pageBtn{font-weight:bold;}"
                "QToolButton#pageBtn:disabled{color:#6c97b4;}"
                "QLineEdit{background-color:#272A5E;}");
    inputBufferAreaStyle.append(".QWidget{background-color:#190724;}");
    hideSoftKeyboardStyle.append(
                "QToolButton:!pressed{background-color:#190724;border-radius:4px;}"
                "QToolButton:pressed{background-color:#190724;border:2px solid #68CBF2;border-radius:4px;}");
}
/*
 *@brief:   初始化软键盘的数字字母(字符)按键
 */
void WgtKeyPad::initNumberLetterBtn()
{
    for(int i = 0; i < 36; i++)
    {
        numberLetterBtn[i] = new QToolButton();
        numberLetterBtn[i]->setToolButtonStyle(Qt::ToolButtonTextOnly);
        numberLetterBtn[i]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        connect(numberLetterBtn[i],SIGNAL(clicked()),this,SLOT(numberLetterBtnSlot()));
    }
}
/*
 *@brief:   初始化软键盘的特殊功能按键,诸如大小写切换/删除等
 */
void WgtKeyPad::initSpecialBtn()
{
    upperOrLowerBtn = new QToolButton();
    upperOrLowerBtn->setObjectName("specialKeyStyle");
    upperOrLowerBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    upperOrLowerBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    upperOrLowerBtn->setText("A/a");
    connect(upperOrLowerBtn,SIGNAL(clicked()),this,SLOT(changeUpperLowerSlot()));

    deleteBtn = new QToolButton();
    deleteBtn->setObjectName("specialKeyStyle");
    deleteBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    deleteBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    deleteBtn->setIcon(QIcon(":/image/backspace.png"));
    deleteBtn->setIconSize(QSize(70, 45));
    deleteBtn->setAutoRepeatDelay(300);
    deleteBtn->setAutoRepeatInterval(60);
    deleteBtn->setAutoRepeat(true);
    connect(deleteBtn,SIGNAL(clicked(bool)),this,SLOT(deleteTextSlot()));

    skinBtn = new QToolButton();
    skinBtn->setObjectName("specialKeyStyle");
    skinBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    skinBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    skinBtn->setIcon(QIcon(":/image/skin.png"));
    skinBtn->setIconSize(QSize(70, 45));
    connect(skinBtn,SIGNAL(clicked()),this,SLOT(changeSkinSlot()));

    letterOrSymbolBtn = new QToolButton();
    letterOrSymbolBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    letterOrSymbolBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    letterOrSymbolBtn->setText("abc");
    connect(letterOrSymbolBtn,SIGNAL(clicked()),this,SLOT(changeLetterSymbolSlot()));

    commaBtn = new QToolButton();
    commaBtn->setMinimumSize(36,16);
    commaBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    commaBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    commaBtn->setText(",");
    connect(commaBtn,SIGNAL(clicked()),this,SLOT(numberLetterBtnSlot()));

    spaceBtn = new QToolButton();
    spaceBtn->setText(" ");
    spaceBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    spaceBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    connect(spaceBtn,SIGNAL(clicked()),this,SLOT(spaceSlot()));

    periodBtn = new QToolButton();
    periodBtn->setMinimumSize(36,16);
    periodBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    periodBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    periodBtn->setText(".");
    connect(periodBtn,SIGNAL(clicked()),this,SLOT(numberLetterBtnSlot()));

    chOrEnBtn = new QToolButton();
    chOrEnBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    chOrEnBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    chOrEnBtn->setText("EN");
    connect(chOrEnBtn,SIGNAL(clicked()),this,SLOT(changeChEnSlot()));

    isENInput = true;

    enterBtn  = new QToolButton();
    enterBtn->setObjectName("specialKeyStyle");
    enterBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    enterBtn->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    enterBtn->setText("  Enter  ");
    connect(enterBtn,SIGNAL(clicked()),this,SLOT(enterSlot()));
}
/*
 *@brief:   设置小写字母显示
 */
void WgtKeyPad::setLetterLow()
{
    this->isLetterInput = true;
    this->isLetterLower = true;
    this->letterOrSymbolBtn->setText("abc");
    QStringList letterLowList;
    letterLowList<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"0"
                 <<"q"<<"w"<<"e"<<"r"<<"t"<<"y"<<"u"<<"i"<<"o"<<"p"
                 <<"a"<<"s"<<"d"<<"f"<<"g"<<"h"<<"j"<<"k"<<"l"
                 <<"z"<<"x"<<"c"<<"v"<<"b"<<"n"<<"m";
    for(int i=0;i<36;i++)
    {
        numberLetterBtn[i]->setText(letterLowList.at(i));
    }
}
/*
 *@brief:   设置大写字母显示
 */
void WgtKeyPad::setLetterUpper()
{
    this->isLetterInput = true;
    this->isLetterLower = false;
    this->letterOrSymbolBtn->setText("abc");
    QStringList letterUpperList;
    letterUpperList<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"0"
                   <<"Q"<<"W"<<"E"<<"R"<<"T"<<"Y"<<"U"<<"I"<<"O"<<"P"
                   <<"A"<<"S"<<"D"<<"F"<<"G"<<"H"<<"J"<<"K"<<"L"
                   <<"Z"<<"X"<<"C"<<"V"<<"B"<<"N"<<"M";
    for(int i = 0; i < 36; i++)
    {
        numberLetterBtn[i]->setText(letterUpperList.at(i));
    }
}
/*
 *@brief:   设置符号显示，包含所有可输入符号(英文状态)
 */
void WgtKeyPad::setSymbolsEN()
{
    this->isLetterInput = false;
    this->letterOrSymbolBtn->setText(":;?");
    QStringList symbolsENList;
    symbolsENList<<"!"<<"@"<<"#"<<"$"<<"%"<<"^"<<"&&"<<"*"<<"("<<")"
               <<"["<<"]"<<"{"<<"}"<<"<"<<">"<<"+"<<"-"<<"_"<<"="
               <<"`"<<"~"<<"|"<<"\\"<<"'"<<"\""<<":"<<";"<<"?"
               <<"/"<<"..."<<"---"<<"www."<<".com"<<".cn"<<".net";
    for(int i=0;i<36;i++)
    {
        numberLetterBtn[i]->setText(symbolsENList.at(i));
    }
}
/*
 *@brief:   设置符号显示，包含所有可输入符号(中文状态)
 */
void WgtKeyPad::setSymbolsCH()
{
    this->isLetterInput = false;
    this->letterOrSymbolBtn->setText(":;?");
    QStringList symbolsCHList;

    symbolsCHList<<QString::fromUtf8("！")<<"@"<<"#"<<QString::fromUtf8("￥")<<"%"<<QString::fromUtf8("…")<<"&&"<<"*"<<QString::fromUtf8("（")<<QString::fromUtf8("）")
            <<QString::fromUtf8("【")<<QString::fromUtf8("】")<<"{"<<"}"<<QString::fromUtf8("《")<<QString::fromUtf8("》")<<"+"<<"-"<<QString::fromUtf8("—")<<"="
            <<"."<<"~"<<"|"<<QString::fromUtf8("、")<<QString::fromUtf8("’")<<QString::fromUtf8("‘")<<QString::fromUtf8("“")<<QString::fromUtf8("”")<<QString::fromUtf8("：")
            <<QString::fromUtf8("；")<<QString::fromUtf8("？")<<QString::fromUtf8("♀")<<QString::fromUtf8("℃")<<QString::fromUtf8("★")<<QString::fromUtf8("→")<<QString::fromUtf8("←");
    for(int i=0;i<36;i++)
    {
        numberLetterBtn[i]->setText(symbolsCHList.at(i));
    }
}
/*
 *@brief:   初始化键盘输入缓存区，临时存放显示输入的内容
 */
void WgtKeyPad::initInputBufferArea()
{    

    inputContentEdit = new QLineEdit();
    inputContentEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    currentLineEdit  = inputContentEdit;

    hideSoftKeyboard = new QToolButton();
    hideSoftKeyboard->setToolButtonStyle(Qt::ToolButtonIconOnly);
    hideSoftKeyboard->setMinimumSize(65, 40);
    hideSoftKeyboard->setMaximumSize(65, 40);
    hideSoftKeyboard->setIcon(QIcon(":/image/pack_up.png"));
    hideSoftKeyboard->setIconSize(QSize(35, 35));
    connect(hideSoftKeyboard, SIGNAL(clicked()), this, SLOT(clearAndCloseSlot()));

    //输入缓存部件
    inputBufferWidget = new QWidget();
    QHBoxLayout *hBoxLayout = new QHBoxLayout(inputBufferWidget);
    hBoxLayout->setContentsMargins(10,6,15,6);
    hBoxLayout->setSpacing(15);
    hBoxLayout->addWidget(inputContentEdit);
    hBoxLayout->addWidget(hideSoftKeyboard);

    //输入缓存区
    inputBufferArea = new QWidget();
    QHBoxLayout *inputBufferAreaLayout = new QHBoxLayout(inputBufferArea);
    inputBufferAreaLayout->setMargin(0);
    inputBufferAreaLayout->addWidget(inputBufferWidget,2);

}
/*
 *@brief:   初始化功能和候选词区域,该区域使用栈部件,同一时刻只显示一个区域
 */
void WgtKeyPad::initFunctionAndCandidateArea()
{
    //功能区
    QString mode;
    if(isENInput)
        mode = QString::fromUtf8("英文模式");
    else
        mode = QString::fromUtf8("中文模式");

    introduceLabel = new QLabel();
    introduceLabel->setAlignment(Qt::AlignCenter);
    introduceLabel->setText(mode);

    functionArea = new QWidget();
    QHBoxLayout *functionAreaLayout = new QHBoxLayout(functionArea);
    functionAreaLayout->setContentsMargins(8,0,8,0);
    functionAreaLayout->addWidget(introduceLabel);

    //候选字母显示框
    candidateLetter = new QLineEdit();
    candidateLetter->setFrame(false);
    candidateLetter->setEnabled(false);
    candidateLetter->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    connect(candidateLetter,SIGNAL(textChanged(QString)),this,SLOT(candidateLetterChangedSlot(QString)));

    //候选词、翻页按钮显示区域
    candidateWordArea = new QWidget();
    QHBoxLayout *candidateWordAreaLayout = new QHBoxLayout(candidateWordArea);
    candidateWordAreaLayout->setMargin(0);
    for(int i=0;i<CANDIDATEWORDNUM;i++)
    {
        candidateWordBtn[i] = new QToolButton();
        candidateWordBtn[i]->setToolButtonStyle(Qt::ToolButtonTextOnly);
        candidateWordBtn[i]->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);   
        connect(candidateWordBtn[i],SIGNAL(clicked()),this,SLOT(candidateWordBtnSlot()));
        candidateWordAreaLayout->addWidget(candidateWordBtn[i]);
    }

    prePageBtn = new QToolButton();
    prePageBtn->setObjectName("pageBtn");
    prePageBtn->setText("   <   ");
    prePageBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    prePageBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    connect(prePageBtn,SIGNAL(clicked()),this,SLOT(candidateWordPrePageSlot()));

    nextPageBtn = new QToolButton();
    nextPageBtn->setObjectName("pageBtn");
    nextPageBtn->setText("   >   ");
    nextPageBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    nextPageBtn->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    connect(nextPageBtn,SIGNAL(clicked()),this,SLOT(candidateWordNextPageSlot()));
    candidateWordAreaLayout->addWidget(prePageBtn);
    candidateWordAreaLayout->addWidget(nextPageBtn);

    candidateArea = new QWidget();
    QVBoxLayout *vBoxLayout = new QVBoxLayout(candidateArea);
    vBoxLayout->setMargin(0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->addWidget(candidateLetter);
    vBoxLayout->addWidget(candidateWordArea);

    //栈部件存放功能区和候选区
    functionAndCandidateArea = new QStackedWidget();
    functionAndCandidateArea->addWidget(functionArea);
    functionAndCandidateArea->addWidget(candidateArea);
}
/*
 *@brief:   初始化键盘按键区域
 */
void WgtKeyPad::initKeysArea()
{
    initNumberLetterBtn();
    initSpecialBtn();
    setLetterLow();

    //第一排按键 10个数字
    QHBoxLayout *firstRowHLayout = new QHBoxLayout();
    for(int i = 0; i < 10; i++)
    {
        firstRowHLayout->addWidget(numberLetterBtn[i]);
    }
    //第二排按键
    QHBoxLayout *secondRowHLayout = new QHBoxLayout();
    for(int i = 10; i < 20; i++)
    {
        secondRowHLayout->addWidget(numberLetterBtn[i]);
    }
    //第三排按键
    QHBoxLayout *thirdRowHLayout = new QHBoxLayout();
    thirdRowHLayout->setContentsMargins(20,0,20,0);
    for(int i = 20; i < 29; i++)
    {
        thirdRowHLayout->addWidget(numberLetterBtn[i]);
    }
    //第四排按键
    QHBoxLayout *fourthRowHLayout = new QHBoxLayout();
    fourthRowHLayout->addWidget(upperOrLowerBtn);
    for(int i = 29; i < 36; i++)
    {
        fourthRowHLayout->addWidget(numberLetterBtn[i]);
    }
    fourthRowHLayout->addWidget(deleteBtn);
    //第五排按键
    QHBoxLayout *fifthRowHLayout = new QHBoxLayout();
    fifthRowHLayout->addWidget(skinBtn,2);
    fifthRowHLayout->addWidget(letterOrSymbolBtn,1);
    fifthRowHLayout->addWidget(commaBtn,1);
    fifthRowHLayout->addWidget(spaceBtn,4);
    fifthRowHLayout->addWidget(periodBtn,1);
    fifthRowHLayout->addWidget(chOrEnBtn,1);
    fifthRowHLayout->addWidget(enterBtn,2);

    keysArea = new QWidget();
    QVBoxLayout *vBoxlayout = new QVBoxLayout(keysArea);
    vBoxlayout->setContentsMargins(8,2,8,8);
    vBoxlayout->addLayout(firstRowHLayout);
    vBoxlayout->addLayout(secondRowHLayout);
    vBoxlayout->addLayout(thirdRowHLayout);
    vBoxlayout->addLayout(fourthRowHLayout);
    vBoxlayout->addLayout(fifthRowHLayout);
}
/*
 *@brief:   读拼音字典，将汉字与对应拼音存放到hash表中
 */
void WgtKeyPad::readDictionary()
{
    QFile pinyinFile(PINYINFILEPATH);
    if(!pinyinFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    else
    {
        QRegExp regExp("[a-z']+");//正则表达式，匹配1个或多个由a-z及 ' 组成的字母串，默认区分大小写
        QString lineText;//存放读取的一行数据 汉字-拼音
        QString linePinyin;//存放正则表达式匹配的拼音
        QString lineChinese;//存放拼音对应的汉字
        int pinyinPosition;//每一行匹配拼音的位置
        while(!pinyinFile.atEnd())
        {
            lineText       = QString(QString::fromUtf8(pinyinFile.readLine()));
            pinyinPosition = regExp.indexIn(lineText,0);//获取读取行的文本中匹配正则表达式的位置
            linePinyin     = regExp.cap(0);//regExp.cap(0)表示完整正则表达式的匹配
            lineChinese    = lineText.left(pinyinPosition);//lineText.left(n)可以获取左边那个字符即对应的汉字
            if(linePinyin.contains("'"))//如果有单引号表示是词组，则进行拆分词组
            {
                splitPhrase(linePinyin,lineChinese);
            }
            else//单个汉字
            {
                chinesePinyin.insert(linePinyin,lineChinese);//往哈希表插入键值对
            }
        }
    }
}
/*
 *@brief:   拆分拼音词组，拼音字典文件词组用'分割，如"ai'qing"该函数的功能便是去掉'，
 * 将简拼、全拼存放到哈希表中
 *@param:   phrase:要处理的拼音词组
 *@param:   chinese:拼音对应的汉字
 */
void WgtKeyPad::splitPhrase(QString phrase,QString chinese)
{
    int count = phrase.count("'");
    if(count == 1 )//两个汉字
    {
        int index       = phrase.indexOf("'");
        QString pinyin1 = phrase.left(1);//两字首字母简拼 例aq
        pinyin1.append(phrase.at(index+1));
        chinesePinyin.insert(pinyin1,chinese);

        QString pinyin2 = phrase.left(index);//全拼+首字母 aiq
        pinyin2.append(phrase.at(index+1));
        if(pinyin2 != pinyin1)//避免同一词组键值对插入哈希表多次 例如 e'xi
        {
            chinesePinyin.insert(pinyin2,chinese);
        }
        QString pinyin3 = phrase.remove("'");//全拼
        if(pinyin3 != pinyin2)
        {
             chinesePinyin.insert(pinyin3,chinese);
        }
    }
    else if(count == 2)//三个汉字
    {
        int index1 = phrase.indexOf("'");
        int index2 = phrase.indexOf("'",index1+1);

        QString pinyin1 = phrase.left(1);//三字首字母简拼
        pinyin1.append(phrase.at(index1+1));
        pinyin1.append(phrase.at(index2+1));
        chinesePinyin.insert(pinyin1,chinese);

        QString pinyin2 = phrase.left(index1);//全拼+首字母+首字母
        pinyin2.append(phrase.at(index1+1));
        pinyin2.append(phrase.at(index2+1));
        if(pinyin2 != pinyin1)//避免同一词组键值对插入哈希表多次 例如 e'xi
        {
            chinesePinyin.insert(pinyin2,chinese);
        }

        QString pinyin3 = phrase.left(index2);//全拼+全拼+首字母
        pinyin3.append(phrase.at(index2+1));
        pinyin3.remove("'");
        if(pinyin3 != pinyin2)//避免同一词组键值对插入哈希表多次 例如 e'xi
        {
            chinesePinyin.insert(pinyin3,chinese);
        }

        QString pinyin4 = phrase.remove("'");//全拼
        if(pinyin4 != pinyin3)//避免同一词组键值对插入哈希表多次 例如 e'xi
        {
            chinesePinyin.insert(pinyin4,chinese);
        }
    }
    else if(count == 3)//四个汉字
    {
        int index1 = phrase.indexOf("'");
        int index2 = phrase.indexOf("'",index1+1);
        int index3 = phrase.indexOf("'",index2+1);

        QString pinyin1 = phrase.left(1);//四字首字母简拼
        pinyin1.append(phrase.at(index1+1));
        pinyin1.append(phrase.at(index2+1));
        pinyin1.append(phrase.at(index3+1));
        chinesePinyin.insert(pinyin1,chinese);

        QString pinyin2 = phrase.left(index1);//全拼+首字母+首字母+首字母
        pinyin2.append(phrase.at(index1+1));
        pinyin2.append(phrase.at(index2+1));
        pinyin2.append(phrase.at(index3+1));
        if(pinyin2 != pinyin1)//避免同一词组键值对插入哈希表多次 例如 e'xing'xun'huan
        {
            chinesePinyin.insert(pinyin2,chinese);
        }

        QString pinyin3 = phrase.left(index2);//全拼+全拼+首字母+首字母
        pinyin3.append(phrase.at(index2+1));
        pinyin3.append(phrase.at(index3+1));
        pinyin3.remove("'");
        if(pinyin3 != pinyin2)//避免同一词组键值对插入哈希表多次 例如 e'xing'xun'huan
        {
            chinesePinyin.insert(pinyin3,chinese);
        }

        QString pinyin4 = phrase.left(index3);//全拼+全拼+全拼+首字母
        pinyin4.append(phrase.at(index3+1));
        pinyin4.remove("'");
        if(pinyin4 != pinyin3)//避免同一词组键值对插入哈希表多次 例如 e'xing'xun'huan
        {
            chinesePinyin.insert(pinyin4,chinese);
        }

        QString pinyin5 = phrase.remove("'");//全拼
        if(pinyin5 != pinyin4)//避免同一词组键值对插入哈希表多次 例如 e'xing'xun'huan
        {
            chinesePinyin.insert(pinyin5,chinese);
        }
    }
}
/*
 *@brief:   根据输入的拼音匹配中文
 *@param:   pinyin:输入的拼音
 */
void WgtKeyPad::matchChinese(QString pinyin)
{
    hanzi.clear();//每次匹配中文都先清空之前的列表
    //哈希表chinesePinyin中存放着拼音-汉字的键值对（一键多值），获取对应拼音的汉字列表
    hanzi = chinesePinyin.values(pinyin);

    pageCount = 1;//每次匹配，候选词初始化显示第一页
}
/*
 *@brief:   显示指定页的候选词
 *@param:   page:指定页,从1开始
 */
void WgtKeyPad::displayCandidateWord(int page)
{
    int hanziCount = hanzi.size();//匹配的汉字列表个数
    if(page == 1)//当显示第一页时向前翻页按钮不可点击
    {
        prePageBtn->setEnabled(false);
    }
    else
    {
        prePageBtn->setEnabled(true);
    }
    if(page*CANDIDATEWORDNUM >= hanziCount)
    {
        nextPageBtn->setEnabled(false);
    }
    else
    {
        nextPageBtn->setEnabled(true);
    }
    /*因为哈希表获取指定的key对应的值列表时,是按照后插入的先获取(后进先出),
     * 所以常见字在后面,这里处理为从列表的最后一个反向获取显示
     */
    int num = hanziCount - 1 - (page-1)*CANDIDATEWORDNUM;
    for(int i = 0; i < CANDIDATEWORDNUM; i++)//每页默认6个候选字词
    {
        if(num>=i)
        {
            candidateWordBtn[i]->setText(hanzi.at(num-i));
            candidateWordBtn[i]->setEnabled(true);
        }
        else
        {
            candidateWordBtn[i]->setText(" ");//清除上一页的缓存
            candidateWordBtn[i]->setEnabled(false);
        }
    }
}
/*
 *@brief:   隐藏中文输入的候选区域
 */
void WgtKeyPad::hideCandidateArea()
{
    candidateLetter->clear();
    functionAndCandidateArea->setCurrentWidget(functionArea);
}
/*
 *@brief:   中文输入时候选字母区域根据内容改变文本框的大小
 *@param:   text:即改变后的文本内容
 */
void WgtKeyPad::candidateLetterChangedSlot(QString text)
{
     ShortBeep();

    int width = candidateLetter->fontMetrics().width(text)+6;
    candidateLetter->setFixedWidth(width);
}
/*
 *@brief:   候选词被点击的响应槽
 */
void WgtKeyPad::candidateWordBtnSlot()
{
    ShortBeep();

    QToolButton *clickedBtn = qobject_cast<QToolButton *>(sender());
    currentLineEdit->insert(clickedBtn->text());
    hideCandidateArea();
}
/*
 *@brief:   候选词向前翻页
 */
void WgtKeyPad::candidateWordPrePageSlot()
{
    ShortBeep();

    pageCount--;
    this->displayCandidateWord(pageCount);
}
/*
 *@brief:   候选词向后翻页
 */
void WgtKeyPad::candidateWordNextPageSlot()
{
    ShortBeep();

    pageCount++;
    this->displayCandidateWord(pageCount);
}
/*
 *@brief:   字母(符号)按键被点击的响应槽
 */
void WgtKeyPad::numberLetterBtnSlot()
{
    ShortBeep();

    QToolButton *clickedBtn = qobject_cast<QToolButton *>(sender());
    if(isENInput)//英文输入
    {
        if(clickedBtn->text() == "&&")//因为可显示控件把&符号当成快捷键标志，一个不显示，所以这个要做下特别处理
        {
            currentLineEdit->insert("&");
        }
        else
        {
            currentLineEdit->insert(clickedBtn->text());//文本输入框插入字母或符号
        }
    }
    else  //中文输入模式 键入的字母放在第二部分输入显示区域的候选字母按钮上
    {
        functionAndCandidateArea->setCurrentWidget(candidateArea);
        candidateLetter->insert(clickedBtn->text());//候选字母输入框插入字母
        this->matchChinese(candidateLetter->text());//匹配中文
        this->displayCandidateWord(pageCount);//显示候选词，默认第一页
    }
}
/*
 *@brief:   大小写切换 也可以切换数字字母与字符界面
 */
void WgtKeyPad::changeUpperLowerSlot()
{
    ShortBeep();

    if(isLetterInput)//当前为字母界面
    {
        if(isLetterLower)
        {
            this->setLetterUpper();
        }
        else
        {
            this->setLetterLow();
        }
    }
    else  //当前为数字字符界面,则返回之前的大小写字母界面
    {
        if(isLetterLower)
        {
            this->setLetterLow();
        }
        else
        {
            this->setLetterUpper();
        }
    }
}
/*
 *@brief:   删除输入内容
 */
void WgtKeyPad::deleteTextSlot()
{
    ShortBeep();

    if(functionAndCandidateArea->currentWidget() == candidateArea)
    {
        candidateLetter->backspace();//删除选中文本或光标前的一个字符，默认光标在最后
        if(candidateLetter->text().isEmpty())//删完了
        {
            hideCandidateArea();
        }
        else
        {
            matchChinese(candidateLetter->text());//重新匹配拼音
            displayCandidateWord(pageCount);//显示候选词
        }
    }
    else
    {
        currentLineEdit->backspace();
    }
}
/*
 *@brief:   切换皮肤
 */
void WgtKeyPad::changeSkinSlot()
{
    ShortBeep();

    if(skinNum == keysAreaStyle.size()-1)
    {
        skinNum = 0;
    }
    else
    {
        skinNum++;
    }
    selectKeyboardStyle(skinNum);
}
/*
 *@brief:   切换数字字母/符号界面
 */
void WgtKeyPad::changeLetterSymbolSlot()
{
    ShortBeep();

    if(isLetterInput)//当前为字母界面
    {
        if(isENInput)
        {
            this->setSymbolsEN();
        }
        else
        {
            this->setSymbolsCH();
        }
    }
    else  //如果当前为符号界面 返回到数字字母界面
    {
        if(isLetterLower)
        {
            this->setLetterLow();
        }
        else
        {
            this->setLetterUpper();
        }
    }
}
/*
 *@brief:   空格响应槽
 */
void WgtKeyPad::spaceSlot()
{
    ShortBeep();

    if(functionAndCandidateArea->currentWidget() == candidateArea)
    {
        currentLineEdit->insert(candidateWordBtn[0]->text());
        hideCandidateArea();
    }
    else
    {
        currentLineEdit->insert(" ");
    }
}
/*
 *@brief:   中英文切换
 */
void WgtKeyPad::changeChEnSlot()
{
    ShortBeep();
    return;

    if(isENInput)
    {
        isENInput = false;
        chOrEnBtn->setText("CH");
        introduceLabel->setText(QString::fromUtf8("中文模式"));
        commaBtn->setText(QString::fromUtf8("，"));
        periodBtn->setText(QString::fromUtf8("。"));
        if(isLetterInput)
        {
            this->setLetterLow();
        }
        else
        {
            this->setSymbolsCH();
        }
    }
    else
    {
        isENInput = true;
        chOrEnBtn->setText("EN");
        introduceLabel->setText(QString::fromUtf8("英文模式"));
        commaBtn->setText(",");
        periodBtn->setText(".");
        if(!isLetterInput)
        {
            this->setSymbolsEN();
        }
    }
}
/*
 *@brief:   回车响应槽
 */
void WgtKeyPad::enterSlot()
{
    ShortBeep();

    //候选字母非空，则将字母插入到编辑框里
    if(!candidateLetter->text().isEmpty())
    {
        currentLineEdit->insert(candidateLetter->text());
        hideCandidateArea();
    }
    else
    {
        //输入缓存区显示时，将缓存的内容通过信号发送出去
        if(inputBufferArea->isVisible())
        {
            emit sendInputBufferAreaText(inputContentEdit->text());
        }
        clearAndCloseSlot();
    }
}
/*
 *@brief:   清理并关闭键盘
 */
void WgtKeyPad::clearAndCloseSlot()
{
    ShortBeep();

    showInputBufferArea();
    hideCandidateArea();
    disconnect(this, 0, 0, 0);
    this->close();
}
