/****************************************************************************
**
** Copyright (C) 2021 The INVT Company . 
**
** This file is part of LcdComm.
** LcdComm is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** LcdComm is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with LcdComm.  If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/


#ifndef WGTKEYPAD_H
#define WGTKEYPAD_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QFile>
#include <QMessageBox>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QPoint>

//默认候选词数量
#define CANDIDATEWORDNUM 6
#define PINYINFILEPATH  "./ChinesePinyin"

class WgtKeyPad : public QWidget
{
    Q_OBJECT

private:
    static WgtKeyPad* _instance;

private:
    explicit WgtKeyPad(QWidget *parent = 0);

public:
    ~WgtKeyPad();

    static WgtKeyPad *Instance(){
        if(_instance == 0)
            _instance = new WgtKeyPad;
        return _instance;
    }

    void selectKeyboardStyle(int num);
    void setMoveEnabled(bool moveEnabled = true);
    void showInputBufferArea(QString inputContent = QString());
    void hideInputBufferArea(QLineEdit *currLineEdit);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void initStyleSheet();
    void initNumberLetterBtn();
    void initSpecialBtn();
    void setLetterLow();
    void setLetterUpper();
    void setSymbolsEN();
    void setSymbolsCH();

    void initInputBufferArea();
    //初始化功能和候选区域
    void initFunctionAndCandidateArea();
    //初始化按键区域
    void initKeysArea();

    //读拼音字典，将汉字与拼音的对应存放到hash表中
    void readDictionary();
    //拆分拼音词组
    void splitPhrase(QString phrase,QString chinese);
    //根据输入的拼音匹配中文
    void matchChinese(QString pinyin);
    //显示指定页的候选词
    void displayCandidateWord(int page);
    //隐藏中文输入显示区域
    void hideCandidateArea();

signals:
    //缓存区文本发出
    void sendInputBufferAreaText(QString text);

private slots:
    //候选字母改变响应槽
    void candidateLetterChangedSlot(QString text);
    //候选词被点击的响应槽
    void candidateWordBtnSlot();
    //候选词向前翻页
    void candidateWordPrePageSlot();
    //候选词向后翻页
    void candidateWordNextPageSlot();
    //数字字母(符号)按键被点击的响应槽
    void numberLetterBtnSlot();
    //切换大小写，也可以切换数字字母与符号界面
    void changeUpperLowerSlot();
    //删除输入
    void deleteTextSlot();
    //切换皮肤
    void changeSkinSlot();
    //数字字母与字符切换
    void changeLetterSymbolSlot();
    //空格被按下时的响应槽
    void spaceSlot();
    //中英文切换
    void changeChEnSlot();
    //回车被按下的响应槽
    void enterSlot();
    //清理并关闭键盘
    void clearAndCloseSlot();

private:
    //存放拼音汉字键值对的哈希表
    QMultiHash<QString,QString> chinesePinyin;
    //存储匹配的汉字词
    QList<QString> hanzi;

    bool        isENInput;          //中英文输入模式
    bool        isLetterInput;      //数字字母或符号输入模式
    bool        isLetterLower;      //大小写模式
    int         skinNum;            //当前皮肤编号
    int         pageCount;          //候选词当前页

    QPoint      cursorGlobalPos;
    bool        isMousePress;
    bool        isMoveEnabled;

    //键盘输入缓存区
    QWidget     *inputBufferArea;
    QWidget     *inputBufferWidget;
    QLineEdit   *inputContentEdit;
    QLineEdit   *currentLineEdit;
    QStringList inputBufferAreaStyle;

    //功能和候选区区域的样式
    QStackedWidget *functionAndCandidateArea;
    QStringList     functionAndCandidateAreaStyle;

    //功能区  后期可以添加各种功能配置的入口按钮
    QWidget     *functionArea;
    QLabel      *introduceLabel;

    //候选词区域 包括候选字母、候选词和翻页按钮
    QWidget     *candidateArea;
    QLineEdit   *candidateLetter;      //中文输入时对应的字母显示
    QWidget     *candidateWordArea;
    QToolButton *candidateWordBtn[CANDIDATEWORDNUM];//中文输入时的候选词
    QToolButton *prePageBtn;
    QToolButton *nextPageBtn;

    QWidget     *keysArea;             //键盘的按键区域
    QStringList  keysAreaStyle;        //按键区域的样式

    //10个数字按键，26个字母按键,同时可以显示符号
    QToolButton *numberLetterBtn[36];

    QToolButton *hideSoftKeyboard;     //隐藏键盘
    QStringList  hideSoftKeyboardStyle;

    QToolButton *upperOrLowerBtn;      //大小写转换按键
    QToolButton *deleteBtn;            //删除按键
    QToolButton *skinBtn;              //切换皮肤
    QToolButton *letterOrSymbolBtn;    //数字字母与符号切换按键
    QToolButton *commaBtn;             //逗号按键
    QToolButton *spaceBtn;             //空格按键
    QToolButton *periodBtn;            //句号按键
    QToolButton *chOrEnBtn;            //中英文切换按键
    QToolButton *enterBtn;             //回车按键
};

#endif // WGTKEYPAD_H
