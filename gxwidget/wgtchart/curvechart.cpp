#include "curvechart.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <QTime>
#include <QFile>
CurveChart::CurveChart(QWidget *parent)
    : QWidget(parent)
{
    //初始数据
    m_maxPointNum = 300;

    m_bgStartColor = QColor(79, 79, 79);
    m_bgEndColor = QColor(51, 51, 51);
    m_textColor = QColor(Qt::white);
    m_pointColor = QColor("#1DFFFC"/*238, 114, 179*/);
    m_min = 0;
    m_max = 40;
    //横坐标步长按，最大点数和空间宽度计算
    m_stepH = (width()-m_chartLeftMargin-m_chartRightMargin)/(m_maxPointNum-1); //5个点组成四个区段，所以要减1


    m_stepV = 10;

    m_chartLeftMargin   = 30;
    m_chartTopMargin    = 20;
    m_chartRightMargin  = 30;
    m_chartBottomMargin = 20;
    m_pointRadious = 2;
    m_showLine = true;
    m_showPoint = false;
    m_showPointBg = true;
    m_showArrow = true;


}

CurveChart::~CurveChart()
{

}

void CurveChart::setBackgroundStartColor(QColor color)
{
    if(color != m_bgStartColor)
    {
        m_bgStartColor = color;
        update();
    }
}

void CurveChart::setBackgroundEndColor(QColor color)
{
    if(color != m_bgEndColor)
    {
        m_bgEndColor = color;
        update();
    }
}

void CurveChart::setTextColor(QColor color)
{
    if(color != m_textColor)
    {
        m_textColor = color;
        update();
    }
}

void CurveChart::setPointColor(QColor color)
{
    if(color != m_pointColor)
    {
        m_pointColor = color;
        update();
    }
}

void CurveChart::setMin(qreal min)
{
    setRange(min, m_max);
}

void CurveChart::setMax(qreal max)
{
    setRange(m_min, max);
}

void CurveChart::setMaxPointNum(int pointNum)
{
    if(pointNum != m_maxPointNum)
    {
        m_maxPointNum = pointNum;
        update();
    }
}

void CurveChart::setStepHoriginal(qreal val)
{
    if(val != m_stepH)
    {
        m_stepH = val;
        update();
    }
}

void CurveChart::setStepVertical(qreal val)
{
    if(val != m_stepV)
    {
        m_stepV = val;
        update();
    }
}

void CurveChart::setChartLeftMargin(qreal margin)
{
    if(margin>=0 && margin != m_chartLeftMargin)
    {
        m_chartLeftMargin = margin;
        update();
    }
}

void CurveChart::setChartTopMargin(qreal margin)
{
    if(margin>=0 && margin != m_chartTopMargin)
    {
        m_chartTopMargin = margin;
        update();
    }
}

void CurveChart::setChartRightMargin(qreal margin)
{
    if(margin>=0 && margin != m_chartRightMargin)
    {
        m_chartRightMargin = margin;
        update();
    }
}

void CurveChart::setChartBottomMargin(qreal margin)
{
    if(margin>=0 && margin != m_chartBottomMargin)
    {
        m_chartBottomMargin = margin;
        update();
    }
}

QString CurveChart::DataString() const
{
    QStringList points;
    for(int i=0; i<m_points.size(); ++i)
    {
        points << QString("%1").arg(m_points[i].pointValue);
    }
    return points.join("|");
}

void CurveChart::addDataStr(QString str)
{
//    clearData();
//    QStringList dataList = str.split("|");
//    foreach(QString valStr, dataList)
//    {
//        bool ok;
//        double val = valStr.toDouble(&ok);
//        if(ok)
//        {
//            addData(val);
//        }
//    }
}

void CurveChart::addData(PointData pointD)
{
    int maxNum = m_maxPointNum;

    if(maxNum < 0) { return; }
    if(m_points.size() >= maxNum)   //容器满
    {
        m_points.pop_front();
    }
    m_points.push_back(pointD);
    update();
}

void CurveChart::addDataStrList(QString txtPath)
{
   // QStringList list;
    QFile file(txtPath);
    if (file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd()){
            QString str = file.readLine();
            //qDebug()<<str;
            QStringList dataList = str.split("|");
                bool ok;
                if(dataList.length()>=2){
                    double val = dataList.at(0).toDouble(&ok);
                    QString timeStr = dataList.at(1);
                    if(ok)
                    {
                        addData(PointData{val,timeStr});
                    }
                }
        }


        file.close();
    }
}

void CurveChart::setTitle(QString str)
{
    if(str != m_title)
    {
        m_title = str;
        update();
    }
}

void CurveChart::setShowLine(bool show)
{
    if(show != m_showLine)
    {
        m_showLine = show;
        update();
    }
}

void CurveChart::setShowPoint(bool show)
{
    if(show != m_showPoint)
    {
        m_showPoint = show;
        update();
    }
}

void CurveChart::setShowPointBackground(bool show)
{
    if(show != m_showPointBg)
    {
        m_showPointBg = show;
        update();
    }
}

void CurveChart::setShowArrow(bool show)
{
    if(show != m_showArrow)
    {
        m_showArrow = show;
        update();
    }
}

