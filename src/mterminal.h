// A simple command line termina that polls for input on standard input

#ifndef M_TERMINAL_H
#define M_TERMINAL_H


#include <QThread>
#include <QMutex>


class MTerminal : public QThread {
    Q_OBJECT

public:
    MTerminal(QObject *parent=NULL);
    ~MTerminal();


public slots:
    void end();        // TODO need a better name (some good ones already are taken by QThread)


private:
    // Super bad-person-only private static members
    static void __rlHandler(char *line);
    static bool __rlDone;

    // Life
    bool __running = true;
    QMutex __runningLock;

    void run() override;

};


#endif // M_TERMINAL_H


