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

#ifndef WGTTITLEBAR_H
#define WGTTITLEBAR_H

#include <QWidget>

namespace Ui {
class wgttitlebar;
}

typedef  void (*pfunction)(void);
class wgttitlebar : public QWidget
{
    Q_OBJECT

public:
    explicit wgttitlebar(QWidget *parent = nullptr);
    ~wgttitlebar();

    //设置LOGO函数
    static void setLogo(QString strLogo);
    //设置静音函数
    static void setMuteFunc(pfunction pf);
    //设置静音图标
    void setMuteIcon(QString strIcon);
    //设置登录函数
    static void setLoginFunc(pfunction pf);
    //设置登录信息
    void setLoginInfo(QString  username, QString account);
    //设置告警信息
    void setAlarmMsg(QString msg, QString color);
    //设置当前页的索引
    void setCurrIndex(int index);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);

private slots:
    void updateTime(void);
    void onButtonClick();

private:
    Ui::wgttitlebar  *ui;
    static pfunction pfMuteFunc;
    static pfunction pfLoginFunc;
    static QString   m_strLogo;
    QString          m_strAccount;
    QString          m_strUserName;
    QString          m_strMuteIcon;
    QString          m_strAlarmMsg;
    QString          m_AlarmColor;

};

#endif // WGTTITLEBAR_H
