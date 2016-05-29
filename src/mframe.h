// Simple Frame data structure for Sprite Animation

#ifndef M_FRAME_H
#define M_FRAME_H

#include <QString>
class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;


class MFrame {
public:
    // Data
    int x;
    int y;
    int w;
    int h;
    QString name;

    // Constructor
    MFrame();
    MFrame(int x_, int y_, int w_, int h_, QString name_="");
    MFrame(const MFrame &f);

    // Extra fun things
    QPoint topLeft();
    QPointF topLeftF();
    QSize size();
    QSizeF sizeF();
    QRect toRect();
    QRectF toRectF();
    QString str();
};


#endif // M_FRAME_H