void CurveChart::clearData()
{
    if(m_points.size()>0)
    {
        m_points.clear();
        update();
    }
}

void CurveChart::paintEvent(QPaintEvent *)
{
    m_stepH = (width()-m_chartLeftMargin-m_chartRightMargin)/(m_maxPointNum-1); //5个点组成四个区段，所以要减1

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //drawBackground(&painter);
    drawTitle(&painter);
    drawBorder(&painter);
    drawText(&painter);
    drawPoints(&painter);
}

void CurveChart::drawBackground(QPainter *p)
{
//    p->save();
//    QLinearGradient linearGradient(QPointF(0,0), QPointF(0,height()));
//    linearGradient.setColorAt(0.0, m_bgStartColor);
//    linearGradient.setColorAt(1.0, m_bgEndColor);
//    p->setPen(Qt::NoPen);
//    p->setBrush(linearGradient);
//    p->drawRect(rect());
//    p->restore();
}

void CurveChart::drawTitle(QPainter *p)
{
    p->save();
    QPointF topLeft(0, 0);
    QPointF bottomRight(width(), m_chartTopMargin);
    QRectF titleRect(topLeft, bottomRight);
    p->setPen(m_textColor);
    p->drawText(titleRect, Qt::AlignCenter, m_title);
    p->restore();
}

void CurveChart::drawBorder(QPainter *p)
{
    p->save();
    //画边框
    QPointF topLeft(m_chartLeftMargin, m_chartTopMargin);
    QPointF bottomRight(width()-m_chartRightMargin, height()-m_chartBottomMargin);
    QPointF bottomLeft(m_chartLeftMargin,height()-m_chartBottomMargin);



    //画坐标线
    QRectF borderRect(topLeft, bottomRight);
    p->setPen(QPen(m_textColor, 0.2));
    //X轴
    p->drawLine(bottomLeft,topLeft);
    //Y轴
    p->drawLine(bottomLeft,bottomRight);
    //p->drawRect(borderRect);

    //画箭头
    if(m_showArrow){
        QPointF arrowTopLeft(m_chartLeftMargin, m_chartTopMargin-15);
        QPointF arrowBottomRight(width()-m_chartRightMargin+15, height()-m_chartBottomMargin);
        //X轴箭头
        QPointF arrowXTop(arrowTopLeft.rx(),arrowTopLeft.ry()-12);
        QPointF arrowXLeft(arrowTopLeft.rx()-7,arrowTopLeft.ry()+5);
        QPointF arrowXRight(arrowTopLeft.rx()+7,arrowTopLeft.ry()+5);

        //Y轴箭头
        QPointF arrowYTop  (arrowBottomRight.rx()+12,arrowBottomRight.ry());
        QPointF arrowYLeft (arrowBottomRight.rx()-5, arrowBottomRight.ry()-7);
        QPointF arrowYRight(arrowBottomRight.rx()-5, arrowBottomRight.ry()+7);


        /*const QPointF pointsX[4] = {
            arrowXTop,
            arrowXLeft,
            arrowTopLeft,
            arrowXRight
          };
        const QPointF pointsY[4] = {
            arrowYTop,
            arrowYLeft,
            arrowBottomRight,
            arrowYRight
          };*/
        //p->drawPolygon(pointsX,4);
        //p->drawPolygon(pointsY,4);
        p->drawLine(topLeft,arrowTopLeft);
        p->drawLine(bottomRight,arrowBottomRight);

    }


    //画线条
    if(m_showLine)
    {
        QPen pen = p->pen();
        pen.setStyle(Qt::SolidLine);
        pen.setWidthF(0.1);
        p->setPen(pen);

        qreal count = (m_max-m_min)/m_stepV;        
        qreal stepY = borderRect.height()/count;
        qreal dy = height()- m_chartBottomMargin-stepY;
        QPointF p1,p2;

        while(dy>borderRect.top()-5)    //
        {

            p1 = QPointF(borderRect.left(), dy);
            p2 = QPointF(borderRect.right(), dy);
            p->drawLine(p1, p2);
            dy -= stepY;
        }
    }
    p->restore();
}

void CurveChart::drawText(QPainter *p)
{
    p->save();

    QFont font;
    //font.setFamily("Microsoft YaHei");
    font.setPixelSize(10);


    qreal textMargin = 5;
    QFontMetrics fm(font);
    int textHeight = fm.height();
    QPointF topLeft(m_chartLeftMargin, m_chartTopMargin);
    QPointF bottomRight(width()-m_chartRightMargin, height()-m_chartBottomMargin);
    QRectF borderRect(topLeft, bottomRight);
    qreal count = (m_max-m_min)/m_stepV;
    qreal stepY = borderRect.height()/count;
    qreal dy = m_chartTopMargin;
    QPointF pos;
    qreal val = m_max;

    p->setFont(font);
    p->setPen(m_textColor);
    while(val >= m_min)
    {
        QString text = QString("%1").arg(val);
        int textWidth = fm.width(text);
        //qDebug()<<__LINE__ << textMargin<< textWidth;
        pos = QPointF(m_chartLeftMargin-textMargin-textWidth /*+ 10*/, dy+textHeight/2.0);
        p->drawText(pos, text);
        dy += stepY;
        val -= m_stepV;
    }
    p->restore();
}

