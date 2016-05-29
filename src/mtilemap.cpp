#include "mtilemap.h"
#include "mtexturestruct.h"
#include "masalaapp.h"
#include "mutil.h"
#include <QPainter>


const QString MTileMap::FrameName = "layer1";


/*!
    Constructs a Tilemap with a given \flareMapFilename.  This class currently
    only supports one layer and one tileset.

    sa __readFlareMapFile()
*/
MTileMap::MTileMap(QString flareMapFilename) :
    MSprite()
{
    // Read in the sprite file
    __spriteFilePath = mApp->mediaDirPath() + flareMapFilename;
    __readFlareMapFile(__spriteFilePath);

    // Load up the tileset as a texture
    __tileset = getTexture(__tilesetFilePath);
    __good &= !__tileset->image->isNull();

    // Create the map image
    QImage *map = __mkMapImage();
    if (map != NULL) {
        __spritesheet = mkTexture(__spriteFilePath, *map);
        __good &= !__spritesheet->image->isNull();
        delete map;
    } else 
        __good = false;

    // Only have one frame
    MFrame layer1(0, 0, __tileWidth * __mapWidth,  __tileHeight * __mapHeight, FrameName);
    __frames[FrameName] = layer1;
    __uvMap[FrameName] = __frameToUV(layer1);
}


MTileMap::~MTileMap() {
}


bool MTileMap::isGood() {
    return __good;
}


int MTileMap::tileWidth() {
    return __tileWidth;
}


int MTileMap::tileHeight() {
    return __tileHeight;
}


int MTileMap::mapWidth() {
    return __mapWidth;
}


int MTileMap::mapHeight() {
    return __mapHeight;
}


QVector<int> MTileMap::mapData() {
    return __mapData;
}


/*!
    Updates the logic of the sprite.  \a dt is unused.  Still call this funciton
    in the engine's update() function.
*/
void MTileMap::update(qreal dt) {
    // Just check the origin and the rotation
    __fixOrigin();
    __fixRotationOrigin();
}


/*!
    Returns this instance of a MTileMap as a pointer to a MSprite.  This is
    necessary so that ChaiScript can use a MTileMap as if it were a MSprite.
*/
MSprite *MTileMap::asSprite() {
    return (MSprite *)this;
}


/*!
    Reads in the data from a Flare Map file.  \a filePath must be relative to
    the current `mediaDirPath`.  The `tileset` field inside of the flare map
    file must also be relative to the location of the map file.

    Will set the internal `__good` flag if all goes well.

    \sa isGod()
*/
void MTileMap::__readFlareMapFile(QString filePath) {
    QString prefix = "[MTileMap::__readFlareMapFile()]";

    // Make sure that we can open the file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << prefix << " error, could not open " << filePath << " for reading.";
        __good = false;
        return;
    }

    QString dirPath = mUtil::getDirPath(filePath);
    __spritesheetFilePath = filePath;

    // Start reading
    QStringList pair, data;
    __ParseState readingState = __ParseState::GeneralInfo;
    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = __cleanLine(in.readLine());

        switch (readingState) {
            case __ParseState::GeneralInfo:
                // Look for info about the tiles
                // Split based on key-value pairs.
                pair = line.split("=");
                if (pair.size() == 2) {
                    // got a key-value pair
                    QString key = pair[0];
                    QString value = pair[1];

                    if (key == __WidthToken)
                        __mapWidth = value.toInt();
                    else if (key == __HeightToken)
                        __mapHeight = value.toInt();
                    else if (key == __TileWidthToken)
                        __tileWidth = value.toInt();
                    else if (key == __TileHeightToken)
                        __tileHeight = value.toInt();
                    else if (key == __TileSetToken) {
                        // A bit more special, get the filename for the spritesheet filepath
                        QStringList fields = value.split(",");
                        __tilesetFilePath = dirPath + QDir::separator() + fields[0];
                    } else if (key == __DataToken) {
                        // Found the Map data section, skip this line but change the reading state
                        readingState = __ParseState::MapData;
                    }
                }
                break;

            case __ParseState::MapData:
                // The lines are now map data (CSV)
                QStringList data = line.split(",");
                for (QString tile : data) {
                    if (!tile.isEmpty())
                        __mapData.append(tile.toInt());
                }
                break;
        }
    }

    // Cleanup
    file.close();

    // Verify the map data
    __good = ((__mapWidth != 0) && (__mapHeight != 0));
    __good &= ((__tileWidth != 0) && (__tileHeight != 0));
    __good &= (__mapData.size() != 0);
    __good &= ((__mapWidth * __mapHeight) == __mapData.size());

    // Verify the tileset file is found relative to the map file's path
    // Get info about the tileset and make a sprite texture for it.

    // Read the map data
    __good = true;
}


/*!
    Creates the image that will be used for the actual sprite.  Make sure that
    __good is set to true or else the funciton will return NULL.  This also
    inclues things like __tileWidth and __mapHeight (etc.) are set to non-zero
    values.

    This will return dynamically allocated memory, so it's on the caller's job
    to free the QImage when done.
*/
QImage *MTileMap::__mkMapImage() {
    // Guard
    if (!__good)
        return NULL;
    
    // Create the map filled with transparency
    QImage *map = new QImage(__mapWidth * __tileWidth, __mapHeight * __tileHeight, QImage::Format_ARGB32_Premultiplied);
    map->fill(Qt::transparent);

    // Blit over the tiles
    QPainter painter(map);
    for (int r = 0; r < __mapHeight; r++) {
        for (int c = 0; c < __mapWidth; c++) {
            int tileNum = __mapData[(r * __mapWidth) + c];
            QRect dest(c * __tileWidth, r * __tileHeight, __tileWidth, __tileHeight);
            painter.drawImage(dest, *(__tileset->image), __getTileRect(tileNum));
        }
    }

    return map;
}


/*!
    Retrives the rectangle that a tilde identified by \a num lives at.  \a num
    should be a non-negative integer.
*/
QRect MTileMap::__getTileRect(int num) {
    num -= 1;
    int tilesetWidth = __tileset->image->width();
    int a = num * __tileWidth;
    int x = a % tilesetWidth;
    int y = a / tilesetWidth;
    return QRect(x, y * __tileHeight, __tileWidth, __tileHeight);
}

