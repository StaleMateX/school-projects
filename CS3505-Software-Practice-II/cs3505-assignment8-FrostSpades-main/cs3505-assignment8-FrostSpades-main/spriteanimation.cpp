#include "spriteanimation.h"

SpriteAnimation::SpriteAnimation(QWidget *parent)
    : QWidget(parent){
    animationDelay = 2000;
    scale = 1;
    // Wait for frames
    // Get frames from mainwindow

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(paint()));
}

void SpriteAnimation::startAnimation(){
    int widths = ourFrames.first().width() * scale;

    // Update animation size realistically
    this->setGeometry(880, 60, widths, widths);

    if (!ourFrames.isEmpty())
        timer->start(animationDelay);
}

void SpriteAnimation::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.drawImage(rect(), ourFrames[currentIndex].scaled(QSize(32, 32)));
}

QImage SpriteAnimation::currentFrame() const{
    auto it = ourFrames.find(currentIndex);
    if (it != ourFrames.end())
        return it.value();
    else
        return QImage(); // Return empty image if index not found
}

// Slots
void SpriteAnimation::updateFrames(QMap<int, QImage> frameVector){
    ourFrames = frameVector;
    startAnimation();
}

void SpriteAnimation::changeDelay(int newDelay){
    animationDelay = newDelay;
    startAnimation();
}

void SpriteAnimation::changeScale(int newScale){
    scale = newScale;
    startAnimation();
}

void SpriteAnimation::paint(){
    currentIndex = (currentIndex + 1) % ourFrames.size();
    update(); // Trigger repaint
}
