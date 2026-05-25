#ifndef QUALIFIEDPIXMAP_H
#define QUALIFIEDPIXMAP_H

#include <QPixmap>

/// \brief Everything the view needs to draw one object in the model: a pixmap and a coordinate to draw the pixmap at.
class QualifiedPixmap {
    public:
        QualifiedPixmap();

        QualifiedPixmap(QPixmap pixmap, int x, int y);

        QPixmap pixmap;
        int x;
        int y;
};

#endif // QUALIFIEDPIXMAP_H
