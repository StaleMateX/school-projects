#include "loserwindowdata.h"

LoserWindowData::LoserWindowData() {}

LoserWindowData::LoserWindowData(int buttonResult, QString content, QString title) : buttonResult(buttonResult), content(content), title(title) {}

LoserWindowData::~LoserWindowData() {}

int LoserWindowData::getButtonResult() {
    return buttonResult;
}

QString LoserWindowData::getContent() {
    return content;
}

QString LoserWindowData::getTitle() {
    return title;
}
