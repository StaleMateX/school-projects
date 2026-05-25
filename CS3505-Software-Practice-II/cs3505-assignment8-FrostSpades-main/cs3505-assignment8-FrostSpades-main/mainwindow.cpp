#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , allPages(new QStackedWidget)
    , loadFileWindow(new LoadFileWindow)
    , newFileWindow(new NewFileWindow)
    , spriteEditorWindow(new SpriteEditorWindow)
    , startupWindow(new StartupWindow)
{
    ui->setupUi(this);
    model = nullptr;

    // Add pages to the main view
    allPages->addWidget(startupWindow);
    allPages->addWidget(spriteEditorWindow);
    setCentralWidget(allPages);

    openStartupWindow();

    setConnections();
    setGlobalPalette();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete allPages;
    delete loadFileWindow;
    delete newFileWindow;
    delete spriteEditorWindow;
    delete startupWindow;
}

void MainWindow::setConnections()
{
    connect(startupWindow, &StartupWindow::newButtonClicked, this, &MainWindow::openNewFileWindow);
    connect(startupWindow, &StartupWindow::loadButtonClicked, this, &MainWindow::openLoadFileWindow);

    connect(newFileWindow, &NewFileWindow::cancelButtonClicked, this, &MainWindow::onNewFileCancel);
    connect(newFileWindow, &NewFileWindow::error, this, &MainWindow::showError);
    connect(newFileWindow, &NewFileWindow::submitButtonClicked, this, &MainWindow::onNewFileSubmit);

    connect(loadFileWindow, &LoadFileWindow::error, this, &MainWindow::showError);
    connect(loadFileWindow, &LoadFileWindow::cancelButtonClicked, this, &MainWindow::onLoadFileCancel);
    connect(loadFileWindow, &LoadFileWindow::submitButtonClicked, this, &MainWindow::onLoadFileSubmit);

    setSpriteEditorConnections();
}

void MainWindow::setSpriteEditorConnections() {
    DrawWindow *drawWindow = spriteEditorWindow->getDrawWindow();
    SpriteAnimation *animationWindow = spriteEditorWindow->getAnimationWindow();

    connect(spriteEditorWindow, &SpriteEditorWindow::updateDelayOfAnimation, animationWindow, &SpriteAnimation::changeDelay);

    connect(spriteEditorWindow, &SpriteEditorWindow::frameSpinBoxChanged, drawWindow, &DrawWindow::changeFrame);
    connect(drawWindow, &DrawWindow::click, spriteEditorWindow, &SpriteEditorWindow::processClick);
    connect(spriteEditorWindow, &SpriteEditorWindow::startMenuButtonClicked, this, &MainWindow::returnToStartPage);
    connect(spriteEditorWindow, &SpriteEditorWindow::setCurrentFrame, drawWindow, &DrawWindow::changeFrame);

    connect(spriteEditorWindow, &SpriteEditorWindow::animationScaleChange, animationWindow, &SpriteAnimation::changeScale);
}

void MainWindow::setGlobalPalette()
{
    // Create a palette with desired colors
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(50, 50, 50)); // Background color
    palette.setColor(QPalette::Button, QColor(50, 50, 50));

    // Apply the palette globally
    this->setPalette(palette);
    loadFileWindow->setPalette(palette);
    newFileWindow->setPalette(palette);
    spriteEditorWindow->setPalette(palette);
    startupWindow->setPalette(palette);
}

void MainWindow::openStartupWindow()
{
    // Set the current page to the startup window
    allPages->setCurrentWidget(startupWindow);
    this->setGeometry(900, 300, 300, 276);
}

void MainWindow::openSpriteEditorWindow()
{
    allPages->setCurrentWidget(spriteEditorWindow);
    this->setGeometry(450, 100, 1041, 618);
}

void MainWindow::openNewFileWindow()
{
    newFileWindow->exec();
}

void MainWindow::openLoadFileWindow()
{
    loadFileWindow->exec();
}

void MainWindow::onNewFileCancel()
{
    newFileWindow->reset();
    newFileWindow->close();
}

void MainWindow::onLoadFileCancel()
{
    loadFileWindow->reset();
    loadFileWindow->close();
}

void MainWindow::showError(QString errorMessage)
{
    QMessageBox::critical(nullptr, "Error", errorMessage);
}

void MainWindow::onNewFileSubmit(QString name, int size, QString path)
{
    openSpriteEditorWindow();
    newFileWindow->reset();
    newFileWindow->close();

    model = new Model(name, size, path);
    setModelConnections();
    model->transmitSize();
}

void MainWindow::setModelConnections()
{
    DrawWindow *drawWindow = spriteEditorWindow->getDrawWindow();
    SpriteAnimation *animationWindow = spriteEditorWindow->getAnimationWindow();

    connect(this, &MainWindow::deleteFrame, model, &Model::deleteFrame);
    connect(this, &MainWindow::addFrame, model, &Model::addFrame);
    connect(this, &MainWindow::drawOnFrame, model, &Model::changePixelData);
    connect(this, &MainWindow::getData, model, &Model::returnFrames);
    connect(this, &MainWindow::reorderFrames, model, &Model::switchFrames);
    connect(this, &MainWindow::saveData, model, &Model::saveModel);

    connect(model, &Model::sendFrames, drawWindow, &DrawWindow::updateFrames);
    connect(model, &Model::sendNumberOfFrames, spriteEditorWindow, &SpriteEditorWindow::setSpinbox);
    connect(model, &Model::sendSize, drawWindow, &DrawWindow::changePixelSize);

    connect(spriteEditorWindow, &SpriteEditorWindow::saveButtonClicked, model, &Model::saveModel);

    connect(model, &Model::sendFrames, animationWindow, &SpriteAnimation::updateFrames);

    connect(spriteEditorWindow, &SpriteEditorWindow::sendClick, model, &Model::changeFrame);
    connect(spriteEditorWindow, &SpriteEditorWindow::addFrame, model, &Model::addFrame);
    connect(spriteEditorWindow, &SpriteEditorWindow::deleteCurrentFrame, model, &Model::deleteFrame);
    connect(spriteEditorWindow, &SpriteEditorWindow::swapFrames, model, &Model::swapFrames);
    connect(spriteEditorWindow, &SpriteEditorWindow::invertColors, model, &Model::invertColors);
    connect(spriteEditorWindow, &SpriteEditorWindow::mirrorHorizontal, model, &Model::mirrorHorizontal);
    connect(spriteEditorWindow, &SpriteEditorWindow::mirrorVertical, model, &Model::mirrorVertical);
}

void MainWindow::onLoadFileSubmit(QString path)
{
    openSpriteEditorWindow();
    loadFileWindow->reset();
    loadFileWindow->close();

    model = new Model(path);
    setModelConnections();
    model->notifyView();
    model->transmitSize();
}

void MainWindow::returnToStartPage()
{
    bool isSaved = model->getIsSaved();

    if (!isSaved) {
        QMessageBox::StandardButton reply
            = QMessageBox::question(this,
                                    "Warning: Unsaved Changes",
                                    "Unsaved Changes: Are you sure you want to exit?",
                                    QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    delete model;

    spriteEditorWindow->reset();

    openStartupWindow();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (allPages->currentWidget() == startupWindow) {
        event->accept();
        return;
    }

    bool isSaved = model->getIsSaved();

    if (!isSaved) {
        QMessageBox::StandardButton reply
            = QMessageBox::question(this,
                                    "Warning: Unsaved Changes",
                                    "Unsaved Changes: Are you sure you want to exit?",
                                    QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}
