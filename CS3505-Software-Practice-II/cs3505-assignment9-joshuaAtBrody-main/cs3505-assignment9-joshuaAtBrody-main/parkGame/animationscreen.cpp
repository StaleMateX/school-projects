#include "animationscreen.h"
#include "ui_animationscreen.h"
#include <iostream>

AnimationScreen::AnimationScreen(QWidget* parent)
    : QWidget(parent), ui(new Ui::AnimationScreen) {
    ui->setupUi(this);

    QPixmap image;
    image.load("://forest.png");
    image = image.scaled(431, 231);
    ui->backgroundLabel->setPixmap(image);
}

AnimationScreen::~AnimationScreen() { delete ui; }

void AnimationScreen::newFrame(int x, int y, QString playerPath, QString otherPath) {

    setLabel(playerPath, ui->personImageLabel);
    setLabel(otherPath, ui->otherImageLabel);


    ui->personImageLabel->setGeometry(0, y - 100 + 50, 300, 300);
    ui->otherImageLabel->setGeometry(200, y - 100 + 50, 300, 300);
}

void AnimationScreen::setLabel(QString path, QLabel* label) {
    QPixmap image;
    // credit
    // https://stackoverflow.com/questions/1939953/how-to-find-if-a-given-key-exists-in-a-stdmap
    if (spriteCache.find(path) == spriteCache.end()) { // Image in cache

        if (!image.load(path)) { // Path had error
            qWarning() << "Failed to load image:" << path;
            label->clear();
        } else { // Path loaded image
            image = image.scaled(200, 200);
            spriteCache[path] = image;
            label->setPixmap(image);
        }

    } else {                                      // Image in cache
        label->setPixmap(spriteCache[path]);
    }
}
