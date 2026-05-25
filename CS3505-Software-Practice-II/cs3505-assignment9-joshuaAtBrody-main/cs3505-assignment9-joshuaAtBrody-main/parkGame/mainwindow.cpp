#include "mainwindow.h"
#include "animationscreen.h"
#include "model.h"

using std::make_tuple;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      model(new Model),
      assessmentTest(new AssessmentTest(model)),
      tutorialWindow(new TutorialWindow),
      switchScreenType(new QStackedWidget),
      titleWindow(new TitleWindow),
    loserWindow(new LoserWindow),
    winnerWindow(new WinnerWindow)

{

    this->setGeometry(200, 100, 960, 540);

    switchScreenType->addWidget(titleWindow);
    switchScreenType->addWidget(tutorialWindow);
    switchScreenType->addWidget(assessmentTest);
    switchScreenType->addWidget(loserWindow);
    switchScreenType->addWidget(winnerWindow);

    setCentralWidget(switchScreenType);

    performSetup();
    setGlobalPalette();
}

MainWindow::~MainWindow() {

    delete model;
    delete switchScreenType;
    delete assessmentTest;
    delete tutorialWindow;
    delete titleWindow;
    delete loserWindow;
    delete winnerWindow;
}

void MainWindow::performSetup() {
    connect(model, &Model::sendWinnerWindowContent, this, &MainWindow::changeWinnerScreen);
    connect(model, &Model::sendWinnerWindowContent, winnerWindow, &WinnerWindow::changeContent);
    connect(winnerWindow, &WinnerWindow::gotoFromWinnerButton, model, &Model::changeScreenData);

    connect(model, &Model::sendLoserWindowContent, this, &MainWindow::changeLoserScreen);
    connect(model, &Model::sendLoserWindowContent, loserWindow, &LoserWindow::changeContent);
    connect(loserWindow, &LoserWindow::gotoFromLoserButton, model, &Model::changeScreenData);

    connect(model, &Model::sendTitleContent, this, &MainWindow::changeTitleScreen);
    connect(titleWindow, &TitleWindow::sendStartClicked, model, &Model::changeScreenData);

    // Next button in test and tutorial is clicked, tell model to change screen!
    connect(assessmentTest, &AssessmentTest::sendOptionClicked, this, &MainWindow::optionClicked);
    connect(tutorialWindow, &TutorialWindow::sendOptionClicked, this, &MainWindow::optionClicked);
    connect(this, &MainWindow::optionClicked, model, &Model::changeScreenData);

    // Model is changing to test, tell test screen to change!
    connect(model, &Model::sendTestScreenContent, this, &MainWindow::changeTestScreen);
    connect(this, &MainWindow::sendTestScreenContent, assessmentTest, &AssessmentTest::recieveScreen);

    // Model is changing to tutorial, tell tutorial window to change!
    connect(model, &Model::sendTutorialContent, this, &MainWindow::changeTutorialScreen);
    connect(this, &MainWindow::sendTutorialScreenContent, tutorialWindow, &TutorialWindow::changeContent);

    connect(assessmentTest, &AssessmentTest::gotoTitleScreenPlease, model, &Model::resetView);


}

void MainWindow::changeTestScreen(std::tuple<QString, QString, QString, QString> content) {
    emit sendTestScreenContent(std::get<0>(content), std::get<1>(content),
                               std::get<2>(content), std::get<3>(content));
    switchScreenType->setCurrentWidget(assessmentTest);
}

void MainWindow::changeTutorialScreen(
    std::tuple<QString, QString, QString> content) {
    emit sendTutorialScreenContent(std::get<0>(content), std::get<1>(content),
                                   std::get<2>(content));
    switchScreenType->setCurrentWidget(tutorialWindow);
}

void MainWindow::changeTitleScreen() {
    switchScreenType->setCurrentWidget(titleWindow);
}

void MainWindow::changeLoserScreen(QString content, QString title) {
    switchScreenType->setCurrentWidget(loserWindow);
}

void MainWindow::changeWinnerScreen(QString title, QString content, QString buttonText) {
    switchScreenType->setCurrentWidget(winnerWindow);
}

void MainWindow::setGlobalPalette() {
    // Create a palette with desired colors
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(25, 25, 25)); // Background color

    // Apply the palette globally
    this->setPalette(palette);
    assessmentTest->setPalette(palette);
}

