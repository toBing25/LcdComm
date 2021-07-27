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

#ifndef WGTLISTVIEWDELEGATE_H
#define WGTLISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

#define ICON1_SIZE       8
#define ICON1_GAP        0
#define ICON2_SIZE       10
#define ICON2_GAP        0

#define TITLE1_ICON1_X   20
#define TITLE1_ICON2_X   (TITLE1_ICON1_X + ICON1_SIZE + ICON1_GAP)
#define TITLE1_TEXT_X    (TITLE1_ICON2_X + ICON2_SIZE + ICON2_GAP)

#define TITLE2_ICON1_X   20
#define TITLE2_ICON2_X   (TITLE2_ICON1_X + ICON1_SIZE + ICON1_GAP)
#define TITLE2_TEXT_X    (TITLE2_ICON2_X + ICON2_SIZE + ICON2_GAP)

#define TITLE3_ICON2_X   30
#define TITLE3_TEXT_X    (TITLE3_ICON2_X + ICON2_SIZE + ICON2_GAP + 5)

#define TITLE_FONT_FAMILY   "Microsoft Yahei"
#define TITLE_FONT_SIZE     12

// Title bar
const QColor color_title_bar(88, 115, 201);

// Navi bar
const QColor color_selected(69, 121, 219);
const QColor color_child_normal(48,166,241);
const QColor color_parent_normal(85, 85, 255);
const QColor color_line(214, 216, 224);
const QColor color_text_normal("#00CCFF");
const QColor color_text_selected("#FFFFFF");

// Menu
const QColor color_menu_hover(224, 232, 241);


class wgtListViewDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
    wgtListViewDelegate(QObject *parent);
    ~wgtListViewDelegate();

public:
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const ;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setPending(bool pending) { m_pending = pending; }

private:
	bool m_pending;
};

#endif // WGTLISTVIEWDELEGATE_H
