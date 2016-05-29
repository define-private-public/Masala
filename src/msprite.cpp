#include <QDebug>
#include <cmath>
#include "msprite.h"
#include "mutil.h"
#include "masalaapp.h"
#include "mtexturestruct.h"
#include "mglwidget.h"
#include <QTextStream>
#include <QStringList>
#include <QRegularExpression>
#include <QFile>
#include <QImage>


/*== Statics ==*/
// For Ids
quint32 MSprite::__nextId = 0;
QHash<quint32, MSprite *> MSprite::sprites;

// For textures
QHash<QString, MTextureStruct *> MSprite::__textures;
QMutex MSprite::__textureLock;
QOpenGLShaderProgram *MSprite::__commonShaderProgram = NULL;
QOpenGLShader *MSprite::__commonVertexShader = NULL;
QOpenGLShader *MSprite::__commonFragmentShader = NULL;
QOpenGLBuffer *MSprite::__vbo = NULL;

// for buffering math
const int MSprite::__numTriangles = 2;                    // Two triangles
const int MSprite::__totalVertices = __numTriangles * 3;    // Three vertices per triangel
const int MSprite::__compPerVertex = 2;                    // <x, y, z, w>
const int MSprite::__compPerUV = 2;                        // <x, y>
/*== End Statics ==*/


/*!
    Default empty constructor.  This should never be used, unless sublcassing.
    
    A Texture will not be set at all and drawing will not take place until it
    is setup by something else.
*/
MSprite::MSprite() :
    QObject(NULL),
    __id(__nextId++)
{
    // Allocate some space for the UV map vector/buffer
    __t.reserve(__compPerUV * __totalVertices);

    // Insert the sprite to the global hash
    sprites[__id] = this;
    
    // Hide by default
    hide();
}


/*!
    Creates a new sprite object from the \a spriteFileName .sprite file
    provided.

    Sprite by default start out hidden, and will not be rendered
    unless you make the visible with show() or play().

    If the sprite loaded up fine, then it will be added to the global
    MasalaApp class.
*/
MSprite::MSprite(QString spriteFileName) :
    QObject(NULL),
    __id(__nextId++)
{
    // Allocate some space for the UV map vector/buffer
    __t.reserve(__compPerUV * __totalVertices);

    // Load the sprite info
    __spriteFilePath = mApp->mediaDirPath() + spriteFileName;
    __readSpriteFile(__spriteFilePath);

    // Grab the spritesheet and verify
    __spritesheet = getTexture(__spritesheetFilePath);
    __good &= !__spritesheet->image->isNull();

    // Precompute the UV map
    for (auto iter = __frames.begin(); iter != __frames.end(); iter++)
        __uvMap[iter->name] = __frameToUV(*iter);

    // Pick out a default frame
    show(__frames.begin()->name);

    // Insert it into the global hash
    sprites[__id] = this;

    // Sprites hidden by default
    hide();

    // Info
//    qDebug() << "[MSprite::MSprite() new] " << str() << " good=" << __good;
}


/*!
    Copy Constructor.  This will perform a deep copy of \a s (including its
    current state).  It will have a different id though.

    On copying the UVBO will not be ready nor will __localGLReady be set to true.
    The first call to draw() will fix this.  Same for the __shaderProgram.
*/
MSprite::MSprite(const MSprite &s) :
    QObject(NULL),
    __id(__nextId++),
    __good(s.__good),
    __spriteFilePath(s.__spriteFilePath),
    __spritesheetFilePath(s.__spritesheetFilePath),
    __frames(s.__frames),
    __sequences(s.__sequences),
    __isHidden(s.__isHidden),
    __isPlaying(s.__isPlaying),
    __isPaused(s.__isPaused),
    __playbackTime(s.__playbackTime),
    __curFrame(s.__curFrame),
    __curSeq(s.__curSeq),
    __prevDrawnFrameName(s.__prevDrawnFrameName),
    __spritesheet(s.__spritesheet),                        // Copying pointer, not actual data, should be fine though
    __t(s.__t),
    __uvMap(s.__uvMap)
{
    // the public members
    pos                  = s.pos;
    z                    = s.z;
    autoCenterOrigin     = s.autoCenterOrigin;
    origin               = s.origin;
    visibility           = s.visibility;
    angle                = s.angle;
    customRotationOrigin = s.customRotationOrigin;
    rotationOrigin       = s.rotationOrigin;
    scale                = s.scale;
    drawOutline          = s.drawOutline;
    outlineColor         = s.outlineColor;

    // Insert it into the global hash
    sprites[__id] = this;

//    qDebug() << "[MSprite::MSprite() copy] " << str() << " good=" << __good;
}


