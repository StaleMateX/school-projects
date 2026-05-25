#include "winnerwindow.h"
#include "ui_winnerwindow.h"

WinnerWindow::WinnerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WinnerWindow)
{
    ui->setupUi(this);
}

WinnerWindow::~WinnerWindow()
{
    delete ui;
}

void WinnerWindow::on_goNextButton_clicked()
{
    emit gotoFromWinnerButton(0);
}

void WinnerWindow::changeContent(QString newTitle, QString newContent, QString newButtonText) {
    ui->winnersTitleLabel->setText(newTitle);
    ui->wonLabel->setText(newContent);
    ui->goNextButton->setText(newButtonText);
}
