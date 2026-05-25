#include "tutorialwindow.h"
#include "ui_tutorialwindow.h"

TutorialWindow::TutorialWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TutorialWindow)
{
    ui->setupUi(this);
}

TutorialWindow::~TutorialWindow()
{
    delete ui;
}

void TutorialWindow::changeContent(QString newTitle, QString newContent, QString newImagePath) {
    ui->title->setText(newTitle);
    ui->Content->setText(newContent);
}

void TutorialWindow::on_pushButton_clicked()
{
    emit sendOptionClicked(0);
}