/*!
    cleansup resources.  This will remove the Sprite from the
    global MasalaApp hash.
*/
MSprite::~MSprite() {
    // Remove from global hash
    sprites.remove(__id);

    // GL stuff
    if (__localGLReady) {
        delete __uvbo;
    }

//    qDebug() << "[MSprite::~MSprite()]" << str();
}


/*!
    Returns a sprint representation of the sprite
*/
QString MSprite::str() {
    QString posStr = "(" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ")";
    return "Sprite [" + QString::number(__id) + "] " + posStr + " " + __spriteFilePath;
}


/*!
    Static method that is called in MGLWidget::initializeGL().  this creates
    all of the necessary shaders, shader programmes, and buffer objects for
    MSprite.  this should only be called one.  return true on success.
*/
bool MSprite::_initGL() {
    // TODO add in a __glReady flag?

    // Flag
    bool good = true;

    // Create the shader program and add in the shaders
    __commonShaderProgram = new QOpenGLShaderProgram;
    __commonVertexShader = new QOpenGLShader(QOpenGLShader::Vertex);                // Create
    __commonFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    good &= __commonVertexShader->compileSourceFile("sprite.vert");                // Compile
    good &= __commonFragmentShader->compileSourceFile("sprite.frag");
    good &= __commonShaderProgram->addShader(__commonVertexShader);                        // Add
    good &= __commonShaderProgram->addShader(__commonFragmentShader);
    good &= __commonShaderProgram->link();                                        // Link

    // Something bad happened with the shader
    if (!good) {
        qDebug()  << "[MSprite::__initGL()] ShaderProgram log=\n" << __commonShaderProgram->log();
        delete __commonShaderProgram;
        delete __commonVertexShader;
        delete __commonFragmentShader;
        __commonVertexShader = NULL;
        __commonFragmentShader = NULL;
        __commonShaderProgram = NULL;
    }

    // Create the shared buffers
    // Create a square. with s=1, we scale the square in the vertex shader
    QPoint a(0, 0), b(1, 0), c(1, 1), d(0, 1);        // a = top-left, then go clockwise for the rest
    MTriangle tri1(a, b, c);
    MTriangle tri2(a, c, d);

    // Shove it in the buffer
    QVector<GLfloat> v;
    v += tri1.glVector(2);
    v += tri2.glVector(2);

    // Put the vertices into the Vertex Buffer Object (VBO)
    __vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    __vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    good &= __vbo->create();
    good &= __vbo->bind();
    __vbo->allocate(v.constData(), v.count() * sizeof(GLfloat));
    __vbo->release();

    if (!good) {
        qDebug() << "[MSprite::__initGL()] Issue with the buffers";
        delete __vbo;
        __vbo = NULL;
    }

    // Cleanup the shaders
    return good;
}


/*!
    Static method that is called in MGLWidget::~MGLWidget().  This cleans up
    all of the static OpenGL related data in the class.
*/
void MSprite::_cleanGL() {
    // TODO replace with __glReady later on? (see above TODO)

    // Cleanout some statics
    if (__commonShaderProgram)
        delete __commonShaderProgram;
    if (__commonVertexShader)
        delete __commonVertexShader;
    if (__commonFragmentShader)
        delete __commonFragmentShader;

    // Cleanup the textures
    __textureLock.lock();
    for (auto iter = __textures.begin(); iter != __textures.end(); iter++)
        delete *iter;

    // Buffers
    if (__vbo)
        delete __vbo;

    __textureLock.unlock();
}


