#include "startupwindow.h"
#include "ui_startupwindow.h"

StartupWindow::StartupWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartupWindow)
{
    ui->setupUi(this);
}

StartupWindow::~StartupWindow()
{
    delete ui;
}

void StartupWindow::on_newButton_clicked()
{
    emit newButtonClicked();
}

void StartupWindow::on_loadButton_clicked()
{
    emit loadButtonClicked();
}
