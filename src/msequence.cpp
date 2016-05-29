#include "msequence.h"


/*!
    Constructs a MSequence without looping.
*/
MSequence::MSequence(QString name_) :
    name(name_)
{
}


/*!
    Copy constructor.  Does a deep one.
*/
MSequence::MSequence(const MSequence &s) :
    name(s.name),
    looping(s.looping),
    __frames(s.__frames),
    __duration(s.__duration)
{
}


/*!
    Adds \a frame to the sequence (at the end) with a delay of \a ms milliseconds
*/
void MSequence::append(MFrame &frame, quint64 ms) {
    __frames.append(DelayedFrame(frame, ms));
    __duration += ms;
}


/*!
    Returns the frame at the index \i.
    This will cause an error if \i is out of the range of the sequence
*/
MFrame MSequence::atIndex(int i) {
    __frames[i].frame;
}


/*!
    Returns the frame at the time \ms in milliseconds.

    If looping=false, then this will clamp the time boundaires. (would either return the first or last frame)
    If looping=true, then this will return the MFrame at the looped time.  (could return any frame)

    This function could cause some sort of problem if there are no frames in the squence but somthing tries to
    be accessed
*/
MFrame MSequence::atTime(quint64 ms) {
    if (looping) {
        // Get a modulo'd time
        ms %= __duration;
    } else {
        // Bounds possibility
        if (ms < 0)
            return __frames.first().frame;
        else if (ms >= __duration)
            return __frames.last().frame;
    }

    // Iterate and count the time
    MFrame *frame = NULL;
    quint64 t = 0;
    for (auto iter = __frames.begin(); iter != __frames.end(); iter++) {
        t += iter->delay;
        if (ms < t) {
            frame = &(iter->frame);
            break;
        }
    }

    return *frame;
}


/*!
    Simple accessor to get the MFrames along with timing informaiton
*/
QVector<MSequence::DelayedFrame> MSequence::frames() {
    return __frames;
}


/*!
    Reports the number of frames in the sequence
*/
int MSequence::numFrames() {
    return __frames.size();
}


/*!
    Reports the length, as in time, of the sequence in milliseconds
*/
quint64 MSequence::duration() {
    return __duration;
}


/*!
    Gets a string representation of the Seuqnce.  set \a frameInfo to true if you
    want it to print frame info as well (default is false)
*/
QString MSequence::str(bool frameInfo) {
    // Simple info
    QString s = name;

    if (looping)
        s += "[L]";

    s += " ";
    s += QString::number(numFrames()) + "f=";
    s += QString::number(__duration) + "ms";

    // They want frame info?  give them frame info
    if (frameInfo) {
        s += ": [";
        auto iter = __frames.begin();
        for (iter; iter != __frames.end(); iter++) {
            s += iter->frame.name + ":" + QString::number(iter->delay) + ", ";
        }
        s.chop(2);
        s += "]";
    }

    return s;
}

