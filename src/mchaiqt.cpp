#include "mchaiqt.h"
#include <QString>
#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QRect>
#include <QRectF>
#include <QColor>
#include <QVariant>
#include <string>
#include "chaiscript/chaiscript.hpp"
#include "chaiscript/utility/utility.hpp"
using namespace std;
using namespace chaiscript;
using namespace chaiscript::utility;


namespace chaiQt {
    // The hack to do a templated class
    #define BIND_QLIST(T, name, chai, m)                         \
        add_class<QList<T>>(*m,                                  \
            name,                                                \
            {                                                    \
                constructor<QList<T>()>(),                       \
            },                                                   \
            {                                                    \
                { fun([](const QList<T> &l) {                    \
                    return string(name);                         \
                }), "to_string" },                               \
                                                                 \
                { fun(&QList<T>::at), "at" },                    \
                { fun(&QList<T>::clear), "clear" },              \
                { fun(&QList<T>::contains), "contains" },        \
                { fun(&QList<T>::empty), "empty" },              \
                { fun(&QList<T>::isEmpty), "isEmpty" },          \
                { fun(&QList<T>::length), "length" },            \
                { fun(&QList<T>::move), "move" },                \
                { fun(&QList<T>::pop_back), "pop_back" },        \
                { fun(&QList<T>::pop_front), "pop_front" },      \
                { fun(&QList<T>::prepend), "prepend" },          \
                { fun(&QList<T>::push_back), "push_back" },      \
                { fun(&QList<T>::push_front), "push_front" },    \
                { fun(&QList<T>::removeAt), "removeAt" },        \
            }                                                    \
        );
    

    // Call this to bind all of the Qt objects
    void bindAll(ChaiScript &chai) {
        ModulePtr m(new Module());

        bindQString(chai, m);
        bindQObject(chai, m);
        bindQPoint(chai, m);
        bindQPointF(chai, m);
        bindQSize(chai, m);
        bindQSizeF(chai, m);
        bindQRect(chai, m);
        bindQRectF(chai, m);
        bindQColor(chai, m);
        bindQVariant(chai, m);

        
        // Some other stuff
        m->add(chaiscript::fun(&qrand), "qrand");
        m->add(chaiscript::fun(&qsrand), "qsrand");

        chai.add(m);
    }


    void bindQString(ChaiScript &chai, ModulePtr &m) {
        // Add very basic QString
        add_class<QString>(*m,
            "QString",
            {
                constructor<QString()>(),
                constructor<QString(const QString &)>(),
//                constructor<QString(QString &&)>(),
//                constructor<QString(const char *)>(),
            },
            {
                { fun(&QString::toStdString), "to_string" },            // ChaiScript

                { fun(&QString::chop), "chop" },
                { fun(&QString::clear), "clear" },
                { fun(&QString::isEmpty), "isEmpty" },
                { fun(&QString::isNull), "isNull" },
                { fun(&QString::toStdString), "toStdString" },
                { fun(&QString::size), "size" },
//                { fun(&QString::simplified), "simplified" },            // Errors
//                { fun(&QString::trimmed), "trimmed" },                  // Errors
                { fun(&QString::truncate), "truncate" },
            }
        );
    }


    void bindQObject(ChaiScript &chai, ModulePtr &m) {
        add_class<QObject>(*m,
            "QObject",
            {
                constructor<QObject()>(),
                constructor<QObject(QObject *)>()
            },
            {
                { fun([](const QObject &o) {
                    return string("QObject");
                }), "to_string" },

                { fun(&QObject::parent), "parent" },
                { fun([](QObject &o, const string &name) { o.setObjectName(name.c_str()); }), "setObjectName" },
                { fun([](QObject &o) { return o.objectName().toStdString(); }), "objectName" }
            }
        );

        // Shorthand for creating a QString in the engine
        m->add(fun([](const string &sourceText) { return QObject::tr(sourceText.c_str()); }), "tr");
    }


