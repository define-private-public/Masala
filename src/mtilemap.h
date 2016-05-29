// A simple tile map, is a subclass of MSprite
// Implements the Flare map format

#ifndef M_TILE_MAM_H
#define M_TILE_MAM_H


#include "msprite.h"
class QImage;


class MTileMap : public MSprite {
    Q_OBJECT

public:
    // Some things
    static const QString FrameName;

    MTileMap(QString flareMapFilename);
//    MTileMap(const MTileMap &tm);
    ~MTileMap();

    // Accessors
    bool isGood();
    int tileWidth();
    int tileHeight();
    int mapWidth();
    int mapHeight();
    QVector<int> mapData();

    // overloads
    void update(qreal dt) override;

    // For access to parent class methods
    MSprite *asSprite();


private:
    // For parsing
    enum __ParseState {GeneralInfo, MapData};
    const QString __WidthToken = "width";
    const QString __HeightToken = "height";
    const QString __TileWidthToken = "tilewidth";
    const QString __TileHeightToken = "tileheight";
    const QString __TileSetToken = "tileset";
    const QString __DataToken = "data";

    // Member data
    bool __good = false;                // Flag if the data is good or not
    int __mapWidth = 0;                    // Columns
    int __mapHeight = 0;                // Rows
    int __tileWidth = 0;                // Tile Widget
    int __tileHeight = 0;                // Tile Height
    QVector<int> __mapData;                // map data represented as a 1D array
    QString __tilesetFilePath;            // Relative filepath to the tileset
    MTextureStruct *__tileset;            // Separate texture struct for the tilset
                                        // The map will use the `__spritesheet` variable

    // Functions
    void __readFlareMapFile(QString filePath);
    QImage *__mkMapImage();
    QRect __getTileRect(int num);

};


#endif

