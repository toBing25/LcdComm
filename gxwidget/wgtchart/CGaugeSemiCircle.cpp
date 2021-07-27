#include "CGaugeSemiCircle.h"

GaugeSemiCircle::GaugeSemiCircle(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    setRange(0, 50);
    setValue(5);
    setPrecision(1);

    setScaleMajor(5);
    setScaleMinor(10);

    setStartAngle(90);
    setEndAngle(90);

    setScaleColor(QColor(Qt::white));
    setTextColor(QColor(255, 0, 255));

    setPointerColor(Qt::blue);

    m_value = 25.4;
}

GaugeSemiCircle::~GaugeSemiCircle()
{
}

void GaugeSemiCircle::paintEvent(QPaintEvent *)
{
    int width  = this->width();
    int height = this->height();
    int side   = qMin(width, height);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 1.5);
    painter.scale(side / 100.0, side / 100.0);

    //绘制外圆
    drawOuterCircle(&painter);

    //绘制刻度线
    drawScale(&painter);

    //绘制刻度值
    drawScaleNum(&painter);

    //绘制指示器
    drawPointerTriangle(&painter);

    //绘制当前值
    drawValue(&painter);
}

void GaugeSemiCircle::drawOuterCircle(QPainter *painter)
{
    int radius = 36;
    painter->save();

    QLinearGradient linear(-radius, 0, radius * 2, 0);
    linear.setColorAt(0, Qt::green);
    linear.setColorAt(0.2, Qt::yellow);
    linear.setColorAt(0.6, Qt::red);
    linear.setColorAt(1, Qt::red);

    painter->setPen(QPen(QBrush(linear), 8, Qt::SolidLine));
    painter->drawArc(-radius, -radius, radius * 2, radius * 2, 0*16, 180*16);

    painter->restore();
}

void GaugeSemiCircle::drawScale(QPainter *painter)
{
    int radius = 58;
    painter->save();

    painter->rotate(90);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (180.0 ) / steps;

    QPen pen;
    pen.setColor(scaleColor);
    pen.setCapStyle(Qt::RoundCap);

    for (int i = 0; i <= steps; i++)
    {
        pen.setWidthF(0.8);
        //pen.setColor(Qt::white);
        painter->setPen(pen);
        painter->drawLine(0, radius - 10, 0, radius);        
        painter->rotate(angleStep);
    }
    painter->restore();
}