    void bindQPoint(ChaiScript &chai, ModulePtr &m) {
        add_class<QPoint>(*m,
            "QPoint",
            {
                constructor<QPoint()>(),
                constructor<QPoint(int, int)>()
            },
            {
                { fun([](const QPoint &point) {
                    return ("QPoint(" + QString::number(point.x()) + ", " + QString::number(point.y()) + ")").toStdString();
                }), "to_string" },

                { fun(&QPoint::isNull), "isNull" },
                { fun(&QPoint::manhattanLength), "manhattanLength" },
                { fun(&QPoint::rx), "rx" },
                { fun(&QPoint::ry), "ry" },
                { fun(&QPoint::setX), "setX" },
                { fun(&QPoint::setY), "setY" },
                { fun(&QPoint::x), "x" },
                { fun(&QPoint::y), "y" },

                { fun([](QPoint &p, float factor) { return p *= factor; }), "*="},
                { fun([](QPoint &p, double factor) { return p *= factor; }), "*="},
                { fun([](QPoint &p, int factor) { return p *= factor; }), "*="},
                { fun(&QPoint::operator+=), "+=" },
                { fun(&QPoint::operator-=), "-=" },
                { fun(&QPoint::operator/=), "/=" },

                // Assignment operator
                { fun([](QPoint &p, const QPoint &other) {
                    if (p != other) {
                        p.rx() = other.x();
                        p.ry() = other.y();
                    }
                    return p;
                }), "=" },
            }
        );

        // QPoint::dotProduct()
        m->add(fun(&QPoint::dotProduct), "dotProduct");

        // Operators
        m->add(fun([](const QPoint &p1, const QPoint &p2) { return p1 != p2; }), "!=");
        m->add(fun([](const QPoint &point, float factor) { return point * factor; }), "*");
        m->add(fun([](const QPoint &point, double factor) { return point * factor; }), "*");
        m->add(fun([](const QPoint &point, int factor) { return point * factor; }), "*");
        m->add(fun([](float factor, const QPoint& point) { return factor * point; }), "*");
        m->add(fun([](double factor, const QPoint& point) { return factor * point; }), "*");
        m->add(fun([](int factor, const QPoint& point) { return factor * point; }), "*");
        m->add(fun([](const QPoint &p1, const QPoint &p2) { return p1 + p2; }), "+");
        m->add(fun([](const QPoint &point) { return +point; }), "+");
        m->add(fun([](const QPoint &p1, const QPoint &p2) { return p1 - p2; }), "-");
        m->add(fun([](const QPoint &point) { return -point; }), "-");
        m->add(fun([](const QPoint &point, qreal divisor) { return point / divisor; }), "/");
        m->add(fun([](const QPoint &p1, const QPoint &p2) { return p1 == p2; }), "==");
    }


    void bindQPointF(ChaiScript &chai, ModulePtr &m) {
        add_class<QPointF>(*m,
            "QPointF",
            {
                constructor<QPointF()>(),
                constructor<QPointF(const QPointF &)>(),
                constructor<QPointF(qreal, qreal)>()
            },
            {
                { fun([](const QPointF &point) {
                    return ("QPointF(" + QString::number(point.x()) + ", " + QString::number(point.y()) + ")").toStdString();
                }), "to_string" },

                { fun(&QPointF::isNull), "isNull" },
                { fun(&QPointF::manhattanLength), "manhattanLength" },
                { fun(&QPointF::rx), "rx" },
                { fun(&QPointF::ry), "ry" },
                { fun(&QPointF::setX), "setX" },
                { fun(&QPointF::setY), "setY" },
                { fun(&QPointF::toPoint), "toPoint" },
                { fun(&QPointF::x), "x" },
                { fun(&QPointF::y), "y" },

                { fun(&QPointF::operator*=), "*=" },
                { fun(&QPointF::operator-=), "-=" },
                { fun(&QPointF::operator+=), "+=" },
                { fun(&QPointF::operator/=), "/=" },

                // Assignment operator
                { fun([](QPointF &p, const QPointF &other) {
                    if (p != other) {
                        p.rx() = other.x();
                        p.ry() = other.y();
                    }
                    return p;
                }), "=" },
            }
        );

        // QPoint::dotProduct()
        m->add(fun(&QPointF::dotProduct), "dotProduct");

        // Operators
        m->add(fun([](const QPointF &p1, const QPointF &p2) { return p1 != p2; }), "!=");
        m->add(fun([](const QPointF &point, qreal factor) { return point * factor; }), "*");
        m->add(fun([](qreal factor, const QPointF& point) { return factor * point; }), "*");
        m->add(fun([](const QPointF &p1, const QPointF &p2) { return p1 + p2; }), "+");
        m->add(fun([](const QPointF &point) { return +point; }), "+");
        m->add(fun([](const QPointF &p1, const QPointF &p2) { return p1 - p2; }), "-");
        m->add(fun([](const QPointF &point) { return -point; }), "-");
        m->add(fun([](const QPointF &point, qreal divisor) { return point / divisor; }), "/");
        m->add(fun([](const QPointF &p1, const QPointF &p2) { return p1 == p2; }), "==");
    }


