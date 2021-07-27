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

#ifndef WGTMENUBAR_EXT_H
#define WGTMENUBAR_EXT_H

#include <QWidget>
#include <QListView>
#include <QStackedWidget>
#include <QSplitter>
#include <QToolButton>
#include <QPushButton>

#include "wgtlistviewdelegate.h"
#include "wgtlistviewmodel.h"

typedef  struct DEV{
     int         titleId1;
     int         titleId2;
     int         devIndex;
}dev_ist;

typedef QList<dev_ist> DEV_LIST;

class WgtMenuItem_Ext;
class WgtMenuBar_Ext : public QWidget
{
    Q_OBJECT
public:
    explicit WgtMenuBar_Ext(QWidget *parent = 0,   int navMinWidth  = 100,
                           int bodyMinWidth = 200, int navInitWidth = 180);
    ~WgtMenuBar_Ext();

    /*
     *		level    :表示当前树的第几层
     *		titleId1 :表示当前树的编号
     *      titleId2 :表示当前树的第几个子节点
     */
    bool addTitle(const QString &label, const QString &decorate, const QString &decorate2, int level, int titleId1, int titleId2 = 0, int titleId3 = 0);
    bool addWidget(QWidget *widget, const QString &label, const QString &decorate, const QString &decorate2, int level, int titleId1, int titleId2 = 0, int titleId3 = 0);
    bool bodyAddWidget( int titleId1, int titleId2, int devtype, QWidget *widget);

    //删除总的节点
    bool removeWidget(int titleId1);
    bool jumpItem(int titleId1, int titleId2);

    bool removeChildWidget(int titleId1, int titleId2);
    bool jumpChildItem(int titleId1, int titleId2, int titleId3);

    void  setAllNodeCollapse(void);

    void setNavStyleSheet(const QString style);
    void setBodyStyleSheet(const QString style);

protected:
    void paintEvent(QPaintEvent * event);
    void showEvent(QShowEvent *event);

private:
    void setBtnChecked(bool clicked);
    void setGuideBtntVisible(void);

public slots:
    void widgetChanged(int titleId1, int titleId2, int titleId3);
    void clicked(const QModelIndex& index);

private:
    QListView           *m_menuListView;
    wgtListViewModel    *m_model;
    wgtListViewDelegate *m_delegate;
    QStackedWidget      *m_body;
    WgtMenuItem_Ext     *m_btn;
    QWidget             *m_wgtMenu;
    QList<quint16>       m_wdgIdList;
    DEV_LIST             m_devList;
    int                  m_navMinWidth;
    int                  m_bodyMinWidth;
    int                  m_navInitWidth;
    int                  m_preiWgtId;

    QPushButton         *m_btnUp;
    QPushButton         *m_btnDown;
    QStringList          m_iconlist;
};

#endif // WGTMENUBAR_EXT_H
