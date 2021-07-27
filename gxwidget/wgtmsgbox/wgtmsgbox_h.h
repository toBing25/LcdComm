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

#ifndef _WGTMSGBOX_H_H_
#define _WGTMSGBOX_H_H_


#define MSGBOX_NO_ID   0
#define MSGBOX_YES_ID  1

#define MSGBOX_TYPE_OK    0  //只有OK键
#define MSGBOX_TYPE_NOKEY 1  //没有按键
#define MSGBOX_TYPE_ALL   2  //全有，OK键和cance键

#include <QDialog>

namespace Ui {
class FormWgtMsgBox_H;
}

class WgtMsgBox_H : public QDialog
{
    Q_OBJECT

public:
    /*
	* Message: 要显示的信息
    * Type   ：要显示的按键  MSGBOX_TYPE_OK:     只有OK键
    *                      MSGBOX_TYPE_NOKEY:  没有按键
    *					   MSGBOX_TYPE_ALL:    全有，OK键和cance键
	*/
    WgtMsgBox_H(const QString &Message, int Type, QWidget *parent = 0);
    ~WgtMsgBox_H();

private slots:
    int  onBtnOkClicked(void);
    int  onBtnCancelClicked(void);

protected:
    void onAutoClose();

protected:
    void showEvent(QShowEvent *e);
    //void paintEvent(QPaintEvent *);

private:
    Ui::FormWgtMsgBox_H *ui_c;
	int                  m_Type;
};

#endif  /* _WGTMSGBOX_H_H_ */

