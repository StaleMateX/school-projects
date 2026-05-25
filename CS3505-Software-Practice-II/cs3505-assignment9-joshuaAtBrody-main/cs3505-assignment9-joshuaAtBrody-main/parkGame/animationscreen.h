/**
 * Header file for the animation QWidget responsible for animating the sprites
 * @date 04/21/24
 */
#ifndef ANIMATIONSCREEN_H
#define ANIMATIONSCREEN_H

#include <Box2D/Box2D.h>
#include <QMainWindow>
#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QWidget>
#include <QLabel>

namespace Ui {
class AnimationScreen;
}

class AnimationScreen : public QWidget {
        Q_OBJECT

    public:
        /**
         * @brief AnimationScreen constructor
         * @param parent
         */
        explicit AnimationScreen(QWidget* parent = nullptr);
        /**
         * @brief ~AnimationScreen destructor for the animation screen widget
         */
        ~AnimationScreen();

    public slots:
        /**
         * @brief newFrame processes incoming new frame data
         */
        void newFrame(int x, int y, QString playerPath, QString otherPath);

    private:
        Ui::AnimationScreen* ui; // reference to the ui
        std::map<QString,QPixmap> spriteCache;

        /**
         * @brief setLabel Sets given label to image at path, or clears label if bad path
         * @param path The path to search for (relative, from QResource file)
         * @param label Pointer to the QLabel to set
         */
        void setLabel(QString path, QLabel* label);
};

#endif // ANIMATIONSCREEN_H
