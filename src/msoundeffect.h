// A simple sound effect object
// It mostly is a wrapper around QSoundEffect, but simplier
// This only does .wav files

#ifndef M_SOUND_EFFECT_H
#define M_SOUND_EFFECT_H


#include <QObject>
#include <QSoundEffect>


class MSoundEffect : public QObject {
    Q_OBJECT

public:
    static const int InfiniteLoop;

    MSoundEffect(QString filename);
    MSoundEffect(const MSoundEffect &se);
    ~MSoundEffect();

    // Info
    QString str() const;
    bool isNull() const;
    QString filename() const;

    // From QSoundEffect
    bool isMuted() const;
    bool isPlaying() const;
    int loopCount() const;
    int loopsRemaining() const;
    void setLoopCount(int loopCount);
    void setMuted(bool muted);
    void setVolume(qreal volume);
    qreal volume();


public slots:
    void play();
    void stop();


private:
    // Member data
    QSoundEffect *__effect = NULL;      // The actual sound effect
    QString __filename;                 // Where the sound effect is stored.

};


#endif // M_SOUND_EFFECT_H