/*!
    Show a specific named frame of the animated sprite.  This will stop
    playing the sprite if it's already playing.  If \a frameName is not
    found then nothing will happen.
*/
void MSprite::show(QString frameName) {
//    qDebug() << "[MSprite::show()] frameName=" << frameName;

    // Make sure the frame is there
    if (!__frames.contains(frameName))
        return;
    else if (!__isPlaying && (__curFrame.name == frameName) && !__isHidden)        // If we are not playing, make sure that we aren't already showing it
        return;

    // Switch frames over and do some state resetting
    __isHidden = false;
    __isPlaying = false;
    __isPaused = false;
    __playbackTime = 0;
    __curSeq = MSequence();                    // No sequence
    __curFrame = __frames[frameName];

    __fixOrigin();
    __fixRotationOrigin();
}


/*!
    Used to mark if the sprite should be hidden.  While hidden, the sprite
    can still have it's playback logic updated, but nothing will be drawn.

    If show() or play() are called while the sprite is hidden, it will then
    unhide the sprite.

    You can pass false to \a hideIt to unhide the sprite
*/
void MSprite::hide(bool hideIt) {
    __isHidden = hideIt;
}


/*!
    Will play the sequence \a seqName from the starting point \a
    startFrom (in milliseconds).

    If something else is already playing, then this will change the playback
    state.

    If \a seqName is not found, then nothing will change.
*/
void MSprite::play(QString seqName, quint64 startFrom) {
    // Check for sequence names
    if (!__sequences.contains(seqName))
        return;
    
    // Reset the playback state
    __isHidden = false;
    __isPlaying = true;
    __isPaused = false;
    __playbackTime = startFrom / 1000.0;        // From milliseconds to seconds
    __curSeq = __sequences[seqName];
    __curFrame = __curSeq.atTime(startFrom);

    __fixOrigin();
    __fixRotationOrigin();
}


/*!
    Pauses sprite playback.  This funciton has no effect if playback
    is not happening or if it's already paused.
*/
void MSprite::pause() {
    if (__isPlaying)
        __isPaused = true;
}


/*!
    Resumes the sprite playback.  This function has no effect if
    playback is not happeing, or if it's not paused already.
*/
void MSprite::resume() {
    if (__isPlaying)
        __isPaused = false;
}


/*!
    This is a method meant to be called in an update loop.  \a dt
    is the amount of time (in seconds) that has passed since the last update.
    This function will update the animation logic of a Sprite.

    If the sprite is paused, then this function will do nothing.
*/
void MSprite::update(qreal dt) {
    // Do nothing if there is a static frame, or if we're playing, but paused
    if (!__isPlaying || __isPaused)
        return;
    
    bool looping = __curSeq.looping;
    qreal durSec = __curSeq.duration() / 1000.0;

    // Add to the playback time
    __playbackTime += dt;
    if (!looping && (__playbackTime >= durSec))
        __playbackTime = durSec;

    // Some data
    qreal ms = 1000 * __playbackTime;

    // Put playback time in the range
    if (looping && (__playbackTime >= durSec))
        __playbackTime = fmod(__playbackTime, durSec);

    // Pick out a frame
    __curFrame = __curSeq.atTime(ms);
    __fixOrigin();
    __fixRotationOrigin();
}


