#include "titlewindowdata.h"

TitleWindowData::TitleWindowData() {}

TitleWindowData::TitleWindowData(int buttonResult) : buttonResult(buttonResult) {}

TitleWindowData::~TitleWindowData() {}

int TitleWindowData::getButtonResult() {
    return buttonResult;
}
