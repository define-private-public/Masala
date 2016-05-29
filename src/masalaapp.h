// QApplication subclass

#ifndef MASALA_APP
#define MASALA_APP


#include <QApplication>
#include <QHash>
#include <QSettings>
#include <QMutex>
#include <QSize>
#include <memory>
class MasalaWindow;
class MTerminal;
class MUpdateThread;
class MGLWidget;
class QMatrix4x4;


// Forward decl
namespace chaiscript {
    class ChaiScript;
    class Module;

    typedef std::shared_ptr<Module> ModulePtr;
};


class MasalaApp : public QApplication {
    Q_OBJECT

public:
    // Public data
    MGLWidget *gl = NULL;
    MasalaWindow *window = NULL;

    MasalaApp(int &argc, char **argv);
    ~MasalaApp();

    static MasalaApp *instance();
    
    // Info
    chaiscript::ChaiScript &chai();
    const QString mediaDirPath();
    const int targetFPS();
    const QMatrix4x4 viewMatrix();
    const QSize windowSize();
    qreal measuredUPS();


public slots:
    void shutdown();                // Requests a shutdown
    void setWindowTitle(QString title);
    void initWindowSize(QSize size);

private slots:
    void __doShutdown();                // Performs the shutdown


protected:
    void customEvent(QEvent *event);


private:
    // Member vars
    MTerminal *__terminal = NULL;
    MUpdateThread *__updateThread = NULL;
    QSettings __settings;
    QSize __size = QSize(640, 480);
    bool __shutdown = false;
    QMutex __shutdownLock;
    chaiscript::ChaiScript *__chai = NULL;

    void __startTerminal();

};


// A macro
#define mApp MasalaApp::instance()


#endif // MASALA_APP

