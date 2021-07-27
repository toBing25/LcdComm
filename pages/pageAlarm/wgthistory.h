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

#ifndef WGTHISTORY_H
#define WGTHISTORY_H

#include <QWidget>
#include "alarmwidget.h"
#include "wgtkeypad.h"
#include <formmain.h>
namespace Ui {
class WgtHistory;
}
class wgtWait;
class WgtHistory : public QWidget
{
    Q_OBJECT

    enum STATUS{
        status_One,
        status_All
    };

public:
    explicit WgtHistory(QWidget *parent = nullptr);
    ~WgtHistory();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent* event);

signals:
    void sendAlmHistoryData(int devType,QVector<int> devNoList,QVector<int> levelList,QDateTime lowerDate,QDateTime upperDate,int page);
private slots:    
    //获取历史记录链表
    void getHistoryList(almDataStruct list);

    //最近一天/最近一周/最近一个月响应槽
    void onBtnLastHistory(bool clicked);

private:
    void setupUi(void);
    void calculationTotalPage(int totalPage);
    void queryAlmDatabase(int page=1);
    void upPageAlmDatabase();
    void downPageAlmDatabase();
    void setHorizontalHeaderLabels(void);

    void retranslateUi(void);
private:
    Ui::WgtHistory *ui;
    QVector<QLineEdit *> lieVector;
    wgtWait     *dlg;


    int          m_currentPage;
    int          m_totalPage;
    int          m_tmpPage;
    STATUS       m_status;
};

#endif // WGTHISTORY_H
