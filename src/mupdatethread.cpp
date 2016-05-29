#include <QDebug>
#include "mupdatethread.h"
#include "masalaapp.h"
#include "mglwidget.h"
#include "masalawindow.h"
#include "chaiscript/chaiscript.hpp"
#include <QElapsedTimer>


MUpdateThread::MUpdateThread(int fps, MasalaApp *parent) :
    __fps(fps),
    QThread(parent)
{
    // Load some data
    __msPerDraw = 1000.0 / __fps;
}


MUpdateThread::~MUpdateThread() {
}


/*!
    Notifies the update thread to end execution of the game loop
*/
void MUpdateThread::end() {
    __runningLock.lock();
    __running = false;
    __runningLock.unlock();
}


/*!
    Contains the update logic for the game
*/
void MUpdateThread::run() {
    bool running = __running;

    // Wait while the game isn't ready to start
    bool ready = false;
    while (!ready && running) {
        try {
            ready = mApp->chai().eval<bool>("GameReady");
        } catch (std::exception &ex) {
            // Quiety catch anything
        }

        // Check for quit
        if (__runningLock.tryLock()) {
            running = __running;
            __runningLock.unlock();
        }

        msleep(10);
    }

    // The real meat of the thread
    qint64 msSinceLastUpdate = 0;
    qint64 msAtLastUpdate = 0;
    qint64 curMS = 0;
    qreal dt = 0;                            // Time (in seconds) since last update
    qreal msSinceLastDraw = 0;                // expressed in double format instead of qint64

    // For metrics
    const qint64 upsLapMs = 0.20 * 1000;    // Every 1/5 seconds
    QElapsedTimer upsStopwatch;
    qint64 updatesInLap = 0;

    // The main game loop
    QElapsedTimer timer;
    timer.start();
    upsStopwatch.start();
    while (running) {
        // For checking the updates
        if (upsStopwatch.elapsed() > upsLapMs) {
            // Set the variable
            if (__upsLock.tryLock()) {
                __ups = (qreal)updatesInLap / (upsStopwatch.elapsed() / 1000.0);
                __upsLock.unlock();
            }

            // Reset
            updatesInLap = 0;
            upsStopwatch.restart();
        }

        // Get the elapsed time since the last logic update
        curMS = timer.elapsed();
        msSinceLastUpdate = curMS - msAtLastUpdate;
        msAtLastUpdate = curMS;

        // Always update
        dt = msSinceLastUpdate / 1000.0;
        try {
            mApp->chai().eval("update(" + std::to_string(dt) + ");");
            mApp->chai().eval(
                "GameTime.ms = " + std::to_string(curMS) + ";" +
                "GameTime.secs = " + std::to_string(curMS / 1000.0) + ";"
            );
        } catch (std::exception &ex) {
            qDebug() << "[MUpdateThread::run()] Update Error:" << ex.what();
        }

        // Has there been enough time to re-draw?
        msSinceLastDraw += msSinceLastUpdate;
        if (msSinceLastDraw > __msPerDraw) {
            // do a draw
            mApp->gl->update();
            msSinceLastDraw -= __msPerDraw;
        }

        // Check for quit
        if (__runningLock.tryLock()) {
            running = __running;
            __runningLock.unlock();
        }

        // Increment the update count a sleep for a short while
        updatesInLap += 1;
        msleep(1);
    }
}


