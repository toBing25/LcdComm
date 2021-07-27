#include "wgtdashboard.h"

#include <qpainter.h>
#include <qpainterpath.h>
#include <qrect.h>
#include <qmath.h>
#include <qstring.h>
#include <QDebug>

wgtDashboard::wgtDashboard(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    _value = 0;
    _min   = 1;
    _max   = 3;
}

wgtDashboard::~wgtDashboard()
{

}

void wgtDashboard::set_range(const double min, const double max) {
    _min = min;
    _max = max;
    update();
}

void wgtDashboard::set_min_range(const double min) {
    _min = min;
    update();
}

void wgtDashboard::set_max_range(const double max) {
    _max = max;
    update();
}

void wgtDashboard::set_unit(const QString& unit) {
    _unit = unit;
    update();
}

void wgtDashboard::set_text(const QString& text) {
    _text = text;
    update();
}

void wgtDashboard::set_value(const double value) {
    //qDebug()<<value;
    if(_value == value)
        return;

    if (value < _min) {
        _value = _min;
    } else if (value > _max) {
        _value = _max;
    } else {
        _value = value;
    }
    update();
}

void wgtDashboard::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    int side = qMin(int(height() / 8.5 * 10.0), width());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 160.0, side / 160.0);
    painter.translate(0, 10);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);

    //画外框
    draw_dash_board(painter);

    //画指针
    draw_hand(painter);

    //画值
    draw_value(painter);
    draw_text(painter);
}

void wgtDashboard::draw_dash_board(QPainter &painter)
{
    painter.save();
    //painter.setPen(QPen(QColor(0, 255, 255), 13, Qt::SolidLine));
    QRectF rectangle(-67, -67, 134, 134);
    int startAngle = -40 * 16;
    int spanAngle  = 260 * 16;

    QLinearGradient linear(rectangle.x(), 0, rectangle.width(), 0);
    linear.setColorAt(0, Qt::green);
    linear.setColorAt(0.2, Qt::yellow);
    linear.setColorAt(0.4, QColor("#ffaa00"));
    linear.setColorAt(0.8, Qt::red);
    linear.setColorAt(1,   Qt::red);

    painter.setPen(QPen(QBrush(linear), 13, Qt::SolidLine));
    painter.drawArc(rectangle, startAngle, spanAngle);

    int val = 260.0 /(_max - _min) * (_value - _min);
    painter.setPen(QPen(QColor(85, 0, 255), 13, Qt::SolidLine));
    painter.drawArc(rectangle, startAngle, (260 - val)*16);

    double range = _max - _min;
    double step  = range / 4.0;
    for (int i = 0; i < 5; ++i) {
        painter.save();
        painter.setPen(Qt::white);
        painter.setFont(QFont("Times", 15));
        painter.rotate(-220.0);
        painter.rotate(65.0 * i);
        painter.translate(95, 0);
        painter.rotate(90.0);
        painter.drawText(QRect(-19, 42, 38, 14), Qt::AlignCenter, QString::number(_min + i * step));
        painter.restore();
    }
    painter.restore();
}

void wgtDashboard::draw_hand(QPainter &painter)
{
    static const QPoint hand[] = {
        QPoint(-5, 3),
        QPoint(0, -55),
        QPoint(5, 3),
        QPoint(0, 10)
    };

    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(85, 170, 255));
    painter.rotate(-130.0);
    painter.rotate(260.0 / (_max - _min) * (_value - _min));

    QPainterPath bigCircle, smallCirecle, path;
    bigCircle.addEllipse(QPoint(0, 10), 4, 4);
    smallCirecle.addEllipse(QPoint(0, 10), 3, 3);
    path = bigCircle - smallCirecle;
    painter.drawPath(path);

    painter.drawConvexPolygon(hand, 4);
    painter.restore();
}

void wgtDashboard::draw_value(QPainter &painter) {
    painter.save();
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Times", 16));
    QString unit;
    if (!_unit.isEmpty()) {
        unit = QString::number(_value) + " " + _unit;
    } else {
        unit = QString::number(_value);
    }
    QRect rect, rect1;
    rect   = QRect(-24, 36, 50, 20);
    rect1  = rect.adjusted(0, 0, 0, 0);
    //painter.fillRect(rect, QColor(85,0,255));

    painter.setPen(Qt::white);
    painter.drawRoundedRect(rect1, 3, 3);
    painter.drawText(QRect(-100, 36, 200, 20), Qt::AlignCenter, unit);
    painter.restore();
}

void wgtDashboard::draw_text(QPainter &painter) {
    painter.save();
    painter.setPen(QColor(96, 96, 96, 245));
    painter.setFont(QFont("Times", 10));
    painter.drawText(QRect(-100, 50, 200, 20), Qt::AlignCenter, _text);
    painter.restore();
}
