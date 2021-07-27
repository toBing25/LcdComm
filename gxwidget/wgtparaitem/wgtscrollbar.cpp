#include "wgtscrollbar.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOptionSlider>

wgtScrollbar::wgtScrollbar(Qt::Orientation orientation, QWidget *parent)
    : QScrollBar(orientation, parent),
    _sliderLength(100)
{

}
void wgtScrollbar::paintEvent(QPaintEvent *e)
{
    QScrollBar::paintEvent(e);
    QPainter painter(this);
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    opt.subControls = QStyle::SC_All;
    QWidget *widget = this;
    QPainter *p = &painter;
    //style()->drawComplexControl(QStyle::CC_ScrollBar, &opt, &p, this);
    if (const QStyleOptionSlider *scrollbar = &opt) {
        // Make a copy here and reset it for each primitive.
        QStyleOptionSlider newScrollbar = *scrollbar;
        QStyle::State saveFlags = scrollbar->state;

        if (scrollbar->subControls & QStyle::SC_ScrollBarSubLine) {
            newScrollbar.state = saveFlags;
            newScrollbar.rect = subControlRect(QStyle::CC_ScrollBar, &newScrollbar, QStyle::SC_ScrollBarSubLine, widget);
            if (newScrollbar.rect.isValid()) {
                if (!(scrollbar->activeSubControls & QStyle::SC_ScrollBarSubLine))
                    newScrollbar.state &= ~(QStyle::State_Sunken | QStyle::State_MouseOver);
                style()->drawControl(QStyle::CE_ScrollBarSubLine, &newScrollbar, p, widget);
            }
        }
        if (scrollbar->subControls & QStyle::SC_ScrollBarAddLine) {
            newScrollbar.rect = scrollbar->rect;
            newScrollbar.state = saveFlags;
            newScrollbar.rect = subControlRect(QStyle::CC_ScrollBar, &newScrollbar, QStyle::SC_ScrollBarAddLine, widget);
            if (newScrollbar.rect.isValid()) {
                if (!(scrollbar->activeSubControls & QStyle::SC_ScrollBarAddLine))
                    newScrollbar.state &= ~(QStyle::State_Sunken | QStyle::State_MouseOver);
                style()->drawControl(QStyle::CE_ScrollBarAddLine, &newScrollbar, p, widget);
            }
        }
        if (scrollbar->subControls & QStyle::SC_ScrollBarSubPage) {
            newScrollbar.rect = scrollbar->rect;
            newScrollbar.state = saveFlags;
            newScrollbar.rect = subControlRect(QStyle::CC_ScrollBar, &newScrollbar, QStyle::SC_ScrollBarSubPage, widget);
            if (newScrollbar.rect.isValid()) {
                if (!(scrollbar->activeSubControls & QStyle::SC_ScrollBarSubPage))
                    newScrollbar.state &= ~(QStyle::State_Sunken | QStyle::State_MouseOver);
                style()->drawControl(QStyle::CE_ScrollBarSubPage, &newScrollbar, p, widget);
            }
        }
        if (scrollbar->subControls & QStyle::SC_ScrollBarAddPage) {
            newScrollbar.rect = scrollbar->rect;
            newScrollbar.state = saveFlags;
            newScrollbar.rect = subControlRect(QStyle::CC_ScrollBar, &newScrollbar, QStyle::SC_ScrollBarAddPage, widget);
            if (newScrollbar.rect.isValid()) {
                if (!(scrollbar->activeSubControls & QStyle::SC_ScrollBarAddPage))
                    newScrollbar.state &= ~(QStyle::State_Sunken | QStyle::State_MouseOver);
                style()->drawControl(QStyle::CE_ScrollBarAddPage, &newScrollbar, p, widget);
            }
        }
        if (scrollbar->subControls & QStyle::SC_ScrollBarFirst) {
            newScrollbar.rect = scrollbar->rect;
            newScrollbar.state = saveFlags;
            newScrollbar.rect = subControlRect(QStyle::CC_ScrollBar, &newScrollbar, QStyle::SC_ScrollBarFirst, widget);
            if (newScrollbar.rect.isValid()) {
                if (!(scrollbar->activeSubControls & QStyle::SC_ScrollBarFirst))
                    newScrollbar.state &= ~(QStyle::State_Sunken | QStyle::State_MouseOver);
                style()->drawControl(QStyle::CE_ScrollBarFirst, &newScrollbar, p, widget);
            }
        }
        if (scrollbar->subControls & QStyle::SC_ScrollBarLast) {
            newScrollbar.rect = scrollbar->rect;
            newScrollbar.state = saveFlags;
            newScrollbar.rect = subControlRect(QStyle::CC_ScrollBar, &newScrollbar, QStyle::SC_ScrollBarLast, widget);
            if (newScrollbar.rect.isValid()) {
                if (!(scrollbar->activeSubControls & QStyle::SC_ScrollBarLast))
                    newScrollbar.state &= ~(QStyle::State_Sunken | QStyle::State_MouseOver);
                style()->drawControl(QStyle::CE_ScrollBarLast, &newScrollbar, p, widget);
            }
        }
        if (scrollbar->subControls & QStyle::SC_ScrollBarSlider) {

            newScrollbar.rect = scrollbar->rect;

            newScrollbar.state = saveFlags;
            QRect rect = subControlRect(QStyle::CC_ScrollBar, &newScrollbar, QStyle::SC_ScrollBarSlider, widget);
            newScrollbar.rect = QRect(rect.topLeft(), QSize(rect.width(), _sliderLength));

            if (newScrollbar.rect.isValid()) {
                if (!(scrollbar->activeSubControls & QStyle::SC_ScrollBarSlider))
                    newScrollbar.state &= ~(QStyle::State_Sunken | QStyle::State_MouseOver);

                style()->drawControl(QStyle::CE_ScrollBarSlider, &newScrollbar, p, widget);

                if (scrollbar->state & QStyle::State_HasFocus) {
                    QStyleOptionFocusRect fropt;
                    fropt.QStyleOption::operator=(newScrollbar);
                    fropt.rect.setRect(newScrollbar.rect.x() + 2, newScrollbar.rect.y() + 2,
                                       newScrollbar.rect.width() - 5,
                                       newScrollbar.rect.height() - 5);
                    style()->drawPrimitive(QStyle::PE_FrameFocusRect, &fropt, p, widget);
                }
            }
        }
    }

}

