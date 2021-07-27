#include "CProgressWater.h"

CProgressWater::CProgressWater(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    m_percentStyle = PercentStyle_Circle;
    m_usedColor    = QColor(56, 163, 226);

    m_textColor    = Qt::white;
    m_boderColor   = Qt::transparent;
    m_outlineColor = QColor("#679cfa");
    m_bgColor      = Qt::transparent;

    m_value        = 40;
    m_minValue     = 0;
    m_maxValue     = 80;
    m_waterDensity = 6; // 水波的密度
    m_waterHeight  = 0.03;
    m_offset       = 50;
    m_borderWidth  = 10;

    //this->startTimer(800);
}

CProgressWater::~CProgressWater()
{

}


void CProgressWater::setValue(int value)
{
    m_value = value;
}

int CProgressWater::getValue()
{
    return m_value;
}

void CProgressWater::setMaxValue(int value)
{
    m_maxValue = value;
}

int CProgressWater::getMaxValue()
{
    return m_maxValue;
}

void CProgressWater::setMinValue(int value)
{
    m_minValue = value;
}

int CProgressWater::getMinValue()
{
    return m_minValue;
}

void CProgressWater::setPercentStyle(CProgressWater::PercentStyle_Type type)
{
    m_percentStyle = type;
}

CProgressWater::PercentStyle_Type CProgressWater::getPercentStyle()
{
    return m_percentStyle;
}

void CProgressWater::setWaterDensity(int value)
{
    m_waterDensity = value;
}

int CProgressWater::getWaterDensity()
{
    return m_waterDensity;
}

void CProgressWater::setColor(QColor color)
{
    m_usedColor = color;
}

QColor CProgressWater::getColor()
{
    return m_usedColor;
}

void CProgressWater::setWaterHeight(double value)
{
    m_waterHeight = value;
}

double CProgressWater::getWaterHeight()
{
    return m_waterHeight;
}

void CProgressWater::setBorderWidth(int value)
{
    m_borderWidth = value;
}

int CProgressWater::getBorderWidth()
{
    return m_borderWidth;
}

void CProgressWater::setTextColor(QColor color)
{
    m_textColor = color;
}

QColor CProgressWater::getTextColor()
{
    return m_textColor;
}

void CProgressWater::setBoderColor(QColor color)
{
    m_boderColor = color;
}

QColor CProgressWater::getBoderColor()
{
    return m_boderColor;
}

void CProgressWater::setBgColor(QColor color)
{
    m_bgColor = color;
}

QColor CProgressWater::getBgColor()
{
    return m_bgColor;
}

