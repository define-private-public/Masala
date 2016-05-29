
// An animated sprite class

// TODO List
// - Default frame (would need to specify it in the .sprite format)

#ifndef M_SPRITE_H
#define M_SPRITE_H


#include <QObject>
#include <string>
#include "mtriangle.h"
#include "mframe.h"
#include "msequence.h"
#include <QHash>
#include <QMutex>
class MTextureStruct;
class QImage;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLBuffer;
using namespace std;


#define M_SPRITE_HIDE_IT_DEFAULT true
#define M_SPRITE_START_FROM_DEFAULT 0


class MSprite : public QObject {
    Q_OBJECT

public:
    // Static data
    static QHash<quint32, MSprite *> sprites;            // Hash of all the sprite with Ids

    // Public state data
    QPoint pos = QPoint(0, 0);                            // Position
    qreal z = 0;                                        // Layer of the sprite in the Z plane    (NOTE does not work, too busy to accutally implement)
    bool autoCenterOrigin = false;                        // Automatically adjust the origin of the sprite to be in the center
    QPointF origin = QPoint(0, 0);                        // Origin point of the sprite's position (0, 0) for top left
    qreal visibility = 1.0;                                // visibily of sprite (should be between 0.0 and 1.0
    qreal angle = 0;                                    // Angle (in degrees) that the sprite is rotated Clock-wise
    bool customRotationOrigin = false;                    // If the user is defining the rotation origin
    QPointF rotationOrigin;                                // Point to rotate the sprite about, default is the center frame
    QSizeF scale = QSizeF(1, 1);                        // Scale
    bool drawOutline = false;                            // Draw an outline for a sprite
    QColor outlineColor = QColor(0xFF, 0x00, 0x00);        // Color for the outline

    MSprite();
    MSprite(QString spriteFileName);
    MSprite(const MSprite &s);
    virtual ~MSprite();

    // Info
    QString str();

    // Static method for MGLWidget's initialization & cleanup
    static bool _initGL();
    static void _cleanGL();

    // Presentation
    void show(QString frameName);
    void hide(bool hideIt=M_SPRITE_HIDE_IT_DEFAULT);
    void play(QString seqName, quint64 startFrom=M_SPRITE_START_FROM_DEFAULT);
    void pause();
    void resume();
    
    // For the engine
    virtual void update(qreal dt);
    virtual void draw();

    // Simple accessors
    quint32 id();
    MFrame curFrame();
    MSequence curSeq();
    quint64 playbackTime();
    bool isHidden();
    bool isPlaying();
    bool isPaused();
    QHash<QString, MFrame> frames();
    QHash<QString, MSequence> sequences();

    // Static accessors
    static quint32 nextId();

    // For textures
    static bool haveTexture(QString filePath);
    static MTextureStruct *getTexture(QString filePath);
    static MTextureStruct *mkTexture(QString filePath, QImage &image);
    static bool allocateGLTexture(MTextureStruct *tex);

    // for (easier) Chaiscript binding
    static inline MSprite *_spriteWithId(quint32 id) { return sprites[id]; }


protected:
    // For sprite file reading
    enum __ParseState { Spritesheet, FrameInfo, SequenceInfo };
    const QString __FrameInfoToken = "frame_info:";
    const QString __SequenceInfoToken = "sequence_info:";
    const QString __LoopingToken = "looping";
    const QString __FrameInfoPattern = "(?<name>\\w+)=(?<width>\\d+)x(?<height>\\d+):(?<x>\\d+),(?<y>\\d+)";
    const QString __SequenceInfoPattern = "(?<name>\\w+)\\[(?<options>(\\w*,*)*)\\]=(?<sequence>((\\w+:\\d+)*,*)*)";
    // Check http://www.regexr.com/ for a good regex testing app, remember to remove those double back slashes

    // Member Created on load
    bool __good = true;
    bool __localGLReady = false;              // For OpenGL stuff, will be true if all OpenGL content is good
    QString __spriteFilePath = "";
    QString __spritesheetFilePath = "";
    QHash<QString, MFrame> __frames;
    QHash<QString, MSequence> __sequences;

    // State data
    bool __isHidden = true;                   // TODO could possibly make this publically visible (don't use a function)
    bool __isPlaying = false;
    bool __isPaused = false;                  // TODO could possibly make this publically visible (don't use a function)
    qreal __playbackTime = 0;                 // Playback time in seconds
    MFrame __curFrame;                        // Which frame is currently being drawn
    MSequence __curSeq;                       // The Seuqnce that is currently being played (if any)
    QString __prevDrawnFrameName = "";        // Used to better optimize drawing 

    // Misc data.
    static quint32 __nextId;                  // Next available Sprite ID
    quint32 __id;                             // Sprite Id

    // OpenGL Stuff
    QOpenGLBuffer *__uvbo;                                    // Texture coordinate buffer object

    // Static data
    static QHash<QString, MTextureStruct *> __textures;        // A hash of all the loaded textures (key == filepath)
    static QMutex __textureLock;                            // Lock for allocating textures
    static QOpenGLShaderProgram *__commonShaderProgram;        // Common shader program for the MSprites
    static QOpenGLShader *__commonVertexShader;                // Common vertex shader for the MSprites
    static QOpenGLShader *__commonFragmentShader;            // Common fragment shader for the MSprites
    static QOpenGLBuffer *__vbo;                            // Vertex buffer object
    // Buffering:==============================
    static const int __numTriangles;                        // See `msprite.cpp` for details of what each value is
    static const int __totalVertices;                        // 
    static const int __compPerVertex;                        // 
    static const int __compPerUV;                            // 

    // For each specific instance
    QOpenGLShaderProgram *__shaderProgram;            // Shader program for this specific sprite
    MTextureStruct *__spritesheet;                    // Texture object of spritesheet
    QVector<GLfloat> __t;                            // UV coordinates
    QHash<QString, QVector<GLfloat>> __uvMap;        // UV MFrame name to UV map

    // Member functions
    void __initLocalGL();
    void __readSpriteFile(QString filePath);
    QString __cleanLine(QString input);
    QVector<GLfloat> __frameToUV(MFrame &frame);
    void __fixOrigin();
    void __fixRotationOrigin();


};


#endif //M_SPRITE_H

