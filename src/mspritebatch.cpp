#include "mspritebatch.h"
#include "msprite.h"
#include <QDebug>


MSpriteBatch::MSpriteBatch() {

}


MSpriteBatch::~MSpriteBatch() {

}


void MSpriteBatch::add(MSprite *sprite) {
    __sprites.append(sprite);
}


/*!
    Add the contents of one batch to another.
*/
void MSpriteBatch::addBatch(const MSpriteBatch &batch) {
    __sprites += batch.__sprites;
}


void MSpriteBatch::clear() {
    __sprites.clear();
}


void MSpriteBatch::drawAll() {
    for (int i = 0; i < __sprites.size(); i++)
        __sprites[i]->draw();
}


