#include "wgtdial.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>


wgtDial::wgtDial(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    m_value   = 1;
    m_maxVal  = 5;
    m_minVal  = 1;

    m_radius  = 90;
    m_startAngle   = 50;
    m_endAngle     = 50;

}

void wgtDial::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true); //平滑像素图

    drawBg(&painter);
    drawScale(&painter);
    drawValue(&painter);
}

void wgtDial::drawBg(QPainter *painter)
{
    painter->save();

    painter->translate(this->width()/2, this->height()/2); // 原点定位
    QPixmap img = QPixmap(":/image/background.png");
    img = img.scaled(149, 149, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    painter->drawPixmap(-img.width()/2, -img.height()/2.5, img); // 背景图

    painter->translate(this->width()/400, this->height()/8); // 原点定位

    QPixmap needle;
    needle.load(":/image/needle.png");
    needle = needle.scaled(6, 55, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    painter->rotate(-130);
    double degRotate = (360.0 - m_startAngle - m_endAngle) / (m_maxVal - m_minVal) * (m_value - m_minVal);
    painter->rotate(degRotate);

    painter->drawPixmap(-needle.width()/2, -needle.height(), needle); // 绘制指针，默认图片针尖在上坐标(0,0)，所以移动使针尾坐标为0

    painter->restore();
}

void wgtDial::drawScale(QPainter *painter)
{
    int side = 90;

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->translate(this->width()/ 2, this->height() / 2);
    painter->scale(side / 100.0, side / 100.0);
    painter->save();

     int r = (int)(m_radius * 0.6);

     painter->setFont(QFont("Arial", 11));
     painter->setPen(QPen(QColor(255,255,255)));
     QFontMetricsF fm = QFontMetricsF(painter->font());

     int gap = (360-m_startAngle*2) / 8;
     for(int i = 0; i<=8; i++)
     {
         int angle = 90+m_startAngle+gap*i;  //角度,8格子画一个刻度值

         float angleArc =( angle % 360) * 3.14 / 180; //转换为弧度
         int x = (r)*cos(angleArc);
         int y = (r)*sin(angleArc);

         QString value = QString::number(0.1 * i * ((m_maxVal - m_minVal +1)) + m_minVal);

         int w = (int)fm.width(value);
         int h = (int)fm.height();
         x = x - w/2;
         y = y + h +5;

         painter->drawText(QPointF(x, y),value);
     }

     painter->restore();
}

void wgtDial::drawValue(QPainter *painter)
{
    painter->save();

    painter->setFont(QFont("Arial", 16));
    painter->setPen(QPen(QColor(255,255,255)));

    int m_precision =1;

    QString value = QString("%1").arg(m_value, 0, 'f',m_precision);

    qreal width = painter->fontMetrics().width(value);
    qreal height = painter->fontMetrics().height();

    QRectF textRect(0 - width / 2, height + 30, width, height);
    painter->drawText(textRect, Qt::AlignCenter, value);

    painter->restore();
}

void wgtDial::valueChange(qreal val)
{
    if(m_value == val)
        return;

    m_value = val;
    update();
}













