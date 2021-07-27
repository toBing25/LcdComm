#include "wgtrose.h"

#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>

wgtRose::wgtRose(QWidget *parent)
    : QWidget(parent)
{
    m_iRadius = 20;
    m_iWindowRadius = 60;

    //默认值
    m_alarmCount  = 0;
    m_valueMap[0] = 0;
    m_valueMap[1] = 0;
    m_valueMap[2] = 0;
    m_sortValueList = m_valueMap.values();

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void wgtRose::setData(QMap<int, int> valueMap, int alarmCount)
{
    if((valueMap[0] == 0) && (valueMap[1] == 0) &&(valueMap[2] == 0))
    {
        valueMap[0] = 1;
        valueMap[1] = 1;
        valueMap[2] = 1;
    }

    m_alarmCount = alarmCount;
    m_valueMap   = valueMap;
    m_sortValueList = valueMap.values();
    //qSort(m_sortValueList.begin(), m_sortValueList.end());

    this->update();
}

void wgtRose::setTitle(QMap<int, QString> titleMap, QMap<int, QColor> colorMap)
{
    m_titleMap = titleMap;
    m_colorMap = colorMap;

    this->update();
}

void wgtRose::setCoreColor(QColor color)
{
    m_coreColor = color;
    this->update();
}

void wgtRose::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    m_iSide = qMin(width(), height());
    int x = (width() - m_iSide) / 2;
    //int y = (height() - m_iSide) / 2;

    // 设置ViewPort
    painter.setViewport(QRect(5/*x/3*/, -3/*x/4*/, m_iSide, m_iSide));
    // 设置窗口 逻辑坐标
    painter.setWindow(QRect(0, 0, 2 * m_iWindowRadius, 2 * m_iWindowRadius));
    painter.setRenderHint(QPainter::Antialiasing);

    // 画玫瑰饼图
    drawRose(painter);
}

void wgtRose::drawRose(QPainter &painter)
{   
    QMap<int, QColor> colorMap  = m_colorMap;
    QMap<int, QString> titleMap = m_titleMap;
    QMap<int, int> valueMap = m_valueMap;
    int totalValue = 0;
    int radiusSpan = 90 / (m_sortValueList.size() * 6);
    for (int i = 0; i < m_sortValueList.size(); i++)
    {
        totalValue += m_sortValueList.at(i);
    }

    int earlierTotalValue = 0;
    for (int i = 0; i < m_sortValueList.size(); i++)
    {
        int value = m_sortValueList.at(i);

        earlierTotalValue += value;
        if(earlierTotalValue == 0)
            earlierTotalValue = 1;

        if(totalValue == 0)
            totalValue = 1;

        int index = i;//getIndexFromaValue(value);
        if(!m_valueMap.contains(index))
            return;

        QColor color  = colorMap.value(index);
        QString title = titleMap.value(index);
        colorMap.remove(index);
        titleMap.remove(index);
        valueMap.remove(index);

        painter.save();
        QPen pen;
        pen.setColor(color);
        pen.setWidth(radiusSpan * (2 * i + 3));
        pen.setCapStyle(Qt::FlatCap);
        painter.setPen(pen);
        int startAngle = 0;
        int spanAngle = 0;
        if(i == 0)
        {
            startAngle = 90 * 16;
        }
        else
        {
            int earlierTotalAngle = 360 * (earlierTotalValue -value) / totalValue;
            startAngle = (90 - earlierTotalAngle) * 16;
        }
        spanAngle =  -(360 * value / totalValue) * 16;
        QRectF rectangle(m_iWindowRadius - (m_iRadius + radiusSpan * (i + 1)),
                         m_iWindowRadius - (m_iRadius + radiusSpan * (i + 1)),
                         2 * (m_iRadius + radiusSpan * (i + 1)),
                         2 * (m_iRadius + radiusSpan * (i + 1)));
        painter.drawArc(rectangle, startAngle -10, spanAngle/* -15*/);
        painter.restore();

        QFont font;
        //font.setFamily("Microsoft YaHei");
        font.setPixelSize(8);
        pen.setColor(color);
        painter.setFont(font);
        painter.setPen(pen);

        QRectF ret(m_iWindowRadius + 60, m_iWindowRadius - i* 20, 15, 10);
        painter.fillRect(ret, color);
        painter.drawText(ret.x() + ret.width() + 5, ret.y() + 8, title);

        pen.setColor(m_coreColor);
        font.setPixelSize(7);
        painter.setFont(font);
        painter.setPen(pen);
        QRectF rectangle1(m_iWindowRadius - (m_iRadius + radiusSpan * (0 + 1)),
                          m_iWindowRadius - (m_iRadius + radiusSpan * (0 + 1)),
                          2 * (m_iRadius + radiusSpan * (0 + 1)) - 32,
                          2 * (m_iRadius + radiusSpan * (0 + 1)) - 10);

        painter.drawText(rectangle1.x() + rectangle1.width()/2+2, rectangle1.y()+rectangle1.height()/2, QObject::tr("告警总数"));


        QFontMetrics fm = painter.fontMetrics();
        int len, len2;
        len  = fm.width("告警总数");
        len2 = fm.width(QString::number(m_alarmCount));

        QString number;
        if(len2 <(len/2))
            number = " " + QString::number(m_alarmCount);
        //else if ((len2 < len) && (len2 > (len/2)))
        //    number = " " + QString::number(m_alarmCount);
        else
            number = QString::number(m_alarmCount);

        QRectF text_rect(rectangle1.x() + rectangle1.width()/2 + 2, rectangle1.y()+rectangle1.height()/2  + 3, len, 10);
        painter.drawText(text_rect, Qt::AlignHCenter|Qt::AlignCenter, number);
        painter.drawText(rectangle1.x() + rectangle1.width()/2 + 13, rectangle1.y()+rectangle1.height()/2  + 20, QObject::tr("个"));
    }
}

int wgtRose::getIndexFromaValue(int value)
{
    int index = -1;

    QMap<int, int>::iterator it = m_valueMap.begin();
    for(; it != m_valueMap.end(); it++)
    {
        if(value == it.value())
            return it.key();
    }
    return index;
}

