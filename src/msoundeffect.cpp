#include <QDebug>
#include "msoundeffect.h"
#include "masalaapp.h"
#include <QFileInfo>


// Statics
const int MSoundEffect::InfiniteLoop = QSoundEffect::Infinite;


/*!
    Creates a MSoundEffect object.  \a filename must be a valid path to a .wav
    file.  If the file doesn't exist or isn't readable, then the this will be
    marked as Null.

    \sa isNull()
*/
MSoundEffect::MSoundEffect(QString filename) :
    QObject(NULL),
    __filename(filename)
{
    // Build the path
    QString path = mApp->mediaDirPath() + filename;

    // First check that it's there and that we can read it
    QFileInfo fi(path);
    if (!fi.exists() || !fi.isReadable())
        return;

    // Check that it's a .wav file
    QString ext = filename.right(4).toLower();
    if (ext == ".wav") {
        // It's there and we can read it, load it
        __effect = new QSoundEffect(this);
        __effect->setSource(QUrl::fromLocalFile(path));
    }
}


/*!
    Creats a sound effect from a copy of another one.  If the \a se is a Null
    sound effect than this one will be Null as well.

    \sa isNull()
*/
MSoundEffect::MSoundEffect(const MSoundEffect &se) :
    MSoundEffect(se.__filename)
{
}


/*!
    Cleansup resources
*/
MSoundEffect::~MSoundEffect() {
    // __effect should be cleaned up automatically
}


/*!
    Gets string representation of the sound effect.
*/
QString MSoundEffect::str() const {
    return QString("MSoundEffect (%1)")
        .arg((__effect != NULL) ? __filename : "Null");
}


/*!
    A Sound Effect is considered Null if the file could not be found or a .wav
    file was not specified.
*/
bool MSoundEffect::isNull() const {
    return (__effect == NULL);
}


/*!
    Returns the filename that was used for this sound effect
*/
QString MSoundEffect::filename() const {
    return __filename;
}


/*!
    Checks if a sound is muted or not.  If the Sound Effect is Null, then this
    will return `true`
*/
bool MSoundEffect::isMuted() const {
    if (__effect)
        return __effect->isMuted();
    else
        return true;
}


/*!
    Will return true if a sound effect is playing.  If Null, then this will
    return false.
*/
bool MSoundEffect::isPlaying() const {
    if (__effect)
        return __effect->isPlaying();
    else
        return false;
}


/*!
    Returns the amount of times this sound effect will play if set to play.
    Will return 0 if effect is Null.
*/
int MSoundEffect::loopCount() const {
    if (__effect)
        return __effect->loopCount();
    else
        return 0;
}


/*!
    If an effect has been set to loop and is playing, this will return the
    amount of loops that are left in the playthrough.  If the effect is Null
    then this will return 0.
*/
int MSoundEffect::loopsRemaining() const {
    if (__effect)
        return __effect->loopsRemaining();
    else
        return 0;
}


/*!
    Sets the loop count.  If \a loopCount is 0 or 1, then this effect will play
    once.  If \a loopCount is -2, then it will loop infinitely.  This function
    has no effect if the effect is Null.
*/
void MSoundEffect::setLoopCount(int loopCount) {
    if (__effect)
        __effect->setLoopCount(loopCount);
}


/*
    Set the sound effect to be muted or not.  This will have no effect if the
    sound effect is Null.
*/
void MSoundEffect::setMuted(bool muted) {
    if (__effect)
        __effect->setMuted(muted);
}


/*!
    Sets the volume of the effect.  Range for \a volume is [0.0, 1.0].  Nothing
    will happen is the effect is Null.
*/
void MSoundEffect::setVolume(qreal volume) {
    if (__effect)
        __effect->setVolume(volume);
}


/*!
    Returns the volume.  If the effect is Null, then this will return 0
*/
qreal MSoundEffect::volume() {
    if (__effect)
        return __effect->volume();
    else
        return 0;
}


/*!
    Sets the sound effect to play.  Does nothing if Null
*/
void MSoundEffect::play() {
    if (__effect)
        __effect->play();
}


/*!
    Stops current playback.  Does nothing if Null
*/
void MSoundEffect::stop() {
    if (__effect)
        __effect->stop();
}

