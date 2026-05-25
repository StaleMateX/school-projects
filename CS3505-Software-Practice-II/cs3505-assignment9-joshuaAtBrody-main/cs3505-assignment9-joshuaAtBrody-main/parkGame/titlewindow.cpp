#include "titlewindow.h"
#include "ui_titlewindow.h"

TitleWindow::TitleWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TitleWindow)
{
    ui->setupUi(this);

    QPixmap image;
    image.load("://parkBackground.jpg");
    image = image.scaled(731, 431);
    ui->background->setPixmap(image);
}

TitleWindow::~TitleWindow()
{
    delete ui;
}

void TitleWindow::on_pushButton_clicked()
{
    emit sendStartClicked(0);
}