void CurveChart::drawPoints(QPainter *p)
{
    if(m_points.isEmpty())
        return;
    p->save();
    QVector<PointCoord> posVec;
    QVector<QPointF> onlyposVec;

    qreal dx = width()-m_chartRightMargin;
    qreal dy = height()-m_chartBottomMargin;
    qreal percent = (height()-m_chartTopMargin-m_chartBottomMargin)/(m_max-m_min);
    QPointF endPoint = QPointF(dx, dy);
    posVec.append(PointCoord{endPoint,PointData{}});//结束点
    onlyposVec.append(endPoint);

    QVector<PointData>::reverse_iterator it;

    for(it = m_points.rbegin(); it!= m_points.rend(); ++it)
    {        
        if(dx<m_chartLeftMargin-1)  //因计算m_stepH有小数点，所以判断的时候加上一个像素
        {
            break;
        }
        QPointF pointF = QPointF(dx, dy-((*it).pointValue-m_min)*percent);

        posVec.append(PointCoord{pointF,*it});
        onlyposVec.append(pointF);
        dx -= m_stepH;
        //qDebug()<<"dx"<<dx;

    }
    //QPointF pointF = QPointF(dx<m_chartLeftMargin?m_chartLeftMargin:dx, dy);
    QPointF pointF = QPointF(dx<m_chartLeftMargin?m_chartLeftMargin:dx, dy);
    //qDebug()<<height();

    //起始点
    pointF = QPointF(dx<m_chartLeftMargin?m_chartLeftMargin:dx, dy);
    posVec.append(PointCoord{pointF,PointData{}});
    onlyposVec.append(pointF);


    if(!m_showPointBg)
    {//不显示背景的话去除起始点和结束点
        posVec.pop_front();
        posVec.pop_back();

        onlyposVec.pop_front();
        onlyposVec.pop_back();
    }
    //画圆点
    if(m_showPoint)
    {
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(10);

        //int radious = m_stepH/2;
        int radious = 3;//8/2;
        p->save();
        p->setFont(font);
        p->setPen(Qt::NoPen);
        p->setBrush(m_pointColor);
       // qDebug()<<"圆点容器 len"<<posVec.length();
//        qDebug()<<"起点 1 "<<posVec.at(0).pointD.timeString<<" "<<"起点 2"<<posVec.at(1).pointD.timeString;
//        qDebug()<<"起点 1 p"<<posVec.at(0).pointD.pointValue<<" "<<"起点 2 p"<<posVec.at(1).pointD.pointValue;
//        qDebug()<<"起点 1 坐标"<<posVec.at(0).pointF<<" "<<"起点 2 坐标"<<posVec.at(1).pointF;
        //qDebug()<<"终点 1 "<<posVec.at(posVec.length()-1).pointD.timeString<<" "<<"终点 2"<<posVec.at(posVec.length()-2).pointD.timeString;

        //qDebug()<<"end"<<posVec.at(0).pointF;
        //qDebug()<<"start"<<posVec.at(posVec.length()-1).pointF;
        int posMax= posVec.size();
        for(int i=0; i<posMax; ++i)
        {

           //如果显示背景区域，要去除头尾数据才是最大值和最小值
           int maxPoint = m_maxPointNum-1;
           int minPoint = 0;
           if(m_showPointBg){   //m_maxPointNum是有效点数长度
               maxPoint = maxPoint + 1;
               minPoint = minPoint + 1;
           }

           if(i == minPoint or i == ((posMax)/2) or i==maxPoint){
                p->drawEllipse(posVec[i].pointF, radious, radious);                

                QPointF timeP(QPointF(posVec[i].pointF.rx()-20,height()-m_chartBottomMargin+15));
                p->setPen(m_textColor);

                //画时间文本
                QString str = posVec[i].pointD.timeString;
                QStringList dataList = str.split(" ");

                if(dataList.length()>=2){
                    //qDebug()<<dataList.at(0);
                    p->drawText(timeP, dataList.at(0));
                    QPointF time = timeP;
                    time = QPointF(time.x() + 2, time.y() + 10);
                     //qDebug()<<dataList.at(1);
                    p->drawText(time, dataList.at(1));
                }

                QPointF textP(posVec[i].pointF.rx(),posVec[i].pointF.ry()-8);
                //画坐标
                p->drawText(textP,QString::number(posVec[i].pointD.pointValue));
            }


        }
        p->restore();
    }
    //画填充内容
    p->setPen(m_pointColor);
    if(m_showPointBg)
    {
        p->setBrush(m_pointColor);
        p->setOpacity(0.2);

        p->drawConvexPolygon(QPolygonF(onlyposVec));
    }
    else
    {
        p->drawPolyline(QPolygonF(onlyposVec));
    }
    p->restore();

    //画时间轴

}

void CurveChart::setRange(qreal min, qreal max)
{
    if(min>max)
        qSwap(min, max);
    m_min = min;
    m_max = max;
    update();
}
