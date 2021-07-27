#include "switchbutton.h"

SwitchButton::SwitchButton(QWidget *parent)
    : QPushButton(parent)
{
    bSwitch    = false;

    bgColorOff = QColor(192,192,192);
    bgColorOn  = QColor(0,  255, 64);

    sliderColorOff = QColor(255,255,255);
    sliderColorOn  = QColor(255,255,255);
    //sliderColor    = sliderColorOff;
    sliderColor    = ":/image/off.png";

    textColorOn  = QColor(255,255,255);
    textColorOff = QColor(255,255,255);

    mouseX = 0;
    bPress = false;
}

SwitchButton::~SwitchButton()
{

}

/**
 * setSwitch
 *
 *
 * @param Switch
 *
 * @return
 */
void SwitchButton::setSwitch(bool Switch)
{
    if (Switch)
    {
        centerPoint.setX(endPoint.x());
        //sliderColor = sliderColorOn;
        sliderColor    = ":/image/on.png";

        if(!bSwitch)
            emit btnState(true);
        bSwitch = true;
    }
    else
    {
        centerPoint.setX(startPoint.x());
        //sliderColor = sliderColorOff;
        sliderColor    = ":/image/off.png";

        if(bSwitch)
            emit btnState(false);
        bSwitch = false;
    }
}

void SwitchButton::mousePressEvent(QMouseEvent *)
{
    /*if (e->button() == Qt::LeftButton)
    {
        if ((e->pos().x() - centerPoint.x())*(e->pos().y() - centerPoint.y()) <= ((rect().height()/2) * (rect().height()/2)))
        {
            bPress = true;
            mouseX = e->pos().x();
            sliderColor = QColor(160,30,30);
            update();
        }
    }*/
    if(!bSwitch)
    {
        centerPoint.setX(endPoint.x());
        //sliderColor = sliderColorOn;
        sliderColor    = ":/image/on.png";

        //emit btnState(true);
        //bSwitch = true;
    }
    else
    {
        centerPoint.setX(startPoint.x());
        //sliderColor = sliderColorOff;
        sliderColor    = ":/image/off.png";

        //emit btnState(false);
        //bSwitch = false;
    }
}

void SwitchButton::mouseMoveEvent(QMouseEvent *e)
{
    if (bPress)
    {
        if ((e->pos().x() >= startPoint.x()) && (e->pos().x() <= endPoint.x()))
        {
            int tempX = e->pos().x();
            centerPoint.setX(tempX - mouseX + centerPoint.x());
            mouseX = centerPoint.x();
            update();
        }
    }
}

void SwitchButton::mouseReleaseEvent(QMouseEvent *)
{
    bPress = false;
    //sliderColor = QColor(245,245,245);

    if (centerPoint.x() >= rect().width()/2)
    {
        setSwitch(true);
    }
    else if (centerPoint.x() < rect().width()/2)
    {
        setSwitch(false);
    }
    update();
}

void SwitchButton::resizeEvent(QResizeEvent *)
{
    //startPoint  = QPoint(rect().height()/2,rect().height()/2);
    //centerPoint = startPoint;
    //endPoint    = QPoint((rect().right() - rect().height()/2), rect().height()/2);

    startPoint  = QPoint(rect().x()+3, rect().y()+3);
    centerPoint = startPoint;
    endPoint    = QPoint((rect().right() - rect().height()+3), rect().height());
}

void SwitchButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    drawBg(painter);
    drawSlidBlock(painter);
    //drawText(painter);
}

void SwitchButton::drawBg(QPainter& painter)
{
    painter.save();
    painter.setPen(Qt::NoPen);

    /*if (bSwitch)
    {
        painter.setBrush(QBrush(bgColorOn));
    }
    else
    {
        painter.setBrush(QBrush(bgColorOff));
    }

    // 绘制按钮外边框
    QPainterPath path;
    int startX = rect().height()/2;
    int startY = rect().top();

    path.moveTo(startX,startY);
    path.arcTo(QRect(rect().left(),rect().top(),rect().height(),rect().height()),90,180);
    path.lineTo((rect().right() - startX),rect().height());
    path.arcTo(QRect((rect().right() - rect().height()),rect().top(),rect().height(),rect().height()),270,180);
    path.lineTo(startX,startY);
    painter.drawPath(path);*/
    painter.drawPixmap(rect(), QPixmap(":/image/switch_bg.png"));
    painter.restore();
}

void SwitchButton::drawSlidBlock(QPainter& painter)
{
    painter.save();
    //painter.setPen(Qt::NoPen);
    //painter.setBrush(QBrush(sliderColor));
    //painter.drawEllipse(centerPoint,rect().height()/2 - 2,rect().height()/2 - 2);

    painter.drawPixmap(QRect(centerPoint.x(), centerPoint.y(), 24, 24), QPixmap(sliderColor));
    painter.restore();
}

void SwitchButton::drawText(QPainter& painter)
{
    painter.save();
    QFont font("Microsoft YaHei", 12, 50, false);
    painter.setFont(font);
    int x,y;

    if (bSwitch)
    {
        painter.setPen(QPen(textColorOn));
        x = rect().left();
        y = rect().top();
        strText = QString::fromLocal8Bit("开");
    }
    else
    {
        painter.setPen(QPen(textColorOff));
        x = rect().right() - rect().height();
        y = rect().top();
        strText = QString::fromLocal8Bit("关");
    }
    painter.drawText(x,y,rect().height(),rect().height(),Qt::AlignCenter,strText);
    painter.restore();
}