    void bindQSize(ChaiScript &chai, ModulePtr &m) {
        // Add a subset of QSize
        add_class<QSize>(*m,
            "QSize",
            {
                constructor<QSize()>(),
                constructor<QSize(int, int)>()
            },
            {
                { fun([](const QSize &size) {
                    return ("QSize(" + QString::number(size.width()) + ", " + QString::number(size.height()) + ")").toStdString();
                }), "to_string" },

                { fun(&QSize::height), "height" },
                { fun(&QSize::isEmpty), "isEmpty" },
                { fun(&QSize::isNull), "isNull" },
                { fun(&QSize::isValid), "isValid" },
                { fun(&QSize::rheight), "rheight" },
                { fun(&QSize::rwidth), "rwidth" },
                { fun(&QSize::setHeight), "setHeight" },
                { fun(&QSize::setWidth), "setWidth" },
                { fun(&QSize::transpose), "transpose" },
                { fun(&QSize::transposed), "transposed" },
                { fun(&QSize::width), "width" },

                { fun(&QSize::operator*=), "*=" },
                { fun(&QSize::operator+=), "+=" },
                { fun(&QSize::operator-=), "-=" },
                { fun(&QSize::operator/=), "/=" },

                // Assignment operator
                { fun([](QSize &s, const QSize &other) {
                    if (s != other) {
                        s.rwidth() = other.width();
                        s.rheight() = other.height();
                    }
                    return s;
                }), "=" },
            }
        );

        // Operators
        m->add(fun([](const QSize &s1, const QSize &s2) { return s1 != s2; }), "!=");
        m->add(fun([](const QSize &size, qreal factor) { return size * factor; }), "*");
        m->add(fun([](qreal factor, const QSize& size) { return factor * size; }), "*");
        m->add(fun([](const QSize &s1, const QSize &s2) { return s1 + s2; }), "+");
        m->add(fun([](const QSize &s1, const QSize &s2) { return s1 - s2; }), "-");
        m->add(fun([](const QSize &size, qreal divisor) { return size / divisor; }), "/");
        m->add(fun([](const QSize &s1, const QSize &s2) { return s1 == s2; }), "==");
    }


    void bindQSizeF(ChaiScript &chai, ModulePtr &m) {
        // Add a subset of QSizeF
        add_class<QSizeF>(*m,
            "QSizeF",
            {
                constructor<QSizeF()>(),
                constructor<QSizeF(const QSize &)>(),
                constructor<QSizeF(qreal, qreal)>()
            },
            {
                { fun([](const QSizeF &size) {
                    return ("QSizeF(" + QString::number(size.width()) + ", " + QString::number(size.height()) + ")").toStdString();
                }), "to_string" },

                { fun(&QSizeF::height), "height" },
                { fun(&QSizeF::isEmpty), "isEmpty" },
                { fun(&QSizeF::isNull), "isNull" },
                { fun(&QSizeF::isValid), "isValid" },
                { fun(&QSizeF::rheight), "rheight" },
                { fun(&QSizeF::rwidth), "rwidth" },
                { fun(&QSizeF::setHeight), "setHeight" },
                { fun(&QSizeF::setWidth), "setWidth" },
                { fun(&QSizeF::toSize), "toSize" },
                { fun(&QSizeF::transpose), "transpose" },
                { fun(&QSizeF::transposed), "transposed" },
                { fun(&QSizeF::width), "width" },

                { fun(&QSizeF::operator*=), "*=" },
                { fun(&QSizeF::operator+=), "+=" },
                { fun(&QSizeF::operator-=), "-=" },
                { fun(&QSizeF::operator/=), "/=" },

                // Assignment operator
                { fun([](QSizeF &s, const QSizeF &other) {
                    if (s != other) {
                        s.rwidth() = other.width();
                        s.rheight() = other.height();
                    }
                    return s;
                }), "=" },
            }
        );

        // Operators
        m->add(fun([](const QSizeF &s1, const QSizeF &s2) { return s1 != s2; }), "!=");
        m->add(fun([](const QSizeF &size, qreal factor) { return size * factor; }), "*");
        m->add(fun([](qreal factor, const QSizeF& size) { return factor * size; }), "*");
        m->add(fun([](const QSizeF &s1, const QSizeF &s2) { return s1 + s2; }), "+");
        m->add(fun([](const QSizeF &s1, const QSizeF &s2) { return s1 - s2; }), "-");
        m->add(fun([](const QSizeF &size, qreal divisor) { return size / divisor; }), "/");
        m->add(fun([](const QSizeF &s1, const QSizeF &s2) { return s1 == s2; }), "==");
    }


