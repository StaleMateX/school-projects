#include "loserwindow.h"
#include "ui_loserwindow.h"

LoserWindow::LoserWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoserWindow)
{
    ui->setupUi(this);
}

LoserWindow::~LoserWindow()
{
    delete ui;
}

void LoserWindow::changeContent(QString content, QString title) {
    ui->youLoseLabel->setText(title);
    ui->lossLabel->setText(content);
}


void LoserWindow::on_tryAgainButton_clicked()
{
    emit gotoFromLoserButton(0);
}

