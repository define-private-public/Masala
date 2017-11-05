#include <QDebug>
#include "masalaapp.h"
#include "masalawindow.h"
#include "meventtypes.h"
#include "mupdatethread.h"
#include "mterminal.h"
#include "mchaiqt.h"
#include "mchai.h"
#include "chaiscript/chaiscript.hpp"
#include "chaiscript/utility/utility.hpp"
#include "chaiscript/chaiscript_stdlib.hpp"
#include <QMatrix4x4>
#include <QTimer>
#include <iostream>
using namespace std;


/*!
    Init the masala app
*/
MasalaApp::MasalaApp(int &argc, char **argv) :
    QApplication(argc, argv),
    __settings("masala.ini", QSettings::IniFormat)
{
    // nab a value
    string scriptDirPath = __settings.value("script_dir_path", "").toString().toStdString();
    bool useTerminal = __settings.value("terminal", false).toBool();

    // Setup the Chaiscript engine
    std::vector<std::string> ev;
    std::vector<std::string> usePaths({scriptDirPath});
    __chai = new chaiscript::ChaiScript(chaiscript::Std_Lib::library(), ev, usePaths);

    // Add stuff to chaiscript
    chaiQt::bindAll(*__chai);
    mChai::bindAll(*__chai);
    chai().add_global(chaiscript::var(false), "GameReady");
    chai().eval(
        "global GameTime = Dynamic_Object();"
        "GameTime.ms = 0;"
        "GameTime.secs = 0.0;"
    );
    chai().add_global(chaiscript::var(this), "mApp");

    // Setup the base scripts
    chai().use("setup.chai");       // Should also initialize the window size
    chai().use("load.chai");
    chai().use("init.chai");
    chai().use("update.chai");
    chai().use("draw.chai");

    // Info
    cout << "Script directory: " << scriptDirPath << endl;
    cout << "Media directory: " << mediaDirPath().toStdString() << endl;
    chai()("print(\"ChaiScript \" + version())");

    // Create the terminal and start the input gathering stuff
    if (useTerminal) {
        __terminal = new MTerminal(this);
        QTimer::singleShot(200, this, &MasalaApp::__startTerminal);
    }

    // Create the update thread
    __updateThread = new MUpdateThread(mApp->targetFPS(), this);
    __updateThread->start();
//    QTimer::singleShot(200, __updateThread, &MUpdateThread::start);
}


/*!
    Destroy the masala applicatoin
*/
MasalaApp::~MasalaApp() {
    delete __chai;
}

/*!
    Override similar to qApp
*/
MasalaApp *MasalaApp::instance() {
    return (MasalaApp *)qApp;
}


/*!
    Returns a reference to the ChaiScript VM
*/
chaiscript::ChaiScript &MasalaApp::chai() {
    return *__chai;
}


/*!
    Returns a string to where the media files for the masala game are stored.
    This could either be relative to the executable or absolute.
*/
const QString MasalaApp::mediaDirPath() {
    return __settings.value("media_dir_path", applicationDirPath()).toString();
}


/*!
    Returns the desired FPS for the game.  Default is 60
*/
const int MasalaApp::targetFPS() {
    return __settings.value("target_fps", 60).toInt();
}


/*!
    Returns the view Matrix that is currently being used.  Most likely
    an orthographical one
*/
const QMatrix4x4 MasalaApp::viewMatrix() {
    QMatrix4x4 matrix;
    qreal w = windowSize().width();
    qreal h = windowSize().height();
    matrix.ortho(0, w, h, 0, -1, 1);        // Traditional 2D screen cooridnates (upside down cartesian)
                                            // Where 1 is closer to the user and -1 is farther away
    return matrix;
}


/*!
    Returns the dimensions of the game window
*/
const QSize MasalaApp::windowSize() {
    return __size;
}


/*!
    Returns how many updates per second were registered
*/
qreal MasalaApp::measuredUPS() {
    return __updateThread->measuredUPS();
}


/*!
    Request a graceful shtudown
*/
void MasalaApp::shutdown() {
    MasalaApp::postEvent(this, new QEvent((QEvent::Type)M_SHUTDOWN_EVENT));
}


/*!
    Changes the window's title
*/
void MasalaApp::setWindowTitle(QString title) {
    window->setWindowTitle(title);
}


/*!
    Changes the logicall window size.  This should be used before actually
    creating the main window, and then never called afterwards.

    \a size the Size you want, should be non-empty
*/
void MasalaApp::initWindowSize(QSize size) {
    if (!size.isEmpty())
        __size = size;
}


/*!
    Do a graceful shutdown
*/
void MasalaApp::__doShutdown() {
    // Check the shtudown flag lock
    __shutdownLock.lock();
    if (__shutdown) {
        __shutdownLock.unlock();
        return;
    }

    // Set the shutdown flag
    __shutdown = true;
    __shutdownLock.unlock();

    // Kill some threads
    __updateThread->end();
    if (__terminal)
        __terminal->end();

    // Wait for some threads
    __updateThread->wait();
    if (__terminal)
        __terminal->wait();


    // Let it handle the rest
    closeAllWindows();
    quit();
}


/*!
    Handle custom events
*/
void MasalaApp::customEvent(QEvent *event) {
    if (event->type() == (QEvent::Type)M_SHUTDOWN_EVENT) {
        // Got a shutdown event, queue up a shutdown
        event->accept();
//        qDebug() << "[MasalaApp::customEvent()] Shutdown Event received";
        QTimer::singleShot(100, this, &MasalaApp::__doShutdown);
    }
}


/*!
    Starts up the terminal thread.  Will only work if `terminal` is set to
    `true` in the .ini file
*/
void MasalaApp::__startTerminal() {
    __terminal->start();
}

