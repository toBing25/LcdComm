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

#ifndef LOGINUI_H
#define LOGINUI_H

#include <QWidget>
#include <QDialog>
#include <QPainter>
#include "wgtkeypad.h"

namespace Ui {
class LoginUI;
}

class LoginUI : public QDialog
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = nullptr);
    ~LoginUI();
protected:
    //void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void btnSubmitSlot();
    void btnQuitSlot();
signals:
    void userInfoSignal(QString account,QString passwd);
public slots:
    void loginSuccess(bool);

private:
    Ui::LoginUI *ui;
    QVector<QLineEdit *> lieVector;
    //WgtKeyPad    m_softKeyboard;

};

#endif // LOGINUI_H
