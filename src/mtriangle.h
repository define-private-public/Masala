// simple triangle for queuing things up to OpenGL

#ifndef M_TRIANGLE
#define M_TRIANGLE

#include <QPointF>
#include <QVector>
#include <QtOpenGL>


class MTriangle {
public:
    QPointF a;
    QPointF b;
    QPointF c;

    MTriangle(QPointF &a_, QPointF &b_, QPointF &c_);
    MTriangle(QPoint &a_, QPoint &b_, QPoint &c_);
    MTriangle(qreal ax, qreal ay, qreal bx, qreal by, qreal cx, qreal cy);

    QVector<GLfloat> glVector(int compLength=4, qreal z=0, qreal w=1);

    MTriangle scale(qreal x, qreal y);

};

#endif // M_TRIANGLE

