#include "signalgraph.h"
#include <QPainter>
#include <QLabel>
#include <QDebug>
SignalGraph::SignalGraph(QWidget *parent) : QWidget(parent)
{    
    setAttribute(Qt::WA_TranslucentBackground, true);
    m_sig = 0;

    lab = new QLabel(this);
    QPixmap *pixmap = new QPixmap(":/image/RSS.png");
    pixmap->scaled(lab->size(), Qt::KeepAspectRatio);
    lab->setScaledContents(true);
    lab->setPixmap(*pixmap);
}

void SignalGraph::paintEvent(QPaintEvent *event)
{    
    lab->setGeometry(0, 0, width()/2, height());
    QPainter painter(this);
    painter.setPen(QPen(Qt::gray, 1));
    painter.setBrush(QColor(113, 130, 220));
    double w = width()/2/5.8;
    double h = height()/5;


    for(int i=0;i<5;++i){
       if (i == m_sig)
           painter.setBrush(QColor(0,0,0,0));
       painter.drawRect(width()/2+ (w+w*0.2)*i, height()-h-h*i -1, w, h+h*i);
    }
}

void SignalGraph::setSigVal(int val)
{
    m_sig = val;
}

