#include "assessmentdata.h"

AssessmentData::AssessmentData() {}

AssessmentData::AssessmentData(QString title, QString option1Text, QString option2Text, QString contentText, std::vector<QString> imagePath, int option1Result, int option2Result) :
    title(title), option1Text(option1Text), option2Text(option2Text), contentText(contentText), imageList(imagePath), option1Result(option1Result), option2Result(option2Result)
{}

AssessmentData::~AssessmentData() {}

QString AssessmentData::getOption1Text() {
    return option1Text;
}

QString AssessmentData::getOption2Text() {
    return option2Text;
}

QString AssessmentData::getContentText() {
    return contentText;
}

std::vector<QString> AssessmentData::getImages() {
    return imageList;
}

int AssessmentData::getOption1Result() {
    return option1Result;
}

int AssessmentData::getOption2Result() {
    return option2Result;
}

QString AssessmentData::getTitle() {
    return title;
}
