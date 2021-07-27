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

#ifndef WGTMENUBAR_H
#define WGTMENUBAR_H

#include "wgtmenuitem.h"
#include "languagePublic.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QList>
#include <QEvent>

//权限更新事件
#define PERMISSION_UPDATE  ((QEvent::Type)(QEvent::User + 1))
//参数更新
#define PARA_UPDATE        ((QEvent::Type)(QEvent::User + 2))

#define Item_Spacing    15
#define Item_Size       60
#define Icon_Size       20
#define Min_Width       70

typedef struct ENABLE_T{
     int  itemId;
     bool enable;
}item_enable;

typedef QVector<item_enable> ENABLE_VECTOR;

class WgtMenuBar : public QWidget
{
    Q_OBJECT
public:
    enum BOX_LAYOUT{
        HBoxLayout,
        VBoxLayout
    };

    explicit WgtMenuBar(QWidget *parent = 0);
    ~WgtMenuBar();

    void setLayouts(const BOX_LAYOUT &layout);

    void addItem_H(const QString &text, const QString &name, const langEnum textId);
    void addItem(const QString &text, const QString &name, const langEnum textId);

    void setAlignment(Qt::AlignmentFlag flag);
    void setCurrIndex(int id);
    void setItemEnable(ENABLE_VECTOR &vector);
    int  itemCount(void);

signals:
    void clicked(int);

protected:
    void changeEvent(QEvent* event);

private slots:
    void onItemClicked(int id);

private:
    void retranslateUi(void);

private:
    QHBoxLayout         *Hlayout;
    QVBoxLayout         *Vlayout;
    BOX_LAYOUT          m_layout;

    QList<WgtMenuItem*> m_itemList;
    QList<langEnum>     m_itemText;
    int                 m_curIndex;
};

#endif // WGTMENUBAR_H