void wgtScrollbar::setSliderLength(int length)
{
    _sliderLength = length;
}

QRect wgtScrollbar::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, /*const*/ QWidget *widget)
{
    QRect ret;
    switch (cc) {
    case QStyle::CC_ScrollBar:
        if (const QStyleOptionSlider *scrollbar = qstyleoption_cast<const QStyleOptionSlider *>(opt)) {
            const QRect scrollBarRect = scrollbar->rect;
            int sbextent = 0;
            if (!style()->styleHint(QStyle::SH_ScrollBar_Transient, scrollbar, widget))
                sbextent = style()->pixelMetric(QStyle::PM_ScrollBarExtent, scrollbar, widget);
            int maxlen = ((scrollbar->orientation == Qt::Horizontal) ?
                              scrollBarRect.width() : scrollBarRect.height()) - (sbextent * 2);
            int sliderlen;

            // calculate slider length
            if (scrollbar->maximum != scrollbar->minimum) {
                uint range = scrollbar->maximum - scrollbar->minimum;
                sliderlen = (qint64(scrollbar->pageStep) * maxlen) / (range + scrollbar->pageStep);

                int slidermin = style()->pixelMetric(QStyle::PM_ScrollBarSliderMin, scrollbar, widget);
                if (_sliderLength < slidermin || range > INT_MAX / 2)
                    _sliderLength = slidermin;
                if (_sliderLength > maxlen)
                    _sliderLength = maxlen;
                //length of the slider
                sliderlen = _sliderLength;
            } else {
                sliderlen = maxlen;
            }

            int sliderstart = sbextent + this->sliderPositionFromValue(scrollbar->minimum,
                                                                       scrollbar->maximum,
                                                                       scrollbar->sliderPosition,
                                                                       maxlen - sliderlen,
                                                                       scrollbar->upsideDown);

            switch (sc) {
            case QStyle::SC_ScrollBarSubLine:            // top/left button
                if (scrollbar->orientation == Qt::Horizontal) {
                    int buttonWidth = qMin(scrollBarRect.width() / 2, sbextent);
                    ret.setRect(0, 0, buttonWidth, scrollBarRect.height());
                } else {
                    int buttonHeight = qMin(scrollBarRect.height() / 2, sbextent);
                    ret.setRect(0, 0, scrollBarRect.width(), buttonHeight);
                }
                break;
            case QStyle::SC_ScrollBarAddLine:            // bottom/right button
                if (scrollbar->orientation == Qt::Horizontal) {
                    int buttonWidth = qMin(scrollBarRect.width()/2, sbextent);
                    ret.setRect(scrollBarRect.width() - buttonWidth, 0, buttonWidth, scrollBarRect.height());
                } else {
                    int buttonHeight = qMin(scrollBarRect.height()/2, sbextent);
                    ret.setRect(0, scrollBarRect.height() - buttonHeight, scrollBarRect.width(), buttonHeight);
                }
                break;
            case QStyle::SC_ScrollBarSubPage:            // between top/left button and slider
                if (scrollbar->orientation == Qt::Horizontal)
                    ret.setRect(sbextent, 0, sliderstart - sbextent, scrollBarRect.height());
                else
                    ret.setRect(0, sbextent, scrollBarRect.width(), sliderstart - sbextent);
                break;
            case QStyle::SC_ScrollBarAddPage:            // between bottom/right button and slider
                if (scrollbar->orientation == Qt::Horizontal)
                    ret.setRect(sliderstart + sliderlen, 0,
                                maxlen - sliderstart - sliderlen + sbextent, scrollBarRect.height());
                else
                    ret.setRect(0, sliderstart + sliderlen, scrollBarRect.width(),
                                maxlen - sliderstart - sliderlen + sbextent);
                break;
            case QStyle::SC_ScrollBarGroove:
                if (scrollbar->orientation == Qt::Horizontal)
                    ret.setRect(sbextent, 0, scrollBarRect.width() - sbextent * 2,
                                scrollBarRect.height());
                else
                    ret.setRect(0, sbextent, scrollBarRect.width(),
                                scrollBarRect.height() - sbextent * 2);
                break;
            case QStyle::SC_ScrollBarSlider:
                if (scrollbar->orientation == Qt::Horizontal)
                    ret.setRect(sliderstart, 0, sliderlen, scrollBarRect.height());
                else
                    ret.setRect(0, sliderstart, scrollBarRect.width(), sliderlen);

                break;
            default:
                break;
            }
            ret = visualRect(scrollbar->direction, scrollBarRect, ret);
        }
        //return ret;
    default:
        break;
    }
    return ret;
}

QRect wgtScrollbar::visualRect(Qt::LayoutDirection direction, const QRect &boundingRect, const QRect &logicalRect)
{
    if (direction == Qt::LeftToRight)
        return logicalRect;
    QRect rect = logicalRect;
    rect.translate(2 * (boundingRect.right() - logicalRect.right()) +
                   logicalRect.width() - boundingRect.width(), 0);
    return rect;
}

int wgtScrollbar::sliderPositionFromValue(int min, int max, int logicalValue, int span, bool upsideDown)
{
    if (span <= 0 || logicalValue < min || max <= min)
        return 0;
    if (logicalValue > max)
        return upsideDown ? span : min;

    uint range = max - min;
    uint p = upsideDown ? max - logicalValue : logicalValue - min;

    if (range > (uint)INT_MAX/4096) {
        double dpos = (double(p))/(double(range)/span);
        return int(dpos);
    } else if (range > (uint)span) {
        return (2 * p * span + range) / (2*range);
    } else {
        uint div = span / range;
        uint mod = span % range;
        return p * div + (2 * p * mod + range) / (2 * range);
    }
    // equiv. to (p * span) / range + 0.5
    // no overflow because of this implicit assumption:
    // span <= 4096
}
