#include "tutorialdata.h"

TutorialData::TutorialData() {}
TutorialData::TutorialData(Screen*& other) {}
TutorialData::~TutorialData(){}
TutorialData::TutorialData(QString title, QString content, QString imagePath, int buttonResult) :
    title(title), content(content), imagePath(imagePath), buttonResult(buttonResult)
{}


QString TutorialData::getTitle() {
    return title;
}

QString TutorialData::getContent() {
    return content;
}

QString TutorialData::getImagePath() {
    return imagePath;
}

int TutorialData::getButtonResult() {
    return buttonResult;
}
