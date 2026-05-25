#ifndef ANIMATIONWIDGET_H
#define ANIMATIONWIDGET_H

#include <Box2D/Box2D.h>
#include <QObject>

/**
 * @brief The AnimationWidget owns and controls the animations: it uses Box2D
 * to generate positions, then sends that with the desired sprite to the view.
 */
class AnimationWidget : public QObject {
        Q_OBJECT

    public:
        /**
         * @brief AnimationWidget Default constructor
         */
        AnimationWidget(std::vector<QString> playerSpritePath);

        AnimationWidget();

        AnimationWidget& operator=(AnimationWidget&&);

        /**
         * @brief start Begins the animation.  This will (later) emit
         * things to the view telling it to draw stuff, so only call this when
         * you want stuff drawn on the screen.
         */
        void start();

        /**
         * @brief stop The animation will stop the frame after this
         * method is called.
         */
        void stop();

        /**
         * @brief setSubject Immediately changes the "other" (non-player)
         * animation sprite.  You should probably call this *after* calling
         * stop().
         * @param pathToEncounteredSprite a QString of the path (the
         * path is obtained by r-clicking on the asset in the QResource file
         * and using "Copy Resource Path to Clipboard").
         */
        void setSubject(std::vector<QString> pathToEncounteredSprite);

    signals:
        /**
         * @brief newFrame TODO incomplete; needs to emit the sprite
         * information (and real X/Y coordinates) after quererying them from
         * Box2D
         * @param x The X coordinate of the image (TODO: WHAT CORNER?
         * CENTER?)
         * @param y The Y coordinate of the image (TODO: WHAT CORNER?
         * CENTER?)
         * @param playerPath Relative (QResource) path to player sprite
         * @param otherPath Relative (QResource) path to other sprite
         */
        void newFrame(int x, int y, QString playerPath, QString otherPath);

    private:
        int FRAME_SPEED = 200; // how many milliseconds each frame takes
        bool continueAnimation = false; // when false, animation timing loop
                                        // will terminate on next frame.
        std::vector<QString> playerSpritePath; // Path to player sprite; constant; @TODO set this
        std::vector<QString> otherSpritePath; // Path to whatever the player is encountering;
                                 // set by setSubject()

        int currentSprite;
        int frameCounter;
        int maxFrames = 1000;

        b2World world;
        b2Body* body;

<<<<<<< HEAD
        /*
         * \brief generateNextFrame recursive method.  start() is the
=======
        int frameCounter = 0; // For debug purposes

        /**
         * @brief generateNextFrame recursive method.  start() is the
>>>>>>> 5fcd1feee3862361a2381a7a511b2e8a275760e7
         * driver; loops every FRAME_SPEED miliseconds until stop() is
         * called.
         */
        void generateNextFrame();
};

#endif // ANIMATIONWIDGET_H
