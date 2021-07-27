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
class WgtHistoryData;
}
class wgtWait;
class WgtHistoryData : public QWidget
{
    Q_OBJECT

    enum STATUS{
        status_One,
        status_All
    };

public:
    explicit WgtHistoryData(QWidget *parent = nullptr);
    ~WgtHistoryData();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent* event);

signals:
    void sendStatsHistoryData(int type, QVector<int>devNoList, QDateTime lower, QDateTime upper, int page);

private slots:    
    //获取历史记录链表
    void getHistoryList(almDataStruct list);
    void getStatsHistoryData(statsDataStruct list);
    void getUpsHistoryData(upsDataStruct list);
    void getMeterHistoryData(meterDataStruct list);
    void getAcHistoryData(acDataStruct list);
    void getThsHistoryData(thsDataStruct list);
    void getPueHistoryData(pueDataStruct list);

    //最近一天/最近一周/最近一个月响应槽
    void onBtnLastHistory(bool clicked);
    void retranslateUi(void);
    void curIndexChanged(int index);
private:
    void setupUi(void);
    void calculationTotalPage(int totalPage);
    void queryHistoryDatabase(int page=1);
    void upPageAlmDatabase();
    void downPageAlmDatabase();

    void setTabColumnWidth();

    void setHeaderLabels(QList<langEnum> labels);
private:
    Ui::WgtHistoryData *ui;
    QVector<QLineEdit *> lieVector;
    wgtWait     *dlg_wait;

    int              m_currentPage;
    int              m_totalPage;
    int              m_tmpPage;
    STATUS           m_status;
    QList<langEnum>  m_statsHeader;
    QFont            m_font;
};

#endif // WGTHISTORY_H
