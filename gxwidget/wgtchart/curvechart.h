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

#ifndef CURVECHART_H
#define CURVECHART_H

#include <QWidget>

#ifdef MSC
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT CurveChart: public QWidget
#else

struct PointData{
    qreal   pointValue;
    QString timeString;
};

struct PointCoord
{
    QPointF pointF;
    PointData pointD;
};

class CurveChart : public QWidget
#endif

{
    Q_OBJECT

    Q_PROPERTY(QColor BackgroundStartColor READ BackgroundStartColor WRITE setBackgroundStartColor)
    Q_PROPERTY(QColor BackgroundEndColor READ BackgroundEndColor WRITE setBackgroundEndColor)
    Q_PROPERTY(QColor TextColor READ TextColor WRITE setTextColor)
    Q_PROPERTY(QColor PointColor READ PointColor WRITE setPointColor)
    Q_PROPERTY(qreal Min READ Min WRITE setMax)
    Q_PROPERTY(qreal Max READ Max WRITE setMax)
    Q_PROPERTY(qreal StepHoriginal READ StepHoriginal WRITE setStepHoriginal)
    Q_PROPERTY(qreal StepVertical READ StepVertical WRITE setStepVertical)
    Q_PROPERTY(qreal ChartLeftMargin READ ChartLeftMargin WRITE setChartLeftMargin)
    Q_PROPERTY(qreal ChartTopMargin READ ChartTopMargin WRITE setChartTopMargin)
    Q_PROPERTY(qreal ChartRightMargin READ ChartRightMargin WRITE setChartRightMargin)
    Q_PROPERTY(qreal ChartBottomMargin READ ChartBottomMargin WRITE setChartBottomMargin)
    Q_PROPERTY(QString DataString READ DataString WRITE addDataStr)

public:
    CurveChart(QWidget *parent = 0);
    ~CurveChart();

    QColor BackgroundStartColor() const { return m_bgStartColor; }
    void setBackgroundStartColor(QColor color);
    QColor BackgroundEndColor() const { return m_bgEndColor; }
    void setBackgroundEndColor(QColor color);
    QColor TextColor() const { return m_textColor; }
    void setTextColor(QColor color);
    QColor PointColor() const { return m_pointColor; }
    void setPointColor(QColor color);
    qreal Min() const { return m_min; }
    void setMin(qreal min);
    qreal Max() const { return m_max; }
    void setMax(qreal max);
    int MaxPointNum() const {return m_maxPointNum;};
    void setMaxPointNum(int pointNum);
    qreal StepHoriginal() const { return m_stepH; }
    void setStepHoriginal(qreal val);
    qreal StepVertical() const { return m_stepV; }
    void setStepVertical(qreal val);
    qreal ChartLeftMargin() const { return m_chartLeftMargin; }
    void setChartLeftMargin(qreal margin);
    qreal ChartTopMargin() const { return m_chartTopMargin; }
    void setChartTopMargin(qreal margin);
    qreal ChartRightMargin() const { return m_chartRightMargin; }
    void setChartRightMargin(qreal margin);
    qreal ChartBottomMargin() const { return m_chartBottomMargin; }
    void setChartBottomMargin(qreal margin);
    QString DataString() const;
    void addDataStr(QString str);
    void addData(PointData pointD);
    void addDataStrList(QString txtPath);

public slots:
    void setTitle(QString str);
    void setShowLine(bool show);
    void setShowPoint(bool show);
    void setShowPointBackground(bool show);
    void setShowArrow(bool show);
    void clearData();

protected:
    void paintEvent(QPaintEvent *);
    void drawBackground(QPainter* p);
    void drawTitle(QPainter* p);
    void drawBorder(QPainter* p);
    void drawText(QPainter* p);
    void drawPoints(QPainter* p);

    QSize minimumSizeHint() const { return QSize(134, 134); }

private:
    void setRange(qreal min, qreal max);

private:
    QColor m_bgStartColor;//????????????
    QColor m_bgEndColor;
    QColor m_textColor;//??????/??????/????????????
    QColor m_pointColor;//???????????????
    QString m_title;//????????????
    qreal m_min;//?????????
    qreal m_max;//?????????
    qreal m_stepH;//????????????
    qreal m_stepV;//????????????
    qreal m_chartLeftMargin;//???????????????
    qreal m_chartTopMargin;
    qreal m_chartRightMargin;
    qreal m_chartBottomMargin;
    int m_pointRadious;//???????????????
    bool m_showLine;//???????????????
    bool m_showPoint;//???????????????
    bool m_showPointBg;//???????????????????????????
    bool m_showArrow;//????????????
    //
    QVector<PointData> m_points;
    int     m_maxPointNum;
};

#endif // CURVECHART_H
