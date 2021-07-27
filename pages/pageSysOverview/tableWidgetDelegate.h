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

#ifndef TABLEWIDGETDELEGATE_H
#define TABLEWIDGETDELEGATE_H

#include <QTableWidget>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QHeaderView>
#include <QPen>
#include <QDebug>

class tableWidgetDelegate : public QStyledItemDelegate
{
public:
    tableWidgetDelegate(QTableView* tableView)
    {
        //int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
        //QColor gridColor = static_cast<QRgb>(gridHint);
        pen = QPen(QColor("#01ADDE")/*gridColor*/, 0, tableView->gridStyle());
        view = tableView;
    }

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyledItemDelegate::paint(painter, option, index);
        QPen oldPen = painter->pen();
        painter->setPen(pen);
        painter->drawLine(option.rect.bottomLeft(),  option.rect.bottomRight());
        painter->drawLine(option.rect.topRight(),    option.rect.bottomRight());
        painter->setPen(oldPen);
    }

private:
    QPen pen;
    QTableView* view;
};

#endif // TABLEWIDGETDELEGATE_H
