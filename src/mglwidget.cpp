#include <QDebug>
#include "mglwidget.h"
#include "mtexturestruct.h"
#include "masalaapp.h"
#include "msprite.h"
#include "chaiscript/chaiscript.hpp"
#include <QSize>
#include <QList>
#include <QElapsedTimer>
#include <iostream>
using namespace std;


/*!
    Builds our OpenGL widget
*/
MGLWidget::MGLWidget(QSize size, QWidget *parent) :
    QOpenGLWidget(parent),
    __size(size)
{
    // Check that we have a min 16x16 size
    if (__size.width() < 16)
        __size.setWidth(16);
    if (__size.height() < 16)
        __size.setHeight(16);

    mApp->chai().add_global(chaiscript::var(&__spriteBatch), "spriteBatch");
}


MGLWidget::~MGLWidget() {
    makeCurrent();

    // Cleanup MSprite statics
    MSprite::_cleanGL();

    doneCurrent();
}


void MGLWidget::initializeGL() {
    // Setup
    initializeOpenGLFunctions();
    cout << "OpenGL Version: " << format().version().first << "." << format().version().second << endl;

    // Init PSPrite's GL stuff
    MSprite::_initGL();

    // turn on alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Turn on Culling, use Clockwise vertices
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // turn on depth test
//    glEnable(GL_DEPTH_TEST);

    // The arrays we need
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Init the other things
    __fpsStopwatch.start();
}


void MGLWidget::paintGL() {
    // If the game isn't ready, then don't draw
    // Wait while the game isn't ready to start
    bool ready = false;
    try {
        ready = mApp->chai().eval<bool>("GameReady");
    } catch (std::exception &ex) {
        // Quiety catch anything
    }
    if (!ready)
        return;

    // Check if engouh time has lapsed to update the fps var
    if (__fpsStopwatch.elapsed() > __fpsLapMs) {
        // Update the FPS count
            if (__fpsLock.tryLock()) {
                __fps = (qreal)__framesInLap / (__fpsStopwatch.elapsed() / 1000.0);
                __fpsLock.unlock();
            }

        // REset the counter and frame count
        __framesInLap = 0;
        __fpsStopwatch.restart();
    }

    // Clear
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw
    try {
        mApp->chai().eval("draw(spriteBatch);");
        __spriteBatch.drawAll();
        __spriteBatch.clear();
    } catch (std::exception &ex) {
        qDebug() << "Draw Error:" << ex.what();
    }

    glFlush();

    // Frame counting
    __framesInLap += 1;
}


void MGLWidget::resizeGL(int w, int h) {
//    qDebug() << "[MGLWidget::resizeGL()] w=" << w << ", h=" << h;
}