    void bindQRect(ChaiScript &chai, ModulePtr &m) {
        add_class<QRect>(*m,
            "QRect",
            {
                constructor<QRect()>(),
                constructor<QRect(const QPoint &, const QPoint &)>(),
                constructor<QRect(const QPoint &, const QSize &)>(),
                constructor<QRect(int, int, int, int)>()
            },
            {
                { fun([](const QRect &rect) {
                    return ("QRect(" +
                            QString::number(rect.x()) + ", " +
                            QString::number(rect.y()) + ", " +
                            QString::number(rect.width()) + ", " +
                            QString::number(rect.height()) + ")")
                    .toStdString();
                }), "to_string" },

                { fun(&QRect::bottom), "bottom" },
                { fun(&QRect::bottomLeft), "bottomLeft" },
                { fun(&QRect::bottomRight), "bottomRight" },
                { fun(&QRect::center), "center" },
                { fun(&QRect::height), "height" },
                { fun(&QRect::intersected), "intersected" },
                { fun(&QRect::intersects), "intersects" },
                { fun(&QRect::isEmpty), "isEmpty" },
                { fun(&QRect::isNull), "isNull" },
                { fun(&QRect::isValid), "isValid" },
                { fun(&QRect::left), "left" },
                { fun(&QRect::moveBottom), "moveBottom" },
                { fun(&QRect::moveBottomLeft), "moveBottomLeft" },
                { fun(&QRect::moveBottomRight), "moveBottomRight" },
                { fun(&QRect::moveCenter), "moveCenter" },
                { fun(&QRect::moveLeft), "moveLeft" },
                { fun(&QRect::moveRight), "moveRight" },
                { fun(&QRect::moveTop), "moveTop" },
                { fun(&QRect::moveTopLeft), "moveTopLeft" },
                { fun(&QRect::moveTopRight), "moveTopRight" },
                { fun(&QRect::normalized), "normalized" },
                { fun(&QRect::right), "right" },
                { fun(&QRect::setBottom), "setBottom" },
                { fun(&QRect::setBottomLeft), "setBottomLeft" },
                { fun(&QRect::setBottomRight), "setBottomRight" },
                { fun(&QRect::setCoords), "setCoords" },
                { fun(&QRect::setLeft), "setLeft" },
                { fun(&QRect::setRect), "setRect" },
                { fun(&QRect::setRight), "setRight" },
                { fun(&QRect::setSize), "setSize" },
                { fun(&QRect::setTop), "setTop" },
                { fun(&QRect::setTopLeft), "setTopLeft" },
                { fun(&QRect::setTopRight), "setTopRight" },
                { fun(&QRect::setWidth), "setWidth" },
                { fun(&QRect::setX), "setX" },
                { fun(&QRect::setY), "setY" },
                { fun(&QRect::size), "size" },
                { fun(&QRect::top), "top" },
                { fun(&QRect::topLeft), "topLeft" },
                { fun(&QRect::topRight), "topRight" },
                { fun(&QRect::united), "united" },
                { fun(&QRect::width), "width" },
                { fun(&QRect::x), "x" },
                { fun(&QRect::y), "y" },
            }
        );

        // Operators
        m->add(fun([](const QRect &r1, const QRect &r2) { return r1 != r2; }), "!=");
        m->add(fun([](const QRect &r1, const QRect &r2) { return r1 == r2; }), "==");
    }


