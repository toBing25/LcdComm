#include "wgtlistviewdelegate.h"
#include "wgtlistviewmodel.h"

#include <QDebug>
#include <QPainter>

wgtListViewDelegate::wgtListViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , m_pending(false)
{

}

wgtListViewDelegate::~wgtListViewDelegate()
{

}

QSize wgtListViewDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    wgtListViewModel::TreeNode* node = (wgtListViewModel::TreeNode*)index.data(Qt::UserRole).toULongLong();

    if(node->level == 1)
    {
        return QSize(50, 36);
    }else{
        return QSize(50, 36);
    }
}

void wgtListViewDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    wgtListViewModel::TreeNode* node = (wgtListViewModel::TreeNode*)index.data(Qt::UserRole).toULongLong();

    painter->setRenderHint(QPainter::Antialiasing);
    int icon1_x, icon2_x, text_x;
    if(node->level == 1)
    {
        icon1_x = TITLE1_ICON1_X;
        icon2_x = TITLE1_ICON2_X;
        text_x = TITLE1_TEXT_X;
    }
    else if(node->level == 2)
    {
        icon1_x = TITLE2_ICON1_X;
        icon2_x = TITLE2_ICON2_X;
        text_x = TITLE2_TEXT_X;
    }
    else
    {
        icon1_x = TITLE3_ICON2_X;
        icon2_x = TITLE3_ICON2_X;
        text_x = TITLE3_TEXT_X;
    }

    // Fill background.
    if( node->level == 1 )
    {
        if( option.state & QStyle::State_Selected )
            painter->drawPixmap(option.rect, QPixmap(":/image/item_select.png"));
        else
            painter->drawPixmap(option.rect, QPixmap(":/image/item_unselect.png"));
    }else {
        if( option.state & QStyle::State_Selected )
            painter->drawPixmap(option.rect, QPixmap(":/image/chileditem_select.png"));
        else
            painter->fillRect(option.rect, Qt::transparent);
    }

    // draw decorate
    if( node->level == 1 )
    {
        QString icon1Path;
        if( option.state & QStyle::State_Selected ){
            icon1Path = node->decorate2;
        }else {
            icon1Path = node->decorate;
        }
        QPixmap icon(icon1Path);

        QRect icon1Rect = option.rect;
        icon1Rect.setWidth(16);
        icon1Rect.setHeight(16);
        QPoint c = option.rect.center();
        c.setX(icon1_x);
        icon1Rect.moveCenter(c);
        painter->drawPixmap(icon1Rect, icon1Path, icon.rect());
    }else {
        QString iconPath;
        iconPath = ":/image/dot.png";

        QPixmap icon(iconPath);
        QRect icon1Rect = option.rect;
        icon1Rect.setWidth(7);
        icon1Rect.setHeight(7);
        QPoint c = option.rect.center();
        c.setX(icon1_x);
        icon1Rect.moveCenter(c);
        painter->drawPixmap(icon1Rect, iconPath, icon.rect());
    }

    // draw text.
    if(node->level == 1)
    {
        QPen textPen( option.state & QStyle::State_Selected ? color_text_selected : color_text_normal);
        painter->setPen(textPen);
    }
    /*else if (node->collapse == false) {
        QPen textPen(color_text_selected);
        painter->setPen(textPen);
    }*/
    else
    {
        QPen textPen( option.state & QStyle::State_Selected ? color_text_selected : color_text_normal);
        painter->setPen(textPen);
    }

    QRect rect = option.rect;
    rect.setWidth(rect.width()/* - text_x*/);
    rect.setX(rect.x() + text_x - 10);

    QFont normalFont(TITLE_FONT_FAMILY, TITLE_FONT_SIZE);
    painter->setFont(normalFont);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, index.data(Qt::DisplayRole).toString() );

    if ( node->childList.count() != 0 )
    {
        QString icon2Path;
        //icon2Path = node->decorate;
        if ( node->collapse )
        {
            icon2Path = ":/image/expand_normal.png";
        }
        else
        {
            icon2Path = ":/image/unexpand_normal.png";
        }

        QFontMetrics fm = painter->fontMetrics();
        int len = fm.width(index.data(Qt::DisplayRole).toString());

        QPixmap icon2(icon2Path);
        QRect icon2Rect = option.rect;
        icon2Rect.setWidth(12);
        icon2Rect.setHeight(12);
        QPoint c = option.rect.center();
        c.setX(rect.x() + len + 15);
        icon2Rect.moveCenter(c);
        painter->drawPixmap(icon2Rect, icon2Path, icon2.rect());
    }
}
