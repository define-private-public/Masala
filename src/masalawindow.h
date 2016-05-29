#ifndef MASALA_WINDOW_H
#define MASALA_WINDOW_H

#include <QMainWindow>
#include "mkeyboardstate.h"
class MGLWidget;


class MasalaWindow : public QMainWindow
{
    Q_OBJECT

public:
    MasalaWindow(QWidget *parent = 0);
    ~MasalaWindow();


private slots:
    void __updateTitle();


protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    MGLWidget *__glWidget;
    MKeyboardState __keyboard;

    MKeyState __keyEventToKeyState(QKeyEvent &e, bool pressed);
    void __keyboardStateHelper(QKeyEvent &e, bool pressed);
};

#endif // MASALA_WINDOW_H
