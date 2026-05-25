#include "loadfilewindow.h"
#include <QFileDialog>
#include "ui_loadfilewindow.h"

LoadFileWindow::LoadFileWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadFileWindow)
{
    ui->setupUi(this);
}

LoadFileWindow::~LoadFileWindow()
{
    delete ui;
}

void LoadFileWindow::on_sourceButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(nullptr,
                                                          "Directory Selection",
                                                          QDir::homePath(),
                                                          QFileDialog::ShowDirsOnly
                                                              | QFileDialog::DontResolveSymlinks);

    ui->textPath->setText(directory);
}

void LoadFileWindow::on_submitButton_clicked()
{
    if (ui->textPath->displayText() == QString("")) {
        emit error(QString("Path cannot be empty"));
        return;
    }

    QDir directory(ui->textPath->displayText());

    if (!directory.exists()) {
        emit error("Directory does not exist");
        return;
    }

    emit submitButtonClicked(ui->textPath->displayText());
}

void LoadFileWindow::on_cancelButton_clicked()
{
    emit cancelButtonClicked();
}

void LoadFileWindow::reset()
{
    ui->textPath->setText(QString(""));
}