/*!
    Does all of the OpenGL rendering stuff.
    This should only be called during an OpenGL rendering phase.

    No drawing will take place unless the OpenGL content has been initialized.
    If everything goes fine (__localGLReady == true, and __good == true),
    then drawing will work.

    Nothing will be drawn if the hidden flag is set, or visibility == 0
*/
void MSprite::draw() {
    // First check to make sure that the OpenGL content is ready
    if (!__localGLReady) {
        __initLocalGL();
    }

    // Don't draw hidden sprites, or if GL stuff isn't ready (or __good is false)
    if (!__good || __isHidden || (visibility < 0))
        return;

    // Setup the buffer objects
    // Texture/UV coodinates
    if (__curFrame.name != __prevDrawnFrameName) {
        // Place in new UV coordinates to the Buffer 
        __t = __uvMap[__curFrame.name];
        __uvbo->bind();
        __uvbo->write(0, __t.constData(), __t.count() * sizeof(GLfloat));
        __uvbo->release();

        // Set the frame that was last drawn
        __prevDrawnFrameName = __curFrame.name;
    }

    // Setup
    __shaderProgram->bind();
    __shaderProgram->enableAttributeArray("vertex");
    __shaderProgram->enableAttributeArray("tex");

    // Set values
    QMatrix4x4 rotMatrix;
    rotMatrix.rotate(angle, 0, 0, -1);                                        // Rotate clockwise
    __shaderProgram->setUniformValue("viewMatrix", mApp->viewMatrix());
    __shaderProgram->setUniformValue("rotMatrix", rotMatrix);
    __shaderProgram->setUniformValue("pos", pos);
    __shaderProgram->setUniformValue("z", (GLfloat)z);
    __shaderProgram->setUniformValue("origin", origin);
    __shaderProgram->setUniformValue("rotationOrigin", rotationOrigin);
    QSizeF shaderScale = __curFrame.sizeF();
    shaderScale.rwidth() *= scale.width();
    shaderScale.rheight() *= scale.height();
    __shaderProgram->setUniformValue("scale", shaderScale);
    __shaderProgram->setUniformValue("visibility", (GLfloat)visibility);

    __vbo->bind();
    __shaderProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, __compPerVertex);
    __vbo->release();

    __uvbo->bind();
    __shaderProgram->setAttributeBuffer("tex", GL_FLOAT, 0, __compPerUV);
    __uvbo->release();

    __spritesheet->glTexture->bind();
    __shaderProgram->setUniformValue("spritesheet", GL_TEXTURE0);

    // Check for draw outline
    if (drawOutline) {
        __shaderProgram->setUniformValue("drawOutline", true);
        __shaderProgram->setUniformValue("outlineColor", outlineColor);
        mApp->gl->glDrawArrays(GL_LINE_LOOP, 0, __totalVertices);
    }

    // Do the draw
    __shaderProgram->setUniformValue("drawOutline", false);
    mApp->gl->glDrawArrays(GL_TRIANGLES, 0, __totalVertices);

    // Teardown
    __spritesheet->glTexture->release();
    __shaderProgram->disableAttributeArray("vertex");
    __shaderProgram->disableAttributeArray("tex");
    __shaderProgram->release();
}


/*!
    Returns the unique Id that has been assigned to this sprite
*/
quint32 MSprite::id() {
    return __id;
}


/*!
    Returns the current Frame that is either being statically shown
    or is playing.
*/
MFrame MSprite::curFrame() {
    return __curFrame;
}


/*!
    Returns the current sequence that is being played.  Note, if
    something is not being played (e.g. a static Frame is being showwn),
    this will return the last played sequence then.
*/
MSequence MSprite::curSeq() {
    return __curSeq;
}


/*!
    Returns the curren time (in milliseconds) that have gone by
    since play() was called.  This will return 0 if a static Frame
    is being shown.

    If a looped sequence is playing, this will return where it is in
    the loop.

    In a non-looped playback, this may return the entire duration of the
    sequence if the sequence is done playing.
*/
quint64 MSprite::playbackTime() {
    return __playbackTime * 1000;
}


/*!
    Returns if this sprite has been marked to be hidden
*/
bool MSprite::isHidden() {
    return __isHidden;
}


/*!
    Returns if Sprite is having an animation played or not.  If a sprite
    is just statically showing a frame, this will return false.  If an
    animation is being played, regardless if the animation is currently
    paused, this will return true.

    \sa isPaused()
*/
bool MSprite::isPlaying() {
    return __isPlaying;
}


/*!
    Returns true if an animation is playing, but has been paused.  If a frame
    is being statically show, then this will return false.

    \sa isPlaying()
*/
bool MSprite::isPaused() {
    return __isPaused;
}


/*!
    Returns a lsit of frames that are part of the sprite
*/
QHash<QString, MFrame> MSprite::frames() {
    return __frames;
}


/*!
    Returns the sequences that are part of this sprite
*/
QHash<QString, MSequence> MSprite::sequences() {
    return __sequences;
}


/*!
    Returns the Id that the next created sprite will have.
*/
quint32 MSprite::nextId() {
    return __nextId;
}


/*!
    Checks to see if we have the texture or not for \a filePath.  This function
    is thread safe.
*/
bool MSprite::haveTexture(QString filePath) {
    __textureLock.lock();
    bool val = __textures.contains(filePath);
    __textureLock.unlock();
    return val;
}