    void bindQRectF(ChaiScript &chai, ModulePtr &m) {
        add_class<QRectF>(*m,
            "QRectF",
            {
                constructor<QRectF()>(),
                constructor<QRectF(const QPointF &, const QSizeF &)>(),
                constructor<QRectF(const QPointF &, const QPointF &)>(),
                constructor<QRectF(qreal, qreal, qreal, qreal)>(),
                constructor<QRectF(const QRect &)>()
            },
            {
                { fun([](const QRectF &rect) {
                    return ("QRectF(" +
                            QString::number(rect.x()) + ", " +
                            QString::number(rect.y()) + ", " +
                            QString::number(rect.width()) + ", " +
                            QString::number(rect.height()) + ")")
                    .toStdString();
                }), "to_string" },

                { fun(&QRectF::bottom), "bottom" },
                { fun(&QRectF::bottomLeft), "bottomLeft" },
                { fun(&QRectF::bottomRight), "bottomRight" },
                { fun(&QRectF::center), "center" },
                { fun(&QRectF::height), "height" },
                { fun(&QRectF::intersected), "intersected" },
                { fun(&QRectF::intersects), "intersects" },
                { fun(&QRectF::isEmpty), "isEmpty" },
                { fun(&QRectF::isNull), "isNull" },
                { fun(&QRectF::isValid), "isValid" },
                { fun(&QRectF::left), "left" },
                { fun(&QRectF::moveBottom), "moveBottom" },
                { fun(&QRectF::moveBottomLeft), "moveBottomLeft" },
                { fun(&QRectF::moveBottomRight), "moveBottomRight" },
                { fun(&QRectF::moveCenter), "moveCenter" },
                { fun(&QRectF::moveLeft), "moveLeft" },
                { fun(&QRectF::moveRight), "moveRight" },
                { fun(&QRectF::moveTop), "moveTop" },
                { fun(&QRectF::moveTopLeft), "moveTopLeft" },
                { fun(&QRectF::moveTopRight), "moveTopRight" },
                { fun(&QRectF::normalized), "normalized" },
                { fun(&QRectF::right), "right" },
                { fun(&QRectF::setBottom), "setBottom" },
                { fun(&QRectF::setBottomLeft), "setBottomLeft" },
                { fun(&QRectF::setBottomRight), "setBottomRight" },
                { fun(&QRectF::setCoords), "setCoords" },
                { fun(&QRectF::setLeft), "setLeft" },
                { fun(&QRectF::setRect), "setRect" },
                { fun(&QRectF::setRight), "setRight" },
                { fun(&QRectF::setSize), "setSize" },
                { fun(&QRectF::setTop), "setTop" },
                { fun(&QRectF::setTopLeft), "setTopLeft" },
                { fun(&QRectF::setTopRight), "setTopRight" },
                { fun(&QRectF::setWidth), "setWidth" },
                { fun(&QRectF::setX), "setX" },
                { fun(&QRectF::setY), "setY" },
                { fun(&QRectF::size), "size" },
                { fun(&QRectF::toAlignedRect), "toAlignedRect" },
                { fun(&QRectF::toRect), "toRect" },
                { fun(&QRectF::top), "top" },
                { fun(&QRectF::topLeft), "topLeft" },
                { fun(&QRectF::topRight), "topRight" },
                { fun(&QRectF::united), "united" },
                { fun(&QRectF::width), "width" },
                { fun(&QRectF::x), "x" },
                { fun(&QRectF::y), "y" },
            }
        );

        // Operators
        m->add(fun([](const QRect &r1, const QRect &r2) { return r1 != r2; }), "!=");
        m->add(fun([](const QRect &r1, const QRect &r2) { return r1 == r2; }), "==");
    }
    
    
    void bindQColor(ChaiScript &chai, ModulePtr &m) {
        add_class<QColor>(*m,
            "QColor",
            {
                constructor<QColor()>(),
                constructor<QColor(int, int, int, int)>(),
                constructor<QColor(const QString &)>(),
                constructor<QColor(const QColor &)>(),
            },
            {
                { fun([](const QColor &clr) {
                    return clr.name().toStdString();
                }), "to_string" },

                { fun(&QColor::alpha), "alpha" },
                { fun(&QColor::alphaF), "alphaF" },
                { fun(&QColor::blue), "blue" },
                { fun(&QColor::blueF), "blueF" },
                { fun(&QColor::green), "green" },
                { fun(&QColor::greenF), "greenF" },
                { fun(&QColor::red), "red" },
                { fun(&QColor::redF), "redF" },
                { fun(&QColor::setAlpha), "setAlpha" },
                { fun(&QColor::setAlphaF), "setAlphaF" },
                { fun(&QColor::setBlue), "setBlue" },
                { fun(&QColor::setBlueF), "setBlueF" },
                { fun(&QColor::setGreen), "setGreen" },
                { fun(&QColor::setGreenF), "setGreenF" },
                { fun(&QColor::setRed), "setRed" },
                { fun(&QColor::setRedF), "setRedF" },
                { fun(&QColor::isValid), "isValid" },


                { fun(&QColor::operator!=), "!=" },
                { fun([](QColor &clr, const QColor &other) {
                    if (clr != other) {
                        clr = other;
                    }
                    return clr;
                }), "=" },
                { fun(&QColor::operator==), "==" },
            }
        );
    }
    
    
    void bindQVariant(ChaiScript &chai, ModulePtr &m) {
        add_class<QVariant>(*m,
            "QVariant",
            {
                constructor<QVariant()>(),
                constructor<QVariant(int)>(),
            },
            {
                { fun([](const QVariant &var) {
                    return string("QVariant");
                }), "to_string" },

                { fun(&QVariant::clear), "clear" },
                { fun(&QVariant::isNull), "isNull" },
                { fun(&QVariant::isValid), "isValid" },

                { fun([](const QVariant &var) { return var.toInt(); }), "toInt" },
            }
        );
    }
};

