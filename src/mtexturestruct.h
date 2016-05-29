// A simple data structure for Textures

#ifndef M_TEXTURE_STRUCT_H
#define M_TEXTURE_STRUCT_H

#include <QDebug>
#include <QString>
#include <QImage>
#include <QOpenGLTexture>


struct MTextureStruct{
    QString filePath;
    QImage *image = NULL;
    QOpenGLTexture *glTexture = NULL;

    MTextureStruct(QString path, QImage *imagePtr, QOpenGLTexture *texPtr) :
        filePath(path),
        image(imagePtr),
        glTexture(texPtr)
    { }

    ~MTextureStruct() {
        // Cleanup what's not NULL
        if (image)
            delete image;
        if (glTexture)
            delete glTexture;
    }
};


#endif //M_TEXTURE_STRUCT_H

