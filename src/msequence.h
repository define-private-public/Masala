// For spritesheet animations, the structure is pretty immutable

#ifndef M_SEQUENCE_H
#define M_SEQUENCE_H


#define M_SEQUENCE_FRAME_INFO_DEFAULT false


#include "mframe.h"
#include <QString>
#include <QVector>


class MSequence {
public:
    // Internal data structor
    struct DelayedFrame {
        DelayedFrame() { };
        DelayedFrame(MFrame &frame_, quint64 delay_) : frame(frame_), delay(delay_) { };
        DelayedFrame(const DelayedFrame &df) : frame(df.frame), delay(df.delay) { };

        QString str() { return frame.name + ": " + QString::number(delay) + "ms"; }

        MFrame frame = MFrame(); 
        quint64 delay = 0;                // In milliseconds
    };

    QString name;
    bool looping = false;

    // Constructor
    MSequence(QString name_="");
    MSequence(const MSequence &s);

    // Methods
    void append(MFrame &frame, quint64 ms);
    // TODO add a remove?

    // Accessors
    MFrame atIndex(int i);
    MFrame atTime(quint64 ms);
    QVector<DelayedFrame> frames();

    // Misc info
    int numFrames();
    quint64 duration();
    QString str(bool frameInfo=M_SEQUENCE_FRAME_INFO_DEFAULT);


private:
    // Member vars
    QVector<DelayedFrame> __frames;
    quint64 __duration = 0;                        // In milliseconds

};


#endif // M_SEQUENCE_H

