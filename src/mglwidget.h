// Create a opengl context

#ifndef M_GL_WIDGET_H
#define M_GL_WIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMutex>
#include <QElapsedTimer>
#include "mspritebatch.h"
class MTextureStruct;


class MGLWidget : public QOpenGLWidget, public QOpenGLFunctions {
    Q_OBJECT

public:
    MGLWidget(QSize size, QWidget *parent=NULL);
    ~MGLWidget();


    inline qreal measuredFPS() {
        return __fps;
    }



protected:
    // Functions
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);


private:
    QSize __size;

    // For measuring FPS
    qreal __fps = 0.0;
    QMutex __fpsLock;
    const qint64 __fpsLapMs = 0.2 * 1000;            // Every 1/5 of second do a recount
    qint64 __framesInLap = 0;
    QElapsedTimer __fpsStopwatch;
    MSpriteBatch __spriteBatch;
};


#endif // M_GL_WIDGET_H