/*!
    Retrives the texture for \a filePath.  If we do not have it, then it will be
    made.  this funciton is threadsafe.  Please note that OpenGL texture will not
    be made until a draw() call.

    \sa mkTexture()
*/
MTextureStruct *MSprite::getTexture(QString filePath) {
    __textureLock.lock();

    // I we have it, then get it, else make it
    MTextureStruct *tex = NULL;
    bool has = __textures.contains(filePath);
    if (has) {
        // Got it!
        tex = __textures[filePath];
    } else {
        // Need to make it
        tex = new MTextureStruct(filePath, new QImage(filePath), NULL);
        __textures[filePath] = tex;
    }
    
    __textureLock.unlock();
    return tex;
}


/*!
    Similar to mkTexture(), but instead, this function will allow ther programmer
    to set a QImage for the texture image data instead of feading it from \a filePath.
    \a filePath is still used to Id the texture, but it's okay if it isn't a well
    known image format.  Please note that the OpenGL part of the texture will not be
    intialized until a draw() call has been made.

    If a texture already exists for \a filePath, then another texture is not made.

    \a image will be deep copied.

    \sa getTexture()
*/
MTextureStruct *MSprite::mkTexture(QString filePath, QImage &image) {
    __textureLock.lock();

    // I we have it, then get it, else make it
    MTextureStruct *tex = NULL;
    bool has = __textures.contains(filePath);
    if (has) {
        // Got it!
        tex = __textures[filePath];
    } else {
        // Need to make it
        tex = new MTextureStruct(filePath, new QImage(image), NULL);
        __textures[filePath] = tex;
    }
    
    __textureLock.unlock();
    return tex;
}


/*!
    For allocating the OpenGL texture.  This function is threadsafe

    Returns true on success
*/
bool MSprite::allocateGLTexture(MTextureStruct *tex) {
    __textureLock.lock();
    bool good = true;

    tex->glTexture = new QOpenGLTexture(*(tex->image));
    tex->glTexture->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    good &= tex->glTexture->isCreated();

    __textureLock.unlock();
    return good;
}


/*!
    Initializes all of the shaders, textures, and buffers for us.  Should only be called
    Once, and during the constructor.

    This will set the MSprite::__localGLReady flag to true regardless if initializing the OpenGL
    content was successful or not.  the __good flag will be fase if there was an issue
*/
void MSprite::__initLocalGL() {
    // If OpenGL stuff has been initliazed, don't do this function again
    if (__localGLReady)
        return;

    // Flag
    bool good = true;

    // Shader program
    __shaderProgram = __commonShaderProgram;
    good &= (__shaderProgram != NULL);

    // Verify texture
    good &= allocateGLTexture(__spritesheet);

    __uvbo = new QOpenGLBuffer;
    __uvbo->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    good &= __uvbo->create();
    good &= __uvbo->bind();
    __uvbo->allocate(__totalVertices * __compPerUV * sizeof(GLfloat));
    __uvbo->release();

    // Set the __localGLReady flag to true
    __localGLReady = true;
    __good &= good;
}


