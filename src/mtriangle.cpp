#include "mtriangle.h"


/*!
    Create a triangle using QPointF objects
*/
MTriangle::MTriangle(QPointF &a_, QPointF &b_, QPointF &c_) :
    a(a_), b(b_), c(c_)
{

}


/*!
    Create a triangle using QPoint objects
*/
MTriangle::MTriangle(QPoint &a_, QPoint &b_, QPoint &c_) :
    a(a_), b(b_), c(c_)
{

}


/*!
    Create a triangle using real values
*/
MTriangle::MTriangle(qreal ax, qreal ay, qreal bx, qreal by, qreal cx, qreal cy) :
    a(ax, ay),
    b(bx, by),
    c(cx, cy)
{

}


/*!
    Returns the vertices of the Triangle in an OpenGL buffer friendly format.

    \a compLength Is the number of components to give the the vertices.  The value
    must either be 2, 3, or 4.  If it isn't then whatever number it's closest to
    will be chosen as the component length.  The default is 4 (x, y, z, w)

    \a z is the value to assign to the Z component of each vertex.  This is moot
    if \a compLength is 2.  Default value is 0.

    \a w is the value to assing the W component of each vertex.  This has no
    effect if \a compLength is 2 or 3.

    Elements will be ordered like so:
    [x y] [x y] [x y]
    [x y z] [x y z] [x y z]
    [x y z w] [x y z w] [x y z w] 
*/
QVector<GLfloat> MTriangle::glVector(int compLength, qreal z, qreal w) {
    // Bounds
    if (compLength < 2)
        compLength = 2;
    else if (compLength > 4)
        compLength = 4;

    // Make the vector
    QVector<GLfloat> v;
    v.reserve(compLength * 3);        // Three vertices in a triangle
    
    // Stuff it
    switch (compLength) {
        case 2:
            v += a.x(); v += a.y();
            v += b.x(); v += b.y();
            v += c.x(); v += c.y();
            break;

        case 3:
            v += a.x(); v += a.y(); v += z;
            v += b.x(); v += b.y(); v += z;
            v += c.x(); v += c.y(); v += z;
            break;

        case 4:
            v += a.x(); v += a.y(); v += z; v += w;
            v += b.x(); v += b.y(); v += z; v += w;
            v += c.x(); v += c.y(); v += z; v += w;
            break;
    }

    return v;
}


/*!
    Returns a scaled copy of the Triangle
    \a x amount to scale along the x axis (set to 1 for no changes)
    \a y amount to scale along the y axis (set to 1 for no changes)
*/
MTriangle MTriangle::scale(qreal x, qreal y) {
    MTriangle p(a.x() * x, a.y() * y,
                b.x() * x, b.y() * y,
                c.x() * x, c.y() * y);
    return p;
}

