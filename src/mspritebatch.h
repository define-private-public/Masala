// A simple collection for sprites to draw

#ifndef M_SPRITE_BATCH
#define M_SPRITE_BATCH

#include <QVector>
class MSprite;

class MSpriteBatch {
public:
    MSpriteBatch();
    ~MSpriteBatch();

    void add(MSprite *sprite);
    void addBatch(const MSpriteBatch &batch);
    friend void addBatch(const MSpriteBatch &batch);
    void clear();

    void drawAll();

private:
    QVector<MSprite *> __sprites;
};

#endif // M_SPRITE_BATCH

