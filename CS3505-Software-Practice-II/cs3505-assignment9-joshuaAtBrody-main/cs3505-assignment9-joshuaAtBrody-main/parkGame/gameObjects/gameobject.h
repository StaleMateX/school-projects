#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../qualifiedpixmap.h"
#include <QPixmap>

/// \brief The gameObject class is the template for all objects with visual representations in the game, e.g., the player or a tree.
class GameObject {
    public:
        /// \brief Default constructor
        GameObject();

        /// \brief paint Returns the information necessary for model to emit this object's data to view.
        /// Objects should "paint" themselves at the bottom center of their sprite.
        /// THIS METHOD SHOULD RETURN SOMETHING.
        virtual QualifiedPixmap paint() = 0;
};

#endif // GAMEOBJECT_H
