#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QImage>
#include <QDebug>

#include "Model.h"

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Gets rid of a certain white line in the background (don't ask why)
    this->centralWidget()->setStyleSheet("background-image:url(\"RedBlueSimonBackground.png\"); background-position: center;");

    // Set regular color of the red button to be red + pressed
    ui->RedButton->setStyleSheet( QString("QPushButton {background-color: rgb(200,50,50);} QPushButton:pressed {background-color: rgb(255,150,150);}"));
    ui->BlueButton->setStyleSheet( QString("QPushButton {background-color: rgb(50,50,200);} QPushButton:pressed {background-color: rgb(150,150,255);}"));

    // Set up game
    ui->loserText->setVisible(false);
    ui->RedButton->setEnabled(false);
    ui->BlueButton->setEnabled(false);

    connect(&model, &Model::updateProgress,
            ui->RoundProgressBar, &QProgressBar::setValue);

    // Start Game button has been clicked! Start the game! (Will disable StartGameButton)
    connect(ui->StartGameButton, &QAbstractButton::clicked,
            &model, &Model::startGame);

    connect(&model, &Model::turnStartButton,
            ui->StartGameButton, &QPushButton::setEnabled);

    // clicked goes to the click slot method in model
    connect(ui->RedButton, &QAbstractButton::clicked, &model, &Model::buttonRedClicked);
    connect(ui->BlueButton, &QAbstractButton::clicked, &model, &Model::buttonBlueClicked);


    // model signals to enable/disable a button
    connect(&model, &Model::switchAbilityOfColorButtons, ui->RedButton, &QPushButton::setEnabled);
    connect(&model, &Model::switchAbilityOfColorButtons, ui->BlueButton, &QPushButton::setEnabled);

    // model signlas view to flash a button
    connect(&model, &Model::viewRedFlash, this, &MainWindow::flashRedButton);
    connect(&model, &Model::viewBlueFlash, this, &MainWindow::flashBlueButton);

    // model signals view to show LOSER text
    connect(&model, &Model::playerFailed, ui->loserText, &QLabel::setVisible);



}

void MainWindow::flashRedButton(bool flashing){
    if(flashing){
        ui->RedButton->setStyleSheet("background-color: rgb(255, 0, 0);");
    } else {
        ui->RedButton->setStyleSheet("background-color: rgb(200,50,50);");
    }
}

void MainWindow::flashBlueButton(bool flashing){
    if(flashing){
        ui->BlueButton->setStyleSheet("background-color: rgb(0, 0, 255);");
    } else {
        ui->BlueButton->setStyleSheet("background-color: rgb(50, 50, 200);");
    }
}





// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}
