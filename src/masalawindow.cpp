#include <QDebug>
#include "masalawindow.h"
#include "masalaapp.h"
#include "mupdatethread.h"
#include "mglwidget.h"
#include "chaiscript/chaiscript.hpp"
#include <QTimer>
#include <QKeyEvent>


MasalaWindow::MasalaWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Masala Game");
    setFixedSize(mApp->windowSize());

    // Create the open gl widget
    __glWidget = new MGLWidget(size(), this);
    setCentralWidget(__glWidget);
    mApp->gl = __glWidget;
    mApp->chai().add_global(chaiscript::var(__glWidget), "gl");
    mApp->chai().add_global(chaiscript::var(&__keyboard), "Keyboard");

    QTimer *windowTitleUpdatetimer = new QTimer(this);
//    connect(windowTitleUpdatetimer, &QTimer::timeout, this, &MasalaWindow::__updateTitle);    // See `NOTE` below
    windowTitleUpdatetimer->setInterval(200);                // 1/5th of a second
    windowTitleUpdatetimer->start();
}


MasalaWindow::~MasalaWindow()
{
}


/*!
    Periodically polls the OpenGL Widget for an FPS reading.  This is called
    via some sort of QTimer signal.

    NOTE this function is kind of defunt/uncessary now.  it was here for
    dubugging purposes in the earlier days.
*/
void MasalaWindow::__updateTitle() {
    // NOTE this was here for measuing logic updates per second and the render FPS,
    //      uncomment if you want to see it
//    setWindowTitle(
//        QString("Masala [%1][%2]")
//            .arg(__glWidget->measuredFPS(), 0, 'f', 1)
//            .arg(mApp->measuredUPS(), 0, 'f', 1)
//    );
}


/*!
    When the X box is clicked this will shutdown the app
*/
void MasalaWindow::closeEvent(QCloseEvent *event) {
    mApp->shutdown();
    QMainWindow::closeEvent(event);
}


void MasalaWindow::keyPressEvent(QKeyEvent *event) {
    __keyboardStateHelper(*event, true);
    QMainWindow::keyPressEvent(event);
}


void MasalaWindow::keyReleaseEvent(QKeyEvent *event) {
    __keyboardStateHelper(*event, false);
    QMainWindow::keyReleaseEvent(event);
}


/*!
    converts a QKeyEvent to a MKeyState
*/
MKeyState MasalaWindow::__keyEventToKeyState(QKeyEvent &e, bool pressed) {
    MKeyState state;
    state.down = pressed;
    state.isAutoRepeat = e.isAutoRepeat();
    state.shift = e.modifiers() & Qt::ShiftModifier;
    state.control = e.modifiers() & Qt::ControlModifier;
    state.alt = e.modifiers() & Qt::AltModifier;

    return state;
}


void MasalaWindow::__keyboardStateHelper(QKeyEvent &e, bool pressed) {
    switch (e.key()) {
        case Qt::Key_0:            __keyboard.zero =         __keyEventToKeyState(e, pressed); break;
        case Qt::Key_1:            __keyboard.one =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_2:            __keyboard.two =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_3:            __keyboard.three =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_4:            __keyboard.four =         __keyEventToKeyState(e, pressed); break;
        case Qt::Key_5:            __keyboard.five =         __keyEventToKeyState(e, pressed); break;
        case Qt::Key_6:            __keyboard.six =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_7:            __keyboard.seven =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_8:            __keyboard.eight =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_9:            __keyboard.nine =         __keyEventToKeyState(e, pressed); break;
        case Qt::Key_A:            __keyboard.a =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_B:            __keyboard.b =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_C:            __keyboard.c =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_D:            __keyboard.d =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_E:            __keyboard.e =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F:            __keyboard.f =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_G:            __keyboard.g =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_H:            __keyboard.h =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_I:            __keyboard.i =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_J:            __keyboard.j =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_K:            __keyboard.k =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_L:            __keyboard.l =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_M:            __keyboard.m =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_N:            __keyboard.n =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_O:            __keyboard.o =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_P:            __keyboard.p =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Q:            __keyboard.q =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_R:            __keyboard.r =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_S:            __keyboard.s =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_T:            __keyboard.t =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_U:            __keyboard.u =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_V:            __keyboard.v =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_W:            __keyboard.w =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_X:            __keyboard.x =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Y:            __keyboard.y =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Z:            __keyboard.z =            __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F1:           __keyboard.F1 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F2:           __keyboard.F2 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F3:           __keyboard.F3 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F4:           __keyboard.F4 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F5:           __keyboard.F5 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F6:           __keyboard.F6 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F7:           __keyboard.F7 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F8:           __keyboard.F8 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F9:           __keyboard.F9 =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F10:          __keyboard.F10 =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F11:          __keyboard.F11 =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_F12:          __keyboard.F12 =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Escape:       __keyboard.escape =       __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Home:         __keyboard.home =         __keyEventToKeyState(e, pressed); break;
        case Qt::Key_End:          __keyboard.end =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Insert:       __keyboard.insert =       __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Delete:       __keyboard.delete_ =      __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Minus:        __keyboard.minus =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Equal:        __keyboard.equal =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Backspace:    __keyboard.backspace =    __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Tab:          __keyboard.tab =          __keyEventToKeyState(e, pressed); break;
        case Qt::Key_BracketLeft:  __keyboard.leftBracket =  __keyEventToKeyState(e, pressed); break;
        case Qt::Key_BracketRight: __keyboard.rightBracket = __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Backslash:    __keyboard.backSlash =    __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Semicolon:    __keyboard.semicolon =    __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Apostrophe:   __keyboard.apostrophe =   __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Return:       __keyboard.enter =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Comma:        __keyboard.comma =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Period:       __keyboard.period =       __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Slash:        __keyboard.forwardSlash = __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Up:           __keyboard.up =           __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Down:         __keyboard.down =         __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Left:         __keyboard.left =         __keyEventToKeyState(e, pressed); break;
        case Qt::Key_Right:        __keyboard.right =        __keyEventToKeyState(e, pressed); break;
        case Qt::Key_PageUp:       __keyboard.pageUp =       __keyEventToKeyState(e, pressed); break;
        case Qt::Key_PageDown:     __keyboard.pageDown =     __keyEventToKeyState(e, pressed); break;
    }
}

