#include "winnerwindowdata.h"

WinnerWindowData::WinnerWindowData() {}

WinnerWindowData::WinnerWindowData(int buttonResult, QString title, QString content, QString buttonText) : buttonResult(buttonResult), title(title), content(content), buttonText(buttonText) {

}

WinnerWindowData::~WinnerWindowData() {}

int WinnerWindowData::getButtonResult() {
    return buttonResult;
}

QString WinnerWindowData::getTitle() {
    return title;
}

QString WinnerWindowData::getContent() {
    return content;
}

QString WinnerWindowData::getButtonText() {
    return buttonText;
}