void CProgressWater::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int height = this->height();
    int width  = this->width();
    int side   = qMin(width, height);

    double percent = 1 - (double)(m_value - m_minValue) / (m_maxValue - m_minValue);
    if(percent > 1 || percent < 0)
    {
        percent = 0.5;
    }

    // 正弦曲线公式 y = A * sin(wx + φ) + k
    // 其中：
    //    A: 振幅，水波的高度
    //    w: 周期，水波的密度
    //    k: 偏移，这里位水位高度
    double w = m_waterDensity * M_PI / width;
    double A = height * m_waterHeight;   
    double k = height * percent;

    // 第一条波浪路径集合
    QPainterPath waterPath1;
    // 第二条波浪路径集合
    QPainterPath waterPath2;

    // 移动到左上角起始点
    waterPath1.moveTo(0, height);
    waterPath2.moveTo(0, height);

    m_offset += 0.6;
    if(m_offset > (width / 2))
    {
        m_offset = 0;
    }

    for(int x = 0; x<= width; x++)
    {
        // 第一条波浪Y轴
        double waterY1 = (double)(A * sin(w * x + m_offset)) + k;
        // 第二条波浪Y轴
        double waterY2 = (double)(A * sin(w * x + m_offset + (width / 2 * w))) + k;

        // 如果当前值位最小值则Y轴为高度
        if(m_value == m_minValue)
        {
            waterY1 = height;
            waterY2 = height;
        }

        // 如果当前值位最大值则Y轴为0
        if(m_value == m_maxValue)
        {
            waterY1 = 0;
            waterY2 = 0;
        }
        waterPath1.lineTo(x, waterY1);
        waterPath2.lineTo(x, waterY2);
    }

    // 移动到右下角结束，整体形成一个闭合路径
    waterPath1.lineTo(width, height);
    waterPath2.lineTo(width, height);

    // 大路径
    QPainterPath bigPath;
    if(m_percentStyle == PercentStyle_Rect)
    {
        width  = width - m_borderWidth * 2;
        height = height - m_borderWidth * 2;
        bigPath.addRect(m_borderWidth, m_borderWidth, width, height);
        painter.setBrush(m_boderColor);
        painter.drawRect(this->rect());
        painter.setBrush(m_bgColor);
        painter.drawRect(m_borderWidth, m_borderWidth, width, height);
    }
    else if(m_percentStyle == PercentStyle_Circle)
    {
        // 大边框
        painter.save();

        painter.setPen(QPen(m_outlineColor, 2, Qt::SolidLine));
        painter.drawEllipse((width - side) / 2 +2, (height - side) / 2 +3, side - 4, side - 5);
        painter.restore();

        painter.setBrush(m_boderColor);
        side = side - m_borderWidth * 2;
        bigPath.addEllipse((width - side) / 2, (height - side) / 2, side, side);
        painter.setBrush(m_bgColor);
        painter.drawEllipse((width - side) / 2, (height - side) / 2, side, side);

        // 小圆边框
        painter.save();
        painter.setPen(QPen(m_outlineColor, 2, Qt::SolidLine));
        painter.drawEllipse((width - side) / 2, (height - side) / 2, side, side);
        painter.restore();
    }
    else if(m_percentStyle == PercentStyle_Ellipse)
    {
        width  = width - m_borderWidth * 2;
        height = height - m_borderWidth * 2;
        bigPath.addEllipse(m_borderWidth, m_borderWidth, width, height);
        painter.setBrush(m_boderColor);
        painter.drawEllipse(this->rect());
        painter.setBrush(m_bgColor);
        painter.drawEllipse(m_borderWidth, m_borderWidth, width,height);
    }
    painter.save();

    // 新路径，用大路径-波浪区域的路径，形成遮罩效果
    QPainterPath path;
    painter.setPen(Qt::NoPen);
    QColor waterColor1 = m_usedColor;
    waterColor1.setAlpha(100);
    QColor waterColor2 = m_usedColor;
    waterColor2.setAlpha(180);

    // 第一条波浪线挖去后的路径
    path = bigPath.intersected(waterPath1);
    painter.setBrush(waterColor1);
    painter.drawPath(path);

    // 第二条波浪线挖去后的路径
    path = bigPath.intersected(waterPath2);
    painter.setBrush(waterColor2);
    painter.drawPath(path);

    painter.restore();

    drawValue(&painter);
}

void CProgressWater::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    this->update();
}

void CProgressWater::drawValue(QPainter *painter)
{
    painter->save();

    /*湿度
    m_font.setPixelSize(this->width() / 6);
    painter->setPen(m_textColor);
    painter->setFont(m_font);
    painter->drawText(this->rect(), Qt::AlignTop, QString("湿度"));
    */

    //RH
    m_font.setPixelSize(this->width() / 8);
    painter->setPen(m_textColor);
    painter->setFont(m_font);
    //painter.drawText(this->rect(), Qt::AlignTop, QString("RH"));
    //painter->drawText(QRect(0, 0, this->width(), this->height() * 0.6), Qt::AlignCenter, QString("RH"));

    //绘制文字
    m_font.setPixelSize(this->height() / 6);
    painter->setPen(m_textColor);
    painter->setFont(m_font);
    if(m_value <= m_maxValue && m_value >= m_minValue)
    {
        painter->drawText(this->rect(), Qt::AlignCenter, QString("%1℃").arg(QString::number(m_value)));
    }
    else
    {
        painter->drawText(this->rect(), Qt::AlignCenter, QString("--℃"));
    }

    painter->restore();
}