void GaugeSemiCircle::drawScaleNum(QPainter *painter)
{
    int radius = 58;
    painter->save();
    painter->setPen(scaleColor);

    double startRad = (360 - startAngle - 90) * (M_PI / 180);
    double deltaRad = (360 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;

    for (int i = 0; i <= scaleMajor; i++)
    {
        if(i > 0 && i < scaleMajor)
        {
            continue;
        }
        double sina = qSin(startRad - i * deltaRad);
        double cosa = qCos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;

        QString strValue = QString::number(value, 'f', 1);
        double textWidth = fontMetrics().width(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight * 0.8;

        if(i == 0)
        {
            x -= textWidth *0;
        }
        else
        {
            x += textWidth *0;
        }
        QFont font;
        font.setPixelSize(radius/5);
        painter->setFont(font);

        painter->drawText(x, y, strValue);
    }
    painter->restore();
}

void GaugeSemiCircle::drawPointerTriangle(QPainter *painter)
{
    int radius = 7;
    int offset = 41;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);

    QPolygon pts;
    pts.setPoints(3, -5, 0 + offset, 5, 0 + offset, 0, radius + offset);

    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (m_value - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);

    painter->restore();
}

void GaugeSemiCircle::drawValue(QPainter *painter)
{
    int radius = 80;
    painter->save();
    painter->setPen(textColor);

    QFont font;
    font.setPixelSize(16);
    painter->setFont(font);

    // 单位
    font.setPixelSize(16);
    painter->setFont(font);
    QRectF textRect1(-radius, -radius*0.9, radius * 2, radius * 1.3);
    QString strValue1 = QString("PUE");
    //painter->drawText(textRect1, Qt::AlignCenter, strValue1);

    // 数值
    font.setPixelSize(23);
    painter->setFont(font);
    painter->setPen(textColor);
    QRectF textRect(-radius, -radius, radius * 2, radius * 2.0);
    QString strValue;
    if(m_value <= maxValue && m_value >= minValue)
    {
        strValue = QString("%1").arg((double)m_value, 0, 'f', precision);
    }
    else
    {
        strValue = QString("--");
    }
    painter->drawText(textRect, Qt::AlignCenter, strValue);

    painter->restore();
}

double GaugeSemiCircle::getMinValue() const
{
    return this->minValue;
}

double GaugeSemiCircle::getMaxValue() const
{
    return this->maxValue;
}

double GaugeSemiCircle::getValue() const
{
    return this->m_value;
}

int GaugeSemiCircle::getPrecision() const
{
    return  this->precision;
}

int GaugeSemiCircle::getScaleMajor() const
{
    return this->scaleMajor;
}

int GaugeSemiCircle::getScaleMinor() const
{
    return this->scaleMinor;
}

int GaugeSemiCircle::getStartAngle() const
{
    return this->startAngle;
}

int GaugeSemiCircle::getEndAngle() const
{
    return this->endAngle;
}

QColor GaugeSemiCircle::getScaleColor() const
{
    return this->scaleColor;
}

QColor GaugeSemiCircle::getPointerColor() const
{
    return this->pointerColor;
}

QColor GaugeSemiCircle::getTextColor() const
{
    return this->textColor;
}

QSize GaugeSemiCircle::sizeHint() const
{
    return QSize(50, 50);
}

QSize GaugeSemiCircle::minimumSizeHint() const
{
    return QSize(10, 10);
}

void GaugeSemiCircle::setRange(double minValue, double maxValue)
{
    if(this->minValue != minValue)
    {
        this->minValue = minValue;
    }
    if(this->maxValue != maxValue)
    {
        this->maxValue = maxValue;
    }
    this->update();
}

void GaugeSemiCircle::setRange(int minValue, int maxValue)
{
    if(this->minValue != minValue)
    {
        this->minValue = minValue;
    }
    if(this->maxValue != maxValue)
    {
        this->maxValue = maxValue;
    }
    this->update();
}

void GaugeSemiCircle::setMinValue(double value)
{
    if(this->minValue != value)
    {
        this->minValue = value;
        this->update();
    }
}

void GaugeSemiCircle::setMaxValue(double value)
{
    if(this->maxValue != value)
    {
        this->maxValue = value;
        this->update();
    }
}

void GaugeSemiCircle::setValue(double value)
{
    if(this->m_value != value)
    {
        this->m_value = value;
        this->update();
    }
}

void GaugeSemiCircle::setValue(int value)
{
    if(this->m_value != value)
    {
        this->m_value = value;
        this->update();
    }
}

void GaugeSemiCircle::setPrecision(int precision)
{
    if(this->precision != precision)
    {
        this->precision = precision;
        this->update();
    }
}

void GaugeSemiCircle::setScaleMajor(int scaleMajor)
{
    if(this->scaleMajor != scaleMajor)
    {
        this->scaleMajor = scaleMajor;
        this->update();
    }
}

void GaugeSemiCircle::setScaleMinor(int scaleMinor)
{
    if(this->scaleMinor != scaleMinor)
    {
        this->scaleMinor = scaleMinor;
        this->update();
    }
}

void GaugeSemiCircle::setStartAngle(int startAngle)
{
    if(this->startAngle != startAngle)
    {
        this->startAngle = startAngle;
        this->update();
    }
}

void GaugeSemiCircle::setEndAngle(int endAngle)
{
    if(this->endAngle != endAngle)
    {
        this->endAngle = endAngle;
        this->update();
    }
}

void GaugeSemiCircle::setScaleColor(const QColor &scaleColor)
{
    if(this->scaleColor != scaleColor)
    {
        this->scaleColor = scaleColor;
        this->update();
    }
}

void GaugeSemiCircle::setPointerColor(const QColor &pointerColor)
{
    if(this->pointerColor != pointerColor)
    {
        this->pointerColor = pointerColor;
        this->update();
    }
}

void GaugeSemiCircle::setTextColor(const QColor &textColor)
{
    if(this->textColor != textColor)
    {
        this->textColor = textColor;
        this->update();
    }
}
