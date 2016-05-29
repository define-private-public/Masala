#include "mframe.h"
#include <QPoint>;
#include <QPointF>;
#include <QSize>;
#include <QSizeF>;
#include <QRect>
#include <QRectF>


/*!
    Default constructor for the MFrame
*/
MFrame::MFrame() :
    x(0),
    y(0),
    w(0),
    h(0),
    name("")
{
}

/*!
    Handly constructor for the MFrame class
*/
MFrame::MFrame(int x_, int y_, int w_, int h_, QString name_) :
    x(x_),
    y(y_),
    w(w_),
    h(h_),
    name(name_)
{
}


/*!
    Copy constructor
*/
MFrame::MFrame(const MFrame &f) :
    x(f.x),
    y(f.y),
    w(f.w),
    h(f.h),
    name(f.name)
{
}


QPoint MFrame::topLeft() {
    return QPoint(x, y);
}


QPointF MFrame::topLeftF() {
    return QPointF(x, y);
}


QSize MFrame::size() {
    return QSize(w, h);
}


QSizeF MFrame::sizeF() {
    return QSizeF(w, h);
}


QRect MFrame::toRect() {
    return QRect(x, y, w, h);
}


QRectF MFrame::toRectF() {
    return QRectF(x, y, w, h);
}


QString MFrame::str() {
    return QString("%1: x=%2, y=%3, w=%4, h=%5").arg(name).arg(x).arg(y).arg(w).arg(h);
}
