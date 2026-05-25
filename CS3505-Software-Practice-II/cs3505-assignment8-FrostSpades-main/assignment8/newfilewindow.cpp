#include "newfilewindow.h"
#include <QFileDialog>
#include "ui_newfilewindow.h"

NewFileWindow::NewFileWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewFileWindow)
{
    ui->setupUi(this);
}

NewFileWindow::~NewFileWindow()
{
    delete ui;
}

void NewFileWindow::on_cancelButton_clicked()
{
    emit cancelButtonClicked();
}

void NewFileWindow::reset()
{
    ui->sizeBox->setValue(8);
    ui->textName->setText(QString(""));
    ui->textPath->setText(QString(""));
}

void NewFileWindow::on_pushButton_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(nullptr,
                                                          "Directory Selection",
                                                          QDir::homePath(),
                                                          QFileDialog::ShowDirsOnly
                                                              | QFileDialog::DontResolveSymlinks);

    ui->textPath->setText(directory);
}

void NewFileWindow::on_pushButton_2_clicked()
{
    // Check if inputs are empty
    if (ui->textName->displayText() == QString("")) {
        emit error("Name field cannot be empty");
        return;
    }

    if (ui->textPath->displayText() == QString("")) {
        emit error("Path field cannot be empty");
        return;
    }

    QDir directory(ui->textPath->displayText());

    if (!directory.exists()) {
        emit error("Directory does not exist");
        return;
    }

    emit submitButtonClicked(ui->textName->displayText(),
                             ui->sizeBox->value(),
                             ui->textPath->displayText());
}