/*!
    Reads the data from the .sprite file (provided) and will set
    some of the member info from it. \a filePath should be an
    existing file, if it isn't, then the __good variable will
    be set to `false`.
*/
void MSprite::__readSpriteFile(QString filePath) {
    // Make sure that we can open the file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "[MSprite::__readSpriteFile()] error, could not open " << filePath << " for reading.";
        __good = false;
        return;
    }
    
    // Get the relative path of the .sprite file
    QString dirPath = mUtil::getDirPath(filePath);

    // Creat the regex matches
    QRegularExpression reFI(__FrameInfoPattern);
    QRegularExpression reSI(__SequenceInfoPattern);
    QRegularExpressionMatch match;

    // Start reading
    __ParseState readingState = __ParseState::Spritesheet;
    QTextStream in(&file);
    QString line;
    int lineNum = 0;
    while (!in.atEnd()) {
        // Only process a line that isn't empty (and chop off the comments)
        line = __cleanLine(in.readLine());
        lineNum++;

        // First check for state switching
        if (line == __FrameInfoToken) {
            // Swtich to Frame Info state
            readingState = FrameInfo;
        } else if (line == __SequenceInfoToken) {
            readingState = SequenceInfo;
        } else if (!line.isEmpty()) {
            // Do a state specific command
            switch (readingState) {
                case Spritesheet:
                    // First non-empty line should be the spritesheet's location in the filesystem
                    __spritesheetFilePath = dirPath + QDir::separator() + line;
                    break;

                case FrameInfo:
                    // Reading in Frame Info
                    match = reFI.match(line);
                    if (match.hasMatch()) {
                        // Get arguments
                        MFrame frame;
                        frame.name = match.captured("name");
                        frame.w = match.captured("width").toInt();
                        frame.h = match.captured("height").toInt();
                        frame.x = match.captured("x").toInt();
                        frame.y = match.captured("y").toInt();

                        // Store the frame
                        if (!__frames.contains(frame.name))
                            __frames[frame.name] = frame;
                    } else {
                        // Bad form
                        qDebug() << "[MSprite::__readSpriteFile()] error line number " << lineNum << " bad in file " << filePath;
                    }
                    break;

                case SequenceInfo:
                    // Reading in the sequence info
                    match = reSI.match(line);
                    if (match.hasMatch()) {
                        // Get arguments
                        QString name = match.captured("name");
                        QStringList options = match.captured("options").split(",");
                        QStringList sequence = match.captured("sequence").split(",");

                        // Create the sequence structure
                        MSequence seq(name);
                        seq.looping = options.contains(__LoopingToken);

                        // Loop through the sequence data to create the animation
                        for (auto iter = sequence.begin(); iter != sequence.end(); iter++) {
                            QStringList parts = iter->split(":");
                            QString frameName = parts[0];
                            int delay = parts[1].toInt();

                            seq.append(__frames[frameName], delay);
                        }
                    
                        if (!__sequences.contains(name))
                            __sequences[name] = seq;
                    } else {
                        // Bad form
                        qDebug() << "[MSprite::__readSpriteFile()] error line number " << lineNum << " bad in file " << filePath;
                    }
                    break;
            }
        }
    }

    // cleanup
    file.close();
}


/*!
    Will clean a line of \a input from all whitespace on the ends and removes
    comments.  May return an empty string.
*/
QString MSprite::__cleanLine(QString input) {
    int i = input.indexOf("//");
    if (i != -1)
        input.truncate(i);
    return input.trimmed();
}


/*!
    Converts the data from a MFrame object with the loaded spritesheet
    to figure out where the UV coordinates for that frame are.
*/
QVector<GLfloat> MSprite::__frameToUV(MFrame &frame) {
    // Get some data first
    QSize size = __spritesheet->image->size();
    qreal sw = size.width();
    qreal sh = size.height();

    // Should all be convertec to float points
    qreal x = frame.x / sw;
    qreal y = frame.y / sh;
    qreal fw = frame.w / sw;
    qreal fh = frame.h / sh;
//    qDebug() << x << y << fw << fw;

    // Points start in the top left, and go clockwise
    QPointF a(x, y),
            b(x + fw, y),
            c(x + fw, y + fh),
            d(x, y + fh);
    MTriangle tri1(a, b, c);
    MTriangle tri2(a, c, d);

    QVector<GLfloat> v;
    v += tri1.glVector(2);
    v += tri2.glVector(2);
    return v;
}


/*!
    This is called in the play(), show(), and update() methods.  If
    `autoCenterOrigin` is set to `true` (the default is `false`) then
    this function will set the `origin` field to be in the center of
    `curFrame` for the sprite.

    This function will do nothing is `autoCenterOrigin` is false.
*/
void MSprite::__fixOrigin() {
    // Pick out the center as the origin
    if (autoCenterOrigin) {
        QSizeF scaledRect = __curFrame.toRectF().size();
        scaledRect.rwidth() *= scale.width();
        scaledRect.rheight() *= scale.height();
        origin = QRectF(QPointF(0, 0), scaledRect).center();
    }
}


/*!
    This is called in the play(), show(), and update() functions.  If
    customRotationOrigin is set to `false` (which is the default), this
    will adjust the rotation origin to be the center of the frame.
*/
void MSprite::__fixRotationOrigin() {
    // Pick out the center as the rotation origin
    if (!customRotationOrigin) {
        QSizeF scaledRect = __curFrame.toRectF().size();
        scaledRect.rwidth() *= scale.width();
        scaledRect.rheight() *= scale.height();
        rotationOrigin = QRectF(QPointF(0, 0), scaledRect).center();
    }
}
