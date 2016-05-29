// Where the update logic goes, and draws are queued

#ifndef M_UPDATE_THREAD_H
#define M_UPDATE_THREAD_H


#include <QThread>
#include <QMutex>
class MasalaApp;


class MUpdateThread : public QThread {
    Q_OBJECT

public:
    MUpdateThread(int fps, MasalaApp *parent);
    ~MUpdateThread();


    /*!
        The updates per second
    */
    inline qreal measuredUPS() {
        return __ups;
    }


public slots:
    void end();        // TODO need a better name (some good ones already are taken by QThread)

private:
    // timing
    const int __fps;
    qreal __msPerDraw = 0;

    // Life
    bool __running = true;
    QMutex __runningLock;

    // For measuring UPS
    qreal __ups = 0.0;
    QMutex __upsLock;

    void run() override;
};



#endif //M_UPDATE_THREAD_H
