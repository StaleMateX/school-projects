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
    connect(loadFileWindow,
            &LoadFileWindow::cancelButtonClicked,
            this,
            &MainWindow::onLoadFileCancel);
    connect(loadFileWindow,
            &LoadFileWindow::submitButtonClicked,
            this,
            &MainWindow::onLoadFileSubmit);
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
    this->setGeometry(450, 100, 1100, 700);
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

    // TODO: Send data to model
    model = new Model(name, size, path);
    setModelConnections();
}

void MainWindow::receiveFrames(QImage frames[]) {

}

void MainWindow::setModelConnections() {
    connect(this, &MainWindow::deleteFrame, model, &Model::deleteFrame);
    connect(this, &MainWindow::addFrame, model, &Model::addFrame);
    connect(this, &MainWindow::drawOnFrame, model, &Model::changePixelData);
    connect(this, &MainWindow::getData, model, &Model::returnFrames);
    connect(this, &MainWindow::reorderFrames, model, &Model::switchFrames);
    connect(this, &MainWindow::saveData, model, &Model::saveModel);

    connect(model, &Model::sendFrames, this, &MainWindow::receiveFrames);
}

void MainWindow::onLoadFileSubmit(QString path)
{
    openSpriteEditorWindow();
    loadFileWindow->reset();
    loadFileWindow->close();

    model = new Model(path);
    setModelConnections();
}
