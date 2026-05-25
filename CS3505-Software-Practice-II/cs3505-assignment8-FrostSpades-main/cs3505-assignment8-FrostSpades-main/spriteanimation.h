/**
 * QWidget that plays an animation of the frames
 * @date 3/29/2024
 */
#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QWidget>
#include "model.h"

namespace Ui {
class SpriteAnimation;
}

class SpriteAnimation : public QWidget{
    Q_OBJECT

public:
    /**
     * @brief SpriteAnimation constructor for the QWidget
     * @param parent parent of the QWidget
     */
    explicit SpriteAnimation(QWidget *parent = nullptr);
    /**
     * @brief startAnimation start the animation
     */
    void startAnimation();

private:
    // pointer to the ui
    Ui::SpriteAnimation *ui;
    // storage of the frames for animation use
    QMap<int, QImage> ourFrames;
    // delay between frames
    int animationDelay;
    // timer for drawing the next frame
    QTimer *timer;
    // scale of image being drawn
    int scale;
    // pointer to the model
    Model *model;
    // index of the current frame in animation
    int currentIndex;
    // current frame in the animation
    QImage currentFrame() const;

protected:
    /**
     * @brief paintEvent paints a QImage
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

public slots:
    /**
     * @brief updateFrames Model sendFrames signal, frame has been drawn on and needs to be updated in view.
     * @param frames All of the frames in the project.
     */
    void updateFrames(QMap<int, QImage> frames);

    /**
     * @brief changeDelay the delay of the fps in the animation has changed and needs to be updated.
     * @param newDelay the new delay variable.
     */
    void changeDelay(int newDelay);

    /**
     * @brief changeScale change the scale of the preview animation
     * @param newScale int to change the scale to
     */
    void changeScale(int newScale);
    /**
     * @brief repaint frame
     */
    void paint();
};

#endif // SPRITEANIMATION_H
