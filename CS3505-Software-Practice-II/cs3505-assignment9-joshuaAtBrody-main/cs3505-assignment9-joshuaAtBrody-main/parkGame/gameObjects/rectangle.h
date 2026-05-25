#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "gameobject.h"
#include "../qualifiedpixmap.h"
#include <QPixmap>

class Rectangle : public GameObject {
    public:
        Rectangle();

        QualifiedPixmap paint();
};

#endif // RECTANGLE_H
