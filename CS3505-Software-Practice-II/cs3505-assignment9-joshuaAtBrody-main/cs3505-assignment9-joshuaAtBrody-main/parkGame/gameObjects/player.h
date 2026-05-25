#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"
#include "../qualifiedpixmap.h"
#include <QPixmap>

class Player : public GameObject {
    public:
        Player();

        QualifiedPixmap paint();
};

#endif // PLAYER_H
